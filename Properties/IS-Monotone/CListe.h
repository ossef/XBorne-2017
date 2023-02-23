#ifndef _LISTE_CHAINEE
#define _LISTE_CHAINEE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "CListe.h"         
#define EPSILON 0.000000000001 // Epsilon flotant 
#define EPSILON2 0.00000001 // Epsilon flotant 


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
Place RechercheV(Liste l,Voisin v);
double Max(double a ,double b);
Liste supprimer(Liste l,int i);  
Liste inverser_liste(Liste l);
Proba kemeP( Liste l,int k);
Voisin kemeV( Liste l,int k);
void   afficher(Liste l);      



#endif


