/* 
 Conversion for a class G matrix given by a descriptor into a matrix stored in a file
 needs file model.dscC
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void  usage1()
{
	printf("usage : DscC2Lam -f model_name Suffixe\n");
	printf("model.dscC must exist before. \n");
	exit(1);
}

void  usage2()
{
	printf("The entries of the first vector must be all equal to 1.0 \n");
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
    int  NSommets, rank, NArcs;
    int  i, j, k ;
    long  double x;
    FILE *pf1, *pf2;
    char s0[20];
    char s1[20];
    char s2[20];
    char s3[20];
    char s4[20];
    char s5[20];
    char suf[2] ;
    
    /* les objets dynamiques a creer par malloc */
    
    long    double *r, *c;
    long    double *P ;
    int            *deg;
    
    /*
     On recupere le nom du modele et on verifie que le fichier descripteur existe
     */
    
	if (argc!=4) usage1();
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
            strcat(s2,".sz");
            strcpy(s3,s0);
            strcat(s3,".cd");
            strcpy(s4,s0);
            /* on verifie si il existe deja */
			if ((pf1=fopen(s1,"r"))==NULL)
			{
				usage1();
			}
            break;
        }
		default	 : usage1();
	}
    ++argv;
    strcpy(suf,*argv);
    if (suf[0]  =='R') {strcat(s4,".Rii");}
    else {strcat(s4,".Cii");}
    /*
     On recupere les tailles et les noms dans le fichier modelname.dscC
     */
    pf1=fopen(s1,"r");
    fscanf(pf1,"%d\n", &rank);
    fscanf(pf1,"%d\n", &NSommets);
    printf("%d\n", rank);
    printf("%d\n", NSommets);
    
    /* 
     On cree les objets ou on exit(1) en cas de pb
     */
    
    if (!(r=(long double *)calloc(NSommets, sizeof(long double)))) ProblemeMemoire();
    if (!(c=(long double *)calloc(NSommets, sizeof(long double)))) ProblemeMemoire();
    if (!(P=(long double *)calloc(NSommets*NSommets, sizeof(long double)))) ProblemeMemoire();
    if (!(deg=(int *)calloc(NSommets, sizeof(int)))) ProblemeMemoire();
    
    for (i = 0; i < NSommets; i++) { 
        for (j = 0; j < NSommets; j++) {
            P[i*NSommets+j]=0.0;
        }
    }
    /*
     Lecture des fichiers pour remplir les 2 vecteurs 
     */
    
    for (k = 0; k < rank ; k++) { 
        
        fscanf(pf1,"%s" , s5);
        pf2=fopen(s5,"r");
        
        for (i = 0; i < NSommets; i++) {
            fscanf(pf2,"%Lf", &x);
            r[i]=x;
        }
        fclose(pf2);
        fscanf(pf1,"%s" , s5);
        pf2=fopen(s5,"r");
        for (i = 0; i < NSommets; i++) {
            fscanf(pf2,"%Lf", &x);
            c[i]=x;
        }
        getc(pf1);
        fclose(pf2);
        
        for (i = 0; i < NSommets; i++) { 
            for (j = 0; j < NSommets; j++) {
                P[i*NSommets+j] = P[i*NSommets+j] +  r[i]*c[j];
            }
        }
    }
    NArcs = 0;    
    if (suf[0] == 'R') {
        for (i = 0; i < NSommets; i++) {
            deg[i]=0;
            for (j = 0; j < NSommets; j++) {
                if (P[i*NSommets+j]>0.0) {deg[i]++; NArcs++;}
            }
        }
        pf2=fopen(s4,"w");
        for (i = 0; i < NSommets; i++) {
            fprintf(pf2,"%d  %d ", i, deg[i]);
            for (j = 0; j < NSommets; j++) {
                if (P[i*NSommets+j]>0.0) {fprintf(pf2, "%Lg %d ",P[i*NSommets+j], j);}
            }
            fprintf(pf2,"\n"); 
        }
        fclose(pf2);
    }
    else {
        for (j = 0; j < NSommets; j++) {
            deg[j]=0;
            for (i = 0; i < NSommets; i++) {
                if (P[i*NSommets+j]>0.0) {deg[j]++; NArcs++;}
            }
        }
        pf2=fopen(s4,"w");
        for (j = 0; j < NSommets; j++) {
            fprintf(pf2,"%d  %d ", j, deg[j]);
            for (i = 0; i < NSommets; i++) {
                if (P[i*NSommets+j]>0.0) {fprintf(pf2, "%Lg %d ",P[i*NSommets+j], i );}
            }
            fprintf(pf2,"\n"); 
        }
        fclose(pf2);
    }
    /* File .sz */ 
    pf2=fopen(s2,"w");
    fprintf(pf2, "%d \n", NArcs);
    fprintf(pf2, "%d \n", NSommets);
    fprintf(pf2, "%d \n", 1);
    
    /* file .cd */ 
    
    pf2=fopen(s3,"w");
    for (i = 0; i < NSommets; i++) {
        fprintf(pf2, "%d %d \n", i,i);        
    }
    printf("Done DscC2Lam\n");
    exit(0);
}


