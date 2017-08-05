#include<stdio.h>
#include<mpi.h>
#define size 256
//#include"timer.h" Get time of the day (in POSIX lib)
int main(){
    double x[size];
    double A[size * size];
    double y[size];
    double start, end;
    for (int i = 0; i < size; i++){
        x[i] = i;
        for (int k = 0; k < size; k++){
            A[i * size + k] += i * size + k;
        }
    }
    start = MPI_Wtime();
    for (int i = 0; i < size; i++){
        y[i] = 0;
        for (int k = 0; k < size; k++){
            y[i] += A[i * size + k] * x[k];
        }
    }
    end = MPI_Wtime();
    printf("Elapsed time = %lf\n", end - start);
}
