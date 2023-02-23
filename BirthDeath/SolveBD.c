/*--------------------------------------------------------*/
/*                                                        */
/*partie de Code specifique au modéle "Naissance et Mort" */
/*             Cas d'une file M/M/B/C                     */
/*--------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define Min(a,b) (a<=b?a:b)

/* 
 inclusions des constantes, types et variables du modèle
 */
#include "const.h"

/* codage des evenements */
/* 1 : arrivee file  */
/* 2 : service file  et depart  */


/* Méthode Taux()
 - A modifier selon le modèle "BirthDeath" à utiliser !!
 - Retourne le taux d'apparition de l'evenement indexevt
 - Exemple du cas d'une file M/M/B/C
*/
double Taux(int x, int indexevt){

  double taux;
  switch (indexevt) {
  case 1: // cas d'une arrivée Lambda(x)
    taux = Lambda;
    break;
  case 2: // cas d'un service Mu(x)
    taux = Min(x, C)*Mu;
    break;
  default:
    printf("Error indexEvent : %d \n", indexevt);
    exit(0);
  }
  return(taux);
}


/* Méthode Solve()
 - Calcul de la distribution stationnaire
 - La stock dans un fichier "MMBC_Queue.pi"
 - Fourni quelques rewards si DEBUG = 1
*/
void Solve(FILE *pf1){

    double *Pi = (double *) calloc(B+1, sizeof(double));
    double s = 0;

    Pi[0] = 1.0;

    for (int i=1; i<=B; i++ ) {       // etats 1 < x <= B
        Pi[i] = Pi[i-1]*(Taux(i-1, 1)/Taux(i, 2));  // formule "Pi(n) = Pi(n-1)x( Lbda(n-1)/Mu(n))" pour n>0
        s += Pi[i];
    }

    s = 1 / ( 1 + s );
    double test = 0;

    for (int i=0; i<=B; i++ ) {       // Normalisation
        Pi[i] *= s;
        test += Pi[i];
    }

    if(fabs(test - 1)>1e10){
        printf("Something went wrong, verify transition rates !");
        exit(0);
    }

#if (DEBUG==1) // Some rewards calculations
    double N = 0 ;   // Mean number of jobs
    double T ;       // Mean response time

    for (int i=0; i<=B; i++ ){
        printf("pi[%d] = %.10f \n",i,Pi[i]);
        N += i*Pi[i];         
    }     
    T = N/(Lambda*(1-Pi[B]));   // Little's law for jobs in the system

    printf("Sum of Probas       = %.10f \n",test);
    printf("Proba Empty Queue   = %.10f \n",Pi[0]);
    printf("Proba Full Queue    = %.10f \n",Pi[B]);
    printf("Mean Number of Jobs = %.10f \n", N);
    printf("Mean response Time  = %.10f \n", T);
#endif

    for (int i=0; i<=B; i++ )
        fprintf(pf1,"%.15E\n", Pi[i]);

    /* On libère le bloc de mémoire alloué dynamiquement */
    free(Pi);
}

/* Programme principal et interface exterieure */
void  usage(char*s)
{
	printf("usage : %s  -f MyBirthDeathModel  \n",s);
	printf("To create file  'MyBirthDeathModel.pi' \n");
	exit(1);
}



int main(int argc, char **argv){
    
    char s1[20];
	FILE * pf1;

	if (argc<2) usage(argv[0]);
	--argc;
	++argv;
	if (**argv != '-') usage(argv[0]);
	switch (*++*argv) {
		case 'f' :  /* on recupere le nom de fichier */
		{++argv;
			strcpy(s1,*argv);
			/* on ajoute .pi*/
			strcat(s1,".pi");
			pf1=fopen(s1,"w");
			break;
		}
		default	 : usage(argv[0]);
	}

    Solve(pf1);
    fclose(pf1);
}