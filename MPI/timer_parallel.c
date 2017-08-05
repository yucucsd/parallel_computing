#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#define size 256
#define thread 2
int main(){
    int comm_sz, my_rank;
    double local_start, local_end, local_time, time_req;
    double x[size];
    double A[size * size];
    double y[size];
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    if (my_rank == 0){
        for (int i = 0; i < size; i++) x[i] = i;
        for (int i = 0; i < size; i++){
            for (int k = 0; k < size; k++){
                A[i * size + k] = i * size + k;
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    local_start = MPI_Wtime();
    MPI_Bcast(x, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double row[size * size / thread];
    MPI_Scatter(A, size * size / thread, MPI_DOUBLE, row, size * size / thread, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double y_p[size / thread];
    for (int i = 0; i < size / thread; i++){
        y_p[i] = 0;
        for (int k = 0; k < size; k++){
            y_p[i] += row[i * size + k] * x[k];
        }
    }
    MPI_Gather(&y_p, size / thread, MPI_DOUBLE, y, size / thread, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    local_end = MPI_Wtime();
    local_time = local_end - local_start;
    MPI_Reduce(&local_time, &time_req, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (my_rank == 0){
        printf("time required is %lf\n", time_req);
    }
    MPI_Finalize();
    return 0;
}
