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

int IsEmpty(height_mat );//check if linked list is empty 1->true, 0->false
height_mat reste(height_mat );//return height_mat linked list wihtout the head
int tete(height_mat );//return value in the height_mat linked list head
void liberer(height_mat);//free height_mat linked list
int dec_hex_dec(FILE *, int);//read the number of bytes specified and convert in hex then decimal
void lire(height_mat);
height_mat consvide();//create empty height_mat linked list
height_mat add_height_val(height_mat , int );//add an element to a height_mat linked list
int bm_check(FILE * );//check if given file is a bitmap
void bm_resolution(FILE * , int * , int * );//return vertical and horizontal resolution
int bm_pix_format(FILE * );//return number of bits that code a pixel
int bm_pix_offset(FILE * );//return bytes after which pixel data begins
int bm_size(FILE *);//return the size of the image in bytes
void bm_data_storage(FILE *, int);
int bm_greyscale_check(FILE *);
int bm_access_data(height_mat , int, int, int);
void bm_greyscale_conversion(FILE *);
void gaussian_blur(FILE *);


#endif
