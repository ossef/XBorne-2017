/*
 generation des etats et des etiquettes de transition a partir d'une equation d'evolution.
 Version isssue de generMarkov de 2012 dont on reprend une grosse partie du code.  
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* 
 inclusions des constantes, types et variables particulieres a chaque modele
 */
#include "const.h"

#define DEBUG 0

/*Variables donnant l'etendue de chaque variable isolee*/

static int Min[NEt], Max[NEt];

/* Pour  les etats  du modele */ 

typedef int Etat[NEt];

/* Pour les lables des transitions */

char Label[NbEvtsPossibles]; 

/*On utilise cet objet pour garder les transitions probabilises de la
 chaine et aussi pour garder les etats en cours de creation*/

struct item {
	Etat voisin;
    int lab[NbEvtsPossibles];
    int hmlab;
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
int NbArcs; /* le nombre de transitions */

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

/*---------------------------------------------*/
/*                                             */
/*Gestion de la liste des successeurs d'un etat*/
/*avec ses probas de transition                */
/*Ajoute Retire InitListeSuccesseurs           */
/*                                             */
/*---------------------------------------------*/

short inferieurEtat(X, Y)
int *X, *Y;
{
	int i, j, k, l;
	i = Etat2Int(X)-1;
	j = Etat2Int(Y)-1;
	k = TableCorrespondance[i];
	l = TableCorrespondance[j];
	if  (k<l) {return(0);}
	return(1);
}

static short egalEtat(X, Y)
int *X, *Y;
{
	int i, j;
	i = Etat2Int(X);
	j = Etat2Int(Y);
	if (i == j) {return(0);}
	return(1) ;
}

// Version qst
item  *InsereEtat(debut,evnb,G)
item *debut;
int evnb;
Etat   G;
{
	item *a_inserer;
	int i;
	
	if (debut == NULL) {
		Nouveau(&debut); 
        debut->lab[0] = evnb;
		debut->hmlab = 1;
 		debut->next = NULL;
		i = Etat2Int(G) -1;
		debut->numero=TableCorrespondance[i];
		succ++;
		for (i=0; i<NEt; i++) {debut->voisin[i] = G[i];} 
		return debut;
	}
	if (egalEtat(debut->voisin,G)==0) {
        debut->lab[debut->hmlab] = evnb;
		debut->hmlab=1+debut->hmlab;
		return debut;
	}
	if (inferieurEtat(debut->voisin, G)==0) {
		debut->next = InsereEtat(debut->next,evnb,G);
		return debut;
	}
	
	Nouveau(&a_inserer);
	for (i=0; i<NEt; i++) {a_inserer->voisin[i]=G[i];}
	a_inserer->next = debut;
    a_inserer->hmlab = 1;
    a_inserer->lab[0] = evnb;
	i = Etat2Int(G)-1;
	a_inserer->numero=TableCorrespondance[i];
	succ++;
	return a_inserer;
}

/*gestion de la liste des successeurs du sommet courant */
/*elle est effectivement implementee sous forme de liste triee*/

void Ajoute(evnb, F)
int evnb;
Etat F;
{
	PointeVisite = InsereEtat(PointeVisite,evnb,F);
}

void Retire(It)
item *It;
{
	/*On rend le premier element de la liste*/
	item *q;
	
	if (PointeVisite == NULL)
    {
		fprintf(stderr,"PointeVisite = NULL\n");
		exit(7);
    }
	q = PointeVisite;
	PointeVisite = PointeVisite->next;
	*It = *q;
	Depose(q);
}


int NbVoisins()
{
	return succ;
}


void InitListeSuccesseurs()
{
	/*initialisation des variables liees a la liste des succeseurs du sommet courant*/
	succ = 0;   /*nombre de successeurs distincts du sommet courant*/
	PointeVisite = NULL;
}


/* Inclusion des fonctions specifiques a un modele */

#include "funLabel.c"

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
	printf("usage : GenerAutomata -f filename \n");
	printf("to create filename.tgf \n");
	printf("to store the graph of the automata in tgf format. \n");
	printf("The file must not exist before. \n");
	exit(1);
}


