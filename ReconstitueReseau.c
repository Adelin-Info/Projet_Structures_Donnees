#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Reseau.h"
#include "Chaine.h"
#include "SVGwriter.h"
#include "Hachage.h"

//Menu qui apparaitra lorsqu'on executera le programme
void menu(){
	printf("Quel méthode souhaitez vous utiliser pour construire un reseau ?\n");
	printf("0-Sortir du programme\n");
	printf("1-Liste Chainée\n");
	printf("2-Table de Hachage\n");
	printf("3-Arbre\n");
}

//demande 1 paramètre lors de l'execution, le nom du fichier à partir duquel on pourra creer une chaine 
int main(int argc, char** argv){
	if (argc != 2){
		printf("Il manque des paramètres\n");
		return -1;
	}
	
	FILE* nom_fic = fopen(argv[1], "r");
	if (nom_fic == NULL){
		printf("Problème d'ouverture du fichier\n");
		return 0;
	}
	Chaines* C = lectureChaines(nom_fic); //creation de la chaine
	fclose(nom_fic);
	
	int mode;
	int taille;
	char nom1[50];
	char nom2[50];
	Reseau* R_LC;
	Reseau* R_H;
	
	do{
		//apparition du menu demandant la methode à l'utilisateur
		menu();  
		printf("Tapez la méthode souhaité: ");
		scanf("%d", &mode);
		switch (mode){
		
		case 1: //Liste Chainee
			R_LC = reconstitueReseauListe(C); //creation du reseau
			
			printf("\nLISTE CHAINEE\n");
			
			//demande à l'utilisateur le nom du ficher dans lequel on enregistera le reseau
			printf("Entrez le nom du ficher pour enregistrer le reseau: ");
			scanf("%s",nom1);
			FILE* f_c = fopen(nom1, "w");
			if (f_c == NULL){
				printf("Problème d'ouverture du fichier\n");
				return 0;
			}
			ecrireReseau(R_LC, f_c);
			fclose(f_c);
			
			//demande le nom du ficher qui contiendra le graphique
			printf("Entrez le nom du fichier qui contiendra le graphe du reseau: ");
			scanf("%s",nom2);
			afficheReseauSVG(R_LC, nom2);
			
			printf("\nLe reseau a bien été crée, ainsi que son graphe, et a été enregistrer dans les fichiers souhaitées\n");
			libererReseau(R_LC);
			break;
		
		case 2: //Table de Hachage
		
			printf("\nTABLE HACHAGE\n");
			
			//demande à l'utilisateur la taille du tableau de hachage qu'il souhaite
			printf("Entrez la taille du tableau de hachage : ");
			scanf("%d",&taille);
			R_H = reconstitueReseauHachage(C,taille); //creation du reseau
			
			//demande à l'utilisateur le nom du ficher dans lequel on enregistera le reseau
			printf("Entrez le nom du fichier dans lequel vous souhaiter enregistrer le reseau: ");
			scanf("%s", nom1);
			FILE* f_h = fopen(nom1, "w");
			if (f_h == NULL){
				printf("Problème d'ouverture du fichier\n");
				return 0;
			}
			ecrireReseau(R_H, f_h);
			fclose(f_h);
			
			//demande le nom du ficher qui contiendra le graphique
			printf("Entrez le nom du fichier qui contiendra le graphe du reseau: ");
			scanf("%s", nom2);
			afficheReseauSVG(R_H, nom2);
			
			printf("\nLe reseau a bien été crée, ainsi que son graphe, et a été enregistrer dans les fichiers souhaitées\n");
			libererReseau(R_H);
			break;
			
		case 3: //Arbre
			break;
		}
	}while(mode!=0);
	libererListeChaine(C);
	printf("Merci et aurevoir!\n");	
	return 1;
}

	