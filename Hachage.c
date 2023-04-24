#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Hachage.h"
#include "Chaine.h"
#include "Reseau.h"


//Allocation et initialisation d'une cellule de noeud
CellNoeud* creerCellNoeudH(Noeud* N){
	CellNoeud* c = (CellNoeud*)malloc(sizeof(CellNoeud));
	if (c == NULL){
		printf("Erreur d'allocation mémoire\n");
		return NULL;
	}
	
	c->nd = N;
	c->suiv = NULL;
	
	return c;
}

//Allocation et Initialisation d'une cellule commodité
CellCommodite* creerCommoditeH(Noeud* A, Noeud* B){
	CellCommodite* C = (CellCommodite*)malloc(sizeof(CellCommodite));
	if (C == NULL){
		printf("Erreur d'allocation mémoire\n");
		return NULL;
	}
	
	C->extrA = A;
	C->extrB = B;
	C->suiv = NULL;
	return C;
}


//Allocation et initialisation de la table de hachage
TableH* creerTableHachage(int M){
	TableH* H = (TableH*)malloc(sizeof(TableH));
	if (H== NULL){
		printf("Erreur d'allocation du réseau\n");
		return NULL;
	}
	H->nb_elements = 0;
	H->taille = M;
	H->noeuds = (CellNoeud**)malloc(M*sizeof(CellNoeud*));
	if (H->noeuds == NULL){
		printf("Erreur d'allocation du réseau\n");
		free(H);
		return NULL;
	}
	for (int i=0; i<M; i++){
		H->noeuds[i] = NULL;
	}
	return H;
}

//Liberation en mémoire d'un noeud
void libererNoeudH(Noeud* N){
	CellNoeud* tmp;
    while(N->voisins){
        tmp = N->voisins->suiv;
        free(N->voisins); 
        N->voisins = tmp;
    }
    free(N);
}

//Liberation en mémoire d'une cellule de noeud
void libererCellNoeudH(CellNoeud* C){
	CellNoeud* tmp;
	while (C){
		tmp = C->suiv;
		libererNoeudH(C->nd);
		free(C);
		C = tmp;
	}
}

//Liberation en mémoire d'une table de hachage
void libererTableH(TableH* T){
	CellNoeud* temp;
	int i;
	for(i=0; i<T->taille; i++){
		while(T->noeuds[i]){
			temp = T->noeuds[i]->suiv;
			free(T->noeuds[i]);
			T->noeuds[i] = temp;
		}
	}
	free(T->noeuds);
	free(T);
}

//Calcule la clef à partir des coordonnées d'un point
double fonctionClef(double x, double y){
	double clef;
	clef = y+(x+y)*(x+y+1)/2;
	return clef;
}

//Creation de la clef hachée
int fonctionHachage(double clef, int M){
	double A = (sqrt(5)-1)/2;
	int p = (int) (clef*A);
	return (int) (M*clef*A - M*p);

}

//Recherche Recherche si un noeud est déjà présent dans le réseau à l'aide d'une table de hachage sinon l'ajoute dans le réseau ET dans la table
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableH* H, double x, double y){
	
	//Création de la clef hachée
	double clef = fonctionClef(x,y);
	int clefH = fonctionHachage(clef, H->taille);
	
	//Parcours la liste des noeuds contenu dans la case du tableau, accédé grâce à la clef hachée
	CellNoeud* n = H->noeuds[clefH];
	while (n){
		//teste pour chaque noeud si ses coordonnées sont égales à (x,y)
		if ((n->nd->x == x)&&(n->nd->y == y)){
			return n->nd; //si oui renvoi le noeud en question
		}
		n = n->suiv;
	}
	
	//Sinon création d'un nouveau noeud et d'une nouvelle cellule, pour réseau et pour la table, qui pointe vers le noeud crée
	Noeud* n_nouv = (Noeud*)malloc(sizeof(Noeud));
	n_nouv->num = (R->nbNoeuds) + 1;
	n_nouv->x = x;
	n_nouv->y = y;
	n_nouv->voisins = creerCellNoeudH(NULL);
	CellNoeud* c = creerCellNoeudH(n_nouv);
	CellNoeud* h = creerCellNoeudH(n_nouv);
	
	//Insertion de la cellule en tete de la liste de noeuds du réseau et de la liste des noeuds de la table 
	h->suiv = H->noeuds[clefH];
	H->noeuds[clefH] = h;
	H->nb_elements = (H->nb_elements)+1;
	
	c->suiv = R->noeuds;
	R->noeuds = c;
	R->nbNoeuds = (R->nbNoeuds) + 1;
	
	return NULL;
}

