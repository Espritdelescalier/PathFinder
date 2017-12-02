Pathfinder

Projet programmation nécessaire:


////////////
/Objective:/
////////////
This programme will, when the required informations are entered, calculate a path between two points (the coordinates are entered by the user),
 and then trace this path on the image (or a copy of the orignal image)through the relief/topography. 
Depending on the parameters some of the map will be unaccessible the goal being to find one of the best (read the most energy efficient ,
well not the most I'm not that presumptuous) path on the map.
 

Ce programme calculera, quand les informations requises seront entrées, un chemin entre deux points(les coordonnées sont entrées par l'utilisateur),
et tracera alors ce chemin sur l'image(ou une copie de l'image originale)au travers du relief/de la topographie.
En fonction des paramètre une partie de la carte sera inaccessible, le but étant de trouver un des meilleur (comprendre un des moins consommateur
énergétiquement, enfin pas tout à fait le moins consommateur je ne suis pas si présomptueux) chemin sur la carte. 

///////////////
/Requirements:/
///////////////
A color coded elevation map (from violet/blue for the lowest elevation to red for the highest) or a  greyscale heightmap (still undecided)

Une carte d'élévation en couleur(de violet/bleu pour la plus basse élévation à rouge pour la plus haute) ou une carte en nuances de gris (toujours en analyse)

//////////////
/User inputs:/
//////////////
The user will be asked to enter:
	-the point of departure
	-the point of arrival
	-the torque of the vehicle(which will determine how much of the map is accessible to the vehicle)
	-lowest elevation on land
	-highest elevation

L'utilisateur devra entrer:
	-le point de départ
	-le point d'arrivée
	-le couple du véhicule(qui déterminera l'étendue de la carte qui lui sera accessible)
	-la plus basse altitude au sol
	-la plus haute altitude



////////////
/structures/
////////////


//Liste de réels (pour plus de granularité, selon analyse supplémentaire utilisation potentielle d'entier)
//représentant les niveaux d'élévation ponctuels

typedef struct list_m{
    double elev;
    struct list_m * succ;
}elem_list_m, * matr_data;


//Matrice (liste de listes) pour recueillir les valeurs d'élévation de l'image (copie de plus basse résolution)
//après convertion des valeurs (RGB ou greyscale) des pixels en élévation

typedef struct matr_col{
    matr_data d;
    struct matr_col * next_c;
}elem_matrix, * img_matrix;



//Liste de point de la matrice traversés en route vers le point d'arrivée. Constituera le chemin a tracer.
//doublement chaînée en cas de blocage pour un retour en amont plus simple pour changer de direction
//deux entiers potentiellement réunit en une struture point

typedef struct list_px{
    int x,y;
    struct list_px * prev_px;
    struct list_px * next_px;
}elem_list_px, * list_path;


//Structure vecteur m_vect
//coordonnées de la tete du vecteur head (head étant une struct pos_grid avec deux entiers x et y)
//coordonnées de la queue du vecteur tail (tail étant une struct pos_grid avec deux entiers x et y)
//Possibilité d'en faire une structure dynamique
//le déplacement entre les deux points sera vraisemblablement (sauf changement dramatique dans l'analyse du problème)
//basé sur le vecteur lié au couple du véhicule et les vecteurs liés au changement d'élévation.

typedef struct{
    pos_grid head, tail;
}m_vect;
