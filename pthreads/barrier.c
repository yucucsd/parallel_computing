#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
/*
mutex cannot reuse counter
semaphore cannot reuse barrier
condition variable risky if the counter condition is tested after broadcast, race condition
*/

long int thread_count;
int counter;
pthread_mutex_t mutex;
sem_t count_sem;
sem_t barrier_sem;
pthread_cond_t cond_var;
void* thread_work(void* rank){
    long my_rank = (long) rank;
    /*
    pthread_mutex_lock(&mutex);
    counter++;
    pthread_mutex_unlock(&mutex);
    while(counter < thread_count);
    */
    /*
    sem_wait(&count_sem);
    if(counter == thread_count - 1){
        counter = 0;
        sem_post(&count_sem);
        for (int i = 0; i < thread_count - 1; i++)
            sem_post(&barrier_sem);
    }else{
        counter++;
        sem_post(&count_sem);
        sem_wait(&barrier_sem);
    }
    */
    pthread_mutex_lock(&mutex);
    counter++;
    if (counter == thread_count){
        counter = 0;
        pthread_cond_broadcast(&cond_var);
    }else{
        while(pthread_cond_wait(&cond_var, &mutex) != 0);
    }
    pthread_mutex_unlock(&mutex);
    printf("Hello world from %ld\n", my_rank);
    sleep(1);
    return NULL;
}
int main(int argc, char* argv[]){
    long thread;
    if (argc < 2){
        printf("Not enough arguments\n");
    }
    pthread_t* thread_handles;
    thread_count = strtol(argv[1], NULL, 10);
    pthread_mutex_init(&mutex, NULL);
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    counter = 0;
    //sem_init(&count_sem, 0, 1);
    //sem_init(&barrier_sem, 0, 0);
    pthread_cond_init(&cond_var, NULL);
    for (thread = 0; thread < thread_count; thread++){
        pthread_create(&thread_handles[thread], NULL, thread_work, (void*) thread);
    }
    for (thread = 0; thread < thread_count; thread++){
        pthread_join(thread_handles[thread], NULL);
    }
    //pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);
    return 0;
}
