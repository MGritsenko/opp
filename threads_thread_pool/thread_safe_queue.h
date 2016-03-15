#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

class Task;
typedef std::shared_ptr<Task> TaskPtr;

/**
* Thread safe queue of tasks.
*/
class ThreadSafeQueue {
public:
	ThreadSafeQueue() {}
	~ThreadSafeQueue() {}

	/// Add task to the queue.
	void push(TaskPtr task);

	/// Get task from the queue.
	/// Wait for a task if the queue is empty.
	TaskPtr pop_or_wait();

private:
	std::queue<TaskPtr> tasks;
	std::mutex mutex;
	std::condition_variable cond;
};