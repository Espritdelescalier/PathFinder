#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "img_read.h"

// constante
#define WIDTH_SCREEN 800
//#define HEIGHT_SCREEN 600

int main(int argc, char **argv){

    // variables necessaire au fonctionnement de SDL2
    SDL_Window *fenetre = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event ev;
    int continuer = 1;
    char file_name[120], filecopy_name[120], name_for_texture[120];
    int bitmap, reso_vert, reso_hor, pix_format, pix_offset, image_size, data_size;
    int i;
    FILE *fp, *gauss_blur, *sobel;

    do{
        printf("Glisser le fichier dans le terminal (ou copier le chemin complet du fichier)\n");
        scanf("%s", file_name);
        printf("%s\n",file_name);
        i = strlen(file_name);
    }while(i>70);

    fp=fopen(file_name,"rb");
    if (fp == NULL){
        printf("Fichier impossible a ouvrir\n");
        return EXIT_FAILURE;
    }
    else{
        printf("Fichier %s ouvert\n", file_name);
    }

    strncpy(name_for_texture, file_name, sizeof(file_name));
    bitmap = bm_check(fp);
    if(bitmap == 1){
        printf("Le fichier est bien un fichier bitmap\n");

        bm_resolution(fp, &reso_vert, &reso_hor);
        printf("* Résolution horizontale: %d pixels\n",reso_hor);
        printf("* Résolution verticale: %d pixels\n",reso_vert);

        pix_format = bm_pix_format(fp);
        printf("* Pixels codés sur %d bits\n", pix_format);

        pix_offset = bm_pix_offset(fp);
        printf("* Données pixels après le %dème octet\n",pix_offset);

        image_size = bm_size(fp);
        printf("* L'image pèse %d octets\n", image_size);

        data_size = image_size-pix_offset;
        printf("* Les données pixel padding inclus pèsent %d octets\n", data_size);
        if(pix_format != 24){
            printf("Format pixel non supporté\n");
            return EXIT_FAILURE;
        }

        //SDL2 to display the original image and take inputs
        //SDL2 variables
        int HEIGHT_SCREEN = (int)(WIDTH_SCREEN * (reso_vert/(double)reso_hor));//scale the window to the image
        int mouse_click = 0, x_start, y_start, x_finish, y_finish;
        double x_ratio, y_ratio;

        if(SDL_Init(SDL_INIT_VIDEO) != 0){
            fprintf(stdout, "echec init sdl %s\n", SDL_GetError());
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
        SDL_Surface * image = SDL_LoadBMP(name_for_texture);
        SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        while((continuer)&&(mouse_click != 2)){
            while(SDL_PollEvent(&ev)){
                switch(ev.type){
                    case SDL_QUIT:
                        continuer = 0;
                        break;
                    case SDL_MOUSEBUTTONDOWN://two clicks to register coordinates
                        if(mouse_click == 0){
                            SDL_GetMouseState(&x_start, &y_start);
                            mouse_click++;
                        }
                        else{
                            SDL_GetMouseState(&x_finish, &y_finish);
                            mouse_click++;
                        }
                        break;
                }
            }
        }

        //calculate ratio window resolution to image resolution and pixel position on image
        x_ratio = (reso_hor/(double)WIDTH_SCREEN);
        y_ratio = (reso_vert/(double)HEIGHT_SCREEN);
        x_start = (int)(x_start * x_ratio);
        y_start = (int)(y_start * y_ratio);
        x_finish = (int)(x_finish * x_ratio);
        y_finish = (int)(y_finish * y_ratio);

        if(bm_grayscale_check(fp) == 1){
            printf("* Grayscale\n");
        }
        else{
            printf("* Couleur\n");
            //change name of the file, file will be saved in the folder of the original file
            file_name[i-4] = '\0';
            strncat(file_name, "_grayscale.bmp",  sizeof("_grayscale.bmp"));
            i = strlen(file_name);

            bm_grayscale_conversion(fp, file_name);
        }
        fclose(fp);

        //change name of the file, file will be saved in the folder of the original file
        gauss_blur=fopen(file_name,"rb");
        file_name[i-4] = '\0';
        strncat(file_name, "_gaussian.bmp",  sizeof("_gaussian.bmp"));
        i = strlen(file_name);
        gaussian_blur(gauss_blur, file_name);

        //change name of the file, file will be saved in the folder of the original file
        sobel = fopen(file_name, "rb");
        file_name[i-4] = '\0';
        strncat(file_name, "_gradient.bmp",  sizeof("_gradient.bmp"));
        i = strlen(file_name);

        strncpy(filecopy_name, file_name, sizeof(file_name));
        filecopy_name[i - 4] = '\0';
        strncat(filecopy_name, "_orientation.bmp",  sizeof("_orientation.bmp"));
        i = strlen(file_name);

        sobel_filter(sobel, file_name, filecopy_name);

        fclose(sobel);
        fclose(gauss_blur);

        printf("* Done!\n");
    }
    else if(bitmap == 0){
        printf("* Le fichier n'est pas un fichier bitmap\n");
        fclose(fp);
        return EXIT_FAILURE;
    }
    return 0;
}
