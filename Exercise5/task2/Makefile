.PHONY: all
all: heat_stencil_2D_serial heat_stencil_2D_parallel

.PHONY: clean
clean:
	$(RM) heat_stencil_2D_serial heat_stencil_2D_parallel

heat_stencil_2D_serial: heat_stencil_2D_serial.c
	gcc -o heat_stencil_2D_serial -std=gnu11 -O2 heat_stencil_2D_serial.c -fopenmp

heat_stencil_2D_parallel: heat_stencil_2D_parallel.c
	gcc -o heat_stencil_2D_parallel -std=gnu11 -O2 heat_stencil_2D_parallel.c -fopenmp

