/**
  * Hello world example with C++11 threads.
 **/

#include <string>
#include <thread>
#include <iostream>
#include <sstream>
#include <vector>

const int NUM_OF_THREADS_DEFAULT = std::thread::hardware_concurrency();

void thread_func(int thread_id) {
    std::ostringstream out;
    out << "Hello, I am thread #" << thread_id << "!\n";
    std::cout << out.str();
}

int main(int argc, char **argv) {
    int num_of_threads = (argc > 1 ? std::stoi(argv[1]) : NUM_OF_THREADS_DEFAULT);

    std::vector<std::thread> threads;

    for (int i = 0; i < num_of_threads; i++) {
        threads.push_back(std::thread(thread_func, i));
    }

    for (auto &thread : threads) {
        thread.join();
    }

    return 0;
}
