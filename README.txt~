Pathfinder

Projet programmation nécessaire:

damien.francois1@etu.univ-lorraine.fr


////////////
/Objective:/
////////////

Ce programme calculera, quand les informations requises seront entrées, un chemin entre deux points(les coordonnées sont entrées par l'utilisateur),
(ou une copie de l'image originale)au travers du relief/de la topographie.
En fonction des paramètre une partie de la carte sera inaccessible. 

This programme will, when the required informations are entered, calculate a path between two points (the coordinates are entered by the user),
(or a copy of the orignal image)through the relief/topography. 
Depending on the parameters some of the map will be unaccessible .
 

///////////////
/Requirements:/
///////////////

Une carte d'élévation en couleur(de bleu pour la plus basse élévation à rouge/violet/blanc pour la plus haute) ou une carte en nuances de gris
Les données du header seront considérés fiable, des données erronées mèneront à des issues imprévues.

A color coded elevation map (from blue for the lowest elevation to red/purple/white for the highest) or a  greyscale heightmap
The header data will be considered correct, erroneous data will lead to undefined behaviors.



//////////////
/User inputs:/
//////////////

L'utilisateur devra entrer:
	-le point de départ (premier clic de la souris)
	-le point d'arrivée (second clic de la souris)
	-le couple du véhicule(qui déterminera l'étendue de la carte qui lui sera accessible)

The user will be asked to enter:
	-the point of departure (first click of the mouse)
	-the point of arrival (second click of the mouse)
	-the torque of the vehicle(which will determine how much of the map is accessible to the vehicle)




///////////////////
/What will happen?/
///////////////////

Chaque nouveau fichier créé le sera dans le dossier source du fichier d'origine.
Chaque fichier est modifié en chargeant les données pixels dans un buffer d'unsigned char le filtre est alors appliqué et les données enregistrées dans un nouveau fichier, le buffer est ensuite libéré.
Le programme determinera si la bitmap est grayscale, si non elle sera converti et un nouveau fichier sera créé
Un flou gaussian sera alors appliqué pour supprimer le bruit, les données seront sauvegardés dans un nouveau fichier
La prochaine étape est de calculer le gradient de l'image avec le filtre de sobel et ensuite son angle codé en RGB(une couleur par arc de 60 degrés un pas est présent (mais commenté) dans le code pour attribuer a chaque degré une couleur différente),
les deux filtres seront appliqués et sauvegardés dans deux fichiers séparés.



Each new file created will be in the original file source folder.
Each file is modified by loading the pixel data in an unsigned char buffer and then applying the appropriate filter and saving the data in a new file, the buffer is then freed.
The software will determine if the bitmap is grayscale, and if it is not convert it and create a new file
Then it will apply a gaussian blur to the image to remove the noise, then save the data in a new file
The next step is to compute the gradient of the image with the sobel filter and then the angle of the gradient coded in RGB (one color per 6O degres span), the two filters will be applied and saved each in a separate file.





