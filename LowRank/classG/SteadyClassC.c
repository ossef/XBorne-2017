/* 
 Steady state for a class G matrix given by a descriptor 
 needs file model.dscC
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void  usage1()
{
	printf("usage : SteadyClassC -f model_name \n");
	printf("model.dscC must exist before. \n");
	printf("The rank must be 2. \n");
	exit(1);
}

void  usage2()
{
	printf("The entries of the first vector must be all equal to 1.0 \n");
	exit(1);
}

void  usage3()
{
	printf("the rank must be 2. \n");
	exit(1);
}

void  usage4()
{
	printf("The product is equal to 1. Impossible to solve in that case \n");
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
    int NSommets, rank;
    int i;
    long  double x , Er, a ;
    FILE *pf1, *pf2;
    char s0[20];
    char s1[20];
    char s2[20];
    
    /* les objets dynamiques a creer par malloc */
    
    long    double *r1,*c1,*r2,*c2;
    long    double *pi;
    
    /*
     On recupere le nom du modele et on verifie que le fichier descripteur existe
     */
    
	if (argc!=3) usage1();
	--argc;
	++argv;
	if (**argv != '-') usage1();
	switch (*++*argv) {
		case 'f' :  /* on recupere le nom de fichier */
		{++argv; 
            strcpy(s0,*argv);
            /* on ajoute .dscC */
            strcpy(s1,s0);
            strcat(s1,".dscC");
            strcpy(s2,s0);
            strcat(s2,".pi");
            /* on verifie si il existe deja */
			if ((pf1=fopen(s1,"r"))==NULL)
			{
				usage1();
			}
            break;
        }
		default	 : usage1();
	}
    
    /*
     On recupere les tailles et les noms dans le fichier modelname.dscC
     */
    pf1=fopen(s1,"r");
    fscanf(pf1,"%d\n", &rank);
    fscanf(pf1,"%d\n", &NSommets);
    printf("%d\n", rank);
    printf("%d\n", NSommets);
    
    if (rank!=2) usage3();  
    
    /* 
     On cree les objets ou on exit(1) en cas de pb
     */
    
    if (!(pi=(long double *)malloc(NSommets*sizeof(long double)))) ProblemeMemoire();
    if (!(r1=(long double *)calloc(NSommets, sizeof(long double)))) ProblemeMemoire();
    if (!(r2=(long double *)calloc(NSommets, sizeof(long double)))) ProblemeMemoire();
    if (!(c1=(long double *)calloc(NSommets, sizeof(long double)))) ProblemeMemoire();
    if (!(c2=(long double *)calloc(NSommets, sizeof(long double)))) ProblemeMemoire();
    
    /*
     Lecture des fichiers pour remplir les 4 vecteurs 
     */
    fscanf(pf1,"%s" , s0);
    pf2=fopen(s0,"r");
    
    for (i = 0; i < NSommets; i++) {
        fscanf(pf2,"%Lf", &x);
        if (x != 1.0) usage2();
        r1[i]=x;
    }
    fclose(pf2);
    fscanf(pf1,"%s" , s0);
    pf2=fopen(s0,"r");
    for (i = 0; i < NSommets; i++) {
        fscanf(pf2,"%Lf", &x);
        c1[i]=x;
    }
    getc(pf1);
    fclose(pf2);
    
    fscanf(pf1,"%s" , s0);
    pf2=fopen(s0,"r");
    for (i = 0; i < NSommets; i++) {
        fscanf(pf2,"%Lf", &x);
        r2[i]=x;
    }
    fclose(pf2);
    
    fscanf(pf1,"%s" , s0);
    pf2=fopen(s0,"r");
    for (i = 0; i < NSommets; i++) {
        fscanf(pf2,"%Lf", &x);
        c2[i]=x;
    }
    fclose(pf1);
    fclose(pf2);
    
    /* Calcul de la distribution stationnaire */
    
    Er = 0.0;
    a = 0.0;
    for (i = 0; i < NSommets; i++) {
        Er += c1[i]*r2[i];
        a  += c2[i]*r2[i];
    }
    if (a==1.0) usage4();
    Er = Er/(1.0-a);
     
    for (i = 0; i < NSommets; i++) {
        pi[i] = c1[i] + Er*c2[i];
    }
    printf("%Lg %Lg \n", a, Er);
    
    pf1=fopen(s2,"w");
    for (i = 0; i < NSommets; i++) fprintf(pf1," %Lf\n", pi[i]);
    fclose(pf1);
    printf("Done SteadyClassC Row\n");
    exit(0);
}


