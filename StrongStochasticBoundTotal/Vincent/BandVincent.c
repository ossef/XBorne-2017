/* 
 Vincent with an internal band format
 Compute Vincent upper bound
 needs .Rxx matrix and .sz size files
 version long double 
Result stored in Rii form. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Vincent.h"

#define MAX(X,Y) ((X) < (Y) ? (Y) : (X))


void  usage()
{
	printf("usage : BandRowVincent -f filename  \n");
	printf("filename.Rii and filename.sz must exist before. \n");
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
	int  NSommets, NArcs, NEt;
	int  i, j, k, c, degre, minl, minr, minp, maxl, maxr, maxp, ll, dest, arc, newmin, newmax; 
	long  double pr;
	FILE *pf1, *pf2;
	char s0[20];
	char s2[20];
	char s3[20];
	char s4[20];
	/* Dynamical structures to be created by malloc */
	long    double *ligne, *p, *r;
	/*
	 Get the model name and check that the files exist
	 */	
	if (argc!=3) usage();
	--argc;
	++argv;
	if (**argv != '-') usage();
	switch (*++*argv) {
		case 'f' :  /* model name */
		{++argv;
			strcpy(s0,*argv);
			/* addind suffix */
			strcpy(s2,s0);
			strcat(s2,".sz");
			strcpy(s4,s0);
			strcat(s4,".V.U.Rii");
			strcpy(s3,s0);
			strcat(s3,".V.U.sz");
			/* Filename.sz exist ?*/
			if ((pf1=fopen(s2,"r"))==NULL)
			{
				usage();
			}
			/* filename.Rii exits ?   */
			strcat(s0,".Rii");
			if ((pf1=fopen(s0,"r"))==NULL)
			{
				usage();
			}
			break;
		}
		default	 : usage();
	}
	
	/*
	 Get the sizes in file filename.sz
	 */
	pf1=fopen(s2,"r");
	fscanf(pf1,"%d\n", &NArcs);
	fscanf(pf1,"%d\n", &NSommets);
	fscanf(pf1,"%d\n", &NEt);
	fclose(pf1);	
	printf("%d\n", NArcs);
	printf("%d\n", NSommets);
	printf("%d\n", NEt);
	/* 
	 Objects creation or exit(1) if any problem 
	 */
	if (!(ligne=(long double *)malloc(NSommets*sizeof(long double)))) ProblemeMemoire();
	if (!(p=(long double *)malloc(NSommets*sizeof(long double)))) ProblemeMemoire();
	if (!(r=(long double *)malloc(NSommets*sizeof(long double)))) ProblemeMemoire();	
	/*
	 Row by row construction of the bound. First read a row in  filename.Rxx
	 */
	pf1=fopen(s0,"r");
	pf2=fopen(s4,"w");

 	/* First row receives a distinct treatment */
	fscanf(pf1,"%d", &c);
    fscanf(pf1,"%d", &degre);
	fscanf(pf1,"%Lf", &pr);
	fscanf(pf1,"%d", &dest);
	minl = dest;
	ligne[dest]=pr;
    
	if (degre==1) {maxl=dest;}
	else {
		ll=dest;
		for (j = 1; j < degre; j++) {
			fscanf(pf1,"%Lf", &pr);
			fscanf(pf1,"%d", &dest);
			for (k = ll+1; k < dest; k++) { ligne[k]=0; }
			ligne[dest]=pr;
			maxl=dest;
			ll=dest;
		}
	}
    
 	r[maxl]=ligne[maxl];
	for (j = maxl-1; j >= minl; j--) {r[j]=r[j+1]+ligne[j]; } 
	maxr=maxl;
	minr=minl;
    printf("init line 0 \n");

	/* the first row is kept unchanged */
	fprintf(pf2,"%d  %d ", c, degre);
	for (i = 0; i < NSommets; i++) 
		if (ligne[i]>0) fprintf(pf2, " %Lf %d ", ligne[i],i);
	fprintf(pf2,"\n");
    arc=degre;
    
 	/* ordinary row : from 1 to NSommet-1 */ 
	
	for (i = 1; i < NSommets ; i++) {
		fscanf(pf1,"%d", &c);
		fscanf(pf1,"%d", &degre);
		fscanf(pf1,"%Lf", &pr);
		fscanf(pf1,"%d", &dest);
		minl = dest;
		ligne[dest]=pr;
		if (degre==1) {maxl=dest;}
		else {
			ll=dest;
			for (j = 1; j < degre; j++) {
				fscanf(pf1,"%Lf", &pr);
				fscanf(pf1,"%d", &dest);
				for (k = ll+1; k < dest; k++) { ligne[k]=0; }
				ligne[dest]=pr;
				maxl=dest;
				ll=dest;
			}
		}
		p[maxl]=ligne[maxl];
		for (j = maxl-1; j >= minl; j--) {p[j]=p[j+1]+ligne[j]; } 
		maxp=maxl;
		minp=minl;
 
		/* p contains the partial sums in band format for that input line */ 
		newmin = MAX(minr,minp);
		if (maxr>maxp) {
			for (j = maxp+1; j<=maxr; j++) {ligne[j]=r[j]; } 
			for (j = newmin; j<=maxp; j++) {ligne[j]=MAX(r[j],p[j]); } 
			newmax = maxr;
		}
		else {
			for (j = maxr+1; j<=maxp; j++) {ligne[j]=p[j]; } 
			for (j = newmin; j<=maxr; j++) {ligne[j]=MAX(r[j],p[j]); } 
			newmax = maxp;
		}
		/* r is the row of the bound in band format */ 
		minr=newmin;
		maxr=newmax;
		for (j = minr; j<=maxr; j++) {r[j]=ligne[j]; } 

        /* storing the result in sparse format */ 	  
		for (j = minr; j<maxr; j++) {
			ligne[j]-=ligne[j+1];
		}
        degre=0;
		for (j = minr; j<=maxr; j++) {
			if (ligne[j]>epsNull) degre++;
		}
        arc +=degre;
		/* write the node number (c) and the degree  */      
		fprintf(pf2,"%d %d ", c, degre);
		for (j = minr; j<=maxr; j++) {
			if (ligne[j]>epsNull) fprintf(pf2, " %Lf %d ", ligne[j],j);
		}
        fprintf(pf2,"\n");       
	}
    fclose(pf1);
    fclose(pf2);
	/* now write the description file */  
	pf1=fopen(s3,"w");
	fprintf(pf1,"%d\n", arc);
	fprintf(pf1,"%d\n", NSommets);
	fprintf(pf1,"%d\n", NEt);
	fclose(pf1);
	printf("Done BandRowVincent Upper Bound\n");
	exit(0);
}
