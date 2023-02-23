/*
 Simulation in continuous time
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 inclusions des constantes, types et variables particulieres a chaque modele
*/
#include "const.h"
#include "constcontinuoussim.h"
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
	printf("usage : SimulContinuous -f filename seed \n");
	printf("to create filename.seed.Cpath to store the path. \n");
	exit(1);
}


int main(argc,argv)
int argc;
char *argv[];
{
	Etat F, G, R;
	int j,i,seed, looping;
	double pro, SP, u1,u2, deltat, t;
	FILE *pf1;
	char s1[20];
    char s4[20];
	double distprob[NbEvtsPossibles+1];
    
	if (argc<3) usage();
        --argc;
	++argv;
	if (**argv != '-') usage();
        switch (*++*argv) {
            case 'f' :
            {++argv;
                strcpy(s1,*argv);
                ++argv;
                strcpy(s4,*argv);
                seed = atoi(s4);
                strcat(s1,s4);
                strcat(s1,".Cpath");
                break;
            }
            default	 : usage();
        }
    
    InitEtendue();
	
    pf1=fopen(s1,"w");
    
#if (DEBUG==1)
    for(j = 0; j < NEt; j++) {
        printf(" %d %d %d %f \n", j, Min[j], Max[j], SumOfRates);
    }
    printf("  \n");
#endif
    
    EtatInitial(G);
    srand(seed);
    
    t=0.0;
    do {
        
        /* give two random numbers u1 and u2 */
        
        u1  = (double)rand()/(double)(RAND_MAX);
        u2  = (double)rand()/(double)(RAND_MAX);
        
        
#if (DEBUG==1)
        printf("random: %f %f \n",u1,u2);
#endif
        
        /* exponential delay with rate SumOfRates */
        deltat  = -log(u2)/SumOfRates;
        
        /* get the distribution of transition probabilities at this state */
        SP=0.0;
        for (j = 1; j <= NbEvtsPossibles; j++) {
			pro = Probabilite(j,G); /* Here it is a rate, despite the name */
            pro /= SumOfRates;
			SP+=pro;
            distprob[j]=SP;
#if (DEBUG==1)
            printf(" probability: %f", pro);
#endif
        }
        
#if (DEBUG==1)
        PrintEtat(G);
        for (j=1; j<=NbEvtsPossibles; j++)
        {
            printf("distribution  %d %f \n", j, distprob[j]);
        }
#endif
        
        /* inverse transform to get the  event number: j */
        for (j = 1; j <= NbEvtsPossibles; j++) {
			if (u1<distprob[j]) break;
        }
        
        /* Apply  event j to get the new state */
        
        Equation(G, j, F, R);
        
#if (DEBUG==1)
        printf("event: %d time %f \n",j,t );
        PrintEtat(F);
#endif
        
        /* check for loop. */
        looping = 1;
        for (i=0;i<NEt;i++)
            if (G[i]!=F[i]) {
                looping=0;
                break;
            }
        
        if (looping==1) { /* In case of loop, reject the transition */
            printf(" loop \n");
        }
        else { /* real transition. Just do it */
            t += deltat;
            for (i=0; i<NEt; i++) {G[i]=F[i];
#if (Compact==1)
                fprintf(pf1, "%f  ",t);
                for(i=0;i<NEt;i++) {
                    fprintf(pf1,"%5d",F[i]);
                }
                fprintf(pf1,"\n");
#endif
#if (Compact==0)
                i = Etat2Int(F)-1;
                fprintf(pf1,"%d %d \n ",t,i);
#endif
            }
        }
    }
    while (t<Tmax);
    fclose(pf1); 
    printf("Done ContinuousSimulation \n");    
    exit(0);
}

