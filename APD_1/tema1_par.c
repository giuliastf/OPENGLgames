// Author: APD team, except where source was noted

#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// #include "pthread_barrier_mac.h"
#include <pthread.h>

#define CONTOUR_CONFIG_COUNT    16
#define FILENAME_MAX_SIZE       50
#define STEP                    8
#define SIGMA                   200
#define RESCALE_X               2048
#define RESCALE_Y               2048

#define CLAMP(v, min, max) if(v < min) { v = min; } else if(v > max) { v = max; }

typedef struct Thread{
    unsigned int id;
    ppm_image *image;
    ppm_image *scaled_image;
    unsigned char **grid;
    ppm_image **contour_map;
    int step_x;
    int step_y;
    int P;
    pthread_barrier_t *barrier;
} Thread;

int min(int a, int b) {
    return a < b ? a : b;
}

// Creates a map between the binary configuration (e.g. 0110_2) and the corresponding pixels
// that need to be set on the output image. An array is used for this map since the keys are
// binary numbers in 0-15. Contour images are located in the './contours' directory.
ppm_image **init_contour_map() {
    ppm_image **map = (ppm_image **)malloc(CONTOUR_CONFIG_COUNT * sizeof(ppm_image *));
    if (!map) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }

    for (int i = 0; i < CONTOUR_CONFIG_COUNT; i++) {
        char filename[FILENAME_MAX_SIZE];
        sprintf(filename, "./contours/%d.ppm", i);
        map[i] = read_ppm(filename);
    }

    return map;
}

// Updates a particular section of an image with the corresponding contour pixels.
// Used to create the complete contour image.
void update_image(ppm_image *image, ppm_image *contour, int x, int y) {
    for (int i = 0; i < contour->x; i++) {
        for (int j = 0; j < contour->y; j++) {
            int contour_pixel_index = contour->x * i + j;
            int image_pixel_index = (x + i) * image->y + y + j;

            image->data[image_pixel_index].red = contour->data[contour_pixel_index].red;
            image->data[image_pixel_index].green = contour->data[contour_pixel_index].green;
            image->data[image_pixel_index].blue = contour->data[contour_pixel_index].blue;
        }
    }
}

// Calls `free` method on the utilized resources.
void free_resources(ppm_image *image, ppm_image **contour_map, unsigned char **grid, int step_x) {
    for (int i = 0; i < CONTOUR_CONFIG_COUNT; i++) {
        free(contour_map[i]->data);
        free(contour_map[i]);
    }
    free(contour_map);

    for (int i = 0; i <= image->x / step_x; i++) {
        free(grid[i]);
    }
    free(grid);

    free(image->data);
    free(image);
}

// Corresponds to step 1 of the marching squares algorithm, which focuses on sampling the image.
// Builds a p x q grid of points with values which can be either 0 or 1, depending on how the
// pixel values compare to the `sigma` reference value. The points are taken at equal distances
// in the original image, based on the `step_x` and `step_y` arguments.
void sample_grid(Thread *data) {
    int p = data->scaled_image->x / data->step_x;
    int q = data->scaled_image->y / data->step_y;

    int start_p = data->id * (double)p / data->P; // atrbui fiecarui thread un segment (formula este din lab =)) )
	int end_p = min((data->id + 1) * (double)p / data->P, p);
    for (int i = start_p; i < end_p; i++) {
        for (int j = 0; j < q; j++) {
            ppm_pixel curr_pixel = data->scaled_image->data[i * data->step_x * data->scaled_image->y + j * data->step_y];

            unsigned char curr_color = (curr_pixel.red + curr_pixel.green + curr_pixel.blue) / 3;

            if (curr_color > SIGMA) {
                data->grid[i][j] = 0;
            } else {
                data->grid[i][j] = 1;
            }
        }
    }
    data->grid[p][q] = 0;

    // last sample points have no neighbors below / to the right, so we use pixels on the
    // last row / column of the input image for them
    
    for (int i = start_p; i < end_p; i++) {
        ppm_pixel curr_pixel = data->scaled_image->data[i * data->step_x * data->scaled_image->y + data->scaled_image->x - 1];

        unsigned char curr_color = (curr_pixel.red + curr_pixel.green + curr_pixel.blue) / 3;

        if (curr_color > SIGMA) {
            data->grid[i][q] = 0;
        } else {
            data->grid[i][q] = 1;
        }
    }

    int start_q = data->id * (double)q / data->P; //dupa q
	int end_q = min((data->id + 1) * (double)q / data->P, q);
    for (int j = start_q; j < end_q; j++) {
        ppm_pixel curr_pixel = data->scaled_image->data[(data->scaled_image->x - 1) * data->scaled_image->y + j * data->step_y];

        unsigned char curr_color = (curr_pixel.red + curr_pixel.green + curr_pixel.blue) / 3;

        if (curr_color > SIGMA) {
            data->grid[p][j] = 0;
        } else {
            data->grid[p][j] = 1;
        }
    }
}

