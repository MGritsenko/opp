/**
  * Hello world example with POSIX threads.
 **/

#include <pthread.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>

const int NUM_OF_THREADS_DEFAULT = 4;

struct ThreadArg {
    int id;
};

void* thread_func(void *arg) {
    int thread_id = static_cast<ThreadArg*>(arg)->id;

    std::ostringstream out;
    out << "Hello, I am thread #" << thread_id << "!\n";
    std::cout << out.str();

    return 0;
}

int main(int argc, char **argv) {    
    int num_of_threads = (argc > 1 ? atoi(argv[1]) : NUM_OF_THREADS_DEFAULT);

    std::vector<pthread_t> threads(num_of_threads);
    std::vector<pthread_attr_t> thread_attrs(num_of_threads);
    std::vector<ThreadArg> thread_args(num_of_threads);

    for (int i = 0; i < num_of_threads; i++) {
        thread_args[i].id = i;
        pthread_attr_init(&thread_attrs[i]);
        pthread_attr_setdetachstate(&thread_attrs[i], PTHREAD_CREATE_JOINABLE);
        pthread_create(&threads[i], &thread_attrs[i], &thread_func, &thread_args[i]);
    }

    for (int i = 0; i < num_of_threads; i++) {
        pthread_join(threads[i], NULL);
        pthread_attr_destroy(&thread_attrs[i]);
    }

    return 0;
}
