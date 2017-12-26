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

//returns the decimal value of a hexadecimal number coded on "nbytes" number of bytes
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

//checks the first two bytes of "file" to determine if it is a bitmap
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

//reads the header of "file" to determine the vertical resolution "vert" and
//the horizontal resolution "horizon"
void bm_resolution(FILE * file, int * vert, int * horizon){
    //initialiser vert et horizon à zéro
    *vert = 0;
    *horizon = 0;
    fseek(file, 18, SEEK_SET);
    *horizon = dec_hex_dec(file, 8);
    *vert = dec_hex_dec(file, 8);
    rewind(file);
}

// reads the header of "file" to find out how many bytes are used to code the pixel data
int bm_pix_format(FILE * file){
    int a;
    fseek(file,28,SEEK_SET);
    a = fgetc(file);
    rewind(file);
    return a;
}

//reads the header of "file" and returns the offset, the number of bytes after which
//the pixel data begins
int bm_pix_offset(FILE * file){
    int a;
    fseek(file, 10, SEEK_SET);
    a = fgetc(file);
    rewind(file);
    return a*pow(16,0);
}

//returns the number of bytes of "file"
int bm_size(FILE *file){
    int data_size = 0;
    fseek(file, EOF, SEEK_END);
    data_size = ftell(file) + 1;
    rewind(file);
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

//takes two lines (one along the middle and one diagonal) of pixels of "file"
// and checks if they are grayscale. If the chosen sample is grayscale the image
//is considered grayscale as a matter of practicality
int bm_grayscale_check(FILE *file){
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
    rewind(file);
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

//copy the header of "file" in unsigned char array "header" and copy it in a new file unchanged
//copy the pixel data of "file" in unsigned char array "buffer" and proceed to convert BGR to grayscale
void bm_grayscale_conversion(FILE *file, char *name){
    FILE *bwclone;
    int offset, siz, i, j;
    unsigned char * buffer;
    unsigned char * header;
    unsigned char B, G, R, grey;
    offset = bm_pix_offset(file);
    siz = bm_size(file)-offset;
    bwclone = fopen("grayscale.bmp", "wb");
    buffer = (unsigned char *)malloc (sizeof(unsigned char)*siz);
    header = (unsigned char *)malloc (sizeof(unsigned char)*offset);

    fread(header, 1, offset, file);
    fread(buffer, 1, siz, file);
    fwrite(header, offset, 1, bwclone);
    free(header);

    for(i = 0; i < siz-2; i=i+3){
        B = buffer[i];
        G = buffer[i+1];
        R = buffer[i+2];
        if (((B==255)&&(G==255))&&(R==255)){
            //if white keep white
            grey = 255;
        }
        else if((B <= 255)&&(B >= 5)&&(R <= 255)&&(R >= 5)){
            grey = 255- ((255-G)/3);
        }
        else if((R == 255)&&(B == 0)){
            grey = (255 - (255/3)) - (G/3);
        }
        else if((G == 255)&&(B == 0)){
            grey = (255/3) - ((255-R)/3);
        }
        else {
            grey = 255;
        }
        for(j = 1; j <= 3; j++){
            putc(grey, bwclone);
        }
    }
    free(buffer);
    rewind(file);
    fclose(bwclone);
}

void gaussian_blur(FILE * file){
    FILE *blur_clone;
    int offset, i, j=0, reso_h, reso_v, grey;
    double gauss_kernel[3] = {0.153388, 0.221461, 0.250301};
    double po, pt, pp, ppo, ppt;
    unsigned char pmintwo, pminone, p, pplusone, pplustwo;
    unsigned char * original_and_vert_pass;
    unsigned char * hor_pass;
    unsigned char * header;
    long siz;
    //size_t result;
    blur_clone = fopen("gaussian_copy.bmp", "wb");
    siz = bm_size(file);
    offset = bm_pix_offset(file);

    original_and_vert_pass = (unsigned char *)malloc (sizeof(unsigned char)*(siz-offset)/3);
    hor_pass = (unsigned char *)malloc (sizeof(unsigned char)*(siz-offset)/3);
    header = (unsigned char *)malloc (sizeof(unsigned char)*offset);
    bm_resolution(file, &reso_v, &reso_h);
    rewind(file);
    //copy and write header from original file in the new one
    fread(header, 1, offset, file);
    fwrite(header, offset, 1, blur_clone);
    free(header);

    //fill the first unsigned char array with pixels value to compute the horizontal pass
    //of the gaussian blur
    for(i=offset; i<siz; i=i+3){
        fseek(file, i+1, SEEK_SET);
        original_and_vert_pass[j] = getc(file);
        j++;
    }

    //fill the second unsigned array with the result of the calculations of the horizontal pass
    //of the gaussian blur
    pmintwo = original_and_vert_pass[0];
    pminone = original_and_vert_pass[1];
    p = original_and_vert_pass[2];
    pplusone = original_and_vert_pass[3];
    pplustwo = original_and_vert_pass[4];
    for(i=0; i < ((siz-offset)/3)-2; i++){
        pt = (pmintwo*gauss_kernel[0]);
        po = (pminone*gauss_kernel[1]);
        pp = (p*gauss_kernel[2]);
        ppo = (pplusone*gauss_kernel[1]);
        ppt = (pplustwo*gauss_kernel[0]);
        grey = (unsigned char)(pt+po+pp+ppo+ppt);
        hor_pass[i+3] = grey;
        /*for(j=1; j<=3; j++)
            putc(grey, blur_clone);*/
        pmintwo = pminone;
        pminone = p;
        p = pplusone;
        pplusone = pplustwo;
        pplustwo = original_and_vert_pass[i+5];
    }

    //refill the first unsigned char array with the result of the calculations of the vertical_pass
    // of the gaussian blur




    free(original_and_vert_pass);
    fclose(blur_clone);
}
