/* Algo Haddad-Moreaux, improved by Busic. 
 * Upper Bound, row version long double format ,
 * inputs lower and upper element wise bounds
 * results Rii sparse format  8/4/2011
 * JMF 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
usage()
{
	printf("usage : HMB -f filename \n");
	printf("filename.EU.Rii, filename.EU.sz, filename.EL.Rii, and filename.EL.szmust exist before. \n");
	exit(1);
}

void
ProblemeMemoire()
{
	printf("Sorry, not enougth memory for the probability vectors \n");
	exit(1);
}


void
ProblemeBloc()
{
	printf("Sorry, both matrices must have the same size... \n");
	exit(1);
}

void
ProblemeArc()
{
	printf("Sorry, Upper bound matrix must have more elemnts than lower bound... \n");
	exit(1);
}

long double valmin(a,b)
long double a,b;
{
	if (a<b) {return(a);}	
	else {return(b);}
}


int
main(argc, argv)
int             argc;
char           *argv[];
{
	int            NRowL, NRowU, NArcsU, NArcsL, NEt;
	int            i, j, c, jx, degre, NZEU;
	int            dU,dL,iU, iL, jL, jU;
	
	long double     VsumU, VsumL, LastLValue;
	long double     x;
	
	FILE           *pf1, *pf2, *pf3;
	char            s0[20];
	char            s1[20];
	char            s2[20];
	char            s3[20];
	char            s4[20];
	char            s5[20];
	char            s6[20];
	
	/* les objets dynamiques a creer par malloc */
	
	long double    *LigneLSValue, *LigneUSValue, *LigneCSValue; 
    int       *LigneLSIndex, *LigneUSIndex, *LigneCSIndex; 
	
	/*
	 On recupere le nom du modele et on verifie que les fichiers existent
	 */
	
	if (argc != 3)
		usage();
	--argc;
	++argv;
	if (**argv != '-')
		usage();
	switch (*++*argv) {
		case 'f':		/* on recupere le nom de fichier */
		{
			++argv;
			strcpy(s0, *argv);
			/*
			 * on ajoute les extensions .sz et
			 * .Rii
			 */
			printf("On y va...\n");
			
			strcpy(s3, s0);
			strcpy(s4, s0);
			strcat(s3, ".EL.sz");
			strcat(s4, ".EL.Rii");
			strcpy(s1, s0);
			strcpy(s2, s0);
			strcat(s1, ".EU.sz");
			strcat(s2, ".EU.Rii");
			/*
			 * on ajoute le suffixe SA.T.U (upper bound of the
			 * stochastic complement by Truffet's algorithm) et
			 * les extensions .sz et .Rii
			 */
			strcpy(s5, s0);
			strcat(s5, ".HM.U.sz");
			strcpy(s6, s0);
			strcat(s6, ".HM.U.Rii");
			
			/* on verifie si les fichiers d'entree existent */
			if ((pf1 = fopen(s1, "r")) == NULL) {
				usage();
			}
			fclose(pf1);
			
			if ((pf1 = fopen(s2, "r")) == NULL) {
				usage();
			}
			if ((pf1 = fopen(s3, "r")) == NULL) {
				usage();
			}
			fclose(pf1);
			
			if ((pf1 = fopen(s4, "r")) == NULL) {
				usage();
			}
			fclose(pf1);
			break;
		}
		default:
			usage();
	}
	/*
	 On recupere les tailles du bloc NW
	 */
	pf1 = fopen(s3, "r");
	fscanf(pf1, "%d \n", &NArcsL);
	fscanf(pf1, "%d \n", &NRowL);
	fscanf(pf1, "%d \n", &NEt);
	fclose(pf1);
	pf1 = fopen(s1, "r");
	fscanf(pf1, "%d\n", &NArcsU);
	fscanf(pf1, "%d \n", &NRowU);
	fscanf(pf1, "%d\n", &NEt);
	fclose(pf1);
	
	
	printf("%d  %d \n", NArcsL, NArcsU);
	printf("%d  %d \n", NRowL, NRowU);
	printf("%d\n", NEt);
	
	/* les matrices doivent etre de meme taille  */
	if (NRowU != NRowL)
		ProblemeBloc();
	if (NArcsU < NArcsL)
		ProblemeArc();
	/*
	 * On cree les objets ou on exit(1) en cas de pb
	 */
	if (!(LigneLSValue = (long double *) malloc(NRowL * sizeof(long double))))
		ProblemeMemoire();
	if (!(LigneLSIndex = (int *) malloc(NRowL * sizeof(int))))
		ProblemeMemoire();
	if (!(LigneUSValue = (long double *) malloc(NRowU * sizeof(long double))))
		ProblemeMemoire();
	if (!(LigneUSIndex = (int *) malloc(NRowU * sizeof(int))))
		ProblemeMemoire();
	if (!(LigneCSValue = (long double *) malloc(NRowU * sizeof(long double))))
		ProblemeMemoire();
	if (!(LigneCSIndex = (int *) malloc(NRowU * sizeof(int))))
		ProblemeMemoire();
	
	/* ALGORITHME PAR LIGNE */
	
	pf1 = fopen(s2, "r");
	pf2 = fopen(s4, "r");
	pf3 = fopen(s6, "w");
	
	NZEU =0;
	
	for (i = 0; i < NRowL; i++) {
		/* Lecture des lignes L et U */
		VsumU=0.0;
		fscanf(pf1, "%d", &c);
		fscanf(pf1, "%d", &degre);
		for (j = 0; j < degre; j++) {
			fscanf(pf1, "%LF", &x);
			fscanf(pf1, "%d", &jx);
			VsumU += x;
			LigneUSIndex[j] = jx; 
			LigneUSValue[j] = VsumU ; 
/*			printf("bcls %d %d %LF %LF \n",j,jx,x,VsumU); */
		}
		dU=degre;	
		getc(pf1);
		VsumL = 0.0;
		fscanf(pf2, "%d", &c);
		fscanf(pf2, "%d", &degre);
		for (j = 0; j < degre; j++) {
			fscanf(pf2, "%LF", &x);
			fscanf(pf2, "%d", &jx);
			LigneLSIndex[j] = jx;
			LigneLSValue[j] = VsumL;
			VsumL+= x;
/*			printf("bcli %d %d %LF %LF \n",j,jx,x,VsumL); */
		}
		getc(pf2);
		dL=degre;
		if (jx<(NRowL-1)) {
		    LigneLSIndex[dL]=jx+1;
		    LigneLSValue[dL]=VsumL;
		    dL++;
		} 
		
/*		for (j = 0; j < dL; j++) {
			printf("inf1 %d %d %LF \n",j,LigneLSIndex[j],LigneLSValue[j]);
		}
*/		
		
		VsumU=LigneUSValue[dU-1];
		
		for (j = dU-1; j >0; j--) {
			LigneUSValue[j]=VsumU-LigneUSValue[j-1];
		}
		LigneUSValue[0]=VsumU;

		if (dL<dU){
		    iL=0;
		    LastLValue=LigneLSValue[0];
		    for (iU=0;iU<dU;iU++) {
			jL=LigneLSIndex[iL];
			jU=LigneUSIndex[iU];
			if (jU==jL) {
			    LigneCSValue[iU]=LigneLSValue[iL]; 
			    LastLValue= LigneLSValue[iL];	    
			    iL++;}
			else {
			    LigneCSValue[iU]=LastLValue;
		    }
		    }    
		    for (j=0;j<dU;j++) {
		    	LigneLSIndex[j] = LigneUSIndex[j];
			LigneLSValue[j] = LigneCSValue[j];
			}
		dL=dU;
		    }

/*		for (j = 0; j < dU; j++) {
			printf("sup %d %d %LF \n",j,LigneUSIndex[j],LigneUSValue[j]);
		}
		for (j = 0; j < dL; j++) {
			printf("inf %d %d %LF \n",j,LigneLSIndex[j],LigneLSValue[j]);
		}
*/		
		for (j = 0; j < dU; j++) {
		    	LigneCSIndex[j] = LigneUSIndex[j];
			LigneCSValue[j] = valmin(LigneUSValue[j],1.0-LigneLSValue[j]);
		}
/*		for (j = 0; j < dU; j++) {
			printf("res %d %d %LF \n",j,LigneCSIndex[j],LigneCSValue[j]);
		}
*/
		LigneLSValue[dL-1] = LigneCSValue[dL-1];
		for (j = dL-2; j>=0; j--) {
				LigneLSValue[j] = LigneCSValue[j]- LigneCSValue[j+1];
		}

		fprintf(pf3,"%d %d ", c, dL);
		for (j = 0; j < dU; j++) {
			fprintf(pf3,"  %LF %d",LigneLSValue[j],LigneLSIndex[j]);
		}
		fprintf(pf3,"\n");
		NZEU+=dL;
}
fclose(pf1);
fclose(pf2);
fclose(pf3);

pf1 = fopen(s5, "w");
fprintf(pf1, "%d\n", NZEU);
fprintf(pf1, "%d\n", NRowU);
fprintf(pf1, "%d\n", NEt);
fclose(pf1);


printf("Done HMB Upper Bound for a set of matrices \n");
exit(0);
}
