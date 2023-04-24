#include <stdio.h>
#include <stdlib.h>
#include "ArbreQuat.h"

//Coordonnées minimales et maximales que peuvent avoir les points de la liste des chaines
void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
	CellChaine* ch_temp = C->chaines;
	//initialisation des coordonnées minimales et maximales avec les coordonnées du premier point de la première chaine
	xmin = ch_temp->points->x;  
	xmax = ch_temp->points->x;	
	ymin = ch_temp->points->y;	
	ymax = ch_temp->points->y;	
	
	while (ch_temp){
		CellPoint* p_temp = ch_temp->points;
		while (p_temp){
			//teste sur la coordonnées x
			if ((p_temp->x < xmin){
				xmin = p_temp->x; 
			}
			else if (p_temp->x > xmax){
				xmax = p_temp->x;
			}
			
			//teste sur la coordonnées y 
			if ((p_temp->y < ymin){
				ymin = p_temp->y;
				
			}
			else if (p_temp->y > ymax){
				ymax = p_temp->y;
			}			
			p_temp = p_temp->suiv;
		}
		ch_temp = ch_temp->suiv;
	}
}

//Allocation et initialisation d'un arbre
ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){
	ArbreQuat* A = (ArbreQuat*)malloc(sizeof(ArbreQuat));
	if (A == NULL){
		printf("Erreur d'allocation mémoire\n");
		return NULL;
	}
	
	A->xc = xc;
	A->yc = yc;
	A->coteX = coteX;
	A->coteY = coteY;
	A->noeud = NULL;
	A->so = NULL;
	A->se = NULL;
	A->no = NULL;
	A->ne = NULL;
	
	return A;
}

	
	