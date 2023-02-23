/* 
 NewSplit: sparse row,  Input Rii 
 needs .Rii matrix and .sz size file
 Write the result as 4 Rii matrices
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

struct item {
	double pro;
	int dest;
};

typedef struct item item;

item *Ouest;
item *Est;

void  usage()
{
	printf("usage : Split -f filename size \n");
	printf("filename.Rii and filename.sz must exist before. Creates finename.block.Rii and inename.block.sz \n");
	exit(1);
}

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory \n");
	exit(1);
}

int main(argc, argv)
int argc;
char *argv[];
{
	int NSommets, NArcs, NEt;
	int i, j, c, jx, degre, e, w, arcnw, arcne, arcsw, arcse, sizeNW ;
	double x;
	char s0[250];
	char s1[250];
	char s2[250];
	char s3[250];
	char s4[250];
	char s5[250];
	char s6[250];
	char s7[250];
	char s8[250];
	char s9[250];
	FILE *pf1,*pf2, *pf3;
	
	/*
	 On recupere le nom du modele et on verifie que les fichiers existent 
	 */
	
	if (argc!=4) usage();
	--argc;
	++argv;
	if (**argv != '-') usage();
	switch (*++*argv) {
		case 'f' :  /* on recupere le nom de fichier */
		{   ++argv;
			strcpy(s0,*argv);
            ++argv;
			strcpy(s1,*argv);
            sizeNW = atoi(s1);
            
			strcpy(s1,s0);
			strcpy(s2,s0);
			strcpy(s3,s0);
			strcpy(s4,s0);
			strcpy(s5,s0);
			strcpy(s6,s0);
			strcpy(s7,s0);
			strcpy(s8,s0);
			strcpy(s9,s0);
			strcat(s0,".Rii");
			strcat(s1,".sz");
			strcat(s2,".NW.Rii");
			strcat(s3,".NE.Rii");
			strcat(s4,".SW.Rii");
			strcat(s5,".SE.Rii");
			strcat(s6,".NW.sz");
			strcat(s7,".NE.sz");
			strcat(s8,".SW.sz");
			strcat(s9,".SE.sz");
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
    fclose(pf1);	
    
    if (!(Ouest=(item *)calloc(NSommets, sizeof(item)))) ProblemeMemoire();
    if (!(Est=(item *)calloc(NSommets, sizeof(item)))) ProblemeMemoire();
    
    if ((NSommets-sizeNW) < 1) {
        printf("The splitting parameter must be smaller than the size");
        exit(1);
    }
    
    pf1=fopen(s0,"r");
    
    /* begin with the North blocks */ 
     
    pf2=fopen(s2,"w");
    pf3=fopen(s3,"w");
    
    arcnw=0; arcne= 0; 	
    for (i = 0; i < sizeNW ; i++) {
        fscanf(pf1,"%d", &c);
        fscanf(pf1,"%d", &degre);
        w =0; e=0;
        for (j = 0; j < degre; j++) {
            fscanf(pf1,"%lf", &x);
            fscanf(pf1,"%d", &jx);
            if (jx<sizeNW) {
                Ouest[w].dest=  jx;
                Ouest[w].pro = x;
                w++;
            }
            else {
                Est[e].dest=jx;
                Est[e].pro=x;
                e++;
            }
        }
        getc(pf1);
        arcnw=arcnw+w;
        fprintf(pf2,"%d %d ", c, w);
        for (j=0; j<w; j++) {
            fprintf(pf2,"%lf %d ", Ouest[j].pro,Ouest[j].dest); 
        }
        fprintf(pf2,"\n");
        
        fprintf(pf3,"%d %d ", c, e);
        arcne=arcne+e;
        for (j=0; j<e; j++) {
            fprintf(pf3,"%lf %d ", Est[j].pro,Est[j].dest); 
        }
        fprintf(pf3,"\n");
    }
    
    fclose(pf2);
    fclose(pf3);
    
    /* Rii is done, build .sz now */
    
    pf2=fopen(s6,"w");
    fprintf(pf2," %d \n %d \n %d \n %d 0 0 \n", arcnw, sizeNW,  NEt, sizeNW);
    fclose(pf2);
    pf3=fopen(s7,"w");
    fprintf(pf3," %d \n %d \n %d \n %d 0 %d \n", arcne , sizeNW, NEt, NSommets-sizeNW, sizeNW );
    fclose(pf3);

    /* Now the South blocks */ 
    
    pf2=fopen(s4,"w");
    pf3=fopen(s5,"w");
    
    arcsw =0; arcse =0;
    for (i = sizeNW; i < NSommets ; i++) {
        fscanf(pf1,"%d", &c);
        fscanf(pf1,"%d", &degre);
        w =0; e=0;
        for (j = 0; j < degre; j++) {
            fscanf(pf1,"%lf", &x);
            fscanf(pf1,"%d", &jx);
            if (jx<sizeNW) {
                Ouest[w].dest=  jx;
                Ouest[w].pro = x;
                w++;
            }
            else {
                Est[e].dest=jx;
                Est[e].pro=x;
                e++;
            }
        }
        getc(pf1);
        arcsw=arcsw+w;
        fprintf(pf2,"%d %d ", c, w);
        for (j=0; j<w; j++) {
            fprintf(pf2,"%lf %d ", Ouest[j].pro,Ouest[j].dest); 
        }
        fprintf(pf2,"\n");
        
        fprintf(pf3,"%d %d ", c, e);
        arcse=arcse+e;
        for (j=0; j<e; j++) {
            fprintf(pf3,"%lf %d ", Est[j].pro,Est[j].dest); 
        }
        fprintf(pf3,"\n");
    }
    
    fclose(pf2);
    fclose(pf3);
    
    pf2=fopen(s8,"w");
    fprintf(pf2," %d \n %d \n %d \n %d %d 0 \n ", arcsw, NSommets-sizeNW,  NEt, sizeNW, sizeNW);
    fclose(pf2);
    pf3=fopen(s9,"w");
    fprintf(pf3," %d \n %d  \n %d \n %d %d %d \n", arcse , NSommets-sizeNW, NEt , NSommets-sizeNW, sizeNW,sizeNW);
    fclose(pf3);
        
    printf("Done Split \n");
    exit(0);
}

