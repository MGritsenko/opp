/**
  * Vector sum example with C++11 threads.
 **/

#include <string>
#include <thread>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

#include "../common/timer.h"

const int NUM_OF_THREADS_DEFAULT = std::thread::hardware_concurrency();
const int VECTOR_SIZE_DEFAULT = 10000;

std::vector<int> get_chunk_sizes(int size, int num_of_chunks) {
    std::vector<int> chunk_sizes(num_of_chunks);
    const int chunk_size = size / num_of_chunks;
    const int rest = size % num_of_chunks;
    for (int i = 0; i < num_of_chunks; i++) {
        chunk_sizes[i] = (i < rest ? chunk_size + 1 : chunk_size);
    }
    return chunk_sizes;
}

using Value = int;
using Vector = std::vector<Value>;

void init_vector(Vector &data) {
    Value value = 0;
    for (auto &elem: data) {
        elem = value++;
    }
}

template <class Iterator, class ValueT>
void thread_func(Iterator start, Iterator end, ValueT *sum) {
    ValueT local_sum = 0;
    for (auto it = start; it != end; it++) {
        local_sum += (*it);
    }
    *sum += local_sum; // sum into shared var
}

Value compute_vector_sum(Vector &data, int num_of_threads) {
    std::vector<std::thread> threads;
    std::vector<int> chunk_sizes = get_chunk_sizes(data.size(), num_of_threads);
    Value sum = 0;

    auto start = data.begin();
    for (int i = 0; i < num_of_threads; i++) {
        auto end = std::next(start, chunk_sizes[i]);
        threads.push_back(std::thread(thread_func<Vector::iterator, Value>,
                                      start, end, &sum));
        start = end;
    }

    for (auto &thread : threads) {
        thread.join();
    }

    return sum;
}

int main(int argc, char **argv) {
    int num_of_threads = (argc > 1 ? std::stoi(argv[1]) : NUM_OF_THREADS_DEFAULT);
    int vector_size = (argc > 2 ? std::atoi(argv[2]) : VECTOR_SIZE_DEFAULT);

    Vector data(vector_size);   

    init_vector(data);

    Timer timer;

    Value sum = compute_vector_sum(data, num_of_threads);

    auto time = timer.elapsed();

    std::cout << "Num of threads: " << num_of_threads << ", vector size: " << vector_size << std::endl;
    std::cout << "Wall time: " << std::fixed << std::setprecision(5) << time << " seconds" << std::endl;
    std::cout << "Vector sum: " << sum << std::endl;

    return 0;
}
