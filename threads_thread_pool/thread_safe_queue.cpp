#include "thread_safe_queue.h"

void ThreadSafeQueue::push(TaskPtr task) {
	std::lock_guard<std::mutex> lock(mutex);
	tasks.push(task);
	cond.notify_one();
}

TaskPtr ThreadSafeQueue::pop_or_wait() {
	std::unique_lock<std::mutex> lock(mutex);
	cond.wait(lock, [this] {return !tasks.empty();});
	TaskPtr task = tasks.front();
	tasks.pop();
	return task;
}