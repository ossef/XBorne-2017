/* 
 Transient distribution for a class G matrix given by a descriptor 
 needs file model.dscC
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void  usage1()
{
	printf("usage : TransientClassC -f model_name date \n");
	printf("model.dscC and model.pi0 must exist before. \n");
	printf("The rank must be 2. \n");
	exit(1);
}

void  usage0()
{
	printf("model.dscC and model.pi0 must exist before. \n");
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
    int k;
    long  double x , alpha, beta, gamma, alk, ak ;
    FILE *pf1, *pf2;
    char s0[20];
    char s1[20];
    char s2[20];
    char s3[20];
    char s4[20];
    
    /* les objets dynamiques a creer par malloc */
    
    long    double *r1,*c1,*r2,*c2;
    long    double *pi, *pi0;
    
    /*
     On recupere le nom du modele 
    */
    
	if (argc!=4) usage1();
	--argc;
	++argv;
	if (**argv != '-') usage1();
	switch (*++*argv) {
		case 'f' :  /* on recupere le nom de fichier */
		{   ++argv; 
            strcpy(s0,*argv);
            ++argv; 
            strcpy(s4,*argv);
            /* on ajoute .dscC */
            strcpy(s1,s0);
            strcat(s1,".dscC");
            strcpy(s2,s0);
            strcat(s2,".pi_0");
            strcpy(s3,s0);
            strcat(s3,".pi_");
            strcat(s3,s4);
            k = atoi(s4);
            /* on verifie si ils existent deja */
			if ((pf1=fopen(s1,"r"))==NULL)
			{
				usage0();
			}
			if ((pf1=fopen(s2,"r"))==NULL)
			{
				usage0();
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
    if (!(pi0=(long double *)malloc(NSommets*sizeof(long double)))) ProblemeMemoire();
    if (!(r1=(long double *)calloc(NSommets, sizeof(long double)))) ProblemeMemoire();
    if (!(r2=(long double *)calloc(NSommets, sizeof(long double)))) ProblemeMemoire();
    if (!(c1=(long double *)calloc(NSommets, sizeof(long double)))) ProblemeMemoire();
    if (!(c2=(long double *)calloc(NSommets, sizeof(long double)))) ProblemeMemoire();
    
    /*
     Lecture des fichiers pour remplir les 5 vecteurs 
     */
    pf2 = fopen(s2,"r");
    for (i = 0; i < NSommets; i++) {
        fscanf(pf2,"%Lf", &x);
        pi0[i]=x;
    }
    fclose(pf2);
    
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
    
    /* Calcul de la distribution transitoire */
    
    alpha  = 0.0;
    beta   = 0.0;
    gamma  = 0.0; 
    for (i = 0; i < NSommets; i++) {
        alpha += c2[i]*r2[i];
        beta  += c1[i]*r2[i];
        gamma += pi0[i]*r2[i];
    }
    alk = pow(alpha,(k-1));
    
    if (alpha!=1) {ak = beta*(1- alk)/(1-alpha) + gamma*alk ;}
    else {ak = gamma + beta*(k-1);}
    
    for (i = 0; i < NSommets; i++) {
        pi[i] = c1[i] + ak * c2[i];
    }
    printf("%Lg  %Lg %Lg %Lg %Lg %d \n", alpha, beta, gamma, alk, ak, k);
/*    for (i = 0; i < NSommets; i++) printf("%ld %Lf %Lf %Lf %Lf %Lf \n", i, r1[i], c1[i], r2[i], c2[i], pi0[i]); */
   
    
    pf1=fopen(s3,"w");
    for (i = 0; i < NSommets; i++) fprintf(pf1," %Lf\n", pi[i]);
    fclose(pf1);
    printf("Done TransientClassC Row\n");
    exit(0);
}


