#include "TaskQueue.h"

namespace Engine {

	// Double-ended queue designed for use with the work-stealing pattern. Each worker thread owns a queue.

	void TaskQueue::PushBack(const Task& task)
	{
		std::lock_guard<std::mutex> lock(this->mutex);
		this->deque.emplace_back(task);
	}

	void TaskQueue::PushFront(const Task& task)
	{
		std::lock_guard<std::mutex> lock(this->mutex);
		this->deque.emplace_front(task);
	}

	// Returns std::nullopt if the queue is empty. 
	std::optional<Task> TaskQueue::PopBack()
	{
		std::lock_guard<std::mutex> lock(this->mutex);
		if (this->deque.empty()) return std::nullopt;
		Task task = deque.back();
		this->deque.pop_back();
		return task;
	}

	std::optional<Task> TaskQueue::StealFront()
	{
		std::lock_guard<std::mutex> lock(this->mutex);
		if (this->deque.empty()) return std::nullopt;
		Task task = this->deque.front();
		this->deque.pop_front();
		return task;
	}

	bool TaskQueue::IsEmpty()
	{
		std::lock_guard<std::mutex> lock(this->mutex);
		return this->deque.empty();
	}
}