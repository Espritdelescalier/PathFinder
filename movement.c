#include <math.h>
#include "movement.h"


//Fonction addition graphique de deux vecteurs m_vect a et b
//Convention: coordonnées écran
//retourne un vecteur m_vect
m_vect add_vect(m_vect a,m_vect b){
    m_vect fin;
    fin.head.x = a.head.x+(b.head.x-b.tail.x);
    fin.head.y = a.tail.x+(b.tail.y-b.head.y);
    fin.tail.x = a.tail.x;
    fin.tail.y = a.tail.y;
    return fin;
}

//Fonction calcul la longueur entière d'un vecteur m_vect.
//retourne un entier
int long_vect(m_vect v){
    return sqrt(pow((v.head.x-v.tail.x), 2)+pow((v.tail.y-v.head.y), 2));
}

//Calcule la pente d'un vecteur
//retourne un réel
double slope_comp(m-vect v){
    double slope;
    slope = (double)(v.head.x-v.tail.x)/(v.tail.y-v.head.y));
    return slope;
}

pos_grid next_pos(m_vect v, pos_grid pos){




}
