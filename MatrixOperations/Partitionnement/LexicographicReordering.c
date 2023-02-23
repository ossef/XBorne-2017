/* Give a new number for reordering the state space according to the lexicographic ordering 
 We only create the permutation to apply  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#define Title 250 
#define DEBUG 1

/* The model state */ 

typedef int Etat[NEt];
static int Min[NEt], Max[NEt];

#include "fun.c"

int *new,*old;
int *et;

void  usage()
{
    printf("usage : LexicographicReordering -f filename  \n");
	printf("filename.cd and filename.sz must exist before \n");
	printf("filename.lexi.perm will be created \n");
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
    char s3[Title];
    char s4[Title];
    char s5[Title];
    
	if (argc!=3) usage();
	--argc;
	++argv;
	if (**argv != '-') usage();
	switch (*++*argv) {
		case 'f' :  /* get the model name */
		{++argv;
            strcpy(s1,*argv);
            ++argv;             
            /* add .sz, and .cd */
            strcpy(s4,s1);
            strcpy(s3,s1);
            strcpy(s5,s1);
            strcat(s3,".sz");
            strcat(s4,".cd");
            strcat(s5,".lexi.perm");
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
    
    /*     Get the sizes in filename.sz   */
    
    pf1=fopen(s3,"r");
    fscanf(pf1,"%d\n", &NArcs);
    fscanf(pf1,"%d\n", &NSommets);
    fclose(pf1);
    printf("Gettin the size of the model\n");

    InitEtendue();
    
    i = Max[0] - Min[0];
    for (j = 1; j < NEt; j++)
    {
        i = i * (Max[j] - Min[j] + 1) + Max[j] - Min[j];
    }
    i++;
    
    if (i!=NSommets) {
        printf("This is only feasable if all the states are reachable, %d, %d \n", NSommets, i);
        exit(1);
    }
    
    /*      Object creation or exit exit(1) in case of memory pb  */
    if (!(new=(int *)calloc(NSommets, sizeof(int)))) ProblemeMemoire();
    if (!(et=(int *)malloc(NEt*sizeof(int)))) ProblemeMemoire();
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
 
        b  = et[0] - Min[0];
        if (DEBUG==1)  { printf(" %d \n", b );}
        
        for (j = 1; j < NEt; j++)
        {
            b = b * (Max[j] - Min[j] + 1) + et[j] - Min[j];
            if (DEBUG==1) {printf(" %d %d %d %d %d\n", b,j, Max[j], Min[j], et[j]);}
        }        
        new[k]=b;
    }
    fclose(pf1);
    
    printf("Generation of new numbers is now completed \n");
    pf1=fopen(s5,"w");
    for (k = 0; k < NSommets; k++) {
        fprintf(pf1,"%d \n ", new[k]);
    }
    fclose(pf1);
    exit(0);
}



