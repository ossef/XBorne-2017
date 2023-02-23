/*
 * JMF Dynamic nabla : borne inf, matrice pleine
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXITERATIONS 20

void
usage()
{
	printf("usage : DynamicL -f filename \n");
	printf("filename.Rii, and filename.sz must exist before. \n");
	exit(1);
}

void
ProblemeMemoire()
{
	printf("Sorry, not enougth memory for the probability vectors \n");
	exit(1);
}

static double max(x, y)
double x, y;
{
    if (x > y)
        return x;
    else
        return y;
}

static double min(x, y)
double x, y;
{
    if (x > y)
        return y;
    else
        return x;
}

int
main(argc, argv)
int             argc;
char           *argv[];
{
	long            NRow, NCol, NArcs, NEt;
	long            i, j, k, c, jx, degre;
	double          R, S;
	double          x;
    
	FILE           *pf1;
	char            s0[200];
	char            s3[200];
	char            s4[200];
	char            s5[200];
    
	/* les objets dynamiques a creer par malloc */
    
	double    *dinf;
    double    *P;
    double    *pinf1, *pinf2;
    
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
			 * on ajoute les extensions .sz et .Rii
			 */
			printf("ok\n");
			
			strcpy(s3, s0);
			strcpy(s4, s3);
			strcat(s3, ".sz");
			strcat(s4, ".Rii");
			strcpy(s5, s0);
			strcat(s5, ".DynFull.L.pi");
            
            
			/* on verifie si les fichiers d'entree existent */
			if ((pf1 = fopen(s3, "r")) == NULL) {
				usage();
			}
			fclose(pf1);
            printf("ok\n");
            
			if ((pf1 = fopen(s4, "r")) == NULL) {
				usage();
			}
			fclose(pf1);
			printf("ok\n");
			break;
		}
        default:
            usage();
	}
    
	/*
     On recupere les tailles 
     */
	pf1 = fopen(s3, "r");
	fscanf(pf1, "%ld\n", &NArcs);
	fscanf(pf1, "%ld %ld \n", &NRow, &NCol);
	fscanf(pf1, "%ld\n", &NEt);
    
	fclose(pf1);
	printf("%ld\n", NArcs);
	printf("%ld %ld\n", NRow, NCol);
	printf("%ld\n", NEt);
    
	/*
	 * On cree les objets ou on exit(1) en cas de pb
	 */
    
    if (!(pinf1 = (double * )malloc(NRow*sizeof(double)))) 
        ProblemeMemoire();
    if (!(P=(double *) calloc(NRow*NRow, sizeof(double)))) 
        ProblemeMemoire();
    if (!(dinf = (double * )malloc(NRow*sizeof(double)))) 
        ProblemeMemoire();
    if (!(pinf2 = (double * )malloc(NRow*sizeof(double)))) 
        ProblemeMemoire();
    
	/* On fait la lecture de la matrice */
    /* et on calcule dinf en meme temps */ 
    
	pf1 = fopen(s4, "r");
    
	for (j = 0; j < NRow; j++) {dinf[j]=1.0;}
 	for (i = 0; i < NRow; i++) {
		/*
		 * Lecture de la ligne
		 */
		/* On commence par le numero et le degre */
		fscanf(pf1, "%ld", &c);
		fscanf(pf1, "%ld", &degre);
		for (j = 0; j < degre; j++) {
			fscanf(pf1, "%lf", &x);
			fscanf(pf1, "%ld", &jx);
			printf("Lect %ld %ld %ld %lf \n", c, degre, jx, x);
			P[c*NRow+jx]=x;
        }
		getc(pf1);
		for (j = 0; j < NRow; j++) {
            dinf[j]= min(P[c*NRow+j],dinf[j]);      
        }
    }
    
    printf("dinf: ");      
    for (j = 0; j < NRow; j++) {
        printf("%.10f  ",dinf[j]);      
    }
    R=1.0;
    for (j = 0; j < NRow; j++) {R-=dinf[j];
        pinf2[j]=dinf[j];
    }
    printf("%lf \n",R);
    
    for (i=1;i< MAXITERATIONS; i++) {
        
        for (j = 0; j < NRow; j++) {
            S=dinf[j]*R;
            for (k = 0; k < NRow; k++) {
                S+=pinf2[k]*P[k*NRow+j];
            }
            pinf1[j]=max(S,pinf2[j]);
        }
        R=1.0;
        for (j = 0; j < NRow; j++) {R-=pinf1[j];
        }
        printf("%ld  & ",i);      
        
        for (j = 0; j < NRow; j++) {
            printf("%.20f & ", pinf1[j]);	 
            pinf2[j]=pinf1[j];}
        printf("%lf \\ \n",R);      
    }
    
    
	pf1 = fopen(s5, "w");
	fprintf(pf1, "%lf\n",dinf[1]);
	fclose(pf1);
    
	printf("Done \n");
	exit(0);
}
