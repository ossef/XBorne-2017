/* 
 calcul des recompenses a partir de la matrice,
 des probabilites stationnaires, du codage des etats des composantes
 
 V1.2 le 12/7/14
 
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

long double *pi; 
long double *marginale;
int *et;

FILE *pf1; 


#include "fun.c" 
#include "funReward.c"


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
    int comp; 
    int maxcompsize; 
    long double Mean, Deviate; 
    char s1[20]; 
    char s2[20]; 
    char s3[20]; 
    char s4[20];
    
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
            strcat(s1,".sz"); 
            strcat(s2,".pi");
            strcat(s3,".cd"); 
            strcat(s4,".rwdC"); 
            /* on verifie si ils existent */ 
            if ((pf1=fopen(s1,"r"))==NULL) usage(); 
            if ((pf1=fopen(s2,"r"))==NULL) usage(); 
            if ((pf1=fopen(s3,"r"))==NULL) usage(); 
            break; 
        } 
        default : usage(); 
    } 
    
    /* On recupere les tailles dans le fichier filename.sz */
    pf1=fopen(s1,"r"); 
    fscanf(pf1,"%12d\n", &NArcs); 
    fscanf(pf1,"%12d\n",&ReachableStates); 
    printf("%12d\n", NArcs); 
    printf("%12d\n",ReachableStates); 
    fclose(pf1); 
    /* On cree les objets ou on exit(1) en cas de pb */ 
    
    InitEtendue(); 
    maxcompsize=0; 
    /* taille de la plus grande marginale */ 
    for (i = 0; i < NEt; i++) { 
        k=Max[i] - Min[i] + 1; 
        if(k>maxcompsize) maxcompsize=k;
    } 
    
    if (!(pi=(long double*)malloc(ReachableStates*sizeof(long double)))) ProblemeMemoire(); 
    if(!(marginale=(long double *)malloc(maxcompsize*sizeof(long double)))) ProblemeMemoire(); 
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
        fscanf(pf1,"%Lg \n", &pi[k]); 
    }
    fclose(pf1);
    printf("fin lecture pi \n"); 
    /* Calculs elementaires */
    pf1=fopen(s4,"w");
    for (comp = 0; comp < NEt; comp++) {
        fprintf(pf1,"distribution marginale, composante %3d\n", comp); 
        k = Max[comp] - Min[comp] + 1; 
        for (i = 0; i < k; i++) {
            marginale[i] = 0.0L;
        }
        for (j = 0; j < ReachableStates;j++) { 
            i = et[j*NEt+comp];
            marginale[i] += pi[j]; 
        } 
        for (i = 0; i < k; i++) {
            fprintf(pf1,"Etat %d, Probabilite : %Lg \n", i + Min[comp],marginale[i]);
        }

        fprintf(pf1,"Moyenne de la composante %3d en utilisant le codage",comp); 
        Mean = 0.0L; 
        for (j = 0; j < k; j++) {
            Mean += (Min[comp]+j) * marginale[j]; 
        }
        fprintf(pf1," %Lg \n", Mean);
                 
        fprintf(pf1,"Fin de la distribution marginale de la composante %d\n",comp); 
        plusieur=10;
        if (k<plusieur) {
            plusieur=k;
        }
        for (i = 1; i <= plusieur; i++) {
            Deviate=0;
            for (j = k-i; j < k; j++) {
                Deviate+=marginale[j];
            }
            fprintf(pf1,"Somme des %d derniers termes : %Lg \n",i,Deviate);
        }							

        fprintf(pf1,"Fonction de Repartition de la marginale %d\n",comp);
        Deviate=0.0L; 
        for (j = 0; j < k; j++) {
            Deviate+=marginale[j];
            fprintf(pf1,"%d %Lg \n",j,Deviate); 
        }
        MarginaleMesCalculs(comp,k);
    } 
    
    /* Calculs supplementaires */
    GlobalMesCalculs(ReachableStates);
    fclose(pf1);
    printf("Done RewardComponent\n");
    exit(0); 
}
