/*
 generation des etats d'une matrice censoree a partir d'une equation d'evolution
 V1.1 en C, le 10/3/11
 generation des blocs NE et NW complets a partir d'etats obligatoires et en tenant compte 
 d'etat tabou et d'un numero maximal d'etat
 
 Hypotheses de fonctionnement : idem GenerMarkov
 
 - on genere un fichier .sz, .cd et .Rii pour les deux blocs.
 - le bloc NW est carre, le bloc NE est rectangulaire (voir le fihcier .sz)
 - le fichier .cd pour le bloc NW est comme pour une chaine classique
 - le fichier .cd pour le bloc NE contient les etats censores et les tabous indiques par un c ou un t
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* 
 inclusions des constantes, types et variables particulieres a chaque modele
 */

#include "const.h"
#include "var.h"

#define DEBUG 0

/*Variables donnant l'etendue de chaque variable isolee*/

static int  Min[NEt], Max[NEt];
static int  TabTabou[NTabou];
static int TabOblig[NOblig][NEt];

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

int NbArcs; /* le nombre de transitions */


/* Les deux structures  de grande taille dimensionnees par Maxsize */
int MaxSize;
short *Mark;
int   *TableCorrespondance;
int NombreDeMark, NombreCensor, NombreTabou;

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
/*Gestion de la liste des successeurs d'un etat*/
/*avec ses probas de transition                */
/*Ajoute Retire InitListeSuccesseurs           */
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

item  *InsereEtat(debut,p,G)
item *debut;
double p;
Etat   G;
{
	item *a_inserer;
	int i;
	
	if (debut == NULL) {
		Nouveau(&debut); 
		debut->prob = p;
		debut->next = NULL;
		i = Etat2Int(G) -1;
		debut->numero=TableCorrespondance[i];
		succ++;
		for (i=0; i<NEt; i++) {debut->voisin[i] = G[i];} 
		return debut;
	}
	if (egalEtat(debut->voisin,G)==0) {
		debut->prob += p;
		return debut;
	}
	if (inferieurEtat(debut->voisin, G)==0) {
		debut->next = InsereEtat(debut->next,p,G);
		return debut;
	}
	
	Nouveau(&a_inserer);
	for (i=0; i<NEt; i++) {a_inserer->voisin[i]=G[i];}
	a_inserer->prob = p;
	a_inserer->next = debut;
	i = Etat2Int(G)-1;
	a_inserer->numero=TableCorrespondance[i];
	succ++;
	return a_inserer;
}

/*gestion de la liste des successeurs du sommet courant */
/*elle est effectivement implementee sous forme de liste triee*/
/*le pointeur de tete de liste est Pointevoisin */

void Ajoute(p, F)
double p;
Etat F;
{
	PointeVisite = InsereEtat(PointeVisite,p,F);
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

#include "fun2Blocks.c"

/*---------------------------------------------*/
/*Gestion des sommets deja visites et a visiter*/
/*  MiseEnFile  FileVide SortieDeFile          */
/* NumeroDeVisite, CreerMark, InitMark         */
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
	if (!(TableCorrespondance=(int  *)calloc(MaxSize,sizeof(int)))) ProblemeMemoire();
}

void InitMark()
{
	NombreDeMark = -1;
	NombreCensor = -1;
	NombreTabou = -1 ; 
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
	int i,j,k;
	short ta; 
	
	i = Etat2Int(E)-1;
	if (Mark[i]>0) /* sommet deja rencontre, il est numerote */ 
    {
		j = TableCorrespondance[i];
    }
	else { /* sommet nouveau, est-il declare tabou ou genere en tant que censore ou tabou ? */ 
		Mark[i]=1; 
		ta=0;
		if (NTabou>0) {
			for (j=0;j<NTabou;j++) if (i==TabTabou[j]) {ta=1; k=j;}
		}
		if (ta==1) { /* sommet declare tabou */ 
			TableCorrespondance[i] = MaximalEtat+k;
			j = TableCorrespondance[i];
		printf("t %d %d \n",i,j); 
		}
		else { 		
			NombreDeMark++;
			if (NombreDeMark>=MaximalEtat) {/* sommet genere tabou */
				NombreTabou++;
				TableCorrespondance[i] = NombreTabou+MaximalEtat+NTabou;
				j = TableCorrespondance[i];	
				printf("n %d %d %d %d \n", NombreTabou, NombreDeMark,i,j);
			}
			else {/* sommet censore */
				NombreCensor++;
				TableCorrespondance[i] = NombreCensor;
				j = TableCorrespondance[i];
				printf("c %d %d %d %d\n", NombreCensor, NombreDeMark,i,j);
			}
		}
	}
	NbSommets = NombreCensor + NombreTabou + NTabou;
	return(j);
}

