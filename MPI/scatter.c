#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
int main(){
    int my_rank;
    int comm_sz;
    int* array_src = NULL;
    int* array_show = NULL;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int array[4];
    if (my_rank == 0){
        int array_src[12];
        for (int i = 0; i < 12; i++){
            array_src[i] = i;
        }
        MPI_Scatter(array_src, 4, MPI_INT, array, 4, MPI_INT, 0, MPI_COMM_WORLD);
        array_show = malloc(12 * sizeof(int));
        MPI_Gather(array, 4, MPI_INT, array_show, 4, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Vector:[");
        for (int i = 0; i < 12; i++){
            printf("%d, ", array_show[i]);
        }
        printf("]\n");
    }else{
        MPI_Scatter(array_src, 4, MPI_INT, array, 4, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Gather(array, 4, MPI_INT, array_show, 4, MPI_INT, 0, MPI_COMM_WORLD);
    }
    int sum = 0;
    int res;
    for (int i = 0; i < 4; i++){
        sum += array[i];
    }
    MPI_Reduce(&sum, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (my_rank == 0){
        printf("res = %d\n", res);
    }

    MPI_Finalize();
    return 0;
}
