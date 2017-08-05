#include<stdio.h>
#include<string.h>
#include<mpi.h>
// compile with mpicc -g -Wall -o mpi_hello mpi_hello.c
//execute with mpiexec -n <num of processes> ./mpi_hello
const int MAX_STRING = 100;

int main(){
    char greeting[MAX_STRING];
    int comm_sz; /* Number of processes */
    int my_rank; /* my process rank */

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if(my_rank != 0){
        sprintf(greeting, "Greetings from process %d of %d!", my_rank, comm_sz);
        MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }else{
        printf("Greetings from process %d of %d!\n", my_rank, comm_sz);
        for (int q = 1; q < comm_sz; q++){
            //MPI_Recv(greeting, MAX_STRING, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Status status;
            MPI_Recv(greeting, MAX_STRING, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            int count;
            MPI_Get_count(&status, MPI_CHAR, &count);
            printf("%d, %d, %d\n", status.MPI_SOURCE, status.MPI_TAG, count);
            printf("%s\n", greeting);
        }
    }
    MPI_Finalize();
    return 0;
}
