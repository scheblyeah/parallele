#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n 1000

void hadamard_v1(int* a[n], int* b[n], int* c[n]) {
	for(size_t i = 0; i < n; ++i) {
		for(size_t j = 0; j < n; ++j) {
			c[i][j] = a[i][j] * b[i][j];
		}
	}
}

void hadamard_v2(int* a[n], int* b[n], int* c[n]) {
	for(size_t j = 0; j < n; ++j) {
		for(size_t i = 0; i < n; ++i) {
			c[i][j] = a[i][j] * b[i][j];
		}
	}
}

int main(int argc, char* argv[]) {
	if((argc != 2) || (atoi(argv[1]) < 1) || (atoi(argv[1]) > 2)) {
		printf("input error function takes exactly 1 argument\n");
		printf("correct usage: %s < X > \n", argv[0]);
		printf("where X can be 1 or 2 representing the version number of the hadamard "
		       "implementation\n");
		return EXIT_FAILURE;
	}

	srand(time(NULL));
	int32_t* a[n];
	int32_t* b[n];
	int32_t* c[n];

	for(size_t i = 0; i < n; i++) {
		a[i] = (int32_t*)malloc(n * sizeof(int32_t));
		b[i] = (int32_t*)malloc(n * sizeof(int32_t));
		c[i] = (int32_t*)malloc(n * sizeof(int32_t));
		for(size_t j = 0; j < n; j++) {
			a[i][j] = (rand() % 10) + 1;
			b[i][j] = (rand() % 10) + 1;
		}
	}

	if(atoi(argv[1]) == 1) {
		hadamard_v1(a, b, c);
	} else {
		hadamard_v2(a, b, c);
	}

	for(size_t i = 0; i < n; i++) {
		free(a[i]);
		free(b[i]);
		free(c[i]);
	}

	return EXIT_SUCCESS;
}

/**
 * valgrind --tool=cachegrind ./ex2 1
 * valgrind --tool=cachegrind ./ex2 2
 * perf stat -e instructions,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores ./ex2 1
 * perf stat -e instructions,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores ./ex2 2
 *
 */
