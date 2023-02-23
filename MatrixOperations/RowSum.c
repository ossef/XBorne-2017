/* 
 Row Sum : sparse row,  Input Rii 
 needs .Rii matrix and .sz size file
 To compute the mttf
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0


void  usage()
{
	printf("usage : RowSum -f filename \n");
	printf("filename.Rii and filename.sz must exist before. Creates filename.rwdT \n");
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
	int i, j, c, jx, degre ;
	double x, sumP ;
	char s0[250];
	char s1[250];
	char s2[250];
	FILE *pf1,*pf2;
	
	/*
	 On recupere le nom du modele et on verifie que les fichiers existent 
	 */
	
	if (argc!=3) usage();
	--argc;
	++argv;
	if (**argv != '-') usage();
	switch (*++*argv) {
		case 'f' :  /* on recupere le nom de fichier */
		{   ++argv;
			strcpy(s0,*argv);            
			strcpy(s1,s0);
			strcpy(s2,s0);
			strcat(s0,".Rii");
			strcat(s1,".sz");
			strcat(s2,".rwdT");
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
    
   
    pf1=fopen(s0,"r");
    pf2=fopen(s2,"w");
    
    for (i = 0; i < NSommets ; i++) {
        fscanf(pf1,"%d", &c);
        fscanf(pf1,"%d", &degre);
        sumP=0.0; 
        for (j = 0; j < degre; j++) {
            fscanf(pf1,"%lf", &x);
            fscanf(pf1,"%d", &jx);
            sumP+=x;
        }
        getc(pf1);
        fprintf(pf2,"%d %lf \n",c, sumP);
    }
    
    fclose(pf2);
    fclose(pf1);
    
     printf("Done RowSum \n");
    exit(0);
}

