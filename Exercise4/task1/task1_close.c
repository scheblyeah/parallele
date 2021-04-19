#include<stdlib.h>
#include<stdio.h>
#include <omp.h>
/**export OMP_NUM_THREADS=4
 * Run with OMP_PLACES=threads | cores | sockets 
 * and with affinity settings: #pragma omp parallel proc_bind(master | close | spread)
 * */

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Usage: %s <int number of incrementations>\n", argv[0]);
        return 0;
    }

    int number = atoi(argv[1]);
    int increment = 0;
    double start = omp_get_wtime();

    #pragma omp parallel proc_bind(close)
    for(int i= 0; i <number; i++){
        #pragma omp atomic
        increment++;
    }

    double end = omp_get_wtime();
    printf("Result: %d\nTime: %fs\n", increment, end-start);


}