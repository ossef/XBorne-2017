/*
 0 use the same interface as the Markov chain generator
 1 Find the complete state space
 2 Simul
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* 
 inclusions des constantes, types et variables particulieres a chaque modele
 */
#include "const.h"
#include "constsim.h"
#include "var.h"

#define DEBUG 0

/*Variables donnant l'etendue de chaque variable isolee*/

static int Min[NEt], Max[NEt];

/* Pour  les etats  du modele */ 

typedef int Etat[NEt];

/*On utilise cet objet pour garder les transitions probabilises de la
 chaine et aussi pour garder les etats en cours de creation*/

struct item {
	Etat voisin;
	double prob;
	int numero;
	struct item *next, *last;
};

typedef struct item item;

/*Variables qui doivent etre invisibles pour le pgm principal*/
/* NE PAS TOUCHER */

item *Poubelle;   /*pointeur vers le sommet de la pile poubelle*/
item *APrendre, *PourAjouter; /*pointeurs sommets en attente de visite*/
item *PointeVisite;   /*pointeur  vers le sommet des successeurs*/

int succ; /* le degre du sommet courant  */
int NbSommets; /* le nombre de sommets atteignables */

/* Les deux structures  de grande taille dimensionnees par Maxsize */
int MaxSize;
int NombreDeMark; 
short *Mark;
int  *TableCorrespondance;

/*---------------------------------------------*/
/* conversion d'un etat en un numero           */
/*  attention, il est possible que cela deborde*/
/*  le numero minimal est 1                    */
/*---------------------------------------------*/

int Etat2Int(E)
int *E;
{
	int i, j;
	i = E[0] - Min[0];
	for (j = 1; j < NEt; j++)
    {
		i = i * (Max[j] - Min[j] + 1) + E[j] - Min[j];
    }
	return (i + 1);
}

/*---------------------------------------------*/
/*gestion usuelle d'une pile de vieux elements */
/*pour eviter d'employer la function de        */
/*liberation memoire                           */
/*---------------------------------------------*/
void InitPoubelle()
{
	Poubelle = NULL;
	PourAjouter = NULL;
	APrendre = NULL;
}

static void Nouveau(p)
item **p;
{
	if (Poubelle != NULL) {
		*p = Poubelle;
		Poubelle = ((*p)->next);
	} else
		*p = (item *)malloc(sizeof(item));
}

void Depose(p)
item *p;
{
	p->next = Poubelle;
	Poubelle = p;
}

/* Inclusion des fonctions specifiques a un modele */

#include "fun.c"

/*---------------------------------------------*/
/*                                             */
/*Gestion des sommets deja visites et a visiter*/
/*  MiseEnFile  FileVide SortieDeFile          */
/* NumeroDeVisite, CreerMark, InitMark         */
/*                                             */
/*---------------------------------------------*/
void MiseEnFile(It)
item It;
{
	item *p;
	
	Nouveau(&p);
	*p = It;
	
	p->next = PourAjouter;
	p->last = NULL;
	if (PourAjouter == NULL)
		APrendre = p;
	else
		PourAjouter->last = p;
	PourAjouter = p;
}

short FileVide()
{
	if (PourAjouter == NULL) {return 1;} 
	return(0);
}

void SortieDeFile(It)
item *It;
{
	item *p;
	
	*It = *APrendre;
	p = APrendre;
	APrendre = APrendre->last;
	if (APrendre == NULL)
		PourAjouter = NULL;
	Depose(p);
}

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory for the product of the components, \n");
	printf("try to change your model decomposition or size of the components \n");
	exit(1);
}


void CreerMark()
{
	int i, j;
	i = 1;
	for (j = 0; j < NEt; j++)
		i *= (Max[j] - Min[j] + 1);
	i++;
	MaxSize=i;
	/*
	 i est la taille des deux structures que l'on doit creer 
	 */
	if (!(Mark=(short *)calloc(MaxSize, sizeof(short)))) ProblemeMemoire();
	printf("First Malloc OK, vector of size %12d\n", MaxSize);
	if (!(TableCorrespondance=(int *)calloc(MaxSize,sizeof(int)))) ProblemeMemoire();
	printf("Second Malloc OK, same size \n");
	
}


void InitMark()
{	
	NombreDeMark = -1;
}


void PrintEtat(E)
Etat E;
{
	int i;
	for(i=0;i<NEt;i++) {
		printf("%5d",E[i]);
	}
    printf("\n");
}

short DejaVisite(It)
item It;
{
	int i;
	short deja;
	i = Etat2Int(It.voisin)-1;
	deja = Mark[i];
	if (Mark[i]==1) Mark[i]=2;
	return deja;
}

int NumeroDeVisite(E)
Etat E;
{
	int i,j;
	i = Etat2Int(E)-1;
	/*  printf("%12d \n",i);   PrintEtat(E); */
	if (Mark[i]>0)
    {
		j = TableCorrespondance[i];
    }
	else {
		Mark[i]=1;
		NombreDeMark++;
		TableCorrespondance[i] = NombreDeMark;
		j = NombreDeMark;
	}
	NbSommets = NombreDeMark + 1;
	return(j);
}



