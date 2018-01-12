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
    img_pt start;
    img_pt finish;
    int continuer = 1;
    file_t fichier;
    char file_name[120], filecopy_name[120], name_for_texture[120];
    int bitmap, pix_format, data_size;
    int i, graycheck, rgbcheck;
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

        bm_resolution(fp, &fichier.reso_v, &fichier.reso_h);
        printf("* Résolution horizontale: %d pixels\n",fichier.reso_h);
        printf("* Résolution verticale: %d pixels\n",fichier.reso_v);

        pix_format = bm_pix_format(fp);
        printf("* Pixels codés sur %d bits\n", pix_format);

        fichier.offset = bm_pix_offset(fp);
        printf("* Données pixels après le %dème octet\n",fichier.offset);

        fichier.siz = bm_size(fp);
        printf("* L'image pèse %d octets\n", fichier.siz);

        data_size = fichier.siz-fichier.offset;
        printf("* Les données pixel padding inclus pèsent %d octets\n", data_size);
        if(pix_format != 24){
            printf("Format pixel non supporté\n");
            return EXIT_FAILURE;
        }

        //SDL2 to display the original image and take inputs
        //SDL2 variables
        int HEIGHT_SCREEN = (int)(WIDTH_SCREEN * (fichier.reso_v/(double)fichier.reso_h));//scale the window to the image
        int mouse_click = 0;
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

        while((continuer)&&(mouse_click != 2)){
            SDL_RenderPresent(renderer);
            while(SDL_PollEvent(&ev)){
                switch(ev.type){
                    case SDL_QUIT:
                        continuer = 0;
                        break;
                    case SDL_MOUSEBUTTONDOWN://two clicks to register coordinates
                        if(mouse_click == 0){
                            SDL_GetMouseState(&start.x, &start.y);
                            mouse_click++;
                        }
                        else{
                            SDL_GetMouseState(&finish.x, &finish.y);
                            mouse_click++;
                        }
                        break;
                }
            }
        }

        if(mouse_click == 2){
            //calculate ratio window resolution to image resolution and pixel position on image if the window wasn't closed before
            //the points were selected
            x_ratio = (fichier.reso_h/(double)WIDTH_SCREEN);
            y_ratio = (fichier.reso_v/(double)HEIGHT_SCREEN);
            start.x = (int)(start.x * x_ratio);
            start.y = (int)(fichier.reso_v-(start.y * y_ratio));
            finish.x = (int)(finish.x * x_ratio);
            finish.y = (int)(fichier.reso_v-(finish.y * y_ratio));
            //printf("x1 %d; y1 %d; x2 %d; y2 %d",start.x, start.y, finish.x, finish.y);
        }

        bm_grayscale_check(fp, &graycheck, &rgbcheck);
        if(graycheck == 1){
            printf("* Grayscale\n");
        }
        else{
            printf("* Couleur\n");
            //change name of the file, file will be saved in the folder of the original file
            file_name[i-4] = '\0';
            strncat(file_name, "_grayscale.bmp",  sizeof("_grayscale.bmp"));
            i = strlen(file_name);

            bm_grayscale_conversion(fp, file_name, rgbcheck, fichier);
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
