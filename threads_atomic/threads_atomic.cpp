/**
  * Example of C++11 atomic variables usage.
 **/

#include <thread>
#include <atomic>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <functional>

const int NUM_OF_THREADS_DEFAULT = std::thread::hardware_concurrency();

template <class ValueT>
void thread_func(const ValueT &val, std::atomic<ValueT> &sum) {
    sum += val; // sum into shared atomic var
}

int compute_sum(int num_of_threads) {
    std::vector<std::thread> threads;    
    std::atomic<int> sum(0);

    for (int i = 0; i < num_of_threads; i++) {
        threads.push_back(std::thread(thread_func<int>, i, std::ref(sum)));
    }

    for (auto &thread : threads) {
        thread.join();
    }

    return sum.load();
}

int main(int argc, char **argv) {
    int num_of_threads = (argc > 1 ? std::stoi(argv[1]) : NUM_OF_THREADS_DEFAULT);

    int sum = compute_sum(num_of_threads);

    std::cout << "Num of threads: " << num_of_threads << std::endl;
    std::cout << "Sum: " << sum << std::endl;

    return 0;
}
