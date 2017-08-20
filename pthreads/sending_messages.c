#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<semaphore.h>
#define MSG_MAX 100
//producer-consumer synchronization
/*
difference between mutex and semaphore:
binary semaphore and mutex mainly differ in concept:
A mutex is locking mechanism used to synchronize access to a resource.
Only the owner can release the lock (mutex).
Semaphore is signaling mechanism, signals the call processing task to wakeup
*/
/*
struct semaphore {
       spinlock_t      lock;
       unsigned int        count;// the value of semaphore must be an unsigned int
       struct list_head    wait_list;
   };
sem_wait(sem_t* sem) decrements (locks) the semaphore pointed to by sem.  If
       the semaphore's value is greater than zero, then the decrement
       proceeds, and the function returns, immediately.  If the semaphore
       currently has the value zero, then the call blocks until either it
       becomes possible to perform the decrement (i.e., the semaphore value
       rises above zero), or a signal handler interrupts the call.
sem_post(sem_t* sem) increase the value by 1, if greater than 0,
       awake the thread blocked by this semaphore
*/
int thread_count;
char** message;
sem_t* semaphores;
void* send_msg(void* rank){
    long my_rank = (long) rank;
    long dest = ((long) my_rank + 1) % thread_count;
    long source = (my_rank + thread_count - 1) % thread_count;
    char* my_msg = malloc(MSG_MAX*sizeof(char));

    message[dest] = my_msg;
    sprintf(my_msg, "Hello to %ld from %ld", dest, my_rank);
    //while (message[my_rank] == NULL);

    sem_post(&semaphores[dest]); //unlock the semaphore of dest
    sem_wait(&semaphores[my_rank]); // wait for its own semaphore
    printf("Thread %ld > %s\n", my_rank, message[my_rank]);
    // if (message[my_rank] != NULL)
    //     printf("Thread %ld > %s\n", my_rank, message[my_rank]);
    // else
    //     printf("Thread %ld > No message from %ld\n", my_rank, source);
    return NULL;
}
int main(int argc, char* argv[]){
    if (argc < 2){
        printf("Not enough arguments!\n");
        exit(0);
    }
    long thread;
    pthread_t* thread_handles;
    thread_count = strtol(argv[1], NULL, 10);
    message = malloc(thread_count * sizeof(char*));
    semaphores = malloc(thread_count * sizeof(sem_t));
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    for (thread = 0; thread < thread_count; thread++){
        int ret = sem_init(&semaphores[thread], 0, 0);
        // if initialize to negative value, error will return
    }
    for (thread = 0; thread < thread_count; thread++){
        pthread_create(&thread_handles[thread], NULL, send_msg, (void*) thread);
    }

    for (thread = 0; thread < thread_count; thread++){
        pthread_join(thread_handles[thread], NULL);
    }
    for (thread = 0; thread < thread_count; thread++){
        sem_destroy(&semaphores[thread]);
    }
    free(thread_handles);
    free(message);
    return 0;
}