short Tabou(It)
item It;
{
	int i,j;
	short ta;
	ta = 0;
	i = Etat2Int(It.voisin)-1;
	if (NTabou>0) {
		for (j=0;j<NTabou;j++) if (i==TabTabou[j]) {ta=1;}
	}
	if (It.numero>MaximalEtat) {ta=1;}
	return(ta);
}

void EtatsInitiaux()
{int i,j;
	item Ite;
	/* range les etats obligatoires dans la file */
	for (i=0; i < NOblig; i++) {
		for (j = 0; j < NEt; j++) {
			Ite.voisin[j]=TabOblig[i][j];
		}
		Ite.next = NULL;
		Ite.last = NULL;
		Ite.prob = 0.0;
		Ite.numero = NumeroDeVisite(Ite.voisin);
		j = Etat2Int(Ite.voisin)-1;
		Mark[j]=2;
		MiseEnFile(Ite);
	}
}


/* Programme principal et interface exterieure */

void  usage()
{
	printf("usage : GenerCensor2Blocks -f modelname \n");
	printf("to create modelname.NW.Rii, modelname.NW.cd, modelname.NW.sz, modelname.NE.Rii, modelname.NE.cd and modelname.NE.sz\n");
	printf("to store the states and the matries of your model \n");
	printf("The files must not exist before. \n");
	exit(1);
}

