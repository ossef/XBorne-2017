/*
Algorithm SLUB
lower and upper bounds at the same time, sparse matrix implementation
Computation of dinf and dsup 
Check that dinf is zero
read an init vector. 
V1.0 15/4/10 jmf
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Nabla.h"


struct element {
  int  ori;
  double prob;};

typedef struct element element;
typedef double *probavector; /* de la taille du nombre d'etats */
typedef long *indexvector; /* de la taille du nombre d'etats */
typedef element *elmtvector; /* de la taille du nombre d'arcs */

void  usage()
{
	printf("usage : SLUB -f filename Suffix \n");
	printf("filename.Suffix and filename.sz must exist before. And the suffix must be Cuu \n");
	printf("Computes elemnet-wise lower bound pf of the stationnary distribution\n");
	exit(1);
}

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory for the matrix and the probability vectors \n");
	exit(1);
}

void  ProblemeVecteur()
{
	printf("Sorry, as vector dinf is not zero, it is better to use the other algorithms \n");
	exit(1);
}

void  ProblemeInit()
{
	printf("Init Vector must be non negative and its norm must be smaller than 1.0 \n");
	exit(1);
}


static double max(x, y)
double x, y;
{
  if (x > y)
    return x;
  else
    return y;
}

static double min(x, y)
double x, y;
{
  if (x > y)
    return y;
  else
    return x;
}


