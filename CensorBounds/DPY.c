/*
 * JMF DPY Upper Bound par ligne version long double Ecrit le resultat en Rii
 * 8/8/2008
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DPY.h"

void 
usage()
{
	printf("usage : DPYUP -f filename Suffix1 Suffix2 \n");
	printf("filename.Suffix1.Rii, filename.Suffix2.Rii and filename.sz must exist before. \n");
	printf("Typically Suffix1 is NW (north west) and Suffix 2 is SW. \n");
	printf("filename.SA.DPY.U.Rii, filename.SA.DPY.U.sz  are created. \n");
	exit(1);
}

void 
ProblemeMemoire()
{
	printf("Sorry, not enougth memory for the probability vectors \n");
	exit(1);
}

void 
ProblemeMatrices()
{
	printf("Sorry, the number of colums is not the same for both matrices \n");
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
	long            NRow, NCol, NRow2, NArcs, NEt;
	long            i, j, c, jx, degre, deg, NZE;
	long double     sum;
	double          x;

	FILE           *pf1, *pf2;
	char            s0[20];
	char            s1[20];
	char            s2[20];
	char            s3[20];
	char            s4[20];
	char            s5[20];
	char            s6[20];
	char            s7[30];
	char            s8[30];

	/* les objets dynamiques a creer par malloc */

	long double    *ligne1, *ligne2, *ligne3;

	/*
	On recupere le nom du modele et on verifie que les fichiers existent
	*/

	if (argc != 5)
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
			++argv;	/* on recupere le second suffixe */
			strcpy(s2, *argv);

			/*
			 * on ajoute le suffixe NW et les extensions .sz et
			 * .Rii
			 */
			strcpy(s3, s0);
			strcat(s3, ".");
			strcat(s3, s1);
			strcpy(s4, s3);
			strcat(s3, ".sz");
			strcat(s4, ".Rii");
			/*
			 * on ajoute le second suffixe (ie SW) et les extensions .sz et
			 * .Rii
			 */
			strcpy(s5, s0);
			strcat(s5, ".");
			strcat(s5, s2);
			strcpy(s6, s5);
			strcat(s5, ".sz");
			strcat(s6, ".Rii");
			/*
			 * on ajoute le suffixe SA.DPY.U (upper bound of the
			 * stochastic complement by DPY) et les extensions
			 * .sz et .Rii
			 */
			strcpy(s7, s0);
			strcat(s7, ".SA.DPY.U");
			strcpy(s8, s7);
			strcat(s7, ".sz");
			strcat(s8, ".Rii");
			/* on verifie si les fichiers d'entree existent */
			if ((pf1 = fopen(s3, "r")) == NULL) {
				usage();
			}
			fclose(pf1);
			if ((pf1 = fopen(s4, "r")) == NULL) {
				usage();
			}
			fclose(pf1);
			if ((pf1 = fopen(s5, "r")) == NULL) {
				usage();
			}
			fclose(pf1);
			if ((pf1 = fopen(s6, "r")) == NULL) {
				usage();
			}
			fclose(pf1);
			break;
		}
	default:
		usage();
	}

	/*
	On recupere les tailles du bloc SW
	*/
	pf1 = fopen(s5, "r");
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

	if (!(ligne1 = (long double *) malloc(NCol * sizeof(long double))))
		ProblemeMemoire();
	if (!(ligne2 = (long double *) malloc(NCol * sizeof(long double))))
		ProblemeMemoire();
	if (!(ligne3 = (long double *) malloc(NCol * sizeof(long double))))
		ProblemeMemoire();

	/* Initialisation du sup a la plus petite ligne stochastique */
	for (j = 1; j < NCol; j++) {
		ligne2[j] = 0.0;
	}
	ligne2[0] = 1.0;

	/* ALGORITHME PAR LIGNE */

	pf1 = fopen(s6, "r");
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
		/*
		 * Normalisation de la ligne si le degre >0 Sinon c'est une
		 * ligne vide, il faut ne rien faire
		 */
		if (degre > 0) {
			for (j = 0; j < NCol; j++) {
				ligne1[j] /= sum;
			}

			/*
			 * Calcul du sup de la ligne courante et du sup
			 * courant dans ligne2 On applique l'operateur r de
			 * sommation sur ligne1
			 */

			for (j = NCol - 2; j > 0; j--) {
				ligne1[j] += ligne1[j + 1];
			}

			/* et on fait le max par element */
			for (j = 0; j < NCol; j++) {
				if (ligne1[j] > ligne2[j]) {
					ligne2[j] = ligne1[j];
				}
			}
		}
	}
	fclose(pf1);

	/* On applique l'inverse de l'operateur r a ligne2 */

	for (i = 0; i < NCol - 1; i++) {
		ligne2[i] -= ligne2[i + 1];
	}

	/* On passe maintenant a la lecture de la matrice NW */
	/* et on cree la matrice borne sup  en meme temps */


	pf1 = fopen(s3, "r");
	fscanf(pf1, "%ld\n", &NArcs);
	fscanf(pf1, "%ld %ld \n", &NRow2, &i);
	fscanf(pf1, "%ld\n", &NEt);
	fclose(pf1);

	printf("%ld\n", NArcs);
	printf("%ld %ld\n", NRow2, i);
	printf("%ld\n", NEt);

	/*
	 * le bloc NW doit etre carre et le nombre de colonnes doit etre le
	 * meme que dans le bloc SW
	 */

	if (i != NCol)
		ProblemeMatrices();
	if (i != NRow2)
		ProblemeBloc();

	pf1 = fopen(s4, "r");
	pf2 = fopen(s8, "w");

	/*
	 * Calculer le nombre d'elements non nul au cours de la construction
	 * de la borne
	 */
	NZE = 0;
	for (i = 0; i < NRow2; i++) {
		/*
		 * Lecture de la ligne
		 */
		for (j = 0; j < NCol; j++) {
			ligne1[j] = 0.0;
		}
		sum = 0.0;
		deg = 0;
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
		/* si la somme valait 1 il n'y a rien a ajouter */
		sum = 1 - sum;
		if (sum > epsNull)
			for (j = 0; j < NCol; j++) {
				ligne1[j] += sum * ligne2[j];
				if (ligne1[j] > 0)
					deg++;
			}
		else
			deg = degre;
		NZE += deg;

		fprintf(pf2, "%ld %ld ", i, deg);
		for (j = 0; j < NCol; j++)
			if (ligne1[j] > 0)
				fprintf(pf2, " %lf %ld ", ligne1[j], j);
		fprintf(pf2, "\n");
	}
	fclose(pf1);
	pf1 = fopen(s7, "w");
	fprintf(pf1, "%ld\n", NZE);
	fprintf(pf1, "%ld\n", NRow2);
	fprintf(pf1, "%ld\n", NEt);
	fclose(pf1);
	printf("Done DPY Up\n");
	exit(0);
}
