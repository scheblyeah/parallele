#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<omp.h>

void print_array(int32_t *array, int n){
    printf("[");
    for(int i = 0; i < n ; i++){
        if(i+1 != n){
            printf("%d, ", array[i]);
        }
        else{
            printf("%d", array[i]);
        }
    }   
    printf("]\n");
}

void exclusive_prefix(int32_t *a, int32_t *b, int n){
    b[0] = 0;
    for (int i = 1; i < n; i++) {
        b[i] = b[i - 1] + a[i - 1];
    }
}

int main(int argc, char* argv[]){

    if(argc != 2){
        printf("Usage: %s <array size>\n", argv[0]);
        return 0;
    }

    int n = atoi(argv[1]);

    int32_t *a= malloc(sizeof(a) * n);
    int32_t *b = malloc(sizeof(b) * n);
    //int b_array [] = {2, 1, 5, 6, 3};

    for(int i = 0; i < n ; i++){
        a[i] = 1; // b_array[i];
    }

    double start = omp_get_wtime();
    exclusive_prefix(a ,b ,n);
    double end = omp_get_wtime();

    printf("Execution time: %f\n", end-start);

    printf("%d\n", b[n-1]);
    //print_array(b, n);
}