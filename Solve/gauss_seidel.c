/*
 Algorithme de Gauss Seidel
 V2.0 22/4/08
 Version initiale jmf
 Modifiee le 11/3/2004 par qst
 et le 22/4/2008 par jmf pour test de fin et forward/backward
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "verif_ext_fic.h"
#include "GaussSeidel.h"

// Possible Extensions
#define DEF_EXT "Cuu"

struct element {
    int ori;
    double prob;};

typedef struct element element;
typedef double *probavector; /* de la taille du nombre d'etats */
typedef int *indexvector; /* de la taille du nombre d'etats */
typedef element *elmtvector; /* de la taille du nombre d'arcs */

void  usage(char *nom)
{
	fprintf(stderr,"usage : %s filename\n",nom);
	fprintf(stderr,"        Stationnary distribution is in file %s_%s.pi\n",NOMBASE,EXT);
	fprintf(stderr,"\nComputes stationnary distribution using Gauss-Seidel algorithm\n");
	exit(1);
}

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory for the matrix and the probability vectors \n");
	exit(1);
}

int main(argc, argv)
int argc;
char *argv[];
{
    int NSommets, NArcs;
    int i, c, j, degre, iter, ji;
    double diff, som, y, norm;
    FILE *pf1;
    char *NOM_FIC_PI;
    /*
     Les structures dynamiques a creer par malloc
     */
    probavector p, op,vop;
    indexvector debut;
    indexvector taille;
    elmtvector arc;
    
    /*
     On recupere le nom du modele et on verifie que les fichiers existent
     */
    
    if (argc != 2) {usage_extension(DEF_EXT); usage(argv[0]);}
    if (verif_ext_fic(argv[1],DEF_EXT) == 0) {usage_extension(DEF_EXT); usage(argv[0]);}
    
    
    
    /*
     On recupere les tailles dans le fichier filename.sz
     */
    if ((pf1=fopen(NOMsz,"r"))==NULL) {perror("fopen");usage_extension(DEF_EXT); usage(argv[0]);}
    fscanf(pf1,"%d\n", &NArcs);
    fscanf(pf1,"%d\n", &NSommets);
    fclose(pf1);
    
    printf("Nb arcs = %12d\n", NArcs);
    printf("Nb som. = %12d\n", NSommets);
    printf("Iteration accuracy = %.0e\n",maxdiff);
    printf("Max number of iterations = %d\n",maxiter);
    /*
     On cree les objets ou on exit(1) en cas de pb
     */
    
    if (!(p=(double *)malloc(NSommets*sizeof(double)))) ProblemeMemoire();
        if (!(op=(double *)malloc(NSommets*sizeof(double)))) ProblemeMemoire();
            if (!(vop=(double *)malloc(NSommets*sizeof(double)))) ProblemeMemoire();
                if (!(debut=(int *)malloc(NSommets*sizeof(int)))) ProblemeMemoire();
                    if (!(taille=(int *)malloc(NSommets*sizeof(int)))) ProblemeMemoire();
                        if (!(arc=(element *)malloc(NArcs*sizeof(element)))) ProblemeMemoire();
                            
                        /*
                         Lecture de la matrice dans le fichier  passe en argument
                         */
                            fprintf(stderr,"Lecture de %s\n",NOM);
                            
                            if ((pf1=fopen(NOM,"r"))==NULL) {perror("fopen");usage_extension(DEF_EXT); usage(argv[0]);}
    c = 0;
    
    for (i = 0; i < NSommets; i++) {
        fscanf(pf1,"%d", &ji);
        fscanf(pf1,"%d", &degre);
        debut[ji] = c;
        taille[ji] = degre;
        for (j = 1; j <= degre; j++)  {
            fscanf(pf1,"%lg", &arc[c].prob);
            fscanf(pf1,"%d", &arc[c].ori);
            c++;
        }
        getc(pf1);
        // Initialisation du vecteur op
        op[i] = 1.0;
    }
    fclose(pf1);
    
    // Normalisation du vecteur op
    som = 0.0;
    for (i = 0; i < NSommets; i++)  som += op[i];
        for (i = 0; i < NSommets; i++) {
            op[i] /= som;
            vop[i]=op[i];
        }
    /*
     On commence la boucle sur la precision
     */
    iter = 0;
    do {
        iter++;
        //Iteration de Gauss Seidel
#if (Frwrd==1) // Forward
        for (i = 0; i < NSommets; i++) {
            som = 0.0;
            norm = 1.0;
            for (j = debut[i]; j < debut[i] + taille[i]; j++)
            {
                if (arc[j].ori != i) {
                    if (arc[j].ori > i) y = op[arc[j].ori];
                    else y = p[arc[j].ori];
                    som += y * arc[j].prob;
                }
                else norm = 1.0 - arc[j].prob;
            }
            p[i] = som/norm;
        }
#endif
#if (Frwrd==0) // Backward
        for (i = NSommets-1; i > 0 ; i--) {
            som = 0.0;
            norm = 1.0;
            for (j = debut[i]; j < debut[i] + taille[i]; j++)
            {
                if (arc[j].ori != i) {
                    if (arc[j].ori > i) y = p[arc[j].ori];
                    else y = op[arc[j].ori];
                    som += y * arc[j].prob;
                }
                else norm = 1.0 - arc[j].prob;
            }
            p[i] = som/norm;
        }
#endif
        //Normalisation et est de la norme de la difference
        //apres mmm etapes
        if ((iter%mmm)==0) {
            som = 0.0;
            for (i = 0; i < NSommets; i++) som += p[i];
            for (i = 0; i < NSommets; i++) p[i] /= som;
#if (TestFin==0)
            diff = 0.0;
            for (i = 0; i < NSommets; i++) diff += fabs(p[i] - vop[i]);
#endif
#if (TestFin==1)
            diff = 0.0;
            for (i = 0; i < NSommets; i++) {
                if (vop[i]>0) {
                    diff += fabs(p[i] - vop[i])/vop[i];
                }
                else {
                    diff += fabs(p[i]);
                }
            }
#endif
        }
        else {diff=1.0;}
        
        if ((iter%mmm)==0) {printf("Iteration %d %e \n", iter,(float)diff);}
        
        for (i = 0; i < NSommets; i++)  op[i] = p[i];
        if ((iter % mmm)==0) {
            for (i = 0; i < NSommets; i++)  vop[i] = p[i];
        }
    } while ((iter != maxiter) && (diff >= maxdiff));
    
    // Ecriture du resultat
    NOM_FIC_PI = (char*)malloc((strlen(NOMBASE)+strlen(EXT)+7)*sizeof(char));
    if (NOM_FIC_PI == NULL) ProblemeMemoire();
        sprintf(NOM_FIC_PI,"%s_%s.pi",NOMBASE,EXT);
        if ((pf1=fopen(NOM_FIC_PI,"w")) == NULL) {perror("fopen");usage_extension(DEF_EXT); usage(argv[0]);}
    for (i = 0; i < NSommets; i++) {fprintf(pf1,"% .20E\n", p[i]);}
    fclose(pf1);
    
    fprintf(stderr,"\n");
    exit(0);
}

