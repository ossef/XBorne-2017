/* 
Vincent en matrice pleine au format Xborne pour borne sup et borne inf
needs .Rxx matrix and .sz size file
version long double pour lire toutes les matrices Rxx
Ecrit le resultat en Rii pour la borne sup et en Rdi pour la borne inf
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Vincent.h"

void  usage()
{
	printf("usage : Vincent -f filename Suffix \n");
	printf("filename.Suffix and filename.sz must exist before. And the suffix must be Rxx \n");
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
int  NSommets, NArcs, NEt;
int  i, j, n, c, jx, degre;
long  double S1,S2,x;
  FILE *pf1;
  char s0[20];
  char s1[20];
  char s2[20];
  char s3[20];
  char s4[20];
  char s5[20];
  char s6[20];

/* les objets dynamiques a creer par malloc */
 
long    double *P;
long    double *ligne;

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
		strcat(s4,".V.U.Rii");
		strcpy(s3,s0);
		strcat(s3,".V.U.sz");
		strcpy(s6,s0);
		strcat(s6,".V.L.Rdi");
		strcpy(s5,s0);
		strcat(s5,".V.L.sz");
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
  fscanf(pf1,"%d\n", &NArcs);
  fscanf(pf1,"%d\n", &NSommets);
  fscanf(pf1,"%d\n", &NEt);
  
  fclose(pf1);	
  printf("%d\n", NArcs);
  printf("%d\n", NSommets);
  printf("%d\n", NEt);

/* 
On cree les objets ou on exit(1) en cas de pb
*/

  if (!(ligne=(long double *)malloc(NSommets*sizeof(long double)))) ProblemeMemoire();
  if (!(P=(long double *)calloc(NSommets*NSommets, sizeof(long double)))) ProblemeMemoire();
  
/*
Lecture de la matrice dans le fichier filename.Rxx
*/
  pf1=fopen(s0,"r");
  for (i = 0; i < NSommets; i++) {
  /* 
  on commence par le numero et le degre
  */
    fscanf(pf1,"%d", &c);
    fscanf(pf1,"%d", &degre);
    for (j = 0; j < degre; j++) {
      fscanf(pf1,"%Lf", &x);
      fscanf(pf1,"%d", &jx);
/*  printf("Lect %d %d %d %lf \n", c,degre, jx, x);  */
      P[c*NSommets+jx]=x; /* P[c,jx]=x */
    }
    getc(pf1);
  }
  fclose(pf1);
/* Upper bound */ 
  
  pf1=fopen(s4,"w");

  /* the first row is kept unchanged */
  jx=0;
  for (i = 0; i <  NSommets; i++) {
      ligne[i]= P[i];
      if (ligne[i]>epsNull) jx++;
  }
  fprintf(pf1,"0 %d ", jx);
  for (i = 0; i < NSommets; i++) 
      if (ligne[i]>epsNull) fprintf(pf1, " %Lf %d ", ligne[i],i);
  fprintf(pf1,"\n");
/* now procced with rows from 1 to the end */
  degre=jx;
  for (n = 1; n< NSommets ; n++) {
      S1 = 0.0L;
      S2 = 0.0L;
      for (i = NSommets-1; i >= 0 ; i--) {
/* compute the max of the partial sum */
	  S1+=P[n*NSommets+i];
	  S2+=ligne[i];
	  if (S1>S2) {
	      ligne[i]=S1;
	  }
	  else {
	      ligne[i]=S2;
	  }
      }
/* inversion of the partial sum */ 
      jx=0;
      for (i = 0; i < NSommets-1; i++) {
	  ligne[i]-=ligne[i+1];
      if (ligne[i]>epsNull) jx++;
      }
      if (ligne[NSommets-1]>epsNull) jx++;
/* write the node number (n) and the degree (jx) */      
      fprintf(pf1,"%d %d ", n, jx);
      degre+=jx;
      for (i = 0; i < NSommets; i++)      
	  if (ligne[i]>epsNull) fprintf(pf1, " %Lf %d ", ligne[i],i);
      fprintf(pf1,"\n");
  }
/* the transitions are stored */ 
  fclose(pf1);
/* now write the description file */  
  pf1=fopen(s3,"w");
  fprintf(pf1,"%d\n", degre);
  fprintf(pf1,"%d\n", NSommets);
  fprintf(pf1,"%d\n", NEt);
  fclose(pf1);
  printf("Done Vincent Upper Bound\n");

  /* Lower bound */ 
  pf1=fopen(s6,"w");
  
  /* the last row is the same as in the original model */
  jx=0;
  for (i = 0; i <  NSommets; i++) {
      ligne[i]= P[(NSommets-1)*NSommets+i];
      if (ligne[i]>epsNull) jx++;
  }
  fprintf(pf1,"%d %d ", NSommets-1,jx);
  for (i = 0; i < NSommets; i++) 
      if (ligne[i]>epsNull) fprintf(pf1, " %Lf %d ", ligne[i],i);
  fprintf(pf1,"\n");

  /* now proceed with rows from the end to 0 */
  degre=jx;
  for (n = NSommets-2; n>=0 ; n--) {
      S1 = 0.0L;
      S2 = 0.0L;
      for (i = NSommets-1; i >= 0 ; i--) {
/* compute the min of the partial sum */	 
	  S1+=P[n*NSommets+i];
	  S2+=ligne[i];
	  if (S1>S2) {
	      ligne[i]=S2;
	  }
	  else {
	      ligne[i]=S1;
	  }
      }
/* inversion of the partial sum */
      jx=0;
      for (i = 0; i < NSommets-1; i++) {
	  ligne[i]-=ligne[i+1];
      if (ligne[i]>epsNull) jx++;
      }
      if (ligne[NSommets-1]>epsNull) jx++;
/* write the transitions out of node n */      
      fprintf(pf1,"%d %d ", n, jx);
      degre+=jx;
      for (i = 0; i < NSommets; i++)      
	  if (ligne[i]>epsNull) fprintf(pf1, " %Lf %d ", ligne[i],i);
      fprintf(pf1,"\n");
  }
/* the transitions are stored */ 
  fclose(pf1);
/* now write the description file */  
  pf1=fopen(s5,"w");
  fprintf(pf1,"%d\n", degre);
  fprintf(pf1,"%d\n", NSommets);
  fprintf(pf1,"%d\n", NEt);
  fclose(pf1);
  printf("Done Vincent Lower Bound\n");
  exit(0);
}