//Creer un réseau à partir d'une liste de chaine à l'aide d'une table de hachage
Reseau* reconstitueReseauHachage(Chaines *C, int M){
	
	//Allocation et initialisation d'un réseau
	Reseau* R = (Reseau*)malloc(sizeof(Reseau));
	if (R == NULL){
		printf("Erreur d'allocation du réseau\n");
		return NULL;
	}
	R->nbNoeuds = 0;
	R->gamma = C->gamma;
	R->noeuds = NULL;
	R->commodites = NULL;
	
	//Allocation et initialisation de la table
	TableH* H = creerTableHachage(M);
	
	int clefH;
	Noeud* n_comA;        //noeud extremite, premier point de la liste de point de la chaine
	Noeud* n_comB;        //noeud extremite, dernier point de la liste de point de la chaine
	Noeud* n_temp1;       
	Noeud* n_temp2;
	CellPoint* p_temp;
	CellCommodite* com;   //cellule commodité crée
	CellNoeud* v_temp;
	CellNoeud* v_nouv;    //nouveau voisin créer
	CellNoeud* v_nouv1;
	CellNoeud* v_nouv2;
	CellNoeud* Cn_temp;
	CellNoeud* h;
	CellChaine* Ch_temp = C->chaines;
	
	
	//Creation des noeuds pour reseau et pour la table, et creation de la liste voisin pour chaque noeuds de la table
	while (Ch_temp){
		p_temp = Ch_temp->points;
		while (p_temp->suiv){
			n_temp1 = rechercheCreeNoeudHachage(R,H,p_temp->x,p_temp->y);
			n_temp2 = rechercheCreeNoeudHachage(R,H,p_temp->suiv->x,p_temp->suiv->y);
			
			//aucun des deux noeuds n'existait dans la table donc on insère en tête de la liste de voisin de n_temp1, n_temp2, et inversement
			if ((n_temp1 == NULL)&&(n_temp2 == NULL)){
				n_temp1 = rechercheCreeNoeudHachage(R,H,p_temp->x,p_temp->y);
				n_temp2 = rechercheCreeNoeudHachage(R,H,p_temp->suiv->x,p_temp->suiv->y);
				n_temp1->voisins->nd = n_temp2;
				n_temp2->voisins->nd = n_temp1;
			}
			
			//au moins un des deux existe déjà dans la table donc on insère en tête de la liste de voisin du noeud existant, le noeud qui n'existait pas
			else if ((n_temp1 != NULL)&&(n_temp2 == NULL)){
				n_temp2 = rechercheCreeNoeudHachage(R,H,p_temp->suiv->x,p_temp->suiv->y);
				v_nouv = creerCellNoeudH(n_temp2);
				v_nouv->suiv = n_temp1->voisins;
				n_temp1->voisins = v_nouv;
				n_temp2->voisins->nd = n_temp1;
			}
			else if ((n_temp1 == NULL)&&(n_temp2 != NULL)){
				n_temp1 = rechercheCreeNoeudHachage(R,H,p_temp->x,p_temp->y);
				v_nouv = creerCellNoeudH(n_temp1);
				v_nouv->suiv = n_temp2->voisins;
				n_temp2->voisins = v_nouv;
				n_temp1->voisins->nd = n_temp2;
			}
			
			//les deux noeuds existent deja dans la table
			else {
				v_temp = n_temp1->voisins;
				//parcours la liste voisin de l'un deux
				while ((v_temp)&&(v_temp->nd->num != n_temp2->num)){
					v_temp = v_temp->suiv;
				}
				//teste si v_temp est NULL, dans ce cas cela veux dire qu'aucune liaison existe entre ces deux noeuds donc on ajoute cette liaison à l'un et à l'autre
				if (v_temp == NULL){
					v_nouv1 = creerCellNoeudH(n_temp2);
					v_nouv2 = creerCellNoeudH(n_temp1);
					v_nouv1->suiv = n_temp1->voisins;
					n_temp1->voisins = v_nouv1;
					v_nouv2->suiv = n_temp2->voisins;
					n_temp2->voisins = v_nouv2;
				}
			}
			p_temp = p_temp->suiv;
		}
		Ch_temp = Ch_temp->suiv;
	}
	
	//Creation des commodité avec insertion en tete de la liste commodité du réseau
	Ch_temp = C->chaines;
	while (Ch_temp){
		p_temp = Ch_temp->points;
		n_comA = rechercheCreeNoeudHachage(R,H,p_temp->x,p_temp->y);
		while (p_temp->suiv){
			p_temp = p_temp->suiv;
		}
		n_comB = rechercheCreeNoeudHachage(R,H,p_temp->x,p_temp->y);
		com = creerCommoditeH(n_comA, n_comB);
		com->suiv = R->commodites;
		R->commodites = com;
		Ch_temp = Ch_temp->suiv;
	}
	
	//Parcours la liste de noeuds du réseau en ajoutant à chacun leur liste de voisin contenu dans la table
	Cn_temp = R->noeuds;
	while (Cn_temp){
		clefH = fonctionHachage(fonctionClef(Cn_temp->nd->x, Cn_temp->nd->y),M);
		h = H->noeuds[clefH];
		while (h){
			if (h->nd->num == Cn_temp->nd->num){
				Cn_temp->nd->voisins =  h->nd->voisins;
				break;
			}
			h = h->suiv;
		}
		Cn_temp = Cn_temp->suiv;
	}
	libererTableH(H);
	return R;
}