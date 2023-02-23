/* 
Vincent algo: sparse row,  Input Rii 
needs .Rii matrix and .sz size file
Write the result as a Rii matrix
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Vincent.h"

struct item {
  double prob;
  int numero;
  struct item *next,*last;
};

typedef struct item item;

/*Variables qui doivent etre invisibles pour le pgm principal*/
/* NE PAS TOUCHER */

item *Poubelle;   /*pointeur vers le sommet de la pile poubelle*/
item *APrendre, *PourAjouter; /*pointeurs sommets en attente de visite*/
item *PointeVisiteP;   /*pointeur  vers le sommet de la structure P */
item *PointeVisiteQ;   /*pointeur  vers le sommet de la structure Q */
item *PointeFinQ;   /*pointeur  vers la fin de la structure Q */

/*---------------------------------------------*/
/*gestion usuelle d'une pile de vieux elements */
/*pour eviter d'employer la function de  */
/*liberation memoire   */
/*---------------------------------------------*/

FILE *pf1,*pf2;

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
/*Ajoute Retire InitListes                     */
/*                                             */
/*---------------------------------------------*/


item  *InsereEtat(debut,p,num)
item *debut;
double p;
int num;
{
item *a_inserer;

  if (debut == NULL) {
    Nouveau(&debut); 
    debut->prob = p;
    debut->next = NULL;
    debut->last = NULL;
    debut->numero=num;
    return debut;
  }
  if (debut->numero==num) {
    debut->prob += p;
    return debut;
  }
  if (debut->numero<num) {
    debut->next = InsereEtat(debut->next,p,num);
    return debut;
  }
  Nouveau(&a_inserer);
  a_inserer->prob = p;
  a_inserer->next = debut;
  a_inserer->numero=num;
  return a_inserer;
}

  /*gestion de la liste des successeurs du sommet courant */
  /*elle est effectivement implementee sous forme de liste triee*/
  /*le pointeur de tete de liste est PointeVisite P ou Q */

void AjouteP(p, num)
double p;
int num;
{
  PointeVisiteP = InsereEtat(PointeVisiteP,p,num);
}

void RetireP(It)
item *It;
{
  /*On rend le premier element de la liste*/
  item *q;

  if (PointeVisiteP == NULL)
    {
      fprintf(stderr,"PointeVisite List P = NULL\n");
      exit(7);
    }
  q = PointeVisiteP;
  PointeVisiteP = PointeVisiteP->next;
  *It = *q;
  Depose(q);
}


void InitLists()
{
  PointeVisiteP = NULL;
  PointeVisiteQ = NULL;
  InsereEtat(PointeVisiteQ,1.0,0);
}


void InsereListP(jx,S1)
int jx;
double S1;
{
  PointeVisiteP = InsereEtat(PointeVisiteP,S1,jx);
}

/* Fusion des listes  */

struct item *MergeListPListQ(struct item *headP, struct item *headQ)
{
struct item *head, *p;

  if(headP == NULL) 
  return headQ;

 if(headQ == NULL) 
  return headP;

 if(headP->numero < headQ->numero) {
  head = headP;
  head->next = MergeListPListQ(headP->next, headQ);
 } else if(headP->numero > headQ->numero) {
  head = headQ;
  headQ->next = MergeListPListQ(headP, headQ->next);
 }
 /* deux elements avec le meme indice provenant de P et de Q */
 /* On prend le max des deux */
 else {
  head = headQ;
  if (headQ->prob<headP->prob) {headQ->prob=headP->prob;}
  p=headP->next;
  Depose(headP);
  headQ->next = MergeListPListQ(p, headQ->next);
 }
 return head;
}
 

void PrintRListQ(pointe)
item *pointe;
{ 
if (pointe->last == NULL) {
     if (pointe->prob>epsNull) 
	 {printf(" %lf %d ", pointe->prob,pointe->numero);}
     }
else  {PrintRListQ(pointe->last);
      if (pointe->prob>epsNull) 
	 {printf(" %lf %d ", pointe->prob,pointe->numero);}
    }
printf("\n");
}


struct item *DoublePointe(struct item *headQ)
{
struct item *head, *p;
headQ->last=NULL;
head=headQ;
p=head->next;
while (p!=NULL) {
    p->last=head;
    head=p;
    p=p->next;
 }
 return head;
}

int MonotoneListQ()
{ 
item *q,*p,*oq;
int s;

s=1;
p=PointeFinQ;
q=p->last;
while (q!=NULL) {
/* deux elements non monotones (la somme des proba est plus grande */
/* pour l'indice le plus grand). On rend la ligne monotone */
/* le cas egalite correspond a une liste trop rgande que l'on peut simplifier */
    if (p->prob >= q->prob) {
        p->last=q->last;
	oq=q;
	q=q->last;
	if (q!=NULL) {q->next=p;}
	Depose(oq);
    }
/* on continue si il n'y a pas de probleme */ 
    else {
	if ((q->prob - p->prob) > epsNull) {s++;}
	p=q;
	q=q->last;    
    } 
}
PointeVisiteQ=p;
return s;
}
 
void PrintListQ(pointe)
item *pointe;
{ 
if (pointe->next == NULL) {
     if (pointe->prob>epsNull) 
	 {printf(" %lf %d ", pointe->prob,pointe->numero);}
     }
else  {PrintListQ(pointe->next);
      if (pointe->prob>epsNull) 
	 {printf(" %lf %d ", pointe->prob,pointe->numero);}
    }
printf("\n");
}


