#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "img_read.h"

// constante
#define WIDTH_SCREEN 800
#define HEIGHT_SCREEN 600

int main(int argc, char **argv){

    // variables necessaire au fonctionnement de SDL2
    SDL_Window *fenetre = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event ev;
    int continuer = 1;
    char file_name[100];
    int bitmap, reso_vert, reso_hor, pix_format, pix_offset, image_size, data_size;
    int data, i;
    FILE *fp, *gauss_blur;

    printf("Glisser le fichier dans le terminal\t");
    scanf("%s", file_name);
    printf("%s\n",file_name);

    i = strlen(file_name);
    printf("%d\n",i);

    fp=fopen(file_name,"rb");
    if (fp == NULL){
        printf("Fichier impossible a ouvrir\n");
        return EXIT_FAILURE;
    }
    else{
        printf("Fichier %s ouvert\n", file_name);
    }

    bitmap = bm_check(fp);
    if(bitmap == 1){
        printf("Le fichier est bien un fichier bitmap\n");

        bm_resolution(fp, &reso_vert, &reso_hor);
        printf("\nRésolution horizontale: %d pixels\n",reso_hor);
        printf("\nRésolution verticale: %d pixels\n",reso_vert);

        pix_format = bm_pix_format(fp);
        printf("\npixels codés sur %d bits\n", pix_format);

        pix_offset = bm_pix_offset(fp);
        printf("\nDonnées pixels après le %dème octet\n",pix_offset);

        image_size = bm_size(fp);
        printf("L'image pèse %d octets\n", image_size);

        data_size = image_size-pix_offset;
        printf("Les données pixel padding inclus pèsent %d octets\n", data_size);

        if(bm_grayscale_check(fp) == 1){
            printf("\nGrayscale\n");
        }
        else{
            printf("\nCouleur\n");
            bm_grayscale_conversion(fp, file_name);
        }
        gauss_blur=fopen("grayscale.bmp","rb");
        gaussian_blur(gauss_blur);

        /*if(SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stdout, "ecjec init sdl %s\n", SDL_GetError());
        return EXIT_FAILURE;
        }

        fenetre = SDL_CreateWindow(
            "PathFinder -- FRANCOIS DAMIEN --",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WIDTH_SCREEN, HEIGHT_SCREEN,0
        );

        renderer = SDL_CreateRenderer(fenetre, -1,
            SDL_RENDERER_ACCELERATED
        );

        while(continuer){
            SDL_WaitEvent(&ev);
            switch(ev.type){
                case SDL_QUIT:
                    continuer = 0;
                    break;
            }
        }*/
        fclose(gauss_blur);
        fclose(fp);
    }
    else if(bitmap == 0){
        printf("Le fichier n'est pas un fichier bitmap\n");
        fclose(fp);
        return EXIT_FAILURE;
    }
    return 0;
}
