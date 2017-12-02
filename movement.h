#ifndef _MOVEMENT_
#define _MOVEMENT_


//Structure point de la grille
//coordonnées entières x et y
typedef struct{
    int x,y;
}pos_grid;


//Structure vecteur m_vect
//coordonnées de la tete du vecteur x_head et y_head
//coordonnées de la queue du vecteur x_tail et y_tail

typedef struct{
    pos_grid head, tail;
}m_vect;

//Fonction addition graphique de deux vecteurs m_vect a et b
//Convention: coordonnées écran
//retourne un vecteur m_vect
m_vect add_vect(m_vect, m_vect);

//Fonction calcul la longueur entière d'un vecteur m_vect.
//retourne un entier
int long_vect(m_vect);

//Calcule la pente d'un vecteur
//retourne un réel
double slope_comp(m-vect);

//Determine le prochain point de la grille vers lequel se déplacer
//retourne un point de la grille  pos_grid

pos_grid next_pos(m_vect, pos_grid);

#endif
