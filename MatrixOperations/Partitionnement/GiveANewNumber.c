/* Give a new number for reordering the state space 
 We only create the permutation to apply  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#define Title 250

/* The model state */ 

typedef int Etat[NEt];

#include "funnum.c"

int *new,*old;
int *et;

void  usage()
{
    printf("usage : GiveANewNumber -f filename name \n");
	printf("filename.cd and filename.sz must exist before \n");
	printf("filename.name.perm will be created \n");
	exit(1);
}

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory for the vectors \n"); exit(1);
}

int main(argc, argv)
int argc;
char *argv[];
{
    int NSommets, NArcs;
    int i, j, k, b, nb;
    FILE *pf1 ;
    char s1[Title];
    char s2[Title];
    char s3[Title];
    char s4[Title];
    char s5[Title];

	if (argc!=4) usage();
	--argc;
	++argv;
	if (**argv != '-') usage();
	switch (*++*argv) {
		case 'f' :  /* get the model name */
		{++argv;
            strcpy(s1,*argv);
            ++argv;             
            strcpy(s2,*argv);
            ++argv;
            /* add .sz, and .cd */
            strcpy(s4,s1);
            strcpy(s3,s1);
            strcat(s3,".sz");
            strcat(s4,".cd");
            /* create new name */
            strcpy(s5,s1);
            strcat(s5,s2);
            strcat(s5,".perm");
            /* does they exist ? */
            if ((pf1=fopen(s3,"r"))==NULL)
			{
                usage();printf("Problem with the .sz file\n");			
			}
            fclose(pf1);
            if ((pf1=fopen(s4,"r"))==NULL)
			{
                usage();printf("Problem with the .cd file\n");
			}
            fclose(pf1);
            break;
		}
		default	 : usage();
	}
    
    /*     Get the sizes in filename.sz */

    printf("Gettin the size of the model\n");
    pf1=fopen(s3,"r");
    fscanf(pf1,"%d\n", &NArcs);
    fscanf(pf1,"%d\n", &NSommets);
    fclose(pf1);	
    
    /*  Object creation or exit exit(1) in case of memory pb  */
    if (!(new=(int *)calloc(NSommets, sizeof(int)))) ProblemeMemoire();
    if (!(old=(int *)calloc(NSommets, sizeof(int)))) ProblemeMemoire();
    if (!(et=(int *)malloc(NEt*sizeof(int)))) ProblemeMemoire();

    for (k = 0; k < NSommets; k++) {
        new[k]=-1;
        old[k]=-1;
    }
    printf("Allocation completed \n");
    
    /*      get the state number (j) and the stade description  (et)  */
    
    pf1=fopen(s4,"r");
    for (k = 0; k < NSommets; k++) {
        fscanf(pf1,"%d", &j);
        for (i = 0; i < NEt; i++)
      	{	fscanf(pf1,"%d",&nb );
      		et[i] = nb;
      	} 
        getc(pf1);
        
        b = NumerodeBlock(et, k);
        
        if (b<0)
  	  	{
            printf("the number must be positive \n"); exit(1);
  	  	}
        if (b>=NSommets)
  	  	{
            printf("the number must be smaller than the number of nodes, %d %d \n",b,j);
            exit(1);
  	  	}
        
        if (old[b]!=-1) {
            printf("Number %d is allready used",b); exit(1);
        }
        old[b]=k;
        new[k]=b;
    }
    
    fclose(pf1);
    printf("Generation of new numbers is now completed \n");
        
    pf1=fopen(s5,"w");
    for (k = 0; k < NSommets; k++) {
        fprintf(pf1,"%d %d \n ", k, new[k]);
    }
    fclose(pf1);
    exit(0);
}



