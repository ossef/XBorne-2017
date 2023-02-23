/* Absorbing states. Creation of a partition */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

#define DEBUG 0

int *listS;
int *listnonS;

int *arcs;
int *degree;
int *debut;


void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory \n");
	exit(1);
}

void InitGraphs(int NS, int NArcs)
{
    if (!(arcs=(int*)calloc(NArcs, sizeof(int)))) ProblemeMemoire();
    if (!(degree=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
    if (!(debut=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
}

void InitStructure(int NS)
{
    if (!(listS=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
    if (!(listnonS=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
}

/* Programme principal et interface exterieure */

void  usage()
{
	printf("usage : Absorbing -f filename  \n");
	printf("The files 'filename.Rii' and 'filename.sz' must exist before. \n");
	exit(1);
}

void printDIGraph(int NS, int NArcs)
{
    int i,j;
    for (i=0;i<NS;i++)
    {
        printf(" node: %d, degree: %d, successors: ", i, degree[i]);
        for (j=0;j < degree[i];j++) {
            printf(" %d ", arcs[debut[i]+j]);
        }
        printf("\n");
    }
}



int main(argc,argv)
int argc;
char *argv[];
{
	FILE *pf1 ;
	char s1[250];
	char s2[250];
	char s4[250];
	int NS, NArcs, v, i, j, d, ou, successeur, inS, outS;
    double proba;
    
	if (argc!=3) usage();
    --argc;
	++argv;
	if (**argv != '-') usage();
    switch (*++*argv) {
        case 'f' :  /* on recupere le nom de fichier */
        {
            ++argv;
            strcpy(s1,*argv);
            strcpy(s2,s1);
            strcpy(s4,s1);
            strcat(s4,".part");
            strcat(s1,".sz");
            strcat(s2,".Rii");
            if (DEBUG==0) {
                printf("%s\n", s2);
                printf("%s\n", s1);
            }
            if ((pf1=fopen(s1,"r"))==NULL)
            {
                fclose(pf1);
                usage();
            }
            else if (DEBUG==0) {printf("OK file .sz \n");}
            if ((pf1=fopen(s2,"r"))==NULL)
            {
                fclose(pf1);
                usage();
            }
            else {if (DEBUG==0) {printf("OK file .Rii \n");}}
            
            printf("Files OK\n"); break;
        }
        default	 : usage();
    }
    
    pf1=fopen(s1,"r");
    fscanf(pf1,"%d \n",&NArcs);
    fscanf(pf1,"%d \n",&NS);
    fclose(pf1);
    if (DEBUG==0) {printf("%d %d \n",NArcs,NS);}
    InitStructure(NS);
    if (DEBUG==0) {printf("Init Structures, done \n");}
    InitGraphs(NS,NArcs);
    if (DEBUG==0) {printf("Init DiGraph,done \n");}
    
    pf1=fopen(s2,"r");
    ou=0;
    for (i=0;i<NS;i++){
        fscanf(pf1,"%d ",&v);
        fscanf(pf1,"%d \n",&d);
        degree[v]=d;
        debut[v]=ou;
        for (j=0;j<d;j++) {
            fscanf(pf1,"%lf %d",&proba,&successeur);
            arcs[ou]=successeur;
            ou++;
        }
    }
    fclose(pf1);
    
    if (DEBUG==0) {printf("Read DiGraph, done \n");}
    
    if (DEBUG==0) {
        printDIGraph(NS,NArcs);
        printf("Write DiGraph, done \n");
    }
    
    inS=0;
    outS=0;
    for(v=0;v<NS;v++) {
        if (DEBUG==0) {printf("%d %d %d \n",v, inS, outS);}
        if ((degree[v]==1) && (v==arcs[debut[v]])) { 
            listS[inS]=v;
            inS++;
        }
        else {
            listnonS[outS]=ou;
            outS++;
        }
    }
    if (inS==0) {printf("No absorbings nodes found. I have not created the partition file. \n");}
    else {
        printf("%d absorbings nodes found. I have created the partition file. \n", inS);
        pf1=fopen(s4,"w");
        fprintf(pf1,"2 \n");
        fprintf(pf1,"%d ",inS);
        for (i=0;i<inS ;i++) {
            fprintf(pf1,"%d ",listS[i]);
        }
        fprintf(pf1,"\n");
        fprintf(pf1,"%d ",outS);
        for (i=0;i<outS ;i++) {
            fprintf(pf1,"%d ",listnonS[i]);
        }
        fprintf(pf1,"\n");
        fclose(pf1);
    }
    exit(1);
}
