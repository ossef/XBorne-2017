/* 
calcul des recompenses a partir de la matrice,
des probabilites stationnaires sans le codage des etats

V1.1 le 12/4/08

Hypotheses  de fonctionnement : 
l'argument est le nom du modele : filename 
la taille est dans le fichiez filename.sz et la distribution dans filename.pi
*/


#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/*
Le nombre d'etats atteignables et de transitions
*/
int ReachableStates, NArcs;
 
/* 
la distribution stationnaire 
*/
double *pi;


void  usage()
{
	printf("usage : RewardIndex -f filename \n");
	printf("filename.pi, filename.sz  must exist before \n");
	exit(1);
}

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory for the probability vector  \n");
	exit(1);
}


int main(argc, argv)
int argc;
char *argv[];
{
int i,j, k, Mode, plusieur;
double Mean, Deviate, pmax;
FILE *pf1;
char s1[20];
char s2[20];
char s3[20];

/*
On recupere le nom du modele et on verifie que les fichiers existent 
*/

	if (argc<2) usage();
	--argc;
	++argv;
	if (**argv != '-') usage();
	switch (*++*argv) {
		case 'f' :  /* on recupere le nom de fichier */
		{++argv;
		strcpy(s1,*argv);
		/* on ajoute .sz, .pi et .cd */
		strcpy(s2,s1);
		strcpy(s3,s1);
		strcat(s1,".sz");
		strcat(s2,".pi");
		strcat(s3,".rwdI");
		/* on verifie si il existe */
		if ((pf1=fopen(s1,"r"))==NULL)
			{
			usage();
			}
		if ((pf1=fopen(s2,"r"))==NULL)
			{
			usage();
			}
		pf1=fopen(s1,"r");
		break;
		}
		default	 : usage();
	}

/*
On recupere les tailles dans le fichier filename.sz
*/

  fscanf(pf1,"%12d\n", &NArcs);
  fscanf(pf1,"%12d\n", &ReachableStates);
  
  printf("%12d\n", NArcs);
  printf("%12d\n", ReachableStates);
  
  fclose(pf1);	

/* 
On cree les objets ou on exit(1) en cas de pb
*/

  if (!(pi=(double *)malloc(ReachableStates*sizeof(double)))) ProblemeMemoire();
 
  
/*
Lecture du vecteur pi dans le fichier filename.pi
*/	
  pf1=fopen(s2,"r");

  for (k = 0; k < ReachableStates; k++) {
    fscanf(pf1,"%lg \n", &pi[k]);
  }
  fclose(pf1);	
/*
Calculs elementaires 
*/	
   
  pf1=fopen(s3,"w");
  fprintf(pf1,"Moyenne :");
  Mean = 0.0;
  k = ReachableStates;
  for (j = 0; j < k; j++) {
    Mean += j * pi[j];
  }
  fprintf(pf1,"%12g \n", Mean);
  Deviate=0.0;
  for (j = 0; j < k; j++)
    Deviate += pi[j] * (j - Mean) * (j - Mean);
  fprintf(pf1, "Variance : %12g \n", Deviate);

  fprintf(pf1,"Fin de la distribution \n");
  plusieur=10;
  if (plusieur>k) {
      plusieur=k;
      }
  for (i = 1; i < plusieur; i++) {
      Deviate=0;
      for (j = k-i; j < k; j++) {Deviate+=pi[j];
      }
      fprintf(pf1,"Somme des %d derniers termes : %12g \n",i,Deviate);
  }	
  fprintf(pf1, "Mode \n");
  pmax = 0.0;
  plusieur = 1;
  for (j = 0; j < k; j++) {
    if (pi[j] > pmax) {
      plusieur=1; 
      pmax = pi[j];
      Mode = j;
    } else if (pi[j] == pmax)
      plusieur++;
  }
  fprintf(pf1,"etat : %12d, probabilite : %12g, occurences : %12d\n", Mode, pmax, plusieur );

  fprintf(pf1,"Fonction de Repartition \n");
  Deviate=0;
  for (j = 0; j < k; j++) 
	  {Deviate+=pi[j];
           fprintf(pf1,"%d %12g \n",j,Deviate);
  }
  printf("Done RewardIndice\n"); 
  fclose(pf1);
  exit(0);
}


