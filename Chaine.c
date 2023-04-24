#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Chaine.h"
#include "SVGwriter.h"


// Allocation et Initialisation d'un point
CellPoint* creer_point(double pos_x, double pos_y){
	CellPoint* P = (CellPoint*)malloc(sizeof(CellPoint));
	if (P == NULL){
		printf("Erreur d'allocation du point");
		return NULL;
	}
	
	P->x = pos_x;
	P->y = pos_y;
	P->suiv = NULL;
	return P;
}

//Allocation et Initialisation d'une chaine
CellChaine* creer_chaine(int num){
	CellChaine* C= (CellChaine*)malloc(sizeof(CellChaine));
	if (C == NULL){
		printf("Erreur d'allocation de chaine");
		return NULL;
	}
	
	C->numero = num;
	C->points = NULL;
	C->suiv = NULL;
	return C;
}

//Allocation et Initialisation d'une liste de chaine
Chaines* creer_ListeChaine(int nb_chaines, int gamma){
	Chaines* LC = (Chaines*)malloc(sizeof(Chaines));
	if (LC == NULL){
		printf("Erreur d'allocation du réseau");
		return NULL;
	}
	
	LC->gamma = gamma;
	LC->nbChaines = nb_chaines;
	LC->chaines = NULL;
	return LC;
}

//Liberation mémoire d'un point
void libererPoint(CellPoint* P){
	free(P);
}

//Liberation mémoire d'une chaine
void libererChaine(CellChaine* C){
	while (C->points){
		CellPoint* P = C->points;
		C->points = C->points->suiv;
		libererPoint(P);
	}
	free(C);
}

//Liberation mémoire d'une liste de chaine
void libererListeChaine(Chaines* LC){
	while (LC->chaines){
		CellChaine* C = LC->chaines;
		LC->chaines = LC->chaines->suiv;
		libererChaine(C);
	}
	free (LC);
}
	
//Creation d'une liste de chaine à partir de la lecture d'un fichier
Chaines* lectureChaines(FILE *f){
	
	double x;
	double y;
	int nb_p;
	int nb_chaine;
	int g;
	Chaines* LC;
	CellChaine* C = NULL;
	CellPoint* P;
	
	fscanf(f, "NbChain: %d\n", &nb_chaine);
	fscanf(f, "Gamma: %d\n", &g);
	LC = creer_ListeChaine(nb_chaine, g);
	
	//permet d'attribuer le numéro à une chaine grâce à i
	for (int i = 0; i<nb_chaine; i++){
		LC->chaines = creer_chaine(i);
		//%*d permet de sauter le numero de la chaine
		fscanf(f, "%*d %d", &nb_p);
		P = NULL;
		for (int j=0; j<nb_p; j++){
			fscanf(f, "%lf %lf", &x, &y);
			//insertion en tete du point crée dans la liste de point
			LC->chaines->points = creer_point(x,y);
			LC->chaines->points->suiv = P;
			P = LC->chaines->points;
		}
		
		LC->chaines->points = P;
		//insertion en tete de la chaine crée dans la liste de chaine
		LC->chaines->suiv = C;
		C = LC->chaines;
	}
	return LC;
}

//Ecriture d'une liste de chaine dans un fichier selon un format d'écriture 
void ecrireChaines(Chaines* C, FILE* f){
	
	int cpt; //nombre de point par chaine
	CellChaine* L = C->chaines;
	CellPoint* P;
	
	fprintf(f, "NbChain: %d\n", C->nbChaines);
	fprintf(f, "Gamma: %d\n", C->gamma);
	
	while (L){
		cpt = 0;
		fprintf(f, "%d ", L->numero);
		P = L->points;
		
		//parcours la liste de point pour savoir combien il y a de point
		while (P){
			cpt++;
			P = P->suiv;
		}
		fprintf(f, "%d ", cpt);
		P = L->points; //P pointe a nouveau sur le premier point de la liste des points de la chaine
		
		//parcours la liste des points pour ecrire leur coordonnées
		while (P){
			fprintf(f, "%.2lf %.2lf ", L->points->x, L->points->y);
			P = P->suiv;
		}
		
		fputc('\n', f);
		L = L->suiv;
	}
}

// Affichage graphique de la liste de chaine
void afficheChainesSVG(Chaines *C, char* nomInstance){
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

//Calcule la longueur d'une chaine
double longueurChaine(CellChaine *c){	

	double s = 0; //longueur totale de la chaine
	double d; //distance entre deux points
	CellPoint* aux1 = c->points;
	CellPoint* aux2 = c->points->suiv;
	
	while (aux2){
		d = sqrt(pow((aux2->x)-(aux1->x),2) + pow((aux2->y)-(aux1->y),2));
		s+= d;
		aux1 = aux2;
		aux2 = aux2->suiv;
	}
	return s;
}


//Calcule la longueur totale d'une liste de chaine
double longueurTotale(Chaines* C){
	
	double s = 0; //longueur totale de la liste de chaine
	CellChaine* aux = C->chaines;
	while (aux){
		s+= longueurChaine(aux);
		aux = aux->suiv;
	}
	return s;
}

//Calcule le nombre total de point de la liste de chaine
int comptePointsTotal(Chaines* C){
	
	int cpt = 0; //nombre totale de points
	CellChaine* aux1 = C->chaines;
	CellPoint* aux2 = C->chaines->points;

	while (aux1){
		while(aux2){
			cpt++;
			aux2 = aux2->suiv;
		}
		aux1=aux1->suiv;
	}
	return cpt;
}
		