#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
int main(){
    int comm_sz;
    int my_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    //an output of a for loop may be the input to another, so assume
    int x[4];
    for (int i = 0; i < 4; i++){
        x[i] = i;
    }
    int* A = NULL;
    int row[4];
    int y[4];
    if (my_rank == 0){
        A = malloc(16 * sizeof(int));
        for (int i = 0; i < 16; i++){
            A[i] = i;
        }
    }
    MPI_Scatter(A, 4, MPI_INT, row, 4, MPI_INT, 0, MPI_COMM_WORLD);
    int y_p = 0;
    for (int i = 0; i < 4; i++){
        y_p += x[i] * row[i];
    }
    MPI_Allgather(&y_p, 1, MPI_INT, y, 1, MPI_INT, MPI_COMM_WORLD);
    for (int i = 0; i < 4; i++){
        printf("Process %d: y[%d] = %d\n", my_rank, i, y[i]);
    }
    MPI_Finalize();
    return 0;
}
