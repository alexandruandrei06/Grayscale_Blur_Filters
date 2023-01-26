#include "util.h"

#define MAX_STRING 50

// functie pentru scrierea headerelor si a bitmap-ului pentru poza
void write_photo(params *param, bmp_photo *photo){
    int padding = 4 - ((photo->infoheader->width * 3) % 4);
	unsigned char pad = 0;

	FILE *output_fd = fopen(param->output_file, "wb");

	fwrite(photo->header, sizeof(bmp_fileheader), 1, output_fd);
	fwrite(photo->infoheader, sizeof(bmp_infoheader), 1, output_fd);

	for(int i = 0; i < photo->infoheader->height; i++){
		for(int j = 0; j < photo->infoheader->width; j++)
		{
			fwrite(&photo->bitmap[i][j].blue, sizeof(unsigned char), 1, output_fd);
			fwrite(&photo->bitmap[i][j].green, sizeof(unsigned char), 1, output_fd);
			fwrite(&photo->bitmap[i][j].red, sizeof(unsigned char), 1, output_fd);
		}
		if(padding < 4){
			for(int k = 0; k < padding; k++)
				fwrite(&pad, sizeof(unsigned char), 1, output_fd);
		}
	}
	fflush(output_fd);
	fclose(output_fd);
}

// functie care citeste bitmap-ul si ignora padding-ul
void read_bitmap(bmp_photo *photo, FILE *input_fd){
    int padding = 4 - ((photo->infoheader->width * 3) % 4);
	for(int i = 0; i < photo->infoheader->height; i++){
		for(int j = 0; j < photo->infoheader->width; j++){
			fread(&photo->bitmap[i][j].blue, sizeof(unsigned char), 1, input_fd);
			fread(&photo->bitmap[i][j].green, sizeof(unsigned char), 1, input_fd);
			fread(&photo->bitmap[i][j].red, sizeof(unsigned char), 1, input_fd);
		}
		if(padding < 4)
			fseek(input_fd, padding, SEEK_CUR);
	}
}

// functie care citeste headerele si bitmap-ul pozei
void read_photo(params *param, bmp_photo *photo){
    FILE *input_fd = fopen(param->input_file,"rb");
	if(input_fd == NULL)
		printf("Nu s-a putut deschide fisierul\n");

	//citire header
	fread(photo->header, sizeof(bmp_fileheader), 1, input_fd);

	//citire infoheader
	fread(photo->infoheader, sizeof(bmp_infoheader), 1, input_fd);
	
    photo->bitmap = alloc_bitmap(photo->infoheader->width, photo->infoheader->height);

    read_bitmap(photo, input_fd);

    fclose(input_fd);
}

// alocare memorie pentru poza (bmp header si infoheader)
bmp_photo *alloc_photo(){
    bmp_photo *photo = (bmp_photo *)calloc(1, sizeof(bmp_photo));
    if(photo == NULL){
        printf("Eroare la alocare");
        exit(1);
    }

    photo->header = (bmp_fileheader *)calloc(1, sizeof(bmp_fileheader));
    if(photo->header == NULL){
        printf("Eroare la alocare");
        exit(1);
    }

    photo->infoheader = (bmp_infoheader *)calloc(1, sizeof(bmp_infoheader));
    if(photo->infoheader == NULL){
        printf("Eroare la alocare");
        exit(1);
    }

    return photo;
}

// alocarea memoriei pentru bitmap
RGB **alloc_bitmap(int width, int height){
    RGB **bitmap = (RGB **)calloc(height, sizeof(RGB *));
    if(bitmap == NULL){
        printf("Eroare la alocare");
        exit(1);
    }

    for(int i = 0; i < height; i++){
        bitmap[i] = (RGB *)calloc(width, sizeof(RGB));
        if(bitmap[i] == NULL){
            printf("Eroare la alocare");
            exit(1);
        }
    }

    return bitmap;
}

// dealocarea memoriei pentru bitmap
void free_bitmap(RGB **bitmap, int height){
    for(int i = 0; i < height; i++){
            free(bitmap[i]);
        }
    free(bitmap);
}

// dealocarea memoriei alocate pentru o poza
void free_photo(bmp_photo *photo){
    free_bitmap(photo->bitmap, photo->infoheader->height);
    free(photo->header);
    free(photo->infoheader);
    free(photo);
}

// citeste argumentele programului
void get_args(int argc, char **argv, params *param)
{
	if (argc < 3) {
		printf("Numar insuficient de parametri:\n\t"
				"./exec input_file output_file ");
		exit(1);
	}

    strcpy(param->input_file, argv[1]);
    strcpy(param->output_file, argv[2]);
}

// aloca memorie pentru parametrii de rulare
params *alloc_params(){
    params *param = (params *)calloc(1, sizeof(params));
    if(param == NULL){
        printf("Eroare la alocare");
        exit(1);
    }

    param->input_file = (char*)calloc(MAX_STRING, sizeof(char));
    if(param->input_file == NULL){
        free(param);
        printf("Eroare la alocare");
        exit(1);
    }

    param->output_file = (char*)calloc(MAX_STRING, sizeof(char));
    if(param->output_file == NULL){
        free(param->input_file);
        free(param);
        printf("Eroare la alocare");
        exit(1);
    }

    return param;
}

// dealoca memoria alocata pentru parametrii de rulare
void free_params(params *param){
    free(param->input_file);
    free(param->output_file);
    free(param);
}