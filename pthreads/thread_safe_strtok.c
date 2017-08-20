#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#define MAX 128

FILE* fp;
long thread_count;
sem_t* semaphores;

void* Tokenize(void* rank){
    long my_rank = (long) rank;
    int next = (my_rank + 1) % thread_count;
    char my_line[MAX];
    char* fg_rv;
    char* my_string;
    char* saveptr;
    sem_wait(&semaphores[my_rank]);
    fg_rv = fgets(my_line, MAX, fp);// read in a line
    sem_post(&semaphores[next]);
    while(fg_rv != NULL){
        printf("Thread %ld > my line = %s\n", my_rank, my_line);

        my_string = strtok_r(my_line, " \t\n", &saveptr);// strtok is not thread safe
        while(my_string != NULL){
            printf("Thread %ld > %s\n", my_rank, my_string);
            my_string = strtok_r(NULL, " \t\n", &saveptr);
        }
        sem_wait(&semaphores[my_rank]);
        fg_rv = fgets(my_line, MAX, fp);
        sem_post(&semaphores[next]);
    }

    return NULL;

}
int main(int argc, char* argv[]){
    long thread;
    pthread_t* thread_handles;
    fp = fopen("../README.md", "r");
    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    semaphores = malloc(thread_count * sizeof(sem_t));
    sem_init(&semaphores[0], 0, 1);
    for (thread = 1; thread < thread_count; thread++)
        sem_init(&semaphores[thread], 0, 0);

    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, Tokenize, (void*) thread);

    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    for (thread = 0; thread < thread_count; thread++)
        sem_destroy(&semaphores[thread]);
    return 0;
}
