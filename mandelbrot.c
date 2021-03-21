#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Include that allows to print result as an image
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Default size of image
#define X 1280
#define Y 720
#define MAX_ITER 10000

void calc_mandelbrot(uint8_t image[Y][X]) {
	for(int x_pixel_idx = 0; x_pixel_idx < X; x_pixel_idx++) {

        for(int y_pixel_idx = 0; y_pixel_idx < Y; y_pixel_idx++) {
            float x = 0;
            float y = 0;
            float cx = -2.5 + x_pixel_idx / (X - 1.0) * (3.5); // diff of -2.5, 1
            float cy = -1 + y_pixel_idx / (Y - 1.0) * (2);     // diff of -1, 1

            int iteration = 0;
            while(((x * x + y * y) <= (2 * 2)) && (iteration < MAX_ITER)) {
                float x_tmp = x * x - y * y + cx;
                y = 2 * x * y + cy;
                x = x_tmp;
                iteration = iteration + 1;
            }
            int norm_iteration = (iteration < 255) ? iteration : 255;
            image[y_pixel_idx][x_pixel_idx] = norm_iteration;
        }
    }
}

int main() {
	uint8_t image[Y][X];

	calc_mandelbrot(image);

	const int channel_nr = 1, stride_bytes = 0;
	stbi_write_png("mandelbrot.png", X, Y, channel_nr, image, stride_bytes);
	return EXIT_SUCCESS;
}