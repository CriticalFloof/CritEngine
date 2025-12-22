#include "TaskCounter.h"

namespace Engine {

	// This helper class tracks the number of pending tasks and is used internally in the TaskScheduler. 
	// The Wait() function is wrapped in the TaskScheduler and is used to synchronize with the completion of a..
	// batch of tasks.

	TaskCounter::TaskCounter() 
		: remaining(0) 
	{

	}

	void TaskCounter::Add(uint32_t count)
	{
		ASSERT(count > 0, "Can't add zero tasks");
		this->remaining.fetch_add(count, std::memory_order_relaxed);
	}

	void TaskCounter::Signal()
	{
		if (this->remaining.fetch_sub(1, std::memory_order_acq_rel) == 1) {
			std::unique_lock<std::mutex> lock(this->mutex);
			this->condition.notify_all();
		}
	}

	void TaskCounter::Wait()
	{
		std::unique_lock<std::mutex> lock(this->mutex);
		this->condition.wait(lock, [this]() {
			return this->remaining.load(std::memory_order_acquire) == 0;
		});
	}

}