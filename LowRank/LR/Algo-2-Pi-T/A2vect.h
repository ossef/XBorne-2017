#ifndef VECTEUR_H // Si la constante n'a pas été définie` le fichier n'a jamais été inclus donc il entre
#define VECTEUR_H  // une fois la consante definie la prochaine fois le fichier ne sera pas inclu
#include <stdio.h>


typedef double Element;
typedef struct Vecteur
{
   int taille;
   Element*relements;
   Element*celements;
}vecteur;

//appel au fonction par prototype qui seront definis apres sur fichier.c
vecteur Alloc(int t);// creer un vecteur de taille donner
int taille(vecteur v);//donne la taille dun vecteur
vecteur inserer_ieme(vecteur *v,int i,Element rval,Element cval);// permert de modifier une composante par une valeur
void afficherMatriceInv(double **mat,int n);
void afficherMatrice(double **mat,int n) ;
void afficher(double*V,int n);// passage par valeur 


#endif
