#include "TaskScheduler.h"

namespace Engine
{
    TaskScheduler::TaskScheduler(size_t threadCount)
        : m_idleSleepTime(WORKER_LOOP_WAIT_NANOSECONDS_DEFAULT), m_stopFlag(false)
    {
        ASSERT(threadCount > 0, "Thread count must be greater than zero");

        for (size_t i = 0; i < threadCount; ++i)
        {
            this->m_queues.push_back(std::make_unique<TaskQueue>());
        }

        this->m_threads.reserve(threadCount);

        for (size_t i = 0; i < threadCount; ++i)
        {
            this->m_threads.emplace_back([this, i]() { workerLoop(i); });
        }
    }

    TaskScheduler::~TaskScheduler()
    {
        this->m_stopFlag.store(true);
        this->m_condition.notify_all();

        for (std::thread& thread : this->m_threads)
        {
            if (thread.joinable()) thread.join();
        }
    }

    // Submits a new task to the scheduler and updates the internal task counter 
    void TaskScheduler::submit(const Task& task)
    {
        this->m_taskCounter.add(1);

        size_t index = randomIndex();
        this->m_queues[index]->pushBack(Task([task, this]()
        {
            task.execute();
            this->m_taskCounter.signal();
        }));

        this->m_condition.notify_one();
    }

    // This is a batch helper for when we wanna run any parallel for-loops
    // For very large loops, we want to create a reasonable amount of tasks that process x-x range.
    // So this creates a series of sub-tasks that process chunks of the loop. This pattern helps us take advantage..
    // of our workers.
    void TaskScheduler::submitBatched(size_t total_items, std::function<void(size_t index)> function_per_item,
                                      size_t desired_tasks)
    {
        if (total_items == 0) return;

        size_t num_workers = m_threads.size();
        size_t num_tasks_to_create = desired_tasks;

        if (num_tasks_to_create == 0)
        {
            // Ensuring there's always *at least* one task but never exceed the number of items.
            // We default to this behavior if desiredTasks isn't set and create a certain amount of tasks per worker thread..
            // to help keep workers busy and productive.
            num_tasks_to_create = std::min(
                total_items, std::max(static_cast<size_t>(1), num_workers * BATCH_SUBMIT_HEURISTIC_MULTIPLIER));
        }
        else
        {
            num_tasks_to_create = std::min(total_items, std::max(static_cast<size_t>(1), num_tasks_to_create));
        }

        size_t batch_size = std::max(static_cast<size_t>(1), total_items / num_tasks_to_create);

        for (size_t task_idx = 0; task_idx < num_tasks_to_create; ++task_idx)
        {
            size_t start_item_index = task_idx * batch_size;
            size_t end_item_index = (task_idx == num_tasks_to_create - 1)
                                      ? total_items
                                      : std::min(start_item_index + batch_size, total_items);

            if (start_item_index >= end_item_index) continue;

            Task batch_task([start_item_index, end_item_index, function_per_item]
            {
                for (size_t i = start_item_index; i < end_item_index; ++i)
                    function_per_item(i);
            });

            submit(batch_task);
        }
    }

    void TaskScheduler::wait()
    {
        m_taskCounter.wait();
    }

    // The main loop executed by every worker thread
    void TaskScheduler::workerLoop(size_t thread_index)
    {
        Task local_task;

        while (!m_stopFlag.load())
        {
            // Try to pop a task from this thread's own queue LIFO
            if (auto task_opt = m_queues[thread_index]->popBack())
            {
                local_task = *task_opt;
                local_task.execute();
            }
            // If our queue is empty, try to steal a task from another thread's queue, which is done FIFO..
            // so the oldest task is always done first
            else if (tryStealTask(thread_index, local_task))
            {
                local_task.execute();
            }
            // Lastly, if no task is found in the queue, we lock the sleep mutex
            // wait_for() is used instead of wait() to prevent deadlocks if a notification is missed..
            // and to allow periodic rechecking of the stop flag
            else
            {
                std::unique_lock<std::mutex> lock(m_sleepMutex);
                m_condition.wait_for(lock, std::chrono::nanoseconds(m_idleSleepTime));
                // Calling std::this_thread::yield() is simpler but incurs busy-waiting.. the conditional variable approach..
                // is faster and wastes less resources when idle
            }
        }
    }

    bool TaskScheduler::tryStealTask(size_t thief_index, Task& out_task)
    {
        const size_t queue_count = m_queues.size();
        size_t start_index = randomIndex();

        for (size_t i = 0; i < queue_count; ++i)
        {
            size_t victim_index = (start_index + i) % queue_count;
            // Don't steal from yourself!
            if (victim_index == thief_index) continue;

            if (auto task_opt = m_queues[victim_index]->stealFront())
            {
                out_task = *task_opt;
                return true;
            }
        }
        return false;
    }

    size_t TaskScheduler::randomIndex() const
    {
        // Each thread gets its own instance of an RNG. This avoids a lot of unnecessary thread contention.
        thread_local std::mt19937 rng{std::random_device{}()};
        std::uniform_int_distribution<size_t> dist(0, m_queues.size() - 1);
        return dist(rng);
    }

    // Sets the frequency of every wake cycle of an idle thread. Lower numbers mean more busy-waiting and CPU cycles being used when idle.. 
    // but potentially will reduce latency
    void TaskScheduler::setSchedulerIdleThreadSleepTime(const long long nanoseconds)
    {
        std::lock_guard<std::mutex> lock(this->m_idleSleepTimeMutex);
        this->m_idleSleepTime = nanoseconds;
    }

    long long TaskScheduler::getSchedulerIdleThreadSleepTime()
    {
        std::lock_guard<std::mutex> lock(this->m_idleSleepTimeMutex);
        return this->m_idleSleepTime;
    }
}
