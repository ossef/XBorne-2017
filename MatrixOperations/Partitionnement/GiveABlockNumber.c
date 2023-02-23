/* Give a Block number for building a partition 
 We only create the description of the blocks
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const.h"

/* The model state */ 

typedef int Etat[NEt];

#include "funnum.c"

long *NewNum;
long *pointe;
long *taille;
long *begin;
long *bloc;
long *et;

void  usage()
{
    printf("usage : GiveBlockNumber -f filename  \n");
	printf("filename.cd and filename.sz must exist before \n");
	printf("filename.block will be created \n");
	exit(1);
}

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory for the vectors \n");
	exit(1);
}

int main(argc, argv)
int argc;
char *argv[];
{
    long NSommets, NArcs;
    long i, j, k, n, debut, blocmax, b, nb;
    FILE *pf1 ;
    char s1[20];
    char s3[20];
    char s4[20];
    char s5[20];

	if (argc!=3) usage();
	--argc;
	++argv;
	if (**argv != '-') usage();
	switch (*++*argv) {
		case 'f' :  /* get the model name */
		{++argv;
            strcpy(s1,*argv);
            ++argv;             
            /* add .sz, abd .cd */
            strcpy(s4,s1);
            strcpy(s3,s1);
            strcpy(s5,s1);
            
            strcat(s3,".sz");
            strcat(s4,".cd");
            strcat(s5,".block");
            
            /* does it exist ? */
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
    
    /*
     Get the sizes in filename.sz
     */
    
    pf1=fopen(s3,"r");
    fscanf(pf1,"%ld\n", &NArcs);
    fscanf(pf1,"%ld\n", &NSommets);
    fclose(pf1);	
    
    printf("Gettin the size of the model\n");
    
    /* 
     Object creation or exit exit(1) in case of memory pb
     */
    if (!(NewNum=(long *)calloc(NSommets, sizeof(long)))) ProblemeMemoire();
    if (!(pointe=(long *)calloc(NSommets, sizeof(long)))) ProblemeMemoire();
    if (!(begin=(long *)calloc(NSommets+1, sizeof(long)))) ProblemeMemoire();
    if (!(taille=(long *)calloc(NSommets, sizeof(long)))) ProblemeMemoire();
    if (!(bloc=(long *)calloc(NSommets, sizeof(long)))) ProblemeMemoire();
    if (!(et=(long *)malloc(NEt*sizeof(long)))) ProblemeMemoire();
    
    printf("Allocation completed \n");
    
    /* 
     get the state number (j) and the stade description  (et)
     */ 
    pf1=fopen(s4,"r");
    blocmax=0;
    for (k = 0; k < NSommets; k++) {
        fscanf(pf1,"%ld", &j); 
        for (i = 0; i < NEt; i++)
      	{	fscanf(pf1,"%ld",&nb );
      		et[i] = nb;
      	} 
        getc(pf1);
        
        b = NumerodeBlock(et, k);
        
        if (b<0)
  	  	{
            printf("the block number must be positive \n");
            exit(1);
  	  	}
        if (b>=NSommets)
  	  	{
            printf("the block number must be smaller than the number of nodes, %ld %ld \n",b,j);
            exit(1);
  	  	}
        
        bloc[k]=b;
        taille[b]++;
        if (b>blocmax) {blocmax=b;}
    }
    
    fclose(pf1);
    printf("Generation of block number completed \n");
    
    /* building the  blocks */ 
    debut = 0;
    j=0;
    for (i =0; i<=blocmax; i++) {
        begin[i]=debut;
        debut += taille[i];
        if (taille[i]>0) {j++;}
        pointe[i]=begin[i];
    } 
    
    /* Header of the model.block file */ 
    pf1=fopen(s5,"w");
    fprintf(pf1,"%ld 0.0 \n ",j);
    
    
    for (k = 0; k < NSommets; k++) {
        b = bloc[k];
        NewNum[pointe[b]]=k;
        pointe[b]++;
    }
    begin[blocmax+1] = pointe[blocmax];
    
    for (i =0; i<=blocmax; i++) {
        if (taille[i]>0) {            
            fprintf(pf1,"%ld ", taille[i]); 
            for(n=begin[i];n < begin[i+1]; n++) {
                fprintf(pf1,"%ld ", NewNum[n]);
            }
            fprintf(pf1," \n");
        }
    }
    fclose(pf1);
    exit(0);
}