int main(argc, argv)
int argc;
char *argv[];
{
  int NSommets, NArcs;
  int  i, c, j, degre, iter, ji;
  double  y, S,mi, mx, R;
  FILE *pf1;
  char s0[20];
  char s1[20];
  char s2[20];
  char s3[40];
  char s4[40];
  char s5[40];

/*
Les structures dynamiques a creer par malloc
*/
  probavector pinf, psup, dinf, dsup;
  indexvector debut;
  indexvector taille;
  elmtvector  arc;
  
/*
On recupere le nom du modele et on verifie que les fichiers existent....
*/

	if (argc!=4) usage();
	--argc;
	++argv;
	if (**argv != '-') usage();
	switch (*++*argv) {
		case 'f' :  /* on recupere le nom de fichier */
		{++argv;
		strcpy(s0,*argv);
		++argv; /* on recupere le nouveau suffixe */
		strcpy(s1,*argv);
		/* on ajoute .sz, et le suffixe */
		strcpy(s2,s0);
		strcat(s2,".sz");
		strcpy(s3,s0);
		strcat(s3,".SLUB.L.pi");
		strcpy(s4,s0);
		strcat(s4,".SLUB.U.pi");
		strcpy(s5,s0);
		strcat(s5,".init.pi");
		/* on verifie si il existe un fichier .sz*/
		if ((pf1=fopen(s2,"r"))==NULL)
			{
			usage();
			}
		/* on verifie si le suffixe est C** */
		if (s1[0]!='C')  
		    	{
			usage();
			}
		/* on verifie que le fichier model.suffix existe */
		strcat(s0,".");
		strcat(s0,s1);
		if ((pf1=fopen(s0,"r"))==NULL)
			{
			usage();
			}
		/* on verifie que le fichier model.init.pi existe */
		if ((pf1=fopen(s5,"r"))==NULL)
			{
			usage();
			}
		break;
		}
		default	 : usage();
	}

/*
On recupere les tailles dans le fichier filename.sz
*/


pf1=fopen(s2,"r");
  
fscanf(pf1,"%d\n", &NArcs);
fscanf(pf1,"%d\n", &NSommets);
fclose(pf1);	

printf("Nb arcs = %12d\n", NArcs);
printf("Nb som. = %12d\n", NSommets);
printf("Max number of iterations = %d \n",maxiter);

/* 
On cree les objets ou on exit(1) en cas de pb
*/

/* 3 vecteurs de proba */ 
  if (!(pinf=(double *)malloc(NSommets*sizeof(double)))) ProblemeMemoire();
  if (!(psup=(double *)malloc(NSommets*sizeof(double)))) ProblemeMemoire();
  if (!(dinf =(double *)malloc(NSommets*sizeof(double)))) ProblemeMemoire();
  if (!(dsup =(double *)malloc(NSommets*sizeof(double)))) ProblemeMemoire();
/* le codage creux de la matrice */ 
  if (!(debut=(long *)malloc(NSommets*sizeof(long)))) ProblemeMemoire();
  if (!(taille=(long *)malloc(NSommets*sizeof(long)))) ProblemeMemoire();
  if (!(arc=(element *)malloc(NArcs*sizeof(element)))) ProblemeMemoire();

/*
Lecture de la matrice et calcul de dinf. 
*/

pf1=fopen(s0,"r");
c = 0;
iter= 0;

for (i = 0; i < NSommets; i++) {
    fscanf(pf1,"%d", &ji);
    fscanf(pf1,"%d", &degre);
    debut[ji] = c;
    mi= 1.0;
    mx=0;
    taille[ji] = degre;
    for (j = 1; j <= degre; j++)  {
    	fscanf(pf1,"%lg", &arc[c].prob);
    	fscanf(pf1,"%d", &arc[c].ori);
	mi = min(mi,arc[c].prob);
	mx = max(mx,arc[c].prob);
/* 
	printf("%ld %ld %ld %lg %ld %lg \n", ji,degre,c, arc[c].prob, arc[c].ori, mi);
*/
	c++;
    }
    getc(pf1); 
    dsup[i]=mx;
    if (degre<NSommets) {dinf[i]=0;}
    else {dinf[i]=mi; if (mi>0) iter=1;}
}
fclose(pf1);		

       printf("dinf: ");      
       for (j = 0; j < NSommets; j++) {
                        printf("%lf  ",dinf[j]);      
                        }
      			     
       printf("\n dsup: ");      
       for (j = 0; j < NSommets; j++) {
                        printf("%lf  ",dsup[j]);      
                        }
 
        printf("\n");
       if (iter==1) ProblemeVecteur();
/* lecture d'un vecteur d'initialisation */

       S=0.0;
       
       pf1=fopen(s5,"r");
       for (i = 0; i < NSommets; i++) {
	   fscanf(pf1,"%lf", &R);
	   dinf[i]=R;
	   if (R<0) ProblemeInit();
	   S+=R;
       }
       if (S>1.0)  ProblemeInit();
       if (S<0.0)  ProblemeInit();
       fclose(pf1);		
       
       
       for (j = 0; j < NSommets; j++) {
	   pinf[j]=dinf[j];
           psup[j]=dsup[j];
       }
       
iter = 0;
do {
    iter++;
    for (i = 0; i < NSommets; i++) {
	S=0;
	R=0;
        for (j = debut[i]; j < debut[i] + taille[i]; j++) 
	    {
	    y = pinf[arc[j].ori];
	    S += y * arc[j].prob;
	    y = psup[arc[j].ori];
	    R += y * arc[j].prob;
	}
        dinf[i] = max(S,pinf[i]);
        dsup[i] = min(R,psup[i]);
      }
      R=0.0;
      S=0.0;
      for (j = 0; j < NSommets; j++) {
	  if (pinf[i]>0.0) {
		  S += (dinf[i] - pinf[i])/pinf[i];
	      }
	      else {
		  S += pinf[i];
	      }
	  if (psup[i]>0.0) {
		  R += (psup[i] - dsup[i])/psup[i];
	      }
	      else {
		  R += psup[i];
	      }
	  }
	  
      printf("%d   Residu Inf  %lf Residu Sup  %lf", iter, S, R);      
	 
      for (j = 0; j < NSommets; j++) {
	   printf("%lg & %lg \n", pinf[j], psup[j]);	 
	   pinf[j]=dinf[j];
	   psup[j]=dsup[j];
       }
 
 } while ((iter != maxiter) && (R >= maxresidu) &&  (S>= maxresidu)  );

// Ecriture du resultat

  pf1=fopen(s3,"w");
  for (i = 0; i < NSommets; i++) fprintf(pf1,"%lg \n", pinf[i]);
  fclose(pf1);
  pf1=fopen(s4,"w");
  for (i = 0; i < NSommets; i++) fprintf(pf1,"%lg \n", psup[i]);
  fclose(pf1);
  printf("Done SLUB \n");

exit(0);
}

