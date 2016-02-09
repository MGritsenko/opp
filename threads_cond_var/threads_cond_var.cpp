/**
  * Example of C++11 mutex and conditional variable usage.
 **/

#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <iomanip>
#include <functional>

const int NUM_OF_THREADS_DEFAULT = std::thread::hardware_concurrency();
const int NUM_OF_VALUES_DEFAULT = 1000;
const int SHUTDOWN = -1;

void thread_func(int id, std::queue<int> &queue, std::mutex &mutex, std::condition_variable &cond) {
    int value = 0;
    while (value != SHUTDOWN) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            cond.wait(lock, [&queue] {return !queue.empty();}); // wait until queue is not empty
            value = queue.front();
            queue.pop();
        }
        std::ostringstream out;
        out << "Thread " << id << ": value is " << value << "\n";
        std::cout << out.str();
    }
}

void do_work(int num_of_threads, int num_of_values) {
    std::vector<std::thread> threads;
    std::mutex mutex;
    std::condition_variable cond;
    std::queue<int> queue;

    for (int i = 0; i < num_of_threads; i++) {
        threads.push_back(std::thread(thread_func,
                                      i, std::ref(queue), std::ref(mutex), std::ref(cond)));
    }

    for (int i = 0; i < num_of_values; i++) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(i);
        cond.notify_one();
    }

    for (int i = 0; i < num_of_threads; i++) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(SHUTDOWN);
        cond.notify_one();
    }

    for (auto &thread : threads) {
        thread.join();
    }    
}

int main(int argc, char **argv) {
    int num_of_threads = (argc > 1 ? std::stoi(argv[1]) : NUM_OF_THREADS_DEFAULT);
    int num_of_values = (argc > 2 ? std::stoi(argv[2]) : NUM_OF_VALUES_DEFAULT);

    do_work(num_of_threads, num_of_values);

    return 0;
}
