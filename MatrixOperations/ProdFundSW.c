/* 
 product of SW block by the fundamental matrix for a absorbing DTMC
 Input Rii for both matrices but  full matrix for the findamental matrix 
 in memory 
 needs .Rii matrix and .sz size file
 Write the result as  a text file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0
#define epsilon 0.0000000001

struct item {
	double prob;
	int dest;
};

typedef struct item item;

item *V; 
double *B ;     /* the fundamental matrix is in B */ 

int Index(i,j,n)
int i;
int j;
int n;
{
    return (i*n+j);
}

void  usage()
{
	printf("usage : ProdFundSW -f model  \n");
	printf("the files model.SW.Cii, model.FUND.Rii, model.SW.sz and model.FUND.sz must exist before.  \n");
	exit(1);
}

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory \n");
	exit(1);
}

void PrintMatrix(s)
int s;
{
    int i,j;
    for (i=0;i<s;i++) 
    {
        for (j=0;j<s;j++)
        {
            printf("%lf ",B[Index(i,j,s)]);
        }
        printf("\n");
    }
}

void PrintVector(s)
int s;
{
    int i;
    for (i=0;i<s;i++) 
    {
            printf(" %d %lf ",V[i].dest,V[i].prob);
    }
    printf("\n");
    
}


int main(argc, argv)
int argc;
char *argv[];
{
	int n1,n2,n3, n4, NArcsSW, NArcsFUND, NEt1, NEt2;
	int i, j, k, c, jx, degre, size, b1, b2, b3, b4;
	double x,sx;
	char s0[250];
	char s1[250];
	char s2[250];
	char s3[250];
	char s4[250];
	char s5[250];
	char s6[250];
	FILE *pf1, *pf2, *pf3;
	
	/*
	 On recupere le nom du modele et on verifie que les fichiers existent 
	 */
	
	if (argc!=3) usage();
	--argc;
	++argv;
	if (**argv != '-') usage();
	switch (*++*argv) {
		case 'f' :  /* on recupere le nom de fichier */
		{++argv;
			strcpy(s0,*argv);
            strcpy(s1,s0);
            strcpy(s2,s0);
            strcpy(s3,s0);
            strcpy(s4,s0);
            strcpy(s5,s0);
            strcpy(s6,s0);
            
            strcat(s2,".FUND.SE.sz");
            strcat(s3,".FUND.SE.Rii");
            strcat(s0,".SW.sz");
            strcat(s1,".SW.Cii");
            
            strcat(s4,".RwdP");
            strcat(s5,".abs.SW.sz");
            strcat(s6,".abs.SW.Cii");
           
            if (DEBUG==0) {
                printf("%s %s %s %s %s \n", s0,s1,s2,s3,s4);
            }
			/* on verifie si les fichiers existent */
			if ((pf1=fopen(s1 ,"r"))==NULL)
			{
				usage();
			}
			fclose(pf1);
			if ((pf1=fopen(s0,"r"))==NULL)
			{
				usage();
            }
 			fclose(pf1);
            
			if ((pf1=fopen(s2,"r"))==NULL)
			{
				usage();
            }
 			fclose(pf1);
			if ((pf1=fopen(s3,"r"))==NULL)
			{
				usage();
            }
            fclose(pf1);
            break;
        }
		default	 : usage();
    }
	/*
	 On recupere les tailles 
	 */
	pf1=fopen(s0,"r");
	fscanf(pf1,"%d \n",  &NArcsSW);
	fscanf(pf1,"%d \n", &n2);
	fscanf(pf1,"%d \n",  &NEt1);
	fscanf(pf1,"%d %d %d \n", &n1,&b1,&b2);
	fclose(pf1);
    
    pf1=fopen(s2,"r");
	fscanf(pf1,"%d\n", &NArcsFUND);
	fscanf(pf1,"%d\n", &n3);
	fscanf(pf1,"%d\n", &NEt2);
    fscanf(pf1,"%d %d %d \n", &n4, &b3, &b4);
	fclose(pf1);	
    
    if (n3!=n2) {
        printf("wrong size between the blocks \n");
        exit(1);
    }
    if (n3!=n4) {
        printf("the fundamental matrix is not sqare \n");
        exit(1);
    }
    
    size= n3;
    if (!(B =(double *)calloc(size*size, sizeof(double)))) ProblemeMemoire();
    if (!(V =(item *)calloc(n2, sizeof(item)))) ProblemeMemoire();
    
	pf1=fopen(s3,"r");
	
	for (i = 0; i < size ; i++) {
		fscanf(pf1,"%d", &c);
		fscanf(pf1,"%d", &degre);
 		for (j = 0; j < degre; j++) {
			fscanf(pf1,"%lf", &x);
			fscanf(pf1,"%d", &jx);
            B[Index(c-b3,jx-b4,size)]=x;
		}
		getc(pf1);
    } 
    
    fclose(pf1);
    /* the fundamental matrix is in B */ 
    
    if (DEBUG==0) {PrintMatrix(size);} 
    
    pf1=fopen(s1,"r");
	pf2=fopen(s4,"w");
	pf3=fopen(s6,"w");
    
    /* compute the result column by column */ 
    
    for (i = 0; i < n1 ; i++) {
        fscanf(pf1,"%d", &c);
        fscanf(pf1,"%d", &degre);
        for (j = 0; j < degre; j++) {
            fscanf(pf1,"%lf", &x);
            fscanf(pf1,"%d", &jx);
            V[j].prob=x;
            V[j].dest=jx;
        }
        getc(pf1);
        
        if (DEBUG==0) {PrintVector(n2);}
        
        /* V is column i of model.SW */ 
        fprintf(pf3,"%d %d ",i,n2);  
        
        for (k=0;k<size;k++) { /* k is the index of row in B */ 
            sx=0.0;
            for (j = 0; j < degre; j++) {
                sx += B[Index(k,V[j].dest-b4,size)] * V[j].prob ;
            }
            fprintf(pf2,"proba ending in %d knowing begin in %d : %lf \n",i,k+b4,sx);  
            fprintf(pf3,"%lf %d ",sx,k+b4);  
        }
        fprintf(pf3,"\n");  
    } 
    fclose(pf1);
    fclose(pf2);
    fclose(pf3);
    pf3=fopen(s5,"w");
    fprintf(pf3,"%d \n %d \n %d \n %d %d %d \n", n1*n2,n2,NEt1,n1,b1,b2);
    fclose(pf3);
   
    printf("Done ProdSWFund \n");
    exit(0);
}

