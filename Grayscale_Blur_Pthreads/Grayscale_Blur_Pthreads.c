#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include "util.h"

typedef struct thread_args {
    params *param;
    bmp_photo *photo;
    bmp_photo *new_photo;
    int id;
} thread_args;

void Grayscale(bmp_photo *photo, int i_start, int i_end){
    for(int i = i_start; i< i_end; i++){
		for(int j = 0; j < photo->infoheader->width; j++) {	
			unsigned char X = trunc((photo->bitmap[i][j].blue + photo->bitmap[i][j].green + photo->bitmap[i][j].red) / 3);
			photo->bitmap[i][j].blue = X;
			photo->bitmap[i][j].green = X;
			photo->bitmap[i][j].red = X;
		}
    }
}

void GaussianBlur(bmp_photo *photo, bmp_photo *new_photo, int i_start, int i_end){
    int dx[] = {-2, -2, -2, -2, -2, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2};
    int dy[] = {-2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2};
    int x, y;
    int red, blue, green;
    for(int i = i_start; i < i_end; i++){
		for(int j = 0; j < photo->infoheader->width; j++) {	
            int contor = 0;
            red = 0;
            blue = 0;
            green = 0;
			for(int k = 0; k < 25; k++){
                x = dx[k] + i;
                y = dy[k] + j;
                if(x >= 0 && x < photo->infoheader->height && 
                    y >= 0 && y < photo->infoheader->width){
                        red += photo->bitmap[x][y].red;
                        green += photo->bitmap[x][y].green;
                        blue += photo->bitmap[x][y].blue;
                        contor++;
                }
            }
            new_photo->bitmap[i][j].red = red / contor;
            new_photo->bitmap[i][j].blue = blue / contor;
            new_photo->bitmap[i][j].green = green / contor;
		}
    }
}


void *thread_function(void *arg){
    struct thread_args *args = (struct thread_args *) arg;
    
    int total_size = args->photo->infoheader->height;
    int i_start = args->id * (double)total_size / args->param->threads_number;
    int i_end = MIN((args->id + 1) * (double)total_size / args->param->threads_number, total_size);

     // apply GaussianBlur
    GaussianBlur(args->photo, args->new_photo, i_start, i_end);

    // apply Grayscale filter
    Grayscale(args->new_photo, i_start, i_end);

    return NULL;
}

int main(int argc, char *argv[]){

    clock_t t;
    t = clock();

    // alocare memorie pentru argumentele de rulare
    params *param = alloc_params();

    // se citesc argumentele programului
	get_args(argc, argv, param);

    bmp_photo *photo = alloc_photo();

    //citirea pozei
    read_photo(param, photo);

    bmp_photo *new_photo = alloc_photo();
    new_photo->bitmap = alloc_bitmap(photo->infoheader->width, photo->infoheader->height);
    memcpy(new_photo->header, photo->header, sizeof(bmp_fileheader));
    memcpy(new_photo->infoheader, photo->infoheader, sizeof(bmp_infoheader));

    pthread_t threads[param->threads_number];
    int r;
    void *status;

    thread_args **args = (thread_args **)calloc(param->threads_number, sizeof(thread_args *));
    if (args == NULL) {
		printf("Eroare la alocare\n");
		exit(1);
	}

    for(int id = 0; id < param->threads_number; id++){
        args[id] = (thread_args *)calloc(1, sizeof(thread_args));
        if (args[id] == NULL) {
            printf("Eroare la alocare\n");
            exit(1);
        }

        args[id]->id = id;
        args[id]->photo = photo;
        args[id]->new_photo = new_photo;
        args[id]->param = param;

        r = pthread_create(&threads[id], NULL, thread_function, args[id]);
        if (r) {
            printf("Eroare la crearea thread-ului %d\n", id);
            exit(-1);
        }
    }


    for (int id = 0; id < param->threads_number; id++) {
        r = pthread_join(threads[id], &status);

        free(args[id]);
        if (r) {
            printf("Eroare la asteptarea thread-ului %d\n", id);
            exit(-1);
        }
    }

    //scrierea pozei
    write_photo(param, new_photo);

    // free memory
    free_params(param);
    free_photo(new_photo);
    free_photo(photo);
    free(args);

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("Algorithm took %f seconds to execute", time_taken);
    
    return 0;
}
