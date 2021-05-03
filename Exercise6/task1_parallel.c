#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<omp.h>

void print_array(int32_t *array, int n){
    printf("[");
    #pragma omp for
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
    //printf("Threads: %d\n", threads);
    int *lastIndexPerThread;
    int threads = -1;

    #pragma omp parallel
    {   
        int threads = omp_get_num_threads();
        lastIndexPerThread = malloc(sizeof(*lastIndexPerThread) * threads-1);  //[threads];
        b[0] = 0;
        #pragma omp for schedule(static)
        for (int i = 1; i < n; i++) {
            b[i] = b[i - 1] + a[i - 1];
            lastIndexPerThread[omp_get_thread_num()] = i;
        }
        #pragma omp barrier 
        //print_array(lastIndexPerThread, threads);
    }
     for(int i = 0; i < threads; i++){
            printf("Last index of thread: %d ",lastIndexPerThread[i]);
        }
      printf("\n");
}


void prefixSum(int32_t* a, int32_t* b, int n) {

    b[0] = 0;
    int threads;
    int *sumAll;
    #pragma omp parallel
    {
        #pragma omp single
        {
            threads = omp_get_num_threads();
            sumAll = malloc(sizeof(*sumAll) * (threads+1));
            sumAll[0] = 0;
        }

        int thread = omp_get_thread_num();
        printf("Thread number of me is: %d\n", thread);

        int lastValue = 0;
        #pragma omp for
        for (int i = 0; i < n; i++) 
        {
            lastValue = b[i - 1] + a[i - 1];
            b[i] = lastValue;
        }
        sumAll[thread+1] = lastValue;

        #pragma omp barrier // Warte auf Threads
        for(int i = 0; i < threads+1; i++){
            printf("%d, ", sumAll[i]);
        }
        printf("\n");

        int off = 0;
        for(int i = 0; i < thread+1; i++) 
            off += sumAll[i];

        #pragma omp for
        for (int i = 0; i < n; i++) 
            b[i] += off;
    }
    printf("With %d threads:\n", threads);
    free(sumAll);
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


    //exclusive_prefix(a ,b ,n); 
    prefixSum(a, b, n);
  
    double end = omp_get_wtime();

    printf("Execution time: %f\n", end-start);

    printf("%d\n", b[n-1]);
    //print_array(b, n);
}