int main(argc,argv)
int argc;
char *argv[];
{
	Etat F, R;
	item It, It2;
	int j, bcli,dt,dc,k;
	int nac, nsc, nat, nst;
	double SommeDeProba;
	short deja, ta;
	double pro;
	FILE *pf1, *pf2, *pf3, *pf4;
 	char s2[20], s3[20],  s1[20], s4[20],  s5[20], s6[20]; 
	char st1[50], sc1[50];
	char st[1000], sc[1000];
	
	if (argc<2) usage();
	--argc;
	++argv;
	if (**argv != '-') usage();
	switch (*++*argv) {
		case 'f' :  /* on recupere le nom de fichier */
		{++argv;
			strcpy(s1,*argv);
			/* on ajoute .sz, .cd ou .Rii */
			strcpy(s2,s1);
			strcpy(s3,s1);
			strcpy(s4,s1);
			strcpy(s5,s1);
			strcpy(s6,s1);
			strcat(s1,".NW.cd");
			strcat(s2,".NW.Rii");
			strcat(s3,".NW.sz");
			strcat(s4,".NE.cd");
			strcat(s5,".NE.Rii");
			strcat(s6,".NE.sz");
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
			if ((pf1=fopen(s4,"r"))!=NULL)
			{
				fclose(pf1);
				usage();
			}
			if ((pf1=fopen(s5,"r"))!=NULL)
			{
				fclose(pf1);
				usage();
			}
			if ((pf1=fopen(s6,"r"))!=NULL)
			{
				fclose(pf1);
				usage();
			}
			pf1=fopen(s1,"w");
			pf2=fopen(s2,"w"); 
			pf3=fopen(s4,"w");
			pf4=fopen(s5,"w"); 
			break;
		}
		default	 : usage();
	}
	
	InitPoubelle();
	InitEtendue();
	InitParticuliere();
	CreerMark();
	InitMark();
	InitTabou();
	InitOblig();
	nac = 0;
	nat = 0;
	nsc=NOblig;
	nst = 0;
	EtatsInitiaux();	
	
	do {
		SortieDeFile(&It);
		InitListeSuccesseurs();
		
		/*on cherche tous les successeurs distincts de It.voisin en generant 
		 tous les evenements et en les appliquant a cet etat. 
		 L'etat resultat est F. On ajoute F dans la liste des 
		 successeurs de l'etat en cours*/
		
		/* One step transition  P */
		SommeDeProba = 0.0;
		for (j = 1; j <= NbEvtsPossibles; j++) {
			Equation(It.voisin, j, F, R);
			pro = Probabilite(j, It.voisin);
			SommeDeProba+=pro;
#if (DEBUG==1)
			printf("Event %d\n",j);
			PrintEtat(It.voisin);
			PrintEtat(F);
			printf("Proba : %lf\n", pro);
#endif 
			if (pro>Epsilon2) {bcli=NumeroDeVisite(F); Ajoute(pro, F);}
		}
		
		/*ecriture du codage des etats censores sur les composantes*/
		
		fprintf(pf1,"%10d", It.numero);
		for (j = 0; j < NEt; j++)
			fprintf(pf1,"%9d", It.voisin[j]);
		fprintf(pf1,"\n");
		fprintf(pf3,"%10d", It.numero);
		for (j = 0; j < NEt; j++)
			fprintf(pf3,"%9d", It.voisin[j]);
		fprintf(pf3," c \n");
		
		/* ecriture du numero de sommet */
		
		fprintf(pf2,"%12d", It.numero);
		fprintf(pf4,"%12d", It.numero);
		
		/*tri des successeurs du sommet pour savoir si ils sont visibles
		 un sommet tabou n'est pas mis dans la liste */
		bcli = NbVoisins();	  
		SommeDeProba = 0.0;
		dc = 0;
		dt = 0;
		strcpy(st,"");
		strcpy(sc,"");
		for (j = 1; j <= bcli; j++) {
			Retire(&It2);
			SommeDeProba += It2.prob;
			/*On examine la liste des successeurs pour poursuivre les visites */
			deja=DejaVisite(It2);
			ta = Tabou(It2);
			
			/* etat tabou */
			if (ta==1) {
				/* si il est nouveau on le compte et on ecrit son codage */
				if (deja<2) { nst++; 
					/* ecriture du numero de sommet dans le fichier cd */
					fprintf(pf3,"%10d", It2.numero);
					for (k = 0; k < NEt; k++)
					{fprintf(pf3,"%9d", It2.voisin[k]);}
					fprintf(pf3," t  \n");
				}
				/* dans tous les cas, ecriture des successeurs dans une chaine, tant qu'on a pas le degre */
				sprintf(st1,"% .15E %10d", It2.prob, It2.numero);
				strcat(st,st1);
				dt++;
				nat++;
			} 
			/* le successeur est un etat censore */
			else {
				/* si il est nouveau, on le met en file */
				if (deja<2)  {nsc++; MiseEnFile(It2);}
				/* 
				 pour chaque successeur, on ecrit la probabilite de transition 
				 puis le numero de l'etat
				 ecriture des successeurs dans une chaine, tant qu'on a pas le degre */
				sprintf(sc1,"% .15E%10d", It2.prob, It2.numero);
				strcat(sc,sc1);
				nac++;
				dc++;
			}
		}
		fprintf(pf2,"%12d", dc);
		fprintf(pf2,"%s \n", sc);
		
		fprintf(pf4,"%12d", dt);
		fprintf(pf4,"%s \n", st);
		if (fabs(SommeDeProba - 1.0) > Epsilon1)
		{
			printf("attention la somme vaut : % .10E\n", SommeDeProba);
		}
		
	} while (!FileVide());
  	
	fclose(pf2);
	fclose(pf1);
	fclose(pf4);
	fclose(pf3);
    
	pf1=fopen(s3,"w"); 
	fprintf(pf1,"%d \n %d \n %d \n %d 0 0 \n", nac,nsc,NEt,nsc);
	fclose(pf1);

    pf2=fopen(s6,"w");
    fprintf(pf2,"%d \n %d \n %d \n %d 0 %d \n", nat, nsc, NEt, nst, nsc);
	fclose(pf2);
	exit(0);
}

