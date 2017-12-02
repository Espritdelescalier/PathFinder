#include"img_read.h"

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
        printf("Liste libérée\n");
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

int bm_check(FILE * file){
    int a, b, res;
    fseek(file,0,SEEK_SET);
    a = fgetc(file);
    b = fgetc(file);
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
    int octets[8];
    int a, i;
    *vert = 0;
    *horizon = 0;
    fseek(file,18,SEEK_SET);
    for(i=0;i<=7;i=i+2){
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
            octets[i] = a;
        }
    }
    for(i=0;i<=7;i++){
        *horizon = *horizon+octets[i]*pow(16,i);
    }
    for(i=0;i<=7;i=i+2){
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
            octets[i] = a;
        }
    }
    for(i=0;i<=7;i++){
        *vert = *vert+octets[i]*pow(16,i);
    }
}

int bm_pix_format(FILE * file){
    int a,b;
    fseek(file,28,SEEK_SET);
    a = fgetc(file);
    b = a*pow(16,0);
    return b;
}

int bm_pix_offset(FILE * file){
    int a;
    fseek(file, 10, SEEK_SET);
    a = fgetc(file);
    return a*pow(16,0);
}

void bm_data_storage(FILE *file, int pix_offset){
    height_mat pixdata = consvide();
    fseek(file, pix_offset, SEEK_SET);
    while(fgetc(file)!=EOF){
        pixdata = add_height_val(pixdata, fgetc(file));
    }
}

int greyscale_check(FILE *file){
    int a,b,c,i=1;

}
