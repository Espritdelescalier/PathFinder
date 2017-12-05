#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "img_read.h"

int main(){
    char file_name[50];
    int bitmap, reso_vert, reso_hor, pix_format, pix_offset, image_size, data_size;
    int data, i;
    height_mat hm;
    FILE *fp;

    printf("Entrez le nom du fichier\t");
    scanf("%s", file_name);
    //strcat(file_name,".bmp");
    printf("%s\n",file_name);

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
    }
    else if(bitmap == 0){
        printf("Le fichier n'est pas un fichier bitmap\n");
        fclose(fp);
        return EXIT_FAILURE;
    }

    bm_resolution(fp, &reso_vert, &reso_hor);
    printf("\nRésolution horizontale: %d pixels\n",reso_hor);
    printf("\nRésolution verticale: %d pixels\n",reso_vert);

    pix_format = bm_pix_format(fp);
    printf("\npixels codés sur %d bits\n", pix_format);

    pix_offset = bm_pix_offset(fp);
    printf("\nDonnées pixels après le %dème octet\n",pix_offset);

    image_size = bm_size(fp);
    printf("L'image pèse %d octets\n", image_size);

    data_size = bm_pix_data_size(fp);
    printf("Les données pixel padding inclus pèsent %d octets\n", data_size);

    hm = consvide();
    fseek(fp, pix_offset, SEEK_SET);
    for(i=0; i<= 300; i=i+3){
        data = fgetc(fp);
        hm = add_height_val(hm, data);
    }

    lire(hm);
    printf("\t\t\t\n%d\n",tete(hm));
    liberer(hm);


    fclose(fp);
    return 1;
}
