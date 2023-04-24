#include <stdio.h>
#include <stdlib.h>
#include "Reseau.h"
#include "Chaine.h"
#include "SVGwriter.h"

// Allocation et initialisation d'une cellule noeud
CellNoeud* creerCellNoeud(Noeud* N){
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
CellCommodite* creerCommodite(Noeud* A, Noeud* B){
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

//Liberation mémoire d'un noeud
void libererNoeud(Noeud* N){
    CellNoeud* tmp;
    while(N->voisins){
        tmp = N->voisins->suiv;
        free(N->voisins); 
        N->voisins = tmp;
    }
    free(N);
}

//Liberation mémoire d'une cellule de noeud
void libererCellNoeud(CellNoeud* CN){
    CellNoeud* tmp;
	while (CN){
		tmp = CN->suiv;
		libererNoeud(CN->nd);
		free(CN);
		CN = tmp;
	}
}

//Liberation mémoire d'une cellule commodite
void libererCommodite(CellCommodite* CC){
	CellCommodite* tmp;
	while (CC){
		tmp = CC->suiv;
		free(CC);
		CC = tmp;
	}
}

//Liberation mémoire d'un reseau
void libererReseau(Reseau* R){
	libererCellNoeud(R->noeuds);
	libererCommodite(R->commodites);
	free(R);
}

//Recherche si un noeud est déjà présent dans le réseau sinon l'ajoute 
Noeud* rechercheCreeNoeudListe(Reseau* R, double x, double y){
	CellNoeud* aux = R->noeuds;
	
	//parcours la liste de noeuds
	while (aux){
		
		//teste pour chaque noeud si ses coordonnées sont égales à (x,y)
		if ((aux->nd->x == x) && (aux->nd->y == y)){
			return aux->nd; //si oui renvoi le noeud en question
		}
		aux = aux->suiv;
	}
	
	//Sinon création d'un nouveau noeud et d'une nouvelle cellule qui pointe vers le noeud crée
	Noeud* n = (Noeud*)malloc(sizeof(Noeud));
	n->num = (R->nbNoeuds) + 1;
	n->x = x;
	n->y = y;
	n->voisins = creerCellNoeud(NULL);
	CellNoeud* c = creerCellNoeud(n);	
	
	//Insertion de la cellule en tete de la liste de noeuds 
	c->suiv = R->noeuds;
	R->noeuds = c;
	R->nbNoeuds = (R->nbNoeuds)+1; //mise à jour du nombre de noeuds du reseau
	
	return n;
}


//Creer un réseau à partir d'une liste de chaine
Reseau* reconstitueReseauListe(Chaines* C){
	
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
	
	CellCommodite* com;  //cellule commodité crée
	Noeud* n_extrA;      //noeud extremite, premier point de la liste de point de la chaine
	Noeud* n_extrB;      //noeud extremite, dernier point de la liste de point de la chaine
	Noeud* n_fa;		 //noeud facultif 
	Noeud* n_temp;       //noeud temporaire
	Noeud* n_prec;       //noeud précédent
	CellNoeud* v_prec;   //voisin précédent
	CellNoeud* v_nouv;   //nouveau voisin crée
	CellNoeud* v_fa;     //voisin facultatif 
	CellPoint* p_temp;   
	CellChaine* c_temp = C->chaines;
	int i = 0;
	
	while (i < C->nbChaines){
		p_temp = c_temp->points;
		n_prec = NULL;
		n_extrA = rechercheCreeNoeudListe(R, p_temp->x, p_temp->y); //stocke le noeuds correspondant au premier point de la chaine
		
		//parcours la liste des points de la chaine
		while (p_temp->suiv){
			n_temp = rechercheCreeNoeudListe(R, p_temp->x, p_temp->y);
			
			
			/* Ajout du noeuds correspondant au point suivant dans la liste des voisins du noeuds correspondant
			au point actuel */
			
			//si la premiere cellule voisin pointe sur un noeud
			if (n_temp->voisins->nd != NULL){
				v_fa = n_temp->voisins;
				n_fa = rechercheCreeNoeudListe(R, p_temp->suiv->x, p_temp->suiv->y);
				
				//parcours la liste des voisins
				while (v_fa){
					//si le noeud est deja présent dans la liste de voisin, on sort de la boucle prématurément
					if (v_fa->nd->num == n_fa->num){
						break;
					}
					v_fa = v_fa->suiv;
				}
				
				//teste si on est sorti prématurément ou non de la boucle d'avant
				if (v_fa == NULL){
					//si non on crée une nouvelle cellule voisin dont on l'insère en tête de la liste des voisins 
					v_nouv = creerCellNoeud(n_fa);
					v_nouv->suiv = n_temp->voisins;
					n_temp->voisins = v_nouv;
				}
			}
			
			//si la première cellule voisin pointe sur rien
			else {
				n_temp->voisins->nd = rechercheCreeNoeudListe(R, p_temp->suiv->x, p_temp->suiv->y);
				n_temp->voisins->suiv = NULL;
			}
			
			
			/* Ajout du noeuds correspondant au point actuel dans la liste des voisins du noeuds correspondant
			au point suivant, en faisant la même démarche */
			
			n_prec = n_temp->voisins->nd;
			if (n_prec->voisins->nd == NULL){
				n_prec->voisins->nd = n_temp;
				n_prec->voisins->suiv = NULL;
			}
			
			else {
				v_fa = n_prec->voisins;
				while (v_fa){
					if (v_fa->nd->num == n_temp->num){
						break;
					}
					v_fa = v_fa->suiv;
				}
				
				if (v_fa == NULL){
					v_prec = creerCellNoeud(n_temp);
					v_prec->suiv = n_prec->voisins;
					n_prec->voisins = v_prec;
				}
			}
			p_temp = p_temp->suiv;
		}
		n_extrB = rechercheCreeNoeudListe(R, p_temp->x, p_temp->y); //stocke le noeuds correspondant au dernier point de la chaine
		
		//Creation d'une nouvelle cellule de commodite en l'insérant en tête de la liste de commodité du réseau
		com = creerCommodite(n_extrA, n_extrB); 
		com->suiv = R->commodites;
		R->commodites = com;
		
		c_temp = c_temp->suiv;
		i++;
	}
	return R;
}

//Calcule le nombre de commodité totale d'un réseau
int nbCommodites(Reseau *R){
	int cpt = 0;
	CellCommodite* com = R->commodites;
	
	while (com){
		cpt++;
		com = com->suiv;
	}
	return cpt;
}

//Calcule le nombre totale de liaisons dun réseau
int nbLiaisons(Reseau* R){
	int cpt = 0;
	CellNoeud* c = R->noeuds;
	
	while (c){
		CellNoeud* v = c->nd->voisins;
		
		while (v){
			cpt++;
			v = v->suiv;
		}
		
		c = c->suiv;
	}
	return cpt/2; //on divise par deux car chaque liaisons existe deux fois
}


//Ecriture d'un réseau dans un fichier selon un format d'écriture
void ecrireReseau(Reseau* R, FILE *f){
	
	fprintf(f, "NbNoeuds: %d\n", R->nbNoeuds);
	fprintf(f, "NbLiaisons: %d\n", nbLiaisons(R));
	fprintf(f, "NbCommodites: %d\n", nbCommodites(R));
	fprintf(f, "Gamma: %d\n", R->gamma);
	fputc('\n',f);
	
	CellNoeud* v;
	
	//Ecrit l'ensemble des noeuds du réseau
	CellNoeud* c = R->noeuds;
	while (c){
		fprintf(f, "v %d %.2lf %.2lf\n", c->nd->num, c->nd->x, c->nd->y);
		c = c->suiv;
	}
	fputc('\n',f);
	
	//Ecrit l'ensemble de liaison du réseau
	c = R->noeuds;
	while (c){
		v = c->nd->voisins;
		while (v){
			fprintf(f, "l %d %d\n", c->nd->num, v->nd->num);
			v = v->suiv;
		}
		c = c->suiv;
	}
	fputc('\n',f);
	
	//Ecrit l'ensemble des commodité du réseau
	CellCommodite* com = R->commodites;
	while (com){
		fprintf(f, "k %d %d\n", com->extrA->num, com->extrB->num);
		com = com->suiv;
	}
}

//Affichage graphique du reseau
void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}