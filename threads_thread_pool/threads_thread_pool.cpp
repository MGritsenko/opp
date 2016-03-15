/**
  * Example of C++11 thread pool implementation.
 **/

#include "thread_pool.h"
#include "task.h"
#include <thread>
#include <string>
#include <sstream>
#include <iostream>

const int NUM_OF_THREADS_DEFAULT = std::thread::hardware_concurrency();
const int NUM_OF_VALUES_DEFAULT = 1000;

class MyTask : public Task {
public:
	MyTask(int value) : 
		value(value) {
	}

	void execute() {
		std::ostringstream out;
		out << "My value is " << value << "\n";
		std::cout << out.str();
	}

private:
	int value;
};

void do_work(int num_of_threads, int num_of_values) {
	try {
		// Thread pool will start automatically and stop upon exit.
		ThreadPool thread_pool(num_of_threads);
		for (int i = 0; i < num_of_values; i++) {
			thread_pool.submit(std::make_shared<MyTask>(i));
		}
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

int main(int argc, char **argv) {
    int num_of_threads = (argc > 1 ? std::stoi(argv[1]) : NUM_OF_THREADS_DEFAULT);
    int num_of_values = (argc > 2 ? std::stoi(argv[2]) : NUM_OF_VALUES_DEFAULT);

    do_work(num_of_threads, num_of_values);

    return 0;
}
