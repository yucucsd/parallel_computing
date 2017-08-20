#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
int thread_count;
double sum = 0;
int n = 1000000;
pthread_mutex_t mutex;
int flag = 0;
void* thread_sum(void* rank){
    long my_rank = (long) rank;
    int start = n / thread_count * my_rank;
    int end = n / thread_count * (my_rank + 1);
    double factor = 1;
    if (start % 2 == 1) factor = -1;

    double local_sum = 0;
    for (int i = start; i < end; i++, factor *= -1){
        local_sum += factor / (2 * i + 1);
    }
    //busy-waiting
    //while (flag != my_rank);
    // for (int i = 0; i < 5; i++){
    //     printf("here%d", i);
    //     sleep(1);
    // }
    // printf("\n");
    // sleep(2);
    pthread_mutex_lock(&mutex);
    sum += local_sum;
    // printf("In mutex\n");
    // for (int i = 0; i < 5; i++){
    //     printf("here%d", i);
    //     sleep(1);
    // }
    // printf("\n");
    pthread_mutex_unlock(&mutex);
    //flag++;
    //very long exceution time implementation
    // for (int i = start; i < end; i++, factor *= -1){
    //     while(flag != my_rank);
    //     sum += factor / (2 * i + 1);
    //     flag = (flag + 1) % thread_count;
    // }
    return NULL;
}

int main(int argc, char* argv[]){
    if (argc < 2){
        printf ("Not Enough Arguments\n");
        exit(0);
    }
    long thread;
    pthread_t* thread_handles;
    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);
    for (thread = 0; thread < thread_count; thread++){
        pthread_create(&thread_handles[thread], NULL, thread_sum, (void*) thread);
    }
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);
    pthread_mutex_destroy(&mutex);
    sum *= 4;
    printf("Sum = %lf\n", sum);
    free(thread_handles);
    return 0;
}
