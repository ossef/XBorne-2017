/*
 Tarjan SCC
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

#define DEBUG 0

int *stack;
int topstack;

int *incomponent;
int *root;
int *visited;
int *label;
int currentlabel;

int *arcs;
int *degre;
int *debut;

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory \n");
	exit(1);
}


void InitStack(int NS)
{
    if (!(root=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
    if (!(visited=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
    if (!(incomponent=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
    if (!(label=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
    currentlabel=0;
}



void InitGraph(int NS, int NArcs)
{
    if (!(arcs=(int*)calloc(NArcs, sizeof(int)))) ProblemeMemoire();
    if (!(degre=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
    if (!(debut=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
}


void InitStructure(int NS)
{
    if (!(stack=(int*)calloc(NS, sizeof(int)))) ProblemeMemoire();
    topstack=0;
}

void Push(int v)
{
    stack[topstack]=v;
    topstack++;
}

int Pop()
{
    topstack--;
    return(stack[topstack]);
}

void PrintStack()
{ int i;
    printf("stack \n");
    for(i=0;i<topstack;i++) {
        printf("%d %d \n",i,stack[i]);
    }
}


/* Programme principal et interface exterieure */


void  usage()
{
	printf("usage : SCC -f filename suffix   \n");
	printf("The file 'filename.suffix' must exist before. \n");
	exit(1);
}

void VISIT(int v)
{
    int de, w,u ;
    
    label[v]=currentlabel;
    currentlabel++;
    visited[v]=1;
    root[v]=label[v];
    incomponent[v]=0;
    
    Push(v);
    for (de=0;de<degre[v];de++) {
        w=arcs[de+debut[v]];
        if (visited[w]==0) {
            if (DEBUG==0) {printf("avant visit %d\n",w);}
            VISIT(w);
        }
        if (DEBUG==0) {printf("retour en %d depuis %d \n",v,w);}
        if (incomponent[w]==0) {root[v]= min(root[v],root[w]);}
    }
    if (DEBUG==0) {printf("apres boucle visit\n");
        PrintStack();
    }
    if (root[v]==v) {
        for (u=0;u<2;)
        {
            w=Pop();
            if (DEBUG==0) {printf("sortie pop :%d  etat courant %d \n",w , v);}
            incomponent[w]=1;
            if (w==v) break;
        }
        if (DEBUG==0) {printf("apres boucle pop\n");
            PrintStack();
        }
    }
}

void printSCC(int NS)
{
    int i;
    for(i=0;i<NS;i++) {
        printf("node: %d , label: %d, root:  %d  \n",i,label[i],root[i]);
    }
}

void printGraph(int NS, int NArcs)
{
    int i,j;
    for (i=0;i<NS;i++)
    {
        printf(" node: %d, degree: %d, successors: ", i, degre[i]);
        for (j=0;j < degre[i];j++) {
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
	char s3[250];
	char s4[250];
    char s0[250];
	int NS, NArcs, v, i, j, d, ou, successeur, nscc;
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
                strcat(s1,".sz");
                strcat(s2,".");
                strcat(s2,s3);
                strcpy(s4,s1);
                strcat(s4,".part");
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
	InitStack(NS);
    if (DEBUG==0) {printf("Inits Stack, done\n");}
	InitStructure(NS);
    if (DEBUG==0) {printf("Init Structures, done \n");}
 	InitGraph(NS,NArcs);
    if (DEBUG==0) {printf("Init DiGraph,done \n");}
    
    pf1=fopen(s2,"r");
    ou=0;
    for (i=0;i<NS;i++){
        fscanf(pf1,"%d ",&v);
        fscanf(pf1,"%d \n",&d);
        degre[v]=d;
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
        printGraph(NS,NArcs);
        printf("Write DiGraph, done \n");
    }
    
    for(v=0;v<NS;v++) {
        if (DEBUG==0) {printf("%d \n",v);}
        if (visited[v]==0) {
            VISIT(v);
        }
    }
    if (DEBUG==0) {printSCC(NS);}
    d=0;
    for (i=0;i<NS;i++) {if (root[i]!=root[0]) {d=1;}}
    if (d==0) {printf("the chain is irreducible.");}
    else {
        printf("The chain is not irreducible, you can find the components in %s.part  \n",s0);
        pf1=fopen(s4,"w");
        for (i=0;i<NS;i++) {incomponent[i]=0;}
        for (i=0;i<NS;i++) {incomponent[root[i]]++;}
        nscc =0;
        for (i=0;i<NS;i++) { if (incomponent[i]>0) {nscc++;}
        }
        debut[0]=0;
        degre[0]=0;
        for (i=1;i<NS;i++) {debut[i]=debut[i-1]+incomponent[i-1];
            degre[i]=debut[i];
        }
        for (i=0;i<NS;i++) {
            ou=root[i];
            visited[degre[ou]]=i;
            degre[ou]++;
        }
        
        fprintf(pf1,"%d \n",nscc);
        for (i=0;i<NS;i++) {
            if (incomponent[i]>0) {
                fprintf(pf1,"%d ",incomponent[i]);
                for (j=0;j<incomponent[i];j++) {fprintf(pf1," %d ",visited[debut[i]+j]);}
                fprintf(pf1,"\n");
            }
        }
        fclose(pf1);
    }
}
