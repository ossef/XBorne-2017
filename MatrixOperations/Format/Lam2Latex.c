/* 
Model2Latex en matrice pleine au format Lamark 
needs .Rxx matrix and .sz size file
version long double pour lire toutes les matrices Rxx
Ecrit le resultat dans un fichier texte sous forme de commande latex
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void  usage()
{
	printf("usage : Model2Latex -f filename Suffix \n");
	printf("filename.Suffix and filename.sz must exist before. And the suffix must be Rxx \n");
	exit(1);
}

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory for the matrix \n");
	exit(1);
}

int main(argc, argv)
int argc;
char *argv[];
{
long NSommets, NArcs, NEt;
long i, j, c, jx, degre;
long  double x;
  FILE *pf1;
  char s0[20]; 
  char s1[20];
  char s2[20];
  char s4[20];

/* les objets dynamiques a creer par malloc */
 
long    double *P;

/*
On recupere le nom du modele et on verifie que les fichiers existent 
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
		strcpy(s4,s0);
		strcat(s4,".tex");
		/* on verifie si il existe un fichier .sz*/
		if ((pf1=fopen(s2,"r"))==NULL)
			{
			usage();
			}
		/* on verifie si le suffixe est Rxx */
		if (s1[0]!='R')  
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
  fscanf(pf1,"%ld\n", &NArcs);
  fscanf(pf1,"%ld\n", &NSommets);
  fscanf(pf1,"%ld\n", &NEt);
  fclose(pf1);	
  printf("%ld\n", NArcs);
  printf("%ld\n", NSommets);
  printf("%ld\n", NEt);

/* 
On cree les objets ou on exit(1) en cas de pb
*/
  if (!(P=(long double *)calloc(NSommets*NSommets, sizeof(long double)))) ProblemeMemoire();
  
/*
Lecture de la matrice
*/

  pf1=fopen(s0,"r");
  for (i = 0; i < NSommets; i++) {
  /* 
  on commence par le numero et le degre
  */
    fscanf(pf1,"%ld", &c);
    fscanf(pf1,"%ld", &degre);
    for (j = 0; j < degre; j++) {
      fscanf(pf1,"%lf", &x);
      fscanf(pf1,"%ld", &jx);
      P[c*NSommets+jx]=x; 
    }
    getc(pf1);
  }
  fclose(pf1);
 
  pf1=fopen(s4,"w");
  fprintf(pf1,"\\left[ \n");
  fprintf(pf1,"\\begin{array}{");
  for (i = 0; i < NSommets; i++) fprintf(pf1,"l");
  fprintf(pf1,"} \n");

  for (i = 0; i <  NSommets; i++) {
      for (j = 0; j <  NSommets; j++) {
	  if (P[i*NSommets+j]>0) {fprintf(pf1,"%lf ",P[i*NSommets+j]);}
	  if (j<NSommets-1) {fprintf(pf1," & ");} 
	  else {fprintf(pf1," \\\\ \n");}
      }
  }
  fprintf(pf1,"\\end{array} \n");
  fprintf(pf1,"\\right] \n");
  fclose(pf1);
  printf("Done Model2Latex\n");
  exit(0);
}
