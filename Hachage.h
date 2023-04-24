#ifndef _TABLE_H_
#define _TABLE_H_
#include "Reseau.h"
#include "Chaine.h"

//Tableau de Hachage 
typedef struct tableH {
    int nb_elements;      //nombre d'élément dans le tableau 
    int taille;           //taille du tableau
    CellNoeud **noeuds;   //cellule suivante 
} TableH;


CellNoeud* creerCellNoeudH(Noeud* N);
CellCommodite* creerCommoditeH(Noeud* A, Noeud* B);
TableH* creerTableHachage(int M);
void libererNoeudH(Noeud* N);
void libererCellNoeudH(CellNoeud* C);
void libererTableH(TableH* T);
double fonctionClef(double x, double y);
int fonctionHachage(double clef, int M);
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableH* H, double x, double y);
Reseau* reconstitueReseauHachage(Chaines *C, int M);
#endif