#include <string>
#include <thread>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

#include "../common/timer.h"

const int NUM_OF_THREADS_DEFAULT = std::thread::hardware_concurrency();
const int VECTOR_SIZE_DEFAULT = 10000;

template <class Iterator, class Value>
void thread_func(Iterator start, Iterator end, Value *sum) {
    Value local_sum = 0;
    for (auto it = start; it != end; it++) {
        local_sum += (*it);
    }
    *sum += local_sum; // sum into shared var
}

std::vector<int> get_chunk_sizes(int size, int num_of_chunks) {
    std::vector<int> chunk_sizes(num_of_chunks);
    const int chunk_size = size / num_of_chunks;
    const int rest = size % num_of_chunks;
    for (int i = 0; i < num_of_chunks; i++) {
        chunk_sizes[i] = (i < rest ? chunk_size + 1 : chunk_size);
    }
    return chunk_sizes;
}

int main(int argc, char **argv) {
    int num_of_threads = (argc > 1 ? std::stoi(argv[1]) : NUM_OF_THREADS_DEFAULT);
    int vector_size = (argc > 2 ? std::atoi(argv[2]) : VECTOR_SIZE_DEFAULT);

    using Value = int;
    using Vector = std::vector<Value>;

    Vector data(vector_size);

    for (int i = 0; i < vector_size; i++) {
        data[i] = i;
    }

    std::vector<std::thread> threads;
    std::vector<int> chunk_sizes = get_chunk_sizes(vector_size, num_of_threads);
    Value sum = 0;

    Timer timer;

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

    auto time = timer.elapsed();
    std::cout << "Wall time is " << std::fixed << std::setprecision(5) << time << " seconds" << std::endl;
    std::cout << "Vector sum is " << sum << std::endl;

    return 0;
}
