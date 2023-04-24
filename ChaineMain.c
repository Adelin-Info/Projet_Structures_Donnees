#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chaine.h"
#include "SVGwriter.h"

// 3 paramètres : les nom des fichiers pour creer la chaine, pour enregistrer la chaine et pour contenir le graphe de la chaine
int main(int argc, char** argv){
	if (argc != 4){
		printf("Il manque des paramètres\n");
		return -1;
	}
	
	//creation de la chaine
	FILE* nom_fic = fopen(argv[1], "r");
	if (nom_fic == NULL){
		printf("Problème d'ouverture du fichier\n");
		return 0;
	}
	Chaines* R = lectureChaines(nom_fic);
	fclose(nom_fic);
	
	//enregistrer la chaine
	FILE* fic = fopen(argv[2], "w");
	ecrireChaines(R, fic);
	fclose(fic);
	
	//enregistrer le graphe
	afficheChainesSVG(R, argv[3]);

	return 1;
}