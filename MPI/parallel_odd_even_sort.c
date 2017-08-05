#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int cmpfunc(const void* a, const void* b){
    return (*(int*)a - *(int*)b);
}
int get_partner(int phase, int rank, int comm_sz){
    int partner;
    if(phase % 2 == 0){
        if (rank % 2 != 0) partner = rank - 1;
        else partner = rank + 1;
    }else{
        if (rank % 2 != 0) partner = rank + 1;
        else partner = rank - 1;
    }
    if (partner == -1 || partner == comm_sz){
        partner = MPI_PROC_NULL;
    }
    return partner;
}
int main(){
    int comm_sz, my_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int array[8];
    for(int i = 0; i < 8; i++){
        array[i] = 32 - (my_rank * 8 + i);
    }
    qsort(array, 8, sizeof(int), cmpfunc);
    for (int phase = 0; phase < comm_sz; phase++){
        int partner = get_partner(phase, my_rank, comm_sz);
        int recv_array[8];
        int res_array[8];
        MPI_Sendrecv(array, 8, MPI_INT, partner, 0, recv_array, 8, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (partner == MPI_PROC_NULL) continue;
        if (partner > my_rank){
            int i_1 = 0;
            int i_2 = 0;
            for (int i = 0; i < 8; i++){
                if (array[i_1] <= recv_array[i_2]){
                    res_array[i] = array[i_1];
                    i_1++;
                }else{
                    res_array[i] = recv_array[i_2];
                    i_2++;
                }
            }
        }else{
            int i_1 = 7;
            int i_2 = 7;
            for (int i = 7; i > -1; i--){
                if (array[i_1] >= recv_array[i_2]){
                    res_array[i] = array[i_1];
                    i_1--;
                }else{
                    res_array[i] = recv_array[i_2];
                    i_2--;
                }
            }
        }
        for (int i = 0; i < 8; i++){
            array[i] = res_array[i];
        }
    }
    for (int i = 0; i < 8; i++){
        printf("Process %d, array[%d] = %d\n", my_rank, i, array[i]);
    }

    MPI_Finalize();
    return 0;
}
