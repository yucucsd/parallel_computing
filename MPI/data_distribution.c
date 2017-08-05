#include<stdio.h>
#include<mpi.h>

int main(){
    int comm_sz = 3;
    int my_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int sum = 0;
    int res;

    if (my_rank == 0){
        int array_src[12];
        for(int i = 0; i < 12; i++){
            array_src[i] = i;
        }
        // block sum
        //MPI_Scatter(array_src, 4, MPI_INT, array, 4, MPI_INT, 0, MPI_COMM_WORLD);

        for (int i = 1; i < comm_sz; i++){
            MPI_Send(&array_src[i * 4], 4, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        for (int i = 0; i < 4; i++){
            sum += array_src[i];
        }
        // cyclic sum
        /*
        for (int i = 1; i < comm_sz; i++){
            int array_trans[4];
            for (int k = 0; k < 4; k++){
                array_trans[k] = array_src[i + k * 3];
            }
            MPI_Send(&array_trans, 4, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        */
        // block-cyclic partition
        /*
        int block_size = 2;
        int n = 12 / block_size;
        for (int i = 1; i < comm_sz; i++){
            int array_trans[4];
            for (int k = 0; k < n / comm_sz; k++){
                for (int j = 0; j < block_size; j++){
                    array_trans[k * block_size + j] = array_src[k * comm_sz * block_size + i * block_size + j];
                }
            }
            MPI_Send(&array_trans, 4, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        for (int k = 0; k < n / comm_sz; k++){
            for (int j = 0; j < block_size; j++){
                sum += array[k * comm_sz * block_size + j];
            }
        }
        */
    }else{
        int array[4];
        MPI_Recv(&array, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < 4; i++){
            sum += array[i];
        }
    }

    MPI_Reduce(&sum, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0){
        printf("the sum in %d is %d\n", my_rank, res);
    }

    MPI_Finalize();
    return 0;
}
