#ifndef _IMG_READ_
#define _IMG_READ_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct{
    int x, y;
}img_pt;

typedef struct{
    unsigned char *header;
    unsigned char *buffer;
    int reso_h;
    int reso_v;
    int offset;
    int siz;
}file_t;

/*typedef struct pt{
    img_pt point;
    struct pt * succ;
}imgpx, *path;*/

/*int IsEmpty(path );//check if linked list is empty 1->true, 0->false
path reste(path );//return linked list wihtout the head
img_pt tete(path );//return value in the linked list head
void liberer(path);//free linked list
int dec_hex_dec(FILE *, int);//read the number of bytes specified and convert in hex then decimal
path consvide();//create empty  linked list
path add_val(path , img_pt );//add an element to linked list*/
int bm_check(FILE * );//check if given file is a bitmap
void bm_resolution(FILE * , int * , int * );//return vertical and horizontal resolution
int bm_pix_format(FILE * );//return number of bits that code a pixel
int bm_pix_offset(FILE * );//return bytes after which pixel data begins
int bm_size(FILE *);//return the size of the image in bytes
void bm_grayscale_check(FILE *, int *, int *);
//int bm_access_data(height_mat , int, int, int);
void bm_grayscale_conversion(FILE *, char *, int, file_t);
void gaussian_blur(FILE *, char*);
void angle_to_rgb(double, unsigned char*, unsigned char*, unsigned char*, unsigned char);
void sobel_filter(FILE * file, char *, char *, file_t);
void next_point(img_pt, img_pt, img_pt *);
//path pathfinder(FILE *, FILE *, int, int, int, img_pt, img_pt, int);
//void finder(img_pt, img_pt, int, int *, int, int);
//int check_next_point(img_pt, img_pt, int, unsigned char, unsigned char *);



#endif
