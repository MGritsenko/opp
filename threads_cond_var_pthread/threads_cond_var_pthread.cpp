/**
  * Example of POSIX threads mutex and conditional variable usage.
 **/

#include <pthread.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <iomanip>
#include <cstdlib>

const int NUM_OF_THREADS_DEFAULT = 4;
const int NUM_OF_VALUES_DEFAULT = 1000;
const int SHUTDOWN = -1;

struct ThreadArg {
    int id;
    std::queue<int> *queue;
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;

    ThreadArg() {}
    ThreadArg(int id, std::queue<int> *q, pthread_mutex_t *mt, pthread_cond_t *cnd) :
        id(id), queue(q), mutex(mt), cond(cnd) {
    }
};

void* thread_func(void *arg) {
    ThreadArg *t_arg = static_cast<ThreadArg*>(arg);
    int id = t_arg->id;
    std::queue<int> *queue = t_arg->queue;
    pthread_mutex_t *mutex = t_arg->mutex;
    pthread_cond_t *cond = t_arg->cond;

    int value = 0;
    while (value != SHUTDOWN) {
        pthread_mutex_lock(mutex);
        while (queue->empty()) {
            pthread_cond_wait(cond, mutex);
        }
        value = queue->front();
        queue->pop();
        pthread_mutex_unlock(mutex);

        std::ostringstream out;
        out << "Thread " << id << ": value is " << value << "\n";
        std::cout << out.str();
    }
    return 0;
}

void do_work(int num_of_threads, int num_of_values) {
    std::vector<pthread_t> threads(num_of_threads);
    std::vector<pthread_attr_t> thread_attrs(num_of_threads);
    std::vector<ThreadArg> thread_args(num_of_threads);
    std::queue<int> queue;

    pthread_mutex_t mutex;
    pthread_cond_t cond;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    for (int i = 0; i < num_of_threads; i++) {
        thread_args[i] = ThreadArg(i, &queue, &mutex, &cond);
        pthread_attr_init(&thread_attrs[i]);
        pthread_attr_setdetachstate(&thread_attrs[i], PTHREAD_CREATE_JOINABLE);
        pthread_create(&threads[i], &thread_attrs[i], &thread_func, &thread_args[i]);
    }

    for (int i = 0; i < num_of_values; i++) {
        pthread_mutex_lock(&mutex);
        queue.push(i);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    for (int i = 0; i < num_of_threads; i++) {
        pthread_mutex_lock(&mutex);
        queue.push(SHUTDOWN);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    for (int i = 0; i < num_of_threads; i++) {
        pthread_join(threads[i], NULL);
        pthread_attr_destroy(&thread_attrs[i]);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

int main(int argc, char **argv) {
    int num_of_threads = (argc > 1 ? atoi(argv[1]) : NUM_OF_THREADS_DEFAULT);
    int num_of_values = (argc > 2 ? atoi(argv[2]) : NUM_OF_VALUES_DEFAULT);

    do_work(num_of_threads, num_of_values);

    return 0;
}
