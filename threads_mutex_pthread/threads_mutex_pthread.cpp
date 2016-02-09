/**
  * Example of POSIX mutex usage.
 **/

#include <pthread.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

const int NUM_OF_THREADS_DEFAULT = 4;

struct ThreadArg {
    int value;
    int *sum;
    pthread_mutex_t *mutex;

    ThreadArg() {}
    ThreadArg(int val, int *sm, pthread_mutex_t *mt) :
        value(val), sum(sm), mutex(mt) {
    }
};

void* thread_func(void *arg) {
    ThreadArg *t_arg = static_cast<ThreadArg*>(arg);
    pthread_mutex_t *mutex = t_arg->mutex;
    int *sum = t_arg->sum;

    pthread_mutex_lock(mutex);
    *sum += t_arg->value;
    pthread_mutex_unlock(mutex);

    return 0;
}

int compute_sum(int num_of_threads) {
    std::vector<pthread_t> threads(num_of_threads);
    std::vector<pthread_attr_t> thread_attrs(num_of_threads);
    std::vector<ThreadArg> thread_args(num_of_threads);

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    int sum = 0;
    for (int i = 0; i < num_of_threads; i++) {
        thread_args[i] = ThreadArg(i, &sum, &mutex);
        pthread_attr_init(&thread_attrs[i]);
        pthread_attr_setdetachstate(&thread_attrs[i], PTHREAD_CREATE_JOINABLE);
        pthread_create(&threads[i], &thread_attrs[i], &thread_func, &thread_args[i]);
    }

    for (int i = 0; i < num_of_threads; i++) {
        pthread_join(threads[i], NULL);
        pthread_attr_destroy(&thread_attrs[i]);
    }

    pthread_mutex_destroy(&mutex);

    return sum;
}

int main(int argc, char **argv) {    
    int num_of_threads = (argc > 1 ? atoi(argv[1]) : NUM_OF_THREADS_DEFAULT);

    int sum = compute_sum(num_of_threads);

    std::cout << "Num of threads: " << num_of_threads << std::endl;
    std::cout << "Sum: " << sum << std::endl;

    return 0;
}
