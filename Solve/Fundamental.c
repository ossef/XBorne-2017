/* 
 Fundamental matrix for a absorbing DTMC
 Input Rii but computation as a full matrix
 needs .Rii matrix for a diagonal block and .sz size file
 Write the result as 4 Rii matrices
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0
#define epsilon 0.0000000001

double *B ;

int Index(i,j,n)
int i;
int j;
int n;
{
    return (i*n+j);
}

void  usage()
{
	printf("usage : Fundamental -f filename suffix \n");
	printf("filename.suffix.Rii and filename.suffix.sz must exist before. Suffix must me SE (or NW) \n");
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


int main(argc, argv)
int argc;
char *argv[];
{
	int NSommets, NArcs, NEt;
	int i, j, k, c, jx, degre, size, arcf, decale, b1,b2,nc;
	double x,sx;
	char s0[250];
	char s1[250];
	char s2[250];
	char s3[250];
	FILE *pf1;
	
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
            ++argv;
			strcpy(s1,*argv);
            strcpy(s2,s0);
            strcat(s2,".FUND.SE");
            
            strcat(s0,".");
            strcat(s0,s1);
            
			strcpy(s1,s0);
			strcat(s0,".Rii");
			strcat(s1,".sz");
            
            strcpy(s3,s2);
  			strcat(s2,".Rii");
			strcat(s3,".sz");
            
            if (DEBUG==0) {
                printf("%s %s %s %s \n", s0,s1,s2,s3);
            }
			/* on verifie si il existe un fichier .sz*/
			if ((pf1=fopen(s1 ,"r"))==NULL)
			{
				usage();
			}
			fclose(pf1);
			/* on verifie que le fichier model.Rii existe */
			if ((pf1=fopen(s0,"r"))==NULL)
			{
				usage();
            }
            fclose(pf1);
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
	fscanf(pf1,"%d\n", &NEt); 
	fscanf(pf1,"%d %d %d \n", &nc,&b1,&b2); 
	fclose(pf1);	
	
    if (b1!=b2) {
        printf("error in the offsets for rows and columns \n"); 
        exit(1);
    }
    
    size= 2*NSommets;
    if (!(B =(double *)calloc(size*size, sizeof(double)))) ProblemeMemoire();
    
    decale=b1;

	pf1=fopen(s0,"r");
	
	for (i = 0; i < NSommets ; i++) {
		fscanf(pf1,"%d", &c);
		fscanf(pf1,"%d", &degre);
 		for (j = 0; j < degre; j++) {
			fscanf(pf1,"%lf", &x);
			fscanf(pf1,"%d", &jx);
            B[Index(NSommets+i,NSommets+jx-decale,size)]=x;
		}
		getc(pf1);
    } 
    for (i = 0; i < NSommets ; i++) {
        B[Index(i,NSommets+i,size)]=1.0;
        B[Index(NSommets+i,i,size)]=1.0;
    }
    
    if (DEBUG==0) {PrintMatrix(size);}
    
    for (k=size;k!=NSommets;k--){
        for(i=0;i<k-1;i++){
            for(j=0;j<k-1;j++)
            {
                sx = B[Index(k-1,j,size)]*B[Index(i,k-1,size)];
                sx /= (1-B[Index(k-1,k-1,size)]);
                B[Index(i,j,size)] += sx;
            }
        }
        if (DEBUG==0) {
            printf("After iteration %d \n",k);
            PrintMatrix(size);
        }
    }
    
    arcf=0;
    for (i = 0; i<NSommets ; i++) {
        for (j=0; j<NSommets; j++) {
            x = B[Index(i,j,size)];
            if (x > epsilon) {
                arcf++ ;
            }
            else if (x < -epsilon){
                printf("negative value %lf entry %d %d \n",x,i,j );
                exit(1);
            }
        }
    }
    pf1=fopen(s3,"w");
    fprintf(pf1," %d \n %d  \n %d \n %d %d %d \n", arcf, NSommets, NEt, NSommets, b1, b2);
    fclose(pf1);
    
    pf1=fopen(s2,"w");
    for (i = 0; i<NSommets ; i++) {
        arcf=0;
        for (j=0; j<NSommets; j++) {
            if  (B[Index(i,j,size)]>epsilon) {
                arcf++;
            }
        }
        fprintf(pf1," %d %d ", i+decale,arcf);
        for (j=0; j<NSommets; j++) {
            if  (B[Index(i,j,size)]>epsilon) {
                fprintf(pf1," %lf %d ",B[Index(i,j,size)] , j+decale);
            }
        }
        fprintf(pf1,"\n");        
    }
    fclose(pf1);
    printf("Done Fundamental \n");
    exit(0);
}