// Corresponds to step 2 of the marching squares algorithm, which focuses on identifying the
// type of contour which corresponds to each subgrid. It determines the binary value of each
// sample fragment of the original image and replaces the pixels in the original image with
// the pixels of the corresponding contour image accordingly.
void march(Thread *data) {
    int p = data->scaled_image->x / data->step_x;
    int q = data->scaled_image->y / data->step_y;

    for (int i = 0; i < p; i++) {
        for (int j = 0; j < q; j++) {
            unsigned char k = 8 * data->grid[i][j] + 4 * data->grid[i][j + 1] + 2 * data->grid[i + 1][j + 1] + 1 * data->grid[i + 1][j];
            update_image(data->scaled_image, data->contour_map[k], i * data->step_x, j * data->step_y);
        }
    }
}



void rescale_image(Thread *data) {
    uint8_t sample[3];

    // we only rescale downwards
    if (!(data->image->x <= RESCALE_X && data->image->y <= RESCALE_Y)) {
        //printf("hello\n");
        // use bicubic interpolation for scaling
        int start, end;
        start = data->id * (double)data->scaled_image->x / data->P;
        end = min((data->id + 1) * (double)data->scaled_image->x / data->P, data->scaled_image->x);
        for (int i = start; i < end; i++) {
            for (int j = 0; j < data->scaled_image->y; j++) {
                float u = (float)i / (float)(data->scaled_image->x - 1);
                float v = (float)j / (float)(data->scaled_image->y - 1);
                sample_bicubic(data->image, u, v, sample);

                data->scaled_image->data[i * data->scaled_image->y + j].red = sample[0];
                data->scaled_image->data[i * data->scaled_image->y + j].green = sample[1];
                data->scaled_image->data[i * data->scaled_image->y + j].blue = sample[2];
            }
        }
    }

    // free(data->image->data);
    // free(data->image);
}

void *thread_function(void *arg)
{
    Thread *data = (Thread*)arg;
    rescale_image(data);
    pthread_barrier_wait(data->barrier); // astept fiecare thread
    sample_grid(data);
    pthread_barrier_wait(data->barrier);
    march(data);

    pthread_exit(NULL);

}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: ./tema1 <in_file> <out_file> <P>\n");
        return 1;
    }

    ppm_image *image = read_ppm(argv[1]);
    int P = atoi(argv[3]); // citesc nr de thread uri
    int step_x = STEP;
    int step_y = STEP;

    // declar bariera
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, P);

    // 0. Initialize contour map
    ppm_image **contour_map = init_contour_map();
    
    // initializare campuri pt rescale (scaled img)
    ppm_image *scaled_image =(ppm_image*)malloc(sizeof(ppm_image)); 
    if(image->x <= RESCALE_X && image->y <= RESCALE_Y) { // ramane imaginea initiala
        scaled_image->x = image->x;
        scaled_image->y = image->y;
        scaled_image->data = image->data;
    } else {
        scaled_image->x = RESCALE_X;
        scaled_image->y = RESCALE_Y;
        scaled_image->data = (ppm_pixel*)malloc(RESCALE_X * RESCALE_Y * sizeof(ppm_pixel));
    }

    // initializare campuri pt grid 
    int p = scaled_image->x / step_x;
    int q = scaled_image->y / step_y;

    unsigned char **grid = (unsigned char **)malloc((p + 1) * sizeof(unsigned char*));
    if (!grid) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    for (int i = 0; i <= p; i++) {
        grid[i] = (unsigned char *)malloc((q + 1) * sizeof(unsigned char));
        if (!grid[i]) {
            fprintf(stderr, "Unable to allocate memory\n");
            exit(1);
        }
    }

    // init structura
    Thread *data = (struct Thread *)malloc(P * sizeof(Thread)); // vector de thread uri de lungime P
    for(int i = 0; i < P; i++) {
        data[i].id = i;
        data[i].contour_map = contour_map;
        data[i].P = P;
        data[i].barrier = &barrier;
        data[i].grid = grid;
        data[i].scaled_image = scaled_image;  
        data[i].image = image; 
        data[i].step_x = step_x;
        data[i].step_y = step_y;
    }


    // init thread uri
    pthread_t threads[P];
    for (int i = 0; i < P; i++) {
		int r = pthread_create(&threads[i], NULL, thread_function, &data[i]);
		if (r) {
			printf("Eroare la crearea thread-ului %d\n", i);
			exit(-1);
		}
	}
	for (int i = 0; i < P; i++) {
		int r = pthread_join(threads[i], NULL);
		if (r) {
			printf("Eroare la asteptarea thread-ului %d\n", i);
			exit(-1);
		}
	}


    // 4. Write output
    write_ppm(data[0].scaled_image, argv[2]);

    free_resources(scaled_image, contour_map, grid, step_x);

    free(data);
    free(image);
    return 0;
}
