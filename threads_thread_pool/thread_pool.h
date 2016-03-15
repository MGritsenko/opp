#pragma once

#include <memory>
#include <thread>
#include <vector>

class ThreadSafeQueue;
class Task;
typedef std::shared_ptr<Task> TaskPtr;

class ThreadPool {
public:
	ThreadPool();
	ThreadPool(int num_of_threads);
	~ThreadPool();	

	void start(int num_of_threads);
	void stop();

	void submit(TaskPtr task);

private:
	void thread_func();

private:
	std::vector<std::thread> threads;
	std::unique_ptr<ThreadSafeQueue> task_queue;
};
