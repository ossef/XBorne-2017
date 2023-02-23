/*
 0 use the same interface as the Markov chain generator
 1 Simul without generating the state space first. 
 */

#define DEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* 
 inclusions des constantes, types et variables particulieres a chaque modele
 */
#include "const.h"
#include "constsim.h"
#include "var.h"


/*Variables donnant l'etendue de chaque variable isolee*/

static int Min[NEt], Max[NEt];

/* Pour  les etats  du modele */ 

typedef int Etat[NEt];

/*On utilise cet objet pour garder les transitions probabilises de la
 chaine et aussi pour garder les etats en cours de creation*/

struct item {
	Etat voisin;
	double prob;
	int numero;
	struct item *next, *last;
};

typedef struct item item;

/*---------------------------------------------*/
/* conversion d'un etat en un numero           */
/*  attention, il est possible que cela deborde*/
/*  le numero minimal est 1                    */
/*---------------------------------------------*/

int Etat2Int(E)
int *E;
{
	int i, j;
	i = E[0] - Min[0];
	for (j = 1; j < NEt; j++)
    {
		i = i * (Max[j] - Min[j] + 1) + E[j] - Min[j];
    }
	return (i + 1);
}


#include "fun.c"


void PrintEtat(E)
Etat E;
{
	int i;
	for(i=0;i<NEt;i++) {
		printf("%5d",E[i]);
	}
    printf("\n");
}


/* Programme principal et interface exterieure */

void  usage()
{
	printf("usage : Simul -f filename seed \n");
	printf("to create filename.seed.Dpath to store the path. \n");
	exit(1);
}


int main(argc,argv)
int argc;
char *argv[];
{
	Etat F, G, R;
	int j,i,t,seed;
	double pro, SP, u, moyenne[NEt];
	FILE *pf1;
	char s1[200];
    char s4[200];
	double distprob[NbEvtsPossibles+1];
    
	if (argc<3) usage();
	--argc;
	++argv;
	if (**argv != '-') usage();
	switch (*++*argv) {
		case 'f' :  /* on recupere le nom de fichier */
		{++argv;
			strcpy(s1,*argv);
            ++argv;
 			strcpy(s4,*argv);
            seed = atoi(s4);
            strcat(s1,".");
			strcat(s1,s4);
			strcat(s1,".Dpath");
			break;
		}
		default	 : usage();
	}
    
    InitEtendue();

#if (DEBUG==1)
        printf(" %s \n", s1);
#endif

    pf1=fopen(s1,"w"); 
    for(i=0;i<NEt;i++) {
        moyenne[i] = 0.0;
    }   
    
#if (DEBUG==1) 
    for(j = 0; j < NEt; j++) {
        printf(" %d %d %d ", j, Min[j], Max[j]);
    }
    printf("  \n");
#endif    
    
    EtatInitial(G);
    srand(seed);
    
    for (t   = 0; t < Tmax; t++) {
        
        
        /* give a random number u */
        
        u  = (double)rand()/(double)(RAND_MAX);
               
#if (DEBUG==1) 
        printf("random: %f  \n",u);
#endif        
        
        /* get the distribution of transition probabilities at this state */
        SP=0.0;
        for (j = 1; j <= NbEvtsPossibles; j++) {
			pro = Probabilite(j,G);
#if (DEBUG==1) 
            printf("%f ", pro);
#endif            
			SP+=pro;
            distprob[j]=SP;
        }
        
        
#if (DEBUG==1) 
        PrintEtat(G);
        for (j=1; j<=NbEvtsPossibles; j++) 
        {
            printf(" %d %f \n", j, distprob[j]);
        }
#endif        
        /* inverse transform to get the  event number: j */
        
        for (j = 1; j <= NbEvtsPossibles; j++) {
			if (u<distprob[j]) break;
        }
        
#if (DEBUG==1) 
        printf("event: %d \n",j);
#endif         
        /* Apply  event j to get the state */ 
        
        Equation(G, j, F, R);
        
        for(i=0;i<NEt;i++) {
            moyenne[i] = moyenne[i]+G[i];
        }

#if (DEBUG==2)
        printf("%d ",t);
        PrintEtat(F);
#endif
        
#if (Compact==1)         
        fprintf(pf1, "%d  ",t);
        for(i=0;i<NEt;i++) {
            fprintf(pf1,"%5d",G[i]);
        }
        fprintf(pf1,"\n");
#endif
#if (Compact==0) 
        i = Etat2Int(G)-1;
        fprintf(pf1,"%d %d \n ",t,i);
#endif
        
        for (i=0; i<NEt; i++) {G[i]=F[i];}  
    }
    fclose(pf1); 
    printf("Done Simulation \n");
    for(i=0;i<NEt;i++) {
        printf("composante %d, moyenne %f \n",i,moyenne[i]/Tmax);
    }   
    
    exit(0);
}

