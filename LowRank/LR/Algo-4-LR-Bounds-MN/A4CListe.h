#ifndef _LISTE_CHAINEE
#define _LISTE_CHAINEE
#include<stdio.h>

typedef double Proba;
typedef int Voisin;

typedef struct cellule *Place;

typedef struct cellule{
        Voisin voisin;
        Proba proba;
        struct cellule *suivant;
        }Cellule;
        
typedef Cellule *Liste;

Liste liste_vide();
int longueur(Liste l);
Liste inserer(Liste l,int i, Voisin vo, Proba pr);
Place acces(Liste l,int k);
Place succ(Liste l, Place p);
Proba contenuP(Liste l,Place p);
Voisin contenuV(Liste l,Place p);
Proba kemeP( Liste l,int k);
Voisin kemeV( Liste l,int k);
Place RechercheV(Liste l,Voisin v);
double Max(double a ,double b);
Liste supprimer(Liste l,int i);  
Liste inverser_liste(Liste l);
void   afficher(Liste l);      



#endif


