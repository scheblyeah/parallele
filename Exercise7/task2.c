#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define repetitions 1000000

int main(int argc, char const* argv[]) {
	if(argc != 2) {
		printf("this program takes exactly one argument!\n");
		printf("try running: %s <N>\nwhere N is the size of the vectors\n", argv[0]);
		return EXIT_FAILURE;
	}

	double start_time, end_time;

	int size;
	size = atoi(argv[1]);


	float a[size];
	float b[size];
	float c[size];

    for(int i = 0; i < size; ++i) {
        a[i] = 3;
        b[i] = 3;
        c[i] = 3;
    }

	start_time = omp_get_wtime();


    #pragma omp simd
	for(int run = 0; run < repetitions; ++run) {
		for(int i = 0; i < size; ++i) {
			a[i] += b[i] * c[i];
		}
	}

	end_time = omp_get_wtime();

/*     for(int i = 0; i < size; i++) {
        printf("%f ", a[i]);
    } */
    printf("[ %f ]", a[0]);

    printf("\n");
	printf("execution time  %f seconds\n", end_time - start_time);
	return 0;
}