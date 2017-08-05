#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<unistd.h>
double cal_area(double a, double b, double h){
    double h_a = a * a;
    double h_b = b * b;
    double area = (h_a + h_b) / 2 * h;
    return area;
}

int main(int argc, char* argv[]){
    int comm_sz = atoi(argv[1]);
    int max_size = 100;
    double a = atoi(argv[2]), b = atoi(argv[3]);
    double h = (b - a) / comm_sz;
    int rank;
    sleep(2);
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double local_a = rank * h;
    double local_b = local_a + h;
    double area = cal_area(local_a, local_b, h);
    double areaq;

    MPI_Reduce(&area, &areaq, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    //MPI_Allreduce(&area, &areaq, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    MPI_Bcast(&areaq, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    /*

    if (rank != 0){
        MPI_Send(&area, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }else{
        for(int q = 1; q < comm_sz; q++){
            MPI_Recv(&areaq, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            area += areaq;
        }
    }
    */
    printf ("the area in %d is %f\n", rank, areaq);
    /*
    if (rank == 0){
        printf("the area is %f\n", areaq);
    }
    */
    MPI_Finalize();
    return 0;

}

/*
int main(){
    int comm_sz = 4;
    int my_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    double input[3];
    if (my_rank == 0){
        scanf("%lf %lf %lf", &input[0], &input[1], &input[2]);
        for (int q = 1; q < comm_sz; q++){
            MPI_Send(input, 3, MPI_DOUBLE, q, 0, MPI_COMM_WORLD);
        }
        double a = input[0], h = (input[0] + input[1]) / input[2], b = a + h;
        double area = cal_area(a, b, h);
        for (int q = 1; q < comm_sz; q++){
            double areaq;
            MPI_Recv(&areaq, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            area += areaq;
        }
        printf("area = %f\n", area);
    }else{
        MPI_Recv(&input, 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        double h = (input[0] + input[1]) / input[2], a = input[0] + my_rank * h, b = a + h;
        double area = cal_area(a, b, h);
        MPI_Send(&area, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;

}
*/
