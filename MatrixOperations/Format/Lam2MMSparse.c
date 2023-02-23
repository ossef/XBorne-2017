/*
Transformer un fichier Lamark en Matrix Market en format creux 
V1.0 6/12/10
Version initiale jmf
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


struct element {
  long ori;
  double prob;};

typedef struct element element;
typedef double *probavector; /* de la taille du nombre d'etats */
typedef long *indexvector; /* de la taille du nombre d'etats */
typedef element *elmtvector; /* de la taille du nombre d'arcs */

void  usage()
{
	printf("usage : Lam2MMSparse -f filename \n");
	printf("filename.sz and filename.Rii must exist\n");
	printf("Conversion of Lamark matrix into a MatrixMarket Sparsefile \n");
	exit(1);
}

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory for the matrix and the probability vectors \n");
	exit(1);
}


void  ProblemeMatrice()
{
	printf("Sorry, not a Rii matrix \n");
	exit(1);
}

int main(argc, argv)
int argc;
char *argv[];
{
  long NSommets, NArcs;
  long i, c, j, degre, ji, ori;
  double prob ;
  FILE *pf1, *pf2;
  char s0[20];
  char s1[20];
  char s2[20];
  char sname[20];

/*
Les structures dynamiques a creer par malloc
*/

  
/*
On recupere le nom du modele et on verifie que les fichiers existent....
*/

	if (argc!=3) usage();
	--argc;
	++argv;
	if (**argv != '-') usage();
	switch (*++*argv) {
		case 'f' :  /* on recupere le nom de fichier */
		{++argv;
		strcpy(s0,*argv);

		/* on ajoute .sz, et le suffixe */
		strcpy(s1,s0);
		strcpy(sname,s0);
		strcpy(s2,s0);
		strcat(s1,".sz");
		strcat(s2,".mm.txt");
		strcat(s0,".Rii");
		/* on verifie si il existe un fichier .sz*/
		if ((pf1=fopen(s1,"r"))==NULL)
			{
			usage();
			}
		/* on verifie que le fichier model.Rii existe */
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



pf1=fopen(s1,"r");
  
fscanf(pf1,"%12ld\n", &NArcs);
fscanf(pf1,"%12ld\n", &NSommets);
fclose(pf1);	

printf("Nb arcs = %12ld\n", NArcs);
printf("Nb som. = %12ld\n", NSommets);

/*
Lecture de la matrice et ecriture immediate ligne par ligne
*/

pf1=fopen(s0,"r");
pf2=fopen(s2,"w");
c = 0;

  fprintf(pf2,"%%%%MatrixMarket matrix coordinate real general \n");
  fprintf(pf2,"%% generated from Lamark \n");
  fprintf(pf2,"%% Model: %s \n",sname);
  fprintf(pf2,"%% \n");
  
  fprintf(pf2," %12ld  %12ld %12ld \n", NSommets, NSommets, NArcs);

for (i = 0; i < NSommets; i++) {
/* lecture et ecriture */
    fscanf(pf1,"%ld", &ji);
    fscanf(pf1,"%ld", &degre);
    printf(" %ld %ld %ld \n ", ji, c, degre);
  
    if (ji == c) {        
	for (j = 1; j <= degre; j++)  {
	    fscanf(pf1,"%lg", &prob);
	    fscanf(pf1,"%ld", &ori);
	    fprintf(pf2," %ld %ld %lg \n", ji, ori, prob);
	}
    }
    else ProblemeMatrice();
    c++;
    getc(pf1);    
}
fclose(pf1);		
fclose(pf2);
printf("Done Lam2MMSparse DTMC\n");

exit(0);
}

