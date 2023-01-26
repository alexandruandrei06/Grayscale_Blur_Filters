#ifndef UTIL_H
#define UTIL_H

#define MIN(a, b) (a > b) ? b : a

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bmp_header.h"

// structura pentru argumentele de rulare
typedef struct params {
    char *input_file;
    char *output_file;
} params;

typedef struct RGB {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} RGB;

typedef struct bmp_photo{
    bmp_fileheader *header;
    bmp_infoheader *infoheader;
    RGB *bitmap;
} bmp_photo;

// functie pentru scrierea headerelor si a bitmap-ului pentru poza
void write_photo(params *param, bmp_photo *photo);

// functie care citeste bitmap-ul si ignora padding-ul
void read_bitmap(bmp_photo *photo, FILE *input_fd);

// functie care citeste headerele si bitmap-ul pozei
void read_photo(params *param, bmp_photo *photo);

// alocarea memoriei pentru bitmap
RGB *alloc_bitmap(int width, int height);

// dealocarea memoriei pentru bitmap
void free_bitmap(RGB *bitmap);

// alocare memorie pentru poza (bmp header si infoheader)
bmp_photo *alloc_photo();

// dealocarea memoriei alocate pentru o poza
void free_photo(bmp_photo *photo);

// citeste argumentele programului
void get_args(int argc, char **argv, params *param);

// aloca memorie pentru argumentele de rulare
params *alloc_params();

// dealoca memoria alocata pentru argumentele de rulare
void free_params(params *param);

#endif /* UTIL_H */