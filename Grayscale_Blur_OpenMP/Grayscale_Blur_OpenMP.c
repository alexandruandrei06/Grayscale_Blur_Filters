#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include "util.h"

#define N 8

void Grayscale(bmp_photo *photo){
    int i;
    #pragma omp parallel for private(i) shared(photo) schedule(dynamic)
    for(i = 0; i < photo->infoheader->height; i++){
		for(int j = 0; j < photo->infoheader->width; j++) {	
			unsigned char X = trunc((photo->bitmap[i][j].blue + photo->bitmap[i][j].green + photo->bitmap[i][j].red) / 3);
			photo->bitmap[i][j].blue = X;
			photo->bitmap[i][j].green = X;
			photo->bitmap[i][j].red = X;
		}
    }
}

void GaussianBlur(bmp_photo *photo, bmp_photo *new_photo){
    int dx[] = {-2, -2, -2, -2, -2, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2};
    int dy[] = {-2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2};
    int i;
    #pragma omp parallel for private(i) shared(photo, new_photo, dx, dy) schedule(dynamic)
    for(i = 0; i < photo->infoheader->height; i++){
		for(int j = 0; j < photo->infoheader->width; j++) {	
            int contor = 0;
            int red = 0;
            int blue = 0;
            int green = 0;
			for(int k = 0; k < 25; k++){
                int x = dx[k] + i;
                int y = dy[k] + j;
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

    //set threads number
    omp_set_num_threads(N);

    // apply GaussianBlur
    GaussianBlur(photo, new_photo);

    // apply Grayscale filter
    Grayscale(new_photo);


    //scrierea pozei
    write_photo(param, new_photo);

    // free memory
    free_params(param);
    free_photo(new_photo);
    free_photo(photo);

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("Algorithm took %f seconds to execute", time_taken);

    return 0;
}