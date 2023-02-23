/* de Rii a Cuu  Version initiale jmf */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct element {
    int indx;
    double prob;
    struct element *next;
} element;

element **pile;
int *pred;

void Depile(i, j, y)
int i, *j;
double *y;
{
    *y = pile[i]->prob;
    *j = pile[i]->indx;
    pile[i] = pile[i]->next;
}

void Empile(i, j, y)
int i, j;
double y;
{
    element *p;
    
    p = (element *)malloc(sizeof(element));
    p->next = pile[i];
    p->prob = y;
    p->indx = j;
    pile[i] = p;
}

void  usage()
{
	printf("usage : Rii2Cuu -f filename   \n");
	printf("filename.sz and filename.Rii must exist\n");
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
  int i, jx, j, degre, ji, ori;
  double prob, x;
  FILE *pf1, *pf2;
  char s0[200];
  char s1[200];
  char s2[200];
  char s3[200];

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
        strcat(s1,".sz");
		strcpy(s2,s0);
        strcat(s2,".Rii");
        strcpy(s3,s0);
        strcat(s3,".Cuu");
		/* on verifie si il existe un fichier .sz*/
		if ((pf1=fopen(s1,"r"))==NULL)
			{
            printf("Probleme with file.sz \n");
			usage();
			}
		/* on verifie que le fichier model.Rii existe */
		if ((pf1=fopen(s2,"r"))==NULL)
			{
            printf("Probleme with file.Rii \n");
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
fscanf(pf1,"%d\n", &NArcs);
fscanf(pf1,"%d\n", &NSommets);
fclose(pf1);	
printf("Nb arcs = %d\n", NArcs);
printf("Nb som. = %d\n", NSommets);
/* 
On cree les objets ou on exit(1) en cas de pb
*/
    if (!(pred=(int *)malloc(NSommets*sizeof(int)))) {ProblemeMemoire();}
    if (!(pile=(element **)malloc(NSommets*sizeof(element*)))) {ProblemeMemoire();}

pf1=fopen(s2,"r");

    for (j = 0; j < NSommets; j++) {pred[j]=0.0; pile[j]=NULL;}

for (i = 0; i < NSommets; i++) {
/* lecture */
    fscanf(pf1,"%d", &ji);
    fscanf(pf1,"%d", &degre);
//    printf(" %d %d \n ", ji, degre);
	for (j = 1; j <= degre; j++)  {
	    fscanf(pf1,"%lf", &prob);
	    fscanf(pf1,"%d", &ori);
        Empile(ori,ji,prob);
        pred[ori]=pred[ori]+1;
	}
    getc(pf1);
}
fclose(pf1);
pf2=fopen(s3,"w");
for (i = 0; i < NSommets; i++) {
/* ecriture */
    fprintf(pf2,"%d ", i );
    fprintf(pf2,"%d ", pred[i]);
    
  for (j = 0; j < pred[i]; j++) {
      Depile(i,&jx,&x);
      fprintf(pf2," %lf %d ",x,jx);
  }
  fprintf(pf2,"\n");
}
fclose(pf2);
printf("Done Rii2Cuu \n");
exit(0);
}
