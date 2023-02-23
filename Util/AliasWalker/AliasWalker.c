/*
 Construction des deux tables de l'algorithme de Walker
 pour utiliser une methode d'alais dans une simulation
 en entree : une distribution de proba
 en sortie : deux vevteur dans un meme fichier
 la taille est dans le fichier .sz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define epsilon 0.000000001

double *dist;
double *prob;
int *other;


void  usage()
{
    printf("usage : AliasWalker -f filename  \n");
	printf("filename.pi and finame.sz must exist before \n");
	printf("filename.alias will be created \n");
	exit(1);
}

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory for the vectors \n");
	exit(1);
}

int main(argc, argv)
int argc;
char *argv[];
{
    int taille;
    int i, k, m, iter;
    double x, delta, S, min, max ;
    FILE *pf1;
    char s0[20];
    char s1[20];
    char s2[20];
    /*
     On recupere le nom du modele et on verifie que les fichiers existent
     */
    if (argc<3) usage();
        --argc;
	++argv;
	if (**argv != '-') usage();
        switch (*++*argv) {
            case 'f' :  /* on recupere le nom de fichier */
            {++argv;
                strcpy(s1,*argv);
                /* on ajoute .sz, et .pi */
                strcpy(s2,s1);
                strcpy(s0,s1);
                strcat(s2,".alias");
                strcat(s1,".sz");
                strcat(s0,".pi");
                /* on verifie si il existe */
                if ((pf1=fopen(s1,"r"))==NULL)
                {
                    usage();printf("Problem with the .sz file\n");
                }
                if ((pf1=fopen(s0,"r"))==NULL)
                {
                    usage();printf("Problem with the .pi file\n");
                }
                break;
            }
            default	 : usage();
        }
    /*
     On recupere la taille dans le fichier filename.sz et on cree les objets dynamiques
     */
    pf1=fopen(s1,"r");
    fscanf(pf1,"%d\n", &taille);
    fclose(pf1);
    
    if (!(dist =(double *)calloc(taille, sizeof(double)))) ProblemeMemoire();
        
        if (!(prob =(double *)calloc(taille, sizeof(double)))) ProblemeMemoire();
            
            if (!(other =(int *)calloc(taille, sizeof(int)))) ProblemeMemoire();
                
                printf("Fin de lecture de la taille et des allocations \n");
                
                
            /*
             On lit la distribution
             */
                pf1=fopen(s0,"r");
                
                for (k = 0; k < taille ; k++) {
                    fscanf(pf1,"%lf", &x);
                    dist[k]= x;
                    getc(pf1);
                }
    fclose(pf1);
    printf("Fin lecture distribution \n");
    /* Init Walker */
    delta = 1.0/taille;
    for (k = 0; k < taille ; k++) {
        dist[k]= dist[k]-delta;
        other[k]=k;
        prob[k]=0.0;
    }
    
    /* Boucle Walker */
    for (iter=0; iter < taille ; iter++)
    {
        min = dist[0];
        k=0;
        for (i = 1; i < taille ; i++) {
            if (dist[i]<min) {min = dist[i]; k=i;}
        }
        max = dist[0];
        m = 0;
        for (i = 1; i < taille ; i++) {
            if (dist[i]>max) {max = dist[i];
                m =i;
            }
        }
        
        S = 0.0;
        for (i = 0; i < taille ; i++) {
            if (dist[i]>0) {S+= dist[i];} else {S -= dist[i];}
        }
        printf(" %d %f \n", iter,S );
        if (S<epsilon) {break;}
        
        other[k]=m;
        x = 1.0*taille*min;
        prob[k]=1.0+ x;
        dist[k]=0.0;
        dist[m]= min+max;
        
    }
    
    printf("Ecriture des vecteurs \n");
    pf1=fopen(s2,"w");
    for (k = 0; k < taille ; k++) {
        fprintf(pf1,"%f  %d \n", prob[k], other[k]+1 );
    }
    printf("Done AliasWalker\n");
    fclose(pf1);
    exit(0);
}