/* Programme principal et interface exterieure */


void  usage()
{
	printf("usage : SimulAndCover -f filename \n");
	printf("to create filename.path, filename.cd and filename.sz \n");
	printf("to store the description of the states and the matrix of your model \n");
	printf("The files must not exist before. \n");
	exit(1);
}


int main(argc,argv)
int argc;
char *argv[];
{
	Etat F, G, R;
	item It, It2;
	int j,i,t, bcli, cov , seed;
	short deja;
	double pro, SP, u, perc ;
	FILE *pf1;
	char s1[20];
	char s2[20];
	char s3[20];
    char s4[20];
	double distprob[NbEvtsPossibles+1];
    
	if (argc<3) usage();
	--argc;
	++argv;
	if (**argv != '-') usage();
	switch (*++*argv) {
		case 'f' :  /* on recupere le nom de fichier */
		{++argv;
			strcpy(s1,*argv);
            ++argv;
 			strcpy(s4,*argv);
            seed = atoi(s4); 
			/* on ajoute .sz, .cd ou .path */
			strcpy(s2,s1);
			strcpy(s3,s1);
			strcat(s1,".cd");
 			strcat(s2,s4);
			strcat(s2,".path");
			strcat(s3,".sz");
			/* on verifie si il existe deja */
			if ((pf1=fopen(s1,"r"))!=NULL)
			{
				fclose(pf1);
				usage();
			}
			if ((pf1=fopen(s2,"r"))!=NULL)
			{
				fclose(pf1);
				usage();
			}
			if ((pf1=fopen(s3,"r"))!=NULL)
			{
				fclose(pf1);
				usage();
			}
			pf1=fopen(s1,"w");
			break;
		}
		default	 : usage();
	}
	
	InitPoubelle();
	InitEtendue();
	InitParticuliere();
	CreerMark();
	InitMark();
	NbSommets = 0;
	EtatInitial(It.voisin);
	It.next = NULL;
	It.last = NULL;
	It.prob = 0.0;
	It.numero = NumeroDeVisite(It.voisin);
	j = Etat2Int(It.voisin)-1;
	Mark[j]=2;
	MiseEnFile(It);
	
	do {
		SortieDeFile(&It);
		
		/* One step transition  P */
		for (j = 1; j <= NbEvtsPossibles; j++) {
			Equation(It.voisin, j, F, R);
			pro = Probabilite(j, It.voisin);
			if (pro>0) {
                bcli=NumeroDeVisite(F); 
                for (i=0; i<NEt; i++) {It2.voisin[i]=F[i];}
                It2.prob = pro ;
                i = Etat2Int(F)-1;
                It2.numero=TableCorrespondance[i];
                deja=DejaVisite(It2);
                if (deja<2)  MiseEnFile(It2);
            }
		}
        
        /*ecriture du codage des etats sur les composantes*/
        
        fprintf(pf1,"%12d", It.numero);
        for (j = 0; j < NEt; j++)
            fprintf(pf1,"%12d", It.voisin[j]);
        fprintf(pf1,"\n");
        
    } while (!FileVide());
    
    fclose(pf1);
    pf1=fopen(s3,"w"); 
    fprintf(pf1,"%12d\n",0);
    fprintf(pf1,"%12d\n", NbSommets);
    fprintf(pf1,"%12d\n", NEt);
    fclose(pf1);
    
	
    printf("Done State Space generation \n");
    
    pf1=fopen(s2,"w"); 
    
    EtatInitial(G);
    i = Etat2Int(G)-1;
    Mark[i]=3; 
    cov=1;
    srand(seed);
    for (t   = 0; t < Tmax; t++) {
        
        /* gener a random number u */
        
        u  = (double)rand()/(double)(RAND_MAX);
        
        /* get the distribution of transition probabilities at this state */
        SP=0.0;
        for (j = 1; j <= NbEvtsPossibles; j++) {
			pro = Probabilite(j,G);
			SP+=pro;
            distprob[j]=SP;
        }
        
        /* inverse transform to get the  event number: j */
        
        for (j = 1; j <= NbEvtsPossibles; j++) {
			if (u<distprob[j]) break;
        }
        
        /* Apply  event j to get the state */ 
        
        Equation(G, j, F, R);
        
        i = Etat2Int(F)-1;
        if (Mark[i]==2) {Mark[i]=3; 
            cov++;}
        
        
#if (Compact==1)         
        fprintf(pf1, "%d  : ",t);
        for(i=0;i<NEt;i++) {
            fprintf(pf1,"%5d",F[i]);
        }
        fprintf(pf1,"\n");
#endif
#if (Compact==0) 
        i = Etat2Int(F)-1;
        j=TableCorrespondance[i];
        fprintf(pf1,"%d \n ",j );
#endif
        
        for (i=0; i<NEt; i++) {G[i]=F[i];}  
    }
    fclose(pf1); 
    printf("Done Simulation \n");
    
    perc = (double) (cov*100)/NbSommets;
    printf("Covering is \%d out of \%d, this is \%f percent. \n", cov, NbSommets, perc);
    
    exit(0);
}

