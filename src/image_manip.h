#ifndef _IMGMANIP_
#define _IMGMANIP_

//Liste de listes pour recueillir les valeurs d'élévation de l'image (copie de plus basse résolution)
//après convertion des valeurs (RGB ou greyscale) des pixels en élévation.
typedef struct matr_col{
    matr_data d;
    struct matr_col * next_c;
}elem_matrix, * img_matrix;

//Liste de réels (pour plus de granularité, selon analyse supplémentaire utilisation potentielle d'entier)
//représentant les niveaux d'élévation ponctuels.
typedef struct list_m{
    double elev;
    struct list_m * succ;
}elem_list_m, * matr_data;

//Liste de point de la matrice traversés en route vers le point d'arrivée.
//doublement chaînée en cas de blocage pour un retour en amont plus simple pour changer de direction
//deux entier potentiellement réunit en une struture point

typedef struct list_px{
    int x,y;
    struct list_px * prev_px;
    struct list_px * next_px;
}elem_list_px, * list_path;




#endif
