#include"img_read.h"
#include <stdint.h>

int IsEmpty(height_mat hm){
    return hm == NULL;
}

height_mat reste(height_mat hm){
    return hm->succ;
}

int tete(height_mat hm){
    return hm->h_value;
}

void liberer(height_mat hm){
    if(IsEmpty(hm) == 0){
        liberer(reste(hm));
        free(hm);
    }
    else{
        //free(hm);
        printf("\nListe libérée\n");
    }
}

height_mat consvide(){
    return NULL;
}

int dec_hex_dec(FILE *file, int nbytes){
    int octets[nbytes];
    int data_size = 0;
    int a, b, i;
    for(i=0;i<=nbytes-1;i=i+2){
        a = fgetc(file);
        if(a==0){
            octets[i] = a;
            octets[i+1] = a;
        }
        else if((a>=1)&&(a<=9)){
            octets[i] = a;
            octets[i+1] = 0;
        }
        else{
            b = a;
            b= a%16;
            octets[i] = b;
            b = (a/16)%16;
            octets[i+1] = b;
        }
    }
    for(i=0;i<=nbytes-1;i++){
        data_size = data_size+octets[i]*pow(16,i);
    }
    return data_size;
}

height_mat add_height_val(height_mat hv, int val){
    elev_mat * m;
    m = (height_mat)malloc(sizeof(elev_mat));
    m->h_value = val;
    m->succ = hv;
    return m;
}

void lire(height_mat hm){
    height_mat hmp = hm;
    while(IsEmpty(hmp) != 1){
        printf("%d\t",tete(hmp));
        hmp = reste(hmp);
    }
}

int bm_check(FILE * file){
    int a, b, res;
    fseek(file,0,SEEK_SET);
    a = fgetc(file);
    b = fgetc(file);
    rewind(file);
    if((a==66) && (b==77)){
        res = 1;
    }
    else{
        res = 0;
    }
    return res;
}

void bm_resolution(FILE * file, int * vert, int * horizon){
    //initialiser vert et horizon à zéro
    *vert = 0;
    *horizon = 0;
    fseek(file, 18, SEEK_SET);
    *horizon = dec_hex_dec(file, 8);
    *vert = dec_hex_dec(file, 8);
    rewind(file);
}

int bm_pix_format(FILE * file){
    int a;
    fseek(file,28,SEEK_SET);
    a = fgetc(file);
    return a;
}

int bm_pix_offset(FILE * file){
    int a;
    fseek(file, 10, SEEK_SET);
    a = fgetc(file);
    return a*pow(16,0);
}

int bm_size(FILE *file){
    int data_size = 0;
    fseek(file, EOF, SEEK_END);
    data_size = ftell(file) + 1;
    return data_size;
}


//unfinished
void bm_data_storage(FILE *file, int pix_offset){
    height_mat pixdata = consvide();
    fseek(file, pix_offset, SEEK_SET);
    while(fgetc(file)!=EOF){
        pixdata = add_height_val(pixdata, fgetc(file));
    }
}

int bm_greyscale_check(FILE *file){
    int i, a, b, c, diag, med, h_reso, v_reso;
    bm_resolution(file, &v_reso, &h_reso);
    for(i = 1; i<=v_reso; i=i+2){
        fseek(file, (3*h_reso)+(i*3), SEEK_SET);
        a = fgetc(file);
        b = fgetc(file);
        c = fgetc(file);
        if((a==b)&&(b==c)){
            diag = 1;
        }
        else
            diag = 0;
    }
    fseek(file, (v_reso/2)*(h_reso*3), SEEK_SET);
    for(i = 0; i <= (v_reso); i++){
        a = fgetc(file);
        b = fgetc(file);
        c = fgetc(file);
        if((a==b)&&(b==c)){
            med = 1;
        }
        else
            med = 0;
    }
    return ((diag==1)&&(med==1));
}

int bm_access_data(height_mat matrix, int col, int line,  int reso){
    int i, j;
    if (col <= reso){
        for(i = 0; i <= line-2; i++){
            for(j = 0; j<= reso-1; j++){
                matrix = reste(matrix);
            }
        }
        for(i = 0; i <= col-1; i++){
            matrix = reste(matrix);
        }
        return tete(matrix);
    }
    else
        return EXIT_FAILURE;
}

void bm_greyscale_conversion(FILE *file){
    FILE *bwclone;
    int offset, siz, i, j;
    short a, b, c, grey;
    bwclone = fopen("greyscale_copy.bmp", "wb");
    offset = bm_pix_offset(file);
    siz = bm_size(file)-offset;
    rewind(file);
    for(i = 1; i <= offset; i++){
        putc(fgetc(file),bwclone);
    }
    for(i = offset; i < siz-3; i=i+3){
        a = fgetc(file);
        b = fgetc(file);
        c = fgetc(file);
        if (((a==255)&&(b==255))&&(c==255)){
            grey = 255;
        }
        else if((a <= 255)&&(a >= 5)&&(c <= 255)&&(c >= 5)){
            grey = 255- ((255-b)/3);
        }
        else if((c == 255)&&(a == 0)){
            grey = (255 - (255/3)) - (b/3);
        }
        else if((b == 255)&&(a == 0)){
            grey = (255/3) - ((255-c)/3);
        }
        else {
            grey = 255;
        }
        for(j = 1; j <= 3; j++){
            putc(grey, bwclone);
        }
    }
    fclose(bwclone);
}

void gaussian_blur(FILE * file){
    FILE *blur_clone;
    int offset, i, j=0, k, reso_h, reso_v, grey;
    double gauss_kernel[3] = {0.06136, 0.24477, 0.38774};
    unsigned char pmintwo, pminone, p, pplusone, pplustwo, a, b, c, d, e;
    unsigned char * buffer;
    unsigned char * header;
    long siz;
    //size_t result;
    blur_clone = fopen("gaussian_copy.bmp", "wb");
    siz = bm_size(file);
    offset = bm_pix_offset(file);

    buffer = (unsigned char *)malloc (sizeof(unsigned char)*(siz-offset)/3);
    header = (unsigned char *)malloc (sizeof(unsigned char)*offset);
    rewind(file);
    fread(header, 1, offset, file);
    for(i=0; i<=offset; i++){
        //printf("%d\n",header[i]);
    }
    bm_resolution(file, &reso_v, &reso_h);
    fwrite(header, offset, 1, blur_clone);
    for(i=offset; i<siz; i=i+3){
        fseek(file, i+1, SEEK_SET);
        buffer[j] = getc(file);
        j++;
    }
    pmintwo = buffer[0];
    pminone = buffer[500];
    p = buffer[2000];
    pplusone = buffer[3000];
    pplustwo = buffer[8000000];
    printf("%d\n",pplustwo);

    /*if((pminone == pmintwo)&&(pminone == p)&&(p == pplusone)&&(pplusone == pplustwo)){

    }
    else{
        putc(getc(file), blur_clone);
    }*/
    free(header);
    free(buffer);
    fclose(blur_clone);
}
