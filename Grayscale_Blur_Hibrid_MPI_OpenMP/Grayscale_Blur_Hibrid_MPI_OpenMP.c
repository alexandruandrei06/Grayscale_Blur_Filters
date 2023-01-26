#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <time.h>
#include <omp.h>
#include "util.h"

#define N 2

void Grayscale(RGB *bitmap, int i_start, int i_end, int width){
    int i, j;
    #pragma omp parallel for private(i, j) shared(bitmap) schedule(dynamic)
    for(i = i_start; i < i_end; i++){
		for(j = 0; j < width; j++) {	
            int idx = (i - i_start) * width + j;
			unsigned char X = trunc((bitmap[idx].blue + bitmap[idx].green + bitmap[idx].red) / 3);
			bitmap[idx].blue = X;
			bitmap[idx].green = X;
			bitmap[idx].red = X;
		}
    }
}

void GaussianBlur(RGB *bitmap, RGB *new_bitmap, int i_start, int i_end, int width, int height){
    int dx[] = {-2, -2, -2, -2, -2, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2};
    int dy[] = {-2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2};
    int i, j;
    #pragma omp parallel for private(i, j) shared(new_bitmap, bitmap, dx, dy) schedule(dynamic)
    for(i = i_start; i < i_end; i++){
		for(j = 0; j < width; j++) {	
            int idx = (i - i_start) * width + j;
            int contor = 0;
            int red = 0;
            int blue = 0;
            int green = 0;
			for(int k = 0; k < 25; k++){
                int x = dx[k] + i;
                int y = dy[k] + j;
                if(x >= 0 && x < height && 
                    y >= 0 && y < width){
                        int index = x * width + y;
                        red += bitmap[index].red;
                        green += bitmap[index].green;
                        blue += bitmap[index].blue;
                        contor++;
                }
            }
            new_bitmap[idx].red = red / contor;
            new_bitmap[idx].blue = blue / contor;
            new_bitmap[idx].green = green / contor;
		}
    }
}


int main(int argc, char *argv[])
{
    clock_t t;
    t = clock();

    int rank, numtasks;
    int height, width;
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Total number of processes.
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); // The current process ID / Rank.

    // set threads number
    omp_set_num_threads(N);

    if(rank == 0){
        // alocare memorie pentru argumentele de rulare
        params *param = alloc_params();

        // se citesc argumentele programului
	    get_args(argc, argv, param);

        bmp_photo *photo = alloc_photo();

        //citirea pozei
        read_photo(param, photo);

        //sending width and height data for problem
        width = photo->infoheader->width;
        height = photo->infoheader->height;

        for(int i = 1; i < numtasks; i++){
            MPI_Send(&width, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&height, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(photo->bitmap, width * height * sizeof(RGB), MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }

        int total_size = height;
        int i_start = rank * (double)total_size / numtasks;
        int i_end = MIN((rank + 1) * (double)total_size / numtasks, total_size);

        int task_size = (i_end - i_start) * width;

        RGB *new_bitmap = (RGB *)calloc(task_size, sizeof(RGB));;
        if(new_bitmap == NULL){
            printf("Eroare la alocare");
            exit(1);
        }

        // apply GaussianBlur
        GaussianBlur(photo->bitmap, new_bitmap, i_start, i_end, width, height);

        // apply Grayscale filter
        Grayscale(new_bitmap, i_start, i_end, width);

        memcpy(photo->bitmap + i_start, new_bitmap, task_size * sizeof(RGB));

        MPI_Status status;
        for(int i = 1; i < numtasks; i++){
            i_start = i * (double)total_size / numtasks;
            i_end = MIN((i + 1) * (double)total_size / numtasks, total_size);
            task_size = (i_end - i_start) * width;

            MPI_Recv(photo->bitmap + (i_start * width), task_size * sizeof(RGB), MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
        }

        //scrierea pozei
        write_photo(param, photo);

        // free memory
        free_params(param);
        free_photo(photo);   
        free(new_bitmap);

        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
        printf("Algorithm took %f seconds to execute", time_taken);
    }else{
        MPI_Status status;
        RGB *bitmap;

        //receive width and height for problem
        MPI_Recv(&width, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&height, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        bitmap = (RGB *)calloc(height * width, sizeof(RGB));
        if(bitmap == NULL){
            printf("Eroare la alocare");
            exit(1);
        }

        MPI_Recv(bitmap, width * height * sizeof(RGB), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);

        int total_size = height;
        int i_start = rank * (double)total_size / numtasks;
        int i_end = MIN((rank + 1) * (double)total_size / numtasks, total_size);

        int task_size = (i_end - i_start) * width;
        
        RGB *new_bitmap = (RGB *)calloc(task_size, sizeof(RGB));;
        if(new_bitmap == NULL){
            printf("Eroare la alocare");
            exit(1);
        }

        // apply GaussianBlur
        GaussianBlur(bitmap, new_bitmap, i_start, i_end, width, height);

        // apply Grayscale filter
        Grayscale(new_bitmap, i_start, i_end, width);

        MPI_Send(new_bitmap, task_size * sizeof(RGB), MPI_CHAR, 0, 0, MPI_COMM_WORLD);

        free(bitmap);
        free(new_bitmap);
    }

    MPI_Finalize();

    return 0;
}
