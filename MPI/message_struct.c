#include<stdio.h>
#include<mpi.h>
/*
int MPI_Type_create_struct(
    int count, number of elements
    int array of blocklengths[], length of each element
    MPI_Aint array of displacements[],
    MPI_Datatype array of types[],
    MPI Datatype∗ new_type_p);
*/
int main(){
    double a, b;
    int n, comm_sz, my_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    int array_of_blocklengths[3] = {1, 1, 1};
    MPI_Aint array_of_displacements[3];
    MPI_Aint a_addr, b_addr, n_addr;
    MPI_Get_address(&a, &a_addr);
    array_of_displacements[0] = 0;
    MPI_Get_address(&b, &b_addr);
    array_of_displacements[1] = b_addr - a_addr;
    MPI_Get_address(&n, &n_addr);
    array_of_displacements[2] = n_addr - a_addr;
    MPI_Datatype array_of_types[3] = {MPI_DOUBLE, MPI_DOUBLE, MPI_INT};
    MPI_Datatype input_mpi_t_p;
    MPI_Type_create_struct(3, array_of_blocklengths, array_of_displacements, array_of_types, &input_mpi_t_p);
    MPI_Type_commit(&input_mpi_t_p);
    if (my_rank == 0){
        scanf("%lf, %lf, %d", &a, &b, &n);
    }
    MPI_Bcast(&a, 1, input_mpi_t_p, 0, MPI_COMM_WORLD);
    printf("Process %d, a = %lf, b = %lf, n = %d\n", my_rank, a, b, n);
    MPI_Type_free(&input_mpi_t_p);
    MPI_Finalize();
    return 0;

}