int main(argc,argv)
int argc;
char *argv[];
{
	Etat F, R;
	item It, It2;
	int j, k, bcli;
	short deja;
	FILE *pf1, *pf2;
	char s1[20];
	char s2[20];
    char c; 
	
	if (argc<2) usage();
	--argc;
	++argv;
	if (**argv != '-') usage();
	switch (*++*argv) {
		case 'f' :  /* on recupere le nom de fichier */
		{++argv;
			strcpy(s1,*argv);
			/* on ajoute .tgf */
			strcpy(s2,s1);
			strcat(s1,".tgf");
			strcat(s2,".tmp");
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
			pf1=fopen(s1,"w");
			pf2=fopen(s2,"w"); 
			break;
		}
		default	 : usage();
	}
	
	InitPoubelle();
	InitEtendue();
	InitParticuliere();
    InitLabels();
	CreerMark();
	InitMark();
	NbArcs = 0;
	NbSommets = 0;
	EtatInitial(It.voisin);
	It.next = NULL;
	It.last = NULL;
	It.numero = NumeroDeVisite(It.voisin);
	j = Etat2Int(It.voisin)-1;
	Mark[j]=2;
	MiseEnFile(It);
	
	do {
		SortieDeFile(&It);
		InitListeSuccesseurs();
		
		/*on cherche tous les successeurs distincts de It.voisin en generant 
		 tous les evenements et en les appliquant a cet etat. 
		 L'etat resultat est F. On ajoute F dans la liste des 
		 successeurs de l'etat en cours*/
		
#if (Polynom==0) 
		/* One step transition  P */
		for (j = 1; j <= NbEvtsPossibles; j++) {
#if (DEBUG==1)			
			printf("j=%d\n",j);
			PrintEtat(It.voisin);
#endif
			Equation(It.voisin, j, F, R);
#if (DEBUG==1)
			PrintEtat(F);
#endif
			bcli=NumeroDeVisite(F); 
            Ajoute(j, F);
#if (DEBUG==1)
			printf("added in the transition list \n");
#endif
		}
#endif 
		
		
#if (Polynom>0) 
        printf("Constant Polynom must be equal to ZERO \n");
        exit(1);
#endif
		
        
        /*ecriture des etats et de leur etiquette-codage sur les composantes*/
        
        fprintf(pf1,"%10d  (", It.numero);
        for (j = 0; j < NEt-1; j++)
        {	fprintf(pf1,"%d,", It.voisin[j]);}
        fprintf(pf1,"%d)\n", It.voisin[NEt-1]);
 
#if (DEBUG==1)
        printf("added in the node list in the tgf file \n");
#endif

        /*ecriture des transitions sur un fichier temp */
        
        bcli = NbVoisins();
        for (j = 1; j <= bcli; j++) {
            Retire(&It2);
            /*On examine la liste des successeurs pour poursuivre les visites */
            deja=DejaVisite(It2);
            if (deja<2)  MiseEnFile(It2);
            /* ecriture de la transition		 */
            fprintf(pf2, "%10d  %10d (", It.numero, It2.numero);
            /* ecriture des labels de la transition */
            for (k = 0; k < It2.hmlab-1; k++) {fprintf(pf2, "%c,", Label[It2.lab[k]-1]);}
            fprintf(pf2,"%c)\n", Label[It2.lab[It2.hmlab-1]-1]);
#if (DEBUG==1)
            printf("added in the transition list in the tmp file \n");
#endif

        }    
    } 
    while (!FileVide());
    
    fprintf(pf1,"# \n ");		    
    fprintf(pf2," \n ");		    
    fclose(pf2);
    
    printf("Concat of files \n");
    pf2=fopen(s2,"r"); 
    while(!feof(pf2))
        { 
            fread(&c,1,1,pf2);
            fwrite(&c,1,1,pf1);
        }
    fclose(pf2);
    fclose(pf1);
    unlink(s2);
	printf("Done GenerAutomata\n");
    exit(0);
 }

