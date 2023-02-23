/*
Transformer un fichier Lamark en Scilab 
V1.0 22/10/10
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
	printf("usage : Sci2Lam -f filename NSommets \n");
	printf("filename.sce  must exist\n");
	printf("Conversion of a matrix in a Scilab file into a Lamark matrix\n");
	exit(1);
}

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory for the matrix and the probability vectors \n");
	exit(1);
}


void  ProblemeMatrice()
{
	printf("Sorry, not a matrix \n");
	exit(1);
}

int main(argc, argv)
int argc;
char *argv[];
{
  long NSommets, NArcs;
  long i, c, j, degre;
  FILE *pf1, *pf2;
  char s0[20];
  char s1[20];
  char s2[20];
  char s4[20];
  char sname[20];
  char ch;
  

/*
Les structures dynamiques a creer par malloc
*/
  probavector p;
  
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
		++argv; /* on recupere l etape */
		strcpy(s4,*argv);
	        NSommets =atoi(s4);
		/* on ajoute .sz, et le suffixe */
		strcpy(s1,s0);
		strcpy(s2,s0);
		strcat(s1,".sz");
		strcat(s2,".sce");
		strcat(s0,".Rii");
		/* on verifie si il existe un fichier .sce*/
		if ((pf1=fopen(s2,"r"))==NULL)
			{
			usage();
			}
		break;
		}
		default	 : usage();
	}

/* 
On cree les objets ou on exit(1) en cas de pb
*/
  if (!(p=(double *)malloc(NSommets*sizeof(double)))) ProblemeMemoire();



/*
Lecture de la premiere ligne 
elimination du nom et  de "  =  [" 
*/

pf1=fopen(s0,"w");
pf2=fopen(s2,"r");
c = 0;

  fscanf(pf2,"%s", sname); 
  printf("%s ", sname);
  fscanf(pf2,"%c", &ch); 
  printf("%c ", ch);
  fscanf(pf2,"%c", &ch); 
  printf("%c ", ch);
  fscanf(pf2,"%c", &ch); 
  printf("%c ", ch);
  fscanf(pf2,"%c", &ch); 
  printf("%c \n", ch);
  NArcs=0;
  j=0;

 /* on continue ligne par ligne */

for (i = 0; i < NSommets; i++) {
/* lecture */
    degre =0;
    
    for (j = 0; j < NSommets; j++) {
	fscanf(pf2,"%lg", &p[j]);
	if (p[j]>0.0) {
	    degre++;
	    NArcs++;
	}
    
    }    
   getc(pf2); 
   
   
    fprintf(pf1,"%ld ", i);
    fprintf(pf1,"%ld ", degre);
 
    for (j = 0; j < degre; j++)  {
	if (p[j]>0.0) {
	    fprintf(pf1,"%lg ", p[j]);
	    fprintf(pf1,"%ld ", j);
	}
    }
    
  fprintf(pf1,"\n");

}

fclose(pf1);		
fclose(pf2);

/*
On met les tailles dans le fichier filename.sz
*/

pf1=fopen(s1,"w");

fprintf(pf1,"%12ld\n", NArcs);
fprintf(pf1,"%9ld\n", NSommets);
fclose(pf1);	

printf("Nb arcs = %12ld\n", NArcs);
printf("Nb som. = %12ld\n", NSommets);

printf("Done Sci2Lam DTMC\n");

exit(0);
}

