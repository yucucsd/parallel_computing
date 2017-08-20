#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
long count;
int m, n;
int** A;
int* x;
int* y;

void* mat_vec_mul(void* my_rank){
    long rank = (long) my_rank;
    int start = m /count * rank;
    int end = m /count * (rank + 1);
    for (int i = start; i < end; i++){
        y[i] = 0;
        for (int k = 0; k < n; k++){
            y[i] += A[i][k] * x[k];
        }
    }
    return NULL;
}
int main(int argc, char* argv[]){
    if (argc < 2) {printf("Not Enough arguments\n"); exit(0);}
    count = strtol(argv[1], NULL, 10);
    scanf("%d %d", &m, &n);
    A = malloc(m * sizeof(int*));
    x = malloc(n * sizeof(int));
    y = malloc(n * sizeof(int));
    for (int i = 0; i < m; i++){
        A[i] = malloc(n * sizeof(int));
        for (int k = 0; k < n; k++){
            A[i][k] = i * n + k;
        }
    }
    for (int i = 0; i < n; i++){
        x[i] = i;
    }
    pthread_t* thread;
    thread = malloc (count * sizeof(pthread_t));
    for (long rank = 0; rank < count; rank++){
        pthread_create(&thread[rank], NULL, mat_vec_mul, (void*) rank);
    }
    for (long rank = 0; rank < count; rank++){
        pthread_join(thread[rank], NULL);
    }
    for (int i = 0; i < m; i++){
        printf("%d ", y[i]);
    }
    printf("\n");
}
