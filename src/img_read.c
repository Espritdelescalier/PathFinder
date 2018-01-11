#include"img_read.h"
#include <stdint.h>

#define PI 3.14159265

int IsEmpty(path hm){
    return hm == NULL;
}

height_mat reste(path hm){
    return hm->succ;
}

int tete(path hm){
    return hm->point;
}

void liberer(path hm){
    if(IsEmpty(hm) == 0){
        liberer(reste(hm));
        free(hm);
    }
    else{
        //free(hm);
        printf("\nListe libérée\n");
    }
}

path consvide(){
    return NULL;
}

path add_val(path hv, img_pt val){
    pt * m;
    m = (path)malloc(sizeof(pt));
    m->pt.x = val.x;
    m->pt.y = val.y;
    m->succ = hv;
    return m;
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


//takes two lines (one along the middle and one diagonal) of pixels of "file"
// and checks if they are grayscale. If the chosen sample is grayscale the image
//is considered grayscale as a matter of practicality
void bm_grayscale_check(FILE *file, int *graycheck, int *rgbcheck){
    int i, a, b, c, diag, med, h_reso, v_reso;
    bm_resolution(file, &v_reso, &h_reso);
    *rgbcheck = 0;
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

        if((a != 0) && (b != 0) && (c != 0)){
            *rgbcheck = 0;
        }
        else
            *rgbcheck = 1;
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

        if((a != 0) && (b != 0) && (c != 0)){
            *rgbcheck = 0;
        }
        else
            *rgbcheck = 1;
    }
    rewind(file);
    *graycheck = ((diag==1)&&(med==1));
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
//copy the pixel data of "file" in unsigned char array "buffer" and proceed to convert BGR to grayscale (conversion differ if the image is pur RGB or not)
void bm_grayscale_conversion(FILE *file, char *file_copy, int pur_rgb){
    FILE *bwclone;
    int offset, siz, i, j;
    unsigned char * buffer;
    unsigned char * header;
    unsigned char B, G, R, grey;
    offset = bm_pix_offset(file);
    siz = bm_size(file)-offset;
    bwclone = fopen(file_copy, "wb");
    buffer = (unsigned char *)malloc (sizeof(unsigned char)*siz);
    header = (unsigned char *)malloc (sizeof(unsigned char)*offset);

    fread(header, 1, offset, file);
    fread(buffer, 1, siz, file);
    fwrite(header, offset, 1, bwclone);
    free(header);
    //if the image is not pure RGB --- I did my best to capture every nuances of color on the test image, I don't know how well it will translate on others
    if(pur_rgb == 0){
        for(i = 0; i < siz-2; i=i+3){
            B = buffer[i];
            G = buffer[i+1];
            R = buffer[i+2];
            if (((B==255)&&(G==255))&&(R==255)){
                //if white keep white
                grey = 255;
            }
            else if((B<=255)&&(B>=50)&&(R<=255)&&(R>=200)&&(G>=10)){
                grey = (255-((255-G)/4));
            }
            else if((B < 255)&&(B >= 1)&&(R < 255)&&(R >= 1)&&((B <= R)||(R<=B))&&((B>=R-50)||(R>B-50))&&(G<10)){
                grey = (255-(255/4)) - (((255-(255/4))-B)/4);
            }
            else if(((R <= 255)&&(R >= 220))&&((B <= 70))){
                grey = (255/2) - (G/4);
            }
            else if(((G <= 255)&&(G >= 220))&&((B <= 70))){
                grey = (255/4) - ((255-R)/4);
            }
            else {
                grey = 255;
            }
            for(j = 1; j <= 3; j++){
                putc(grey, bwclone);
            }
        }
    }
    //pure RGB --- I implemented a few tolerances in the conversion in those images as well
    else{
        for(i = 0; i < siz-2; i=i+3){
            B = buffer[i];
            G = buffer[i+1];
            R = buffer[i+2];
            if (((B==255)&&(G==255))&&(R==255)){
                //if white keep white
                grey = 255;
            }
            else if((B == 255)&&(R == 255)&&(G<255)){
                grey = (255-((255-G)/4));
            }
            else if((R == 255)&&(G == 0)){
                grey = (190) - (((255-(255/4))-B)/4);
            }
            else if(((R <= 255)&&(R >= 220))&&((B == 0))){
                grey = (127) - (G/4);
            }
            else if((G <= 255)&&(G >= 220)&&(B  <= 20)){
                grey = (64) - ((255-R)/4);
            }
            else {
                grey = 255;
            }
            for(j = 1; j <= 3; j++){
                putc(grey, bwclone);
            }
        }
    }
    free(buffer);
    rewind(file);
    fclose(bwclone);
}

void gaussian_blur(FILE * file, char *gauss_copy){
    FILE *blur_clone;
    int offset, i, j=0, k, reso_h, reso_v, grey, siz;
    double gauss_kernel[3] = {0.06136, 0.24477, 0.38774};
    double po, pt, pp, ppo, ppt;
    unsigned char pmintwo, pminone, p, pplusone, pplustwo;
    unsigned char * original_and_vert_pass;
    unsigned char * hor_pass;
    unsigned char * header;

    blur_clone = fopen(gauss_copy, "wb");
    siz = bm_size(file);
    offset = bm_pix_offset(file);
    bm_resolution(file, &reso_v, &reso_h);

    original_and_vert_pass = (unsigned char *)malloc (sizeof(unsigned char)*(reso_h*reso_v));
    hor_pass = (unsigned char *)malloc (sizeof(unsigned char)*(reso_h*reso_v));
    header = (unsigned char *)malloc (sizeof(unsigned char)*offset);
    rewind(file);

    //copy and write header from original file in the new one
    fread(header, 1, offset, file);
    fwrite(header, offset, 1, blur_clone);
    free(header);
    //fill the first unsigned char buffer with pixels value to compute the horizontal pass
    //of the gaussian blur
    for(i=offset; i<siz; i=i+3){
        fseek(file, i+1, SEEK_SET);
        original_and_vert_pass[j] = getc(file);
        j++;
    }

    //fill the second unsigned char buffer with the result of the calculations of the horizontal pass
    //of the gaussian blur
    pmintwo = original_and_vert_pass[0];
    pminone = original_and_vert_pass[1];
    p = original_and_vert_pass[2];
    pplusone = original_and_vert_pass[3];
    pplustwo = original_and_vert_pass[4];
    hor_pass[0] = pmintwo;
    hor_pass[1] = pminone;
    for(i=2; i < (reso_h*reso_v)-2; i++){
        pt = (pmintwo*gauss_kernel[0]);
        po = (pminone*gauss_kernel[1]);
        pp = (p*gauss_kernel[2]);
        ppo = (pplusone*gauss_kernel[1]);
        ppt = (pplustwo*gauss_kernel[0]);
        grey = (unsigned char)(pt+po+pp+ppo+ppt);
        hor_pass[i] = grey;
        pmintwo = pminone;
        pminone = p;
        p = pplusone;
        pplusone = pplustwo;
        pplustwo = original_and_vert_pass[i+3];
    }
    hor_pass[(reso_h*reso_v)-2] = original_and_vert_pass[(reso_h*reso_v)-2];
    hor_pass[(reso_h*reso_v)-1] = original_and_vert_pass[(reso_h*reso_v)-1];


    //refill the first unsigned char buffer with the result of the calculations of the vertical_pass
    // of the gaussian blur

    for(i = 0; i < 2*reso_h; i++){
        original_and_vert_pass[i] = hor_pass[i];
    }
    for(i = 0; i < reso_h; i++){
        for(j = 2; j< reso_v-2; j++){
            pmintwo = hor_pass[((j-2)*reso_h)+i];
            pminone = hor_pass[((j-1)*reso_h)+i];
            p = hor_pass[(j*reso_h)+i];
            pplusone = hor_pass[((j+1)*reso_h)+i];
            pplustwo = hor_pass[((j+2)*reso_h)+i];

            pt = (pmintwo*gauss_kernel[0]);
            po = (pminone*gauss_kernel[1]);
            pp = (p*gauss_kernel[2]);
            ppo = (pplusone*gauss_kernel[1]);
            ppt = (pplustwo*gauss_kernel[0]);
            grey = (unsigned char)(pt+po+pp+ppo+ppt);
            original_and_vert_pass[(j*reso_h)+i] = grey;
        }
    }
    for(i = ((reso_h*reso_v)-1)-(2*reso_h); i < reso_h*reso_v; i++){
        original_and_vert_pass[i] = hor_pass[i];
    }
    for(k=0;k < (reso_h*reso_v); k++){
        for(j=0; j<3; j++){
            putc(original_and_vert_pass[k], blur_clone);
        }
    }
    free(hor_pass);
    free(original_and_vert_pass);
    fclose(blur_clone);
}


void angle_to_rgb(double angle, unsigned char *B, unsigned char *G, unsigned char *R, unsigned char saturation){
    double rad_to_deg = 180.0/PI;
    double ratio_sat, temp, pas = 250.0/60;

    angle = angle * rad_to_deg;
    saturation = 255 - saturation;
    //printf("%f\t ori\t",angle);
    if (saturation < 1){
        *B=0;*G=0;*R=0;
    }
    else{
        ratio_sat = saturation/255.0;
        if((angle < 0)&&(angle>=-360)){
            angle = (360 + angle);
        }
        //printf("%f\n",angle);
        if((angle >= 0)&&(angle < 60)){
            *B = 0;
            *R = 255-(255*ratio_sat);
            temp = (angle*pas);
            *G = (unsigned char)(temp-(temp* ratio_sat));
        }
        else if((angle >= 60)&&(angle < 120)){
            *B = 0;
            temp = (255 - ((angle-60)*pas));
            *R = (unsigned char)(temp-(temp * ratio_sat));
            *G = 255 - (255*ratio_sat);
        }
        else if((angle >= 120)&&(angle < 180)){
            temp = ((angle-120)*pas);
            *B = (unsigned char)(temp-(temp * ratio_sat));
            *R = 0;
            *G = 255 - (255*ratio_sat);
        }
        else if((angle >= 180)&&(angle < 240)){
            *B = 255 - (255*ratio_sat);
            *R = 0;
            temp = (255 - ((angle-180)*pas));
            *G = (unsigned char)(temp-(temp * ratio_sat));
        }
        else if((angle >= 240)&&(angle < 300)){
            *B = 255 - (255*ratio_sat);
            temp = ((angle-240)*pas);
            *R = (unsigned char)(temp-(temp * ratio_sat));
            *G = 0;
        }
        else if((angle >= 300)&&(angle < 360)){
            temp = (255 - ((angle-300)*pas));
            *B = (unsigned char)(temp-(temp * ratio_sat));
            *R = 255 - (255*ratio_sat);
            *G = 0;
        }
    }
}
void sobel_filter(FILE * file, char *grad, char *grad_dir){
    FILE *sobel, *sobel_angle;
    int offset, i, j=0, reso_h, reso_v, siz;
    int sobel_kernel[2][3] = {{-1, -2, -1},{1, 2, 1}};
    unsigned char pixel_matrix[3][3] = {{0, 0, 0},{0, 0, 0},{0, 0, 0}};
    unsigned char * original;
    unsigned char * filter_pass;
    unsigned char * orientation_pass;
    unsigned char * header;
    unsigned char R = 0, G = 0, B = 0;
    double angle, pixel, gx, gy;

    sobel = fopen(grad, "wb");
    sobel_angle = fopen(grad_dir, "wb");
    siz = bm_size(file);
    offset = bm_pix_offset(file);
    bm_resolution(file, &reso_v, &reso_h);
    rewind(file);

    header = (unsigned char *)malloc (sizeof(unsigned char)*offset);

    //copy and write header from original file in the new ones
    fread(header, 1, offset, file);
    fwrite(header, offset, 1, sobel);
    fwrite(header, offset, 1, sobel_angle);
    free(header);

    original = (unsigned char *)malloc (sizeof(unsigned char)*(reso_h*reso_v));
    filter_pass = (unsigned char *)malloc (sizeof(unsigned char)*(reso_h*reso_v));
    orientation_pass = (unsigned char *)malloc (sizeof(unsigned char)*((reso_h*reso_v)*3));

    //fill the first unsigned char buffer with pixels value to compute the horizontal pass
    //of the gaussian blur
    for(i=offset; i<siz; i=i+3){
        fseek(file, i+1, SEEK_SET);
        original[j] = getc(file);
        j++;
    }

    //fill the first line of the file in white so it's impossible to leave the image
    for(i = 0; i<reso_h; i++){
        filter_pass[i] = 255;
        for(j=0; j<3; j++){
            orientation_pass[(i*3)+j] = 255;
        }
    }
    //fill the last line of the file in white so it's impossible to leave the image
    for(i = ((reso_h*reso_v)-1)-reso_h; i<reso_h*reso_v; i++){
        filter_pass[i] = 255;
        for(j=0; j<3; j++){
            orientation_pass[(i*3)+j] = 255;
        }
    }

    //fill the first pixel matrix with the appropriate pixels
    pixel_matrix[0][0] = original[0];
    pixel_matrix[1][0] = original[1];
    pixel_matrix[2][0] = original[2];
    pixel_matrix[0][1] = original[reso_h];
    pixel_matrix[1][1] = original[reso_h+1];
    pixel_matrix[2][1] = original[reso_h+2];
    pixel_matrix[0][2] = original[(2*reso_h)];
    pixel_matrix[1][2] = original[(2*reso_h)+1];
    pixel_matrix[2][2] = original[(2*reso_h)+2];

    for(i = 1; i < reso_v-1; i++){
        for(j = 1; j < reso_h-1; j++){
        //compute the gradient for the first file
            gx = (pixel_matrix[2][0]*sobel_kernel[1][0])+(pixel_matrix[2][1]*sobel_kernel[1][1])+(pixel_matrix[2][2]*sobel_kernel[1][2])
                        +(pixel_matrix[0][0]*sobel_kernel[0][0]) + (pixel_matrix[0][1]*sobel_kernel[0][1])+(pixel_matrix[0][2]*sobel_kernel[0][2]);
            gy = (pixel_matrix[2][0]*sobel_kernel[0][0])+(pixel_matrix[1][0]*sobel_kernel[0][1])+pixel_matrix[0][0]*sobel_kernel[0][2]
                        +(pixel_matrix[2][2]*sobel_kernel[1][0])+(pixel_matrix[1][2]*sobel_kernel[1][1])+(pixel_matrix[0][2]*sobel_kernel[1][2]);
            pixel = sqrt(pow(gx,2)+pow(gy,2));
            filter_pass[(i*reso_h)+j] = (unsigned char)pixel;

            //move the pixel matrix one pixel further along the x axis
            pixel_matrix[0][0] = pixel_matrix[1][0];
            pixel_matrix[1][0] = pixel_matrix[2][0];
            pixel_matrix[2][0] = original[(((i-1)*reso_h)+(j+2))];
            pixel_matrix[0][1] = pixel_matrix[1][1];
            pixel_matrix[1][1] = pixel_matrix[2][1];
            pixel_matrix[2][1] = original[((i*reso_h)+(j+2))];
            pixel_matrix[0][2] = pixel_matrix[1][2];
            pixel_matrix[1][2] = pixel_matrix[2][2];
            pixel_matrix[2][2] = original[(((i+1)*reso_h)+(j+2))];

            //compute the gradient direction for the second file
            angle = atan2(gy, gx);
            angle_to_rgb(angle, &B, &G, &R, (unsigned char)pixel);

            orientation_pass[(i*(reso_h*3))+(j*3)] = B;
            orientation_pass[(i*(reso_h*3))+(j*3)+1] = G;
            orientation_pass[(i*(reso_h*3))+(j*3)+2] = R;
        }

        //move the pixel matrix one pixel further along the y axis
        pixel_matrix[0][0] = original[(i)*reso_h];
        pixel_matrix[1][0] = original[((i)*reso_h)+1];
        pixel_matrix[2][0] = original[((i)*reso_h)+2];
        pixel_matrix[0][1] = original[(i+1)*reso_h];
        pixel_matrix[1][1] = original[((i+1)*reso_h)+1];
        pixel_matrix[2][1] = original[((i+1)*reso_h)+2];
        pixel_matrix[0][2] = original[((i+2)*reso_h)];
        pixel_matrix[1][2] = original[((i+2)*reso_h)+1];
        pixel_matrix[2][2] = original[((i+2)*reso_h)+2];
    }
    free(original);

    //fill the first and last row of the file in white
    for(i = 0; i < reso_h; i = i+reso_h){
        for(j = 0; j < reso_v; j++){
            filter_pass[(j*reso_h)+i] = 255;
        }
    }

    fwrite(orientation_pass, siz-offset, 1, sobel_angle);

    for(i = 0; i < reso_h*reso_v; i++){
        for(j=0; j<3; j++){
            putc(filter_pass[i], sobel);
        }
    }

    free(filter_pass);
    free(orientation_pass);
    fclose(sobel);
    fclose(sobel_angle);
}

void next_point(img_pt start, img_pt finish, img_pt *new_pt){
    double dy, dx;
    double slope;
    dx = finish.x -  start.x;
    dy = finish.y - start.y;

    if(start.x == finish.x){
        new_pt->x = start.x;
        if(dy>0){
            new_pt->y = start.y + 1;
        }
        else if(dy<0){
            new_pt->y = start.y - 1;
        }
    }
    else if(start.y == finish.y){
        new_pt->y = start.y;
        if(dx>0){
            new_pt->x = start.x + 1;
        }
        else if(dx<0){
            new_pt->x = start.x - 1;
        }
    }
    else if((start.x == finish.x)&&(start.y == finish.y)){
        new_pt->x = finish.x;
        new_pt->y = finish.y;
    }
    else if(dx >= dy){
        new_pt->x = start.x + 1;
        new_pt->y = (int)(start.y + dy * (new_pt->x - start.x)/dx);
    }
    else if(dx < dy){
        new_pt->y = start.y + 1;
        new_pt->x = (int)(start.x + dx * (new_pt->y - start.y)/dy);
    }
}

path pathfinder(FILE * gradient, FILE * grad_angle, int header_offset, int reso_h, int reso_v, img_pt start, img_pt fin, int threshold){
    char * grad;
    char * grad_orientation;
    int x, y, slide;
    img_pt next_pt, temp;
    unsigned char gr_intensity;
    unsigned char[3] gr_bgr;

    fseek(gradient, header_offset, SEEK_SET);
    fseek(grad_angle, header_offset, SEEK_SET);

    grad = (unsigned char *)malloc (sizeof(unsigned char)*((reso_h*reso_v)*3));
    grad_orientation = (unsigned char *)malloc (sizeof(unsigned char)*((reso_h*reso_v)*3));

    fread(grad, 1, (reso_h*reso_v)*3, gradient);
    fread(grad_orientation, 1, (reso_h*reso_v)*3, grad_angle);

    threshold >= 255-16 ? slide = threshold : slide = threshold + 15);

    next_pt.x = start.x;
    next_pt.y = start.y;
    while(((next_pt.x != fin.x)&&(next_pt.y != fin.y))){
        temp.x = next_pt.x;
        temp.y = next_pt.y;
        next_point(next_pt, fin, &next_pt);
        x = next_pt.x;
        y = next_pt.y;
        gr_intensity = grad[((y*reso_h)+(x-1)*3)];
        gr_bgr[0] = grad[((y*reso_h)+(x-1)*3)];
        gr_bgr[1] = grad[((y*reso_h)+(x-1)*3)+1];
        gr_bgr[2] = grad[((y*reso_h)+(x-1)*3)+2];



        //printf("x: %d ; y: %d\n",next_pt.x, next_pt.y);
    }





}
