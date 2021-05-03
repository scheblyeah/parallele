#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <omp.h>
#include <string.h>

#define RESOLUTION_WIDTH 50
#define RESOLUTION_HEIGHT 50

#define PERROR fprintf(stderr, "%s:%d: error: %s\n", __FILE__, __LINE__, strerror(errno))
#define PERROR_GOTO(label) \
	do { \
		PERROR; \
		goto label; \
	} while (0)


// -- vector utilities --

#define IND(y, x) ((y) * (N) + (x))

void printTemperature(double *m, int N, int M);

// -- simulation code ---

int main(int argc, char **argv) {
    // 'parsing' optional input parameter = problem size
    int N = 200;
    if (argc > 1) {
        N = atoi(argv[1]);
    }
    int T = N * 10;
    printf("Computing heat-distribution for room size %dX%d for T=%d timesteps\n", N, N, T);

    // ---------- setup ----------

    // create a buffer for storing temperature fields
    double *A =  malloc(sizeof(double) * N * N);

    if(!A) PERROR_GOTO(error_a);

    double start = omp_get_wtime();

    // set up initial conditions in A
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[IND(i,j)] = 273; // temperature is 0° C everywhere (273 K)
        }
    }

    // and there is a heat source
    int source_x = N / 2;
    int source_y = N / 2;
    A[IND(source_x,source_y)] = 273 + 60;

    printf("Initial:");
    printTemperature(A, N, N);
    printf("\n");

    // ---------- compute ----------

    // create a second buffer for the computation
    double *B = malloc(sizeof(double) * N * N);
    if(!B) PERROR_GOTO(error_b);
    // for each time step ..
    for (int t = 0; t < T; t++) {

        #pragma omp parallel for collapse(2) //using collapse clause for nested loops
        for (int x = 0; x < N; x++)
        {
            for (int y = 0; y < N; y++)
            {   
                //skip heatsource
                if (x == source_x && y == source_y)
                    continue;

                // left, top, right, bottom neighbor
                int neighbor [4][2] = {{x-1, y}, {x, y-1},{x+1, y},{x, y+1} };
                
                //setting neighbors outside the boundary
                if (x == 0){
                    neighbor[0][0] = x;
                }
                if (x == N-1){
                    neighbor[2][0] = x;
                }
                if (y == 0){
                    neighbor[1][1] = y;
                }
                if (y == N-1){
                    neighbor[3][1] = y;
                }

                //heat of pixel = average heat of neighbors
                double neighbor_sum = 0;
                for (int i = 0; i < 4; i++)
                {
                    neighbor_sum += A[IND(neighbor[i][0], neighbor[i][1])];
                }
                A[IND(x,y)] = neighbor_sum/4;
            }
        }

        // every 1000 steps show intermediate step
        if (!(t % 1000)) {
            printf("Step t=%d\n", t);
            printTemperature(A, N, N);
            printf("\n");
        }
    }

     double end = omp_get_wtime();

    // ---------- check ----------

    printf("Final:");
    printTemperature(A, N, N);
    printf("\n");

    // simple verification if nowhere the heat is more then the heat source
    int success = 1;
    for (long long i = 0; i < N; i++) {
        for (long long j = 0; j < N; j++) {
            double temp = A[IND(i,j)];
            if (273 <= temp && temp <= 273 + 60)
                continue;
            success = 0;
            break;
        }
    }

    printf("Verification: %s\n", (success) ? "OK" : "FAILED");

    printf("\n N = %d - wall time: %f\n",N, end-start);

    // todo ---------- cleanup ----------
    error_b:
    free(B);
    error_a:
    free(A);
    return (success) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void printTemperature(double *m, int N, int M) {
    const char *colors = " .-:=+*^X#%@";
    const int numColors = 12;

    // boundaries for temperature (for simplicity hard-coded)
    const double max = 273 + 30;
    const double min = 273 + 0;

    // set the 'render' resolution
    int W = RESOLUTION_WIDTH;
    int H = RESOLUTION_HEIGHT;

    // step size in each dimension
    int sW = N / W;
    int sH = M / H;

    // upper wall
    printf("\t");
    for (int u = 0; u < W + 2; u++) {
        printf("X");
    }
    printf("\n");
    // room
    for (int i = 0; i < H; i++) {
        // left wall
        printf("\tX");
        // actual room
        for (int j = 0; j < W; j++) {
            // get max temperature in this tile
            double max_t = 0;
            for (int x = sH * i; x < sH * i + sH; x++) {
                for (int y = sW * j; y < sW * j + sW; y++) {
                    max_t = (max_t < m[IND(x,y)]) ? m[IND(x,y)] : max_t;
                }
            }
            double temp = max_t;

            // pick the 'color'
            int c = ((temp - min) / (max - min)) * numColors;
            c = (c >= numColors) ? numColors - 1 : ((c < 0) ? 0 : c);

            // print the average temperature
            printf("%c", colors[c]);
        }
        // right wall
        printf("X\n");
    }
    // lower wall
    printf("\t");
    for (int l = 0; l < W + 2; l++) {
        printf("X");
    }
    printf("\n");
}