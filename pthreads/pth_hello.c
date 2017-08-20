#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
//compile with gcc pth_hello.c -o pth_hello.out -lpthread
//execute with ./pth_hello.out <number of threads>

int thread_count;

void* Hello(void* rank){
    long my_rank = (long) rank;
    printf("Hello from thread %ld of %d\n", my_rank, thread_count);
    return NULL;
}

int main(int argc, char* argv[]){
    long thread;
    pthread_t* thread_handles;

    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    for (thread = 0; thread < thread_count; thread++){
        pthread_create(&thread_handles[thread], NULL, Hello, (void*) thread);
        //int pthread_create(pthread_t *thread, const pthread_attr_t *attr (NULL for default),
        //                  void *(*start_routine) (void *), void *arg);
    }
    printf("Hello world from main thread\n");
    for (thread = 0; thread < thread_count; thread++){
        pthread_join(thread_handles[thread], NULL);
        //int pthread join(pthread_t thread, void∗∗ ret_val_p);
        //The second argument can be used to receive any return value computed by the thread.
    }
    free(thread_handles);
    return 0;
}
