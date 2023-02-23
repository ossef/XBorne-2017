/*
 Use the same interface as the Markov chain generator
 Simul without generating the state space first.
 Divide the path into regenerative cycles
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
#include "constregeneration.h"
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

/*-------------------------------------------------------------*/
/* conversion d'un etat en un numero; le numero minimal est 1  */
/*-------------------------------------------------------------*/

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
	printf("usage : RegenerativeSimul -f filename seed \n");
	printf("to create filename.seed.cycle.Dpath to store the path divided into regenerative cycles. \n");
	exit(1);
}


int main(argc,argv)
int argc;
char *argv[];
{
	Etat F, G, R;
	int j,i,t,seed, cycle, indexforregeneration, lencycl, realcycle  ;
	double pro, SP, u;
	FILE *pf1;
	char s1[200];
    char s2[10];
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
 			break;
		}
		default	 : usage();
	}
    
    InitEtendue();
    cycle =0;
    lencycl=0;
    realcycle=0;
    strcpy(s4,s1);
    i= sprintf(s2,".%d.path", cycle);
    strcat(s4,s2);
    pf1=fopen(s4,"w");

    
#if (DEBUG==1) 
    for(j = 0; j < NEt; j++) {
        printf(" %d %d %d ", j, Min[j], Max[j]);
    }
    printf("  \n");
#endif    
    
    EtatInitial(G);
    srand(seed);
    indexforregeneration = Etat2Int(G);
    for (t   = 0; t < Tmax; t++) {
        
        lencycl++;
        /* give a random number u */
        u  = (double)rand()/(double)(RAND_MAX);
               
#if (DEBUG==1) 
        printf("random: %f  \n",u);
#endif        
        
        /* get the distribution of transition probabilities at this state */
        SP=0.0;
        for (j = 1; j <= NbEvtsPossibles; j++) {
			pro = Probabilite(j,G);
            SP+=pro;
            distprob[j]=SP;
#if (DEBUG==1)
            printf("%f ", pro);
#endif            
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

#if (DEBUG==2)
        printf("%d ",t);
        PrintEtat(F);
#endif
        
        
        for (i=0; i<NEt; i++) {G[i]=F[i];}
        
        /* checking for reneration point */
        i= Etat2Int(G);
        
        if (i!=indexforregeneration) {realcycle=1;}
        if ((i==indexforregeneration) && (lencycl> MinimalCycleLength) && (realcycle==1)) {
            cycle++;
            realcycle=0;
            lencycl=0;
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
            fclose(pf1);
            strcpy(s4,s1);
            i= sprintf(s2,".%d.Dpath", cycle);
            strcat(s4,s2);
            pf1=fopen(s4,"w");
        }
        
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

    }
    fclose(pf1);
    printf("Done RegenerativeSimulation \n");
    exit(0);
}

