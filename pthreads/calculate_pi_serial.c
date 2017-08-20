#include<stdio.h>
#include<stdlib.h>
int main(int argc, char* argv[]){
    if (argc < 2) {printf("Not enough arguments!\n"); exit(0);}
    int n = atoi(argv[1]);
    double factor = 1.0;
    double sum = 0;
    for (double i = 0; i < n; i++, factor = -factor){
        sum += factor * 1 / (2 * i + 1);
    }
    sum *= 4;
    printf("pi = %lf\n", sum);
}
