/*
Appliquer l'uniformisation avec parametre delta passe en parametre
sur un matrice en Rii pour stockcer une matrice de CTMC
et la tranformer en DTMC 
V1.0 6/11/10
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
	printf("usage : Uniformise -f filename delta \n");
	printf("filename.sz and filename.Rii must exist\n");
	printf("Uniformisation with parameter delta \n");
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
  double delta, prob ;
  FILE *pf1, *pf2;
  char s0[20];
  char s1[20];
  char s2[20];
  char s3[20];

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
/* on recuepre delta */
		++argv;
		strcpy(s3,*argv);
		delta = atof(s3);
		/* on ajoute .sz, et le suffixe */
		strcpy(s1,s0);
		strcpy(s2,s0);
		strcat(s2,"_");
		strcat(s2,s3);
		strcat(s1,".sz");
		strcat(s0,".Rii");
		strcpy(s3,s2);
		strcat(s2,".sz");
		strcat(s3,".Rii");
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
On cree les objets ou on exit(1) en cas de pb
*/
  if (!(p=(double *)malloc(NSommets*sizeof(double)))) ProblemeMemoire();

/*
Lecture de la matrice et ecriture immediate ligne par ligne
*/

pf1=fopen(s0,"r");
pf2=fopen(s3,"w");
c = 0;
NArcs=0;


for (i = 0; i < NSommets; i++) {
/* lecture */
    for (j = 0; j < NSommets; j++) {p[j]=0.0;}
    
    fscanf(pf1,"%ld", &ji);
    fscanf(pf1,"%ld", &degre);
    printf(" %ld %ld %ld \n ", ji, c, degre);
  
    if (ji == c) {        
	for (j = 1; j <= degre; j++)  {
	    fscanf(pf1,"%lg", &prob);
	    fscanf(pf1,"%ld", &ori);
	    p[ori]= delta*prob;
	}
    }
    else ProblemeMatrice();
    c++;
    getc(pf1);
    
/* T-transform */
    p[i]+= 1.0 ;
    
    for (j = 0; j < NSommets; j++) {
	if (p[j]<1.0e-12) {
	    p[j]=0.0;
	    degre--;
	}
    }
      
/* ecriture */
    fprintf(pf2,"%ld ", ji);
    fprintf(pf2,"%ld ", degre);
    
  for (j = 0; j < NSommets; j++) {
      if (p[i]>0.0) {fprintf(pf2," %lg %ld", p[j], j);}    
  }
  fprintf(pf2,"\n");
  NArcs+=degre;
}

fclose(pf1);		
fclose(pf2);

pf1=fopen(s2,"w"); 
fprintf(pf1,"%12ld\n ", NArcs);
fprintf(pf1,"%12ld\n",  NSommets);
fclose(pf1);	

printf("Done Uniformise \n");
exit(0);
}