void  usage()
{
	printf("usage : RowVincent -f filename \n");
	printf("filename.Rii and filename.sz must exist before. And the suffix must be Rxx \n");
	exit(1);
}

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory for the probability vectors \n");
	exit(1);
}


void VisitPrintListQ(pointe)
item *pointe;
{ 
    if (pointe->next == NULL) {
        if (pointe->prob>epsNull) 
        {fprintf(pf2, " %lf %d ", pointe->prob,pointe->numero);}
    }
    else  {     
        if ((pointe->prob-pointe->next->prob) > epsNull) 
        {fprintf(pf2, " %lf %d ", pointe->prob-pointe->next->prob,pointe->numero);}
        VisitPrintListQ(pointe->next); 
    }
}


int main(argc, argv)
int argc;
char *argv[];
{
int NSommets, NArcs, NEt;
int i, j, c, jx, degre, siz, ns;
double S1, x;
  char s0[20];
  char s1[20];
  char s2[20];
  char s3[20];
  char s4[20];
  

/*
On recupere le nom du modele et on verifie que les fichiers existent 
*/

	if (argc!=3) usage();
	--argc;
	++argv;
	if (**argv != '-') usage();
	switch (*++*argv) {
		case 'f' :  /* on recupere le nom de fichier */
		{++argv;
		strcpy(s0,*argv);
		strcpy(s1,".Rii");
		/* on ajoute .sz, et le suffixe */
		strcpy(s2,s0);
		strcat(s2,".sz");
		strcpy(s4,s0);
		strcat(s4,".V.U.Rii");
		strcpy(s3,s0);
		strcat(s3,".V.U.sz");
		/* on verifie si il existe un fichier .sz*/
		if ((pf1=fopen(s2,"r"))==NULL)
			{
			usage();
			}
		/* on verifie que le fichier model.Rii existe */
		strcat(s0,s1);
		if ((pf1=fopen(s0,"r"))==NULL)
			{
			usage();
			}
		break;
		}
		default	 : usage();
	}

/*
On recupere les tailles dans le fichier filename.sz
*/
  pf1=fopen(s2,"r");
  fscanf(pf1,"%d\n", &NArcs);
  fscanf(pf1,"%d\n", &NSommets);
  fscanf(pf1,"%d\n", &NEt); 
  fclose(pf1);	
  printf("%d\n", NArcs);
  printf("%d\n", NSommets);
  printf("%d\n", NEt);

/* 
On cree les objets ou on exit(1) en cas de pb
*/
  InitPoubelle();
  InitLists();
/*
Lecture de la matrice dans le fichier filename.Rii
*/
  pf1=fopen(s0,"r");
  pf2=fopen(s4,"w");
//  printf("Done Init\n");
ns=0;

  for (i = 0; i < NSommets; i++) {
  /* 
  on commence par le numero et le degre
  */
    fscanf(pf1,"%d", &c);
    fscanf(pf1,"%d", &degre);
    S1=1.0L;
    for (j = 0; j < degre; j++) {
      fscanf(pf1,"%lf", &x);
      fscanf(pf1,"%d", &jx);
// printf("Lect %d %d %d %Lf \n", c,degre, jx, x);  
      InsereListP(jx,S1);
      S1-=x; 
      /* P[c,jx]=x */
    }
    getc(pf1);
 
//      printf("Done Lecture \n");
//      PrintListQ(PointeVisiteP);
      

 /* listeP contient les sommes des proba de i a NSommets pour les */
 /* indices i ou cette somme change */
 /* listeP est simplement chaine */
 /* listeQ est doublement chaine */
 
 /* on fusionne les listes P et Q par ordre croissant des indices */
 
 PointeVisiteQ=MergeListPListQ(PointeVisiteP,PointeVisiteQ); 
 
//  printf("Done Merge\n");
//  PrintListQ(PointeVisiteQ);

 /* on retablit le pointage double qui a ete detruit par le merge */
 
 PointeFinQ=DoublePointe(PointeVisiteQ);
 
//  printf("Done  DoublePointe\n");
  
/*  PrintRListQ(PointeFinQ); */
  
  /* on rend la liste monotone et on enleve les fausses transitions */
  /* on cherche aussi le degre. Attention on enleve les elements trop petits */
  /* au moment de l'ecriture mais ils restent dans la liste */

 siz=MonotoneListQ();

 
// printf("Done Monotone \n");
// PrintListQ(PointeVisiteQ);

 /* on commence par imprimer le numero et le degre */ 
 fprintf(pf2,"%d %d ", c, siz);
 
 /* on imprime la liste */ 

 VisitPrintListQ(PointeVisiteQ);
 fprintf(pf2,"\n");
 PointeVisiteP = NULL;
 ns+=siz;
// printf("Done Print \n");
 }
  fclose(pf1);
  fclose(pf2);  
  pf1=fopen(s3,"w");
  fprintf(pf1,"%d\n", ns);
  fprintf(pf1,"%d\n", NSommets);
  fprintf(pf1,"%d\n", NEt);
  fclose(pf1);
  printf("Done Vincent Upper Bound\n");
  exit(0);
}

