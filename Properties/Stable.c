/* Stable */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

#define DEBUG 0

int *marked;
int *listS;
int *listnonS;

int *arcs;
int *degreein;
int *degreeout;
int *debut;
int *temp;

int *edges;
int *degree;
int *edgedebut;

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory \n");
	exit(1);
}

void InitGraphs(int NS, int NArcs)
{
    if (!(arcs=(int*)calloc(NArcs, sizeof(int)))) ProblemeMemoire();
    if (!(degreein=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
    if (!(degreeout=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
    if (!(debut=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
    if (!(edges=(int*)calloc(2*NArcs, sizeof(int)))) ProblemeMemoire();
    if (!(degree =(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
    if (!(edgedebut=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
    if (!(temp=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
}

void InitStructure(int NS)
{
    if (!(marked=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
    if (!(listS=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
    if (!(listnonS=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
}

/* Programme principal et interface exterieure */

void  usage()
{
	printf("usage : Stable -f filename suffix \n");
	printf("The files 'filename.suffix' and 'filename.sz' must exist before. \n");
	exit(1);
}

void printDIGraph(int NS, int NArcs)
{
    int i,j;
    for (i=0;i<NS;i++)
    {
        printf(" node: %d, degree: %d, successors: ", i, degreein[i]);
        for (j=0;j < degreein[i];j++) {
            printf(" %d ", arcs[debut[i]+j]);
        }
        printf("\n");
    }
}


void printGraph(int NS, int NArcs)
{
    int i,j;
    for (i=0;i<NS;i++)
    {
        printf(" node: %d, degree: %d, neighbors: ", i, degree[i]);
        for (j=0;j < degree[i];j++) {
            printf(" %d ", edges[edgedebut[i]+j]);
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
	char s3[250];
	char s4[250];
    char s0[250];
	int NS, NArcs, v, i, j, d, ou, successeur, ori, dest, inS, outS;
    double proba;
    
	if (argc!=4) usage();
    --argc;
	++argv;
	if (**argv != '-') usage();
    switch (*++*argv) {
        case 'f' :  /* on recupere le nom de fichier */
        {
            ++argv;
            strcpy(s1,*argv);
            ++argv;
            strcpy(s3,*argv);
            strcpy(s2,s1);
            strcpy(s4,s1);
            strcat(s4,"stable.part");
            strcat(s1,".sz");
            strcat(s2,".");
            strcat(s2,s3);
             strcpy(s0,s1);
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
            else if (DEBUG==0) {printf("OK file .Rii \n");}
            
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
        degreein[v]=d;
        debut[v]=ou;
        for (j=0;j<d;j++) {
            fscanf(pf1,"%lf %d",&proba,&successeur);
            arcs[ou]=successeur;
            degreeout[successeur]++;
            ou++;
        }
    }
    fclose(pf1);
    
    if (DEBUG==0) {printf("Read DiGraph, done \n");}
    
    if (DEBUG==0) {
        printDIGraph(NS,NArcs);
        printf("Write DiGraph, done \n");
    }
    
    for (i=0;i<NS;i++) {
        degree[i]=degreein[i]+degreeout[i];
    }
    
    edgedebut[0]=0;
    temp[0]=0;
    for (i=1;i<NS;i++) {
        edgedebut[i]=edgedebut[i-1]+degree[i-1];
        temp[i]=edgedebut[i];
    }
    for (i=0;i<NS;i++){
        for (j=0;j<degreein[i];j++) {
            ori=i;
            dest=arcs[j+debut[i]];
            printf("%d %d \n", ori,dest);
            edges[temp[ori]]=dest;
            temp[ori]++;
            edges[temp[dest]]=ori;
            temp[dest]++;
        }
    }
    if (DEBUG==0) {printf("Building the unoriented graph, done \n");}
    
    if (DEBUG==0) {
        printGraph(NS,NArcs);
        printf("Write Graph, done \n");
    }

    inS=0;
    outS=0;
    for(v=0;v<NS;v++) {
        if (DEBUG==0) {printf("%d %d %d \n",v, inS, outS);}
        if (marked[v]==0) {
            marked[v]=1;
            listS[inS]=v;
            inS++;
            for (i=0;i<degree[v];i++) {
                ou = edges[edgedebut[v]+i];
                if (marked[ou]==0) {
                    marked[ou]=1;
                    listnonS[outS]=ou;
                    outS++;
                }
            }
        }
    }
    
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
    exit(1);
}
