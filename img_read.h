#ifndef _IMG_READ_
#define _IMG_READ_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct height_val{
    int h_value;
    struct height_val * succ;
}elev_mat, * height_mat;

int IsEmpty(height_mat );
height_mat reste(height_mat );
int tete(height_mat );
void liberer(height_mat);
void lire(height_mat);
height_mat consvide();
height_mat add_height_val(height_mat , int );
int bm_check(FILE * );
void bm_resolution(FILE * , int * , int * );
int bm_pix_format(FILE * );
int bm_pix_offset(FILE * );
void bm_data_storage(FILE *, int);
int greyscale_check(FILE *);


#endif
