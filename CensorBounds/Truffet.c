/*
 * JMF Truffet Upper and Lower Bound, par ligne version long double,
 * resultat en Rii 8/8/2008
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Truffet.h"

void
usage()
{
	printf("usage : Truffet -f filename Suffix \n");
	printf("filename.Suffix.Rii, and filename.Suffix.sz must exist before. \n");
	printf("Typically Suffix is NW (north west). \n");
	printf("filename.SA.T.U.Rii, filename.SA.T.U.sz, filename.SA.T.L.Rii, filename.SA.T.L.sz are created. \n");
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
	printf("Sorry, the block is not square \n");
	exit(1);
}

int
main(argc, argv)
	int             argc;
	char           *argv[];
{
	long            NRow, NCol, NArcs, NEt;
	long            i, j, c, jx, degre, NZEU, NZEL;
	long double     sum, S;
	double          x;

	FILE           *pf1, *pf2, *pf3;
	char            s0[20];
	char            s1[20];
	char            s3[20];
	char            s4[20];
	char            s5[20];
	char            s6[20];
	char            s7[30];
	char            s8[30];

	/* les objets dynamiques a creer par malloc */

	long double    *ligne1;

	/*
	On recupere le nom du modele et on verifie que les fichiers existent
	*/

	if (argc != 4)
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
			++argv;	/* on recupere le premier suffixe */
			strcpy(s1, *argv);

			/*
			 * on ajoute le suffixe NW et les extensions .sz et
			 * .Rii
			 */
			printf("ok\n");
			
			strcpy(s3, s0);
			strcat(s3, ".");			
			strcat(s3, s1);
			strcpy(s4, s3);
			strcat(s3, ".sz");
			strcat(s4, ".Rii");
			/*
			 * on ajoute le suffixe SA.T.U (upper bound of the
			 * stochastic complement by Truffet's algorithm) et
			 * les extensions .sz et .Rii
			 */
			strcpy(s7, s0);
			strcat(s7, ".SA.T.U");
			strcpy(s8, s7);
			strcat(s7, ".sz");
			strcat(s8, ".Rii");
			/*
			 * on ajoute le suffixe SA.T.L (upper bound of the
			 * stochastic complement by Truffet's algorithm) et
			 * les extensions .sz et .Rii
			 */
			strcpy(s5, s0);
			strcat(s5, ".SA.T.L");
			strcpy(s6, s5);
			strcat(s5, ".sz");
			strcat(s6, ".Rii");

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
	On recupere les tailles du bloc NW
	*/
	pf1 = fopen(s3, "r");
	fscanf(pf1, "%ld\n", &NArcs);
	fscanf(pf1, "%ld %ld \n", &NRow, &NCol);
	fscanf(pf1, "%ld\n", &NEt);

	fclose(pf1);
	printf("%ld\n", NArcs);
	printf("%ld %ld\n", NRow, NCol);
	printf("%ld\n", NEt);

	/* le bloc NW doit etre carre  */
	if (NCol != NRow)
		ProblemeBloc();


	/*
	 * On cree les objets ou on exit(1) en cas de pb
	 */

	if (!(ligne1 = (long double *) malloc(NCol * sizeof(long double))))
		ProblemeMemoire();


	/* ALGORITHME PAR LIGNE */
	/* On fait la lecture de la matrice NW */
	/* et on cree les matrices borne sup et borne inf en meme temps */

	/*
	 * Calculer le nombre d'elements non nul au cours de la construction
	 * de la borne
	 */
	NZEU = 0;
	NZEL = 0;


	pf1 = fopen(s4, "r");
	pf2 = fopen(s6, "w");
	pf3 = fopen(s8, "w");

	for (i = 0; i < NRow; i++) {
		/*
		 * Lecture de la ligne
		 */

		/* Initialisation */
		for (j = 0; j < NCol; j++) {
			ligne1[j] = 0.0;
		}
		sum = 0.0;
		/* On commence par le numero et le degre */
		fscanf(pf1, "%ld", &c);
		fscanf(pf1, "%ld", &degre);
		for (j = 0; j < degre; j++) {
			fscanf(pf1, "%lf", &x);
			fscanf(pf1, "%ld", &jx);
			if (DEBUG == 1) {
				printf("Lect %ld %ld %ld %lf \n", c, degre, jx, x);
			}
			ligne1[jx] = x;
			sum += x;
		}
		getc(pf1);

		/* si la somme est 1 ne rien changer a la ligne lue */
		sum = 1.0 - sum;
		if (sum > epsNull) {
			/* Borne Inf */
			S = ligne1[0];
			if (S == 0.0)
				degre++;
			S += sum;
			fprintf(pf2, "%ld %ld ", i, degre);
			fprintf(pf2, " %lf 0 ", S);
			for (j = 1; j < NCol; j++)
				if (ligne1[j] > 0)
					fprintf(pf2, " %lf %ld ", ligne1[j], j);
			fprintf(pf2, "\n");
			NZEL += degre;
			/* Borne Sup */
			jx = NCol - 1;
			S = ligne1[jx];
			if (S == 0.0)
				degre++;
			S += sum;
			fprintf(pf3, "%ld %ld ", i, degre);
			for (j = 0; j < jx; j++)
				if (ligne1[j] > 0)
					fprintf(pf3, " %lf %ld ", ligne1[j], j);
			fprintf(pf3, " %lf %ld", S, jx);
			fprintf(pf3, "\n");
			NZEU += degre;
		} else {
			/* Borne Inf */
			fprintf(pf2, "%ld %ld ", i, degre);
			for (j = 0; j < NCol; j++)
				if (ligne1[j] > 0)
					fprintf(pf2, " %lf %ld ", ligne1[j], j);
			fprintf(pf2, "\n");
			NZEL += degre;
			/* Borne Sup */
			fprintf(pf3, "%ld %ld ", i, degre);
			for (j = 0; j < NCol; j++)
				if (ligne1[j] > 0)
					fprintf(pf3, " %lf %ld ", ligne1[j], j);
			fprintf(pf3, "\n");
			NZEU += degre;
		}
	}
	fclose(pf1);
	fclose(pf2);
	fclose(pf3);

	pf1 = fopen(s7, "w");
	fprintf(pf1, "%ld\n", NZEU);
	fprintf(pf1, "%ld\n", NRow);
	fprintf(pf1, "%ld\n", NEt);
	fclose(pf1);
	pf1 = fopen(s5, "w");
	fprintf(pf1, "%ld\n", NZEL);
	fprintf(pf1, "%ld\n", NRow);
	fprintf(pf1, "%ld\n", NEt);
	fclose(pf1);

	printf("Done Truffet Upper and Lower Bound of the Stochastic Complement \n");
	exit(0);
}
