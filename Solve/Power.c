/*
Algorithme de la puissance
V1.0 10/4/10
Version initiale jmf
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Power.h"

struct element {
  int ori;
  double prob;};

typedef struct element element;
typedef double *probavector; /* de la taille du nombre d'etats */
typedef int *indexvector; /* de la taille du nombre d'etats */
typedef element *elmtvector; /* de la taille du nombre d'arcs */

void  usage()
{
	printf("usage : Power -f filename Suffix \n");
	printf("filename.Suffix and filename.sz must exist before. And the suffix must be Cuu \n");
	printf("Computes stationnary distribution by the Power algorithm\n");
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
  double diff, som, y;
  FILE *pf1;
  char s0[20];
  char s1[20];
  char s2[20];
  char s3[20];

/*
Les structures dynamiques a creer par malloc
*/
  probavector p, op;
  indexvector debut;
  indexvector taille;
  elmtvector arc;
  
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
		strcat(s3,".pi");
		/* on verifie si il existe un fichier .sz*/
		if ((pf1=fopen(s2,"r"))==NULL)
			{
			usage();
			}
		/* on verifie si le suffixe est Rxx */
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
printf("Iteration accuracy = %.0e\n",maxdiff);
printf("Max number of iterations = %d\n",maxiter);

/* 
On cree les objets ou on exit(1) en cas de pb
*/

/* 3 vecteurs de proba */ 
  if (!(p=(double *)malloc(NSommets*sizeof(double)))) ProblemeMemoire();
  if (!(op=(double *)malloc(NSommets*sizeof(double)))) ProblemeMemoire();
/* le codage creux de la matrice */ 
  if (!(debut=(int *)malloc(NSommets*sizeof(int)))) ProblemeMemoire();
  if (!(taille=(int *)malloc(NSommets*sizeof(int)))) ProblemeMemoire();
  if (!(arc=(element *)malloc(NArcs*sizeof(element)))) ProblemeMemoire();

/*
Lecture de la matrice 
*/

pf1=fopen(s0,"r");
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
}
fclose(pf1);		

// Normalisation des vecteurs p,op,vop  
som = 0.0;
for (i = 0; i < NSommets; i++) {op[i] = 1.0; som += op[i];}
for (i = 0; i < NSommets; i++) {
    op[i] /= som;
}
iter = 0;
do {
    iter++;
    for (i = 0; i < NSommets; i++) {
	som = 0.0;
        for (j = debut[i]; j < debut[i] + taille[i]; j++) 
	    {
	    y = op[arc[j].ori];
	    som += y * arc[j].prob;
	}
        p[i] = som;
      }
for (i = 0; i < NSommets; i++) printf("% .20E\n", p[i]);

      
     diff = 0.0;
     for (i = 0; i < NSommets; i++) {
	  if (op[i]>0) {
		  diff += fabs(p[i] - op[i])/op[i];
	      }
	      else {
		  diff += fabs(p[i]);
	      }
	  }
printf("Iteration %d %e \n", iter,(float)diff);

     for (i = 0; i < NSommets; i++)  op[i] = p[i];

 } while ((iter != maxiter) && (diff >= maxdiff));

// Ecriture du resultat

  pf1=fopen(s3,"w");
  for (i = 0; i < NSommets; i++) fprintf(pf1," %.20E\n", p[i]);
  fclose(pf1);
  printf("Done Power \n");

exit(0);
}

