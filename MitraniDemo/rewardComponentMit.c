// fichier rewardComponent.c pour modele de Mitrani

/* 
 calcul des recompenses a partir de la matrice,
 des probabilites stationnaires, du codage des etats des composantes
 
 V2.0 le 04/05/16
 
 Hypotheses  de fonctionnement : 
 l'argument est le nom du modele : filename 
 la taille est dans le fichiez filename.sz
 le codage dans filename.cd, 
 et la distribution dans filename.pi
 */

#include <stdio.h> 
#include <math.h>
#include <string.h>
#include <stdlib.h>

/* 
 inclusions des constantes, types et variables particulieres a chaque modele
 */

#include "const.h"

/* Variables donnant l'etendue de chaque variable isolee */ 
int Min[NEt],Max[NEt]; 
typedef int Etat[NEt];

/* Le nombre d'etats atteignables et de transitions */ 

int ReachableStates, NArcs;

/* la distribution stationnaire et la marginale en cours */ 

double *pi; 
long double *marginale;
int *et;

FILE *pf1; 


#include "fun.c" 


void usage() { 
    printf("usage : RewardComponent -f filename \n");
    printf("filename.pi, filename.sz and filename.cd must exist before \n");
    exit(1); 
}

void ProblemeMemoire() { printf("Sorry, not enougth memory \n"); exit(1); }


int main(argc, argv) 
int argc; 
char *argv[]; 
{ 
    int i,j, k, plusieur, nb ; 
    int up,down;
    int comp; 
    int maxcompsize; 
    long double Mean, Deviate; 
    char s1[20]; 
    char s2[20]; 
    char s3[20]; 
    char s4[20];
    char s5[20];
    
    /* On recupere le nom du modele et on verifie que les fichiers existent */
    
    if (argc!=3) usage();
    --argc; ++argv; 
    if (**argv != '-') usage(); 
    switch (*++*argv) { 
        case 'f' : { 
            /* on recupere le nom de fichier */
            ++argv;
            strcpy(s1,*argv); 
            /* on ajoute .sz, .pi et .cd */ 
            strcpy(s2,s1);
            strcpy(s3,s1); 
            strcpy(s4,s1); 
            strcpy(s5,s1); 
            strcat(s1,".sz"); 
            strcat(s2,".pi");
            strcat(s3,".cd"); 
            //strcat(s4,".rwdC"); 
            strcat(s5,".dat"); 
            /* on verifie si ils existent */ 
            if ((pf1=fopen(s1,"r"))==NULL) usage(); 
            if ((pf1=fopen(s2,"r"))==NULL) usage(); 
            if ((pf1=fopen(s3,"r"))==NULL) usage(); 
            if ((pf1=fopen(s5,"r"))==NULL) usage(); 
            
            break; 
        } 
        default : usage(); 
    } 
    
    /* On recupere les tailles dans le fichier filename.sz */
    pf1=fopen(s1,"r"); 
    fscanf(pf1,"%12d\n", &NArcs);  // elements non nul
    fscanf(pf1,"%12d\n",&ReachableStates); // nombre d'etats
    printf("%12d\n", NArcs); 
    printf("%12d\n",ReachableStates); 
    fclose(pf1); 
    
     /* On recupere donnes up et down filename.dat */
    pf1=fopen(s5,"r"); 
    fscanf(pf1,"%d\n", &up);  // elements non nul
    fscanf(pf1,"%12d\n",&down); // nombre d'etats
    printf("up : %12d\n",up ); 
    printf("down : %12d\n",down); 
    fclose(pf1); 
  
    
    if (!(pi=(double*)malloc(ReachableStates*sizeof(double)))) ProblemeMemoire(); 
    if (!(et=(int*)malloc(ReachableStates*NEt*sizeof(int)))) ProblemeMemoire();
    
    printf("debut lecture code \n"); 
    
    /* Lecture de la matrice de codage dans le fichier filename.cd */
    pf1=fopen(s3,"r"); 
    for (k = 0; k < ReachableStates; k++) {
        fscanf(pf1,"%d", &j);
        for (i = 0; i < NEt; i++) {
            fscanf(pf1,"%d",&nb);
            et[i+NEt*j] = nb; 
        } /*et[j][i] */ 
        getc(pf1);
    }
    fclose(pf1);
    printf("fin lecture code \n"); 
    
    /*   Lecture du vecteur pi dans le fichier filename.pi */
    pf1=fopen(s2,"r");
    printf("debut lecture pi \n");
    for (k = 0; k < ReachableStates; k++) {
        fscanf(pf1,"%lf \n", &pi[k]); 
    }
    fclose(pf1);
    printf("fin lecture pi \n"); 
    
    /* Calcule de la probabilité de perte de clients dans le systéme */
    int indice1,indice2;
    indice1=-1;indice2=-1;
    for(i=0; i<ReachableStates*NEt ; i=i+2){
        printf ("%d , %d \n",et[i],et[i+1]);  
    }
    for (k = 0; k < ReachableStates; k++) {
        printf("pi[%d] = %f \n",k,pi[k]); 
    }
    for(i=0; i<ReachableStates*2 ; i=i+2){
        if(et[i] == BufferSize & et[i+1] == 1)
        indice1 =  (int)i/2;
        if(et[i] == BufferSize & et[i+1] == 2)  
        indice2 = (int)i/2;
    }
    printf("La probabilite de perte est : %f \n",pi[indice1]+pi[indice2]); 
    
    fclose(pf1);
    printf("Done RewardComponent\n");
    exit(0); 
}
