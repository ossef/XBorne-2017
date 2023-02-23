#include "A4CListe.h"
#define EPSILON 0.0000000001 // precision de comparaison des nombres flottants
#define KMAX 1500  // une borneMax fixé par le programmeur



Liste liste_vide(){return NULL;}

int longueur(Liste l){
    int taille =0;
    Liste p=l;
    while(p){
             taille++;
             p=p->suivant;
            }
            return taille ;
                   }
                   
Liste inserer(Liste l,int i, Voisin vo, Proba pr){
      
      if(i<0||i>longueur(l))
      {
                            printf("erreur");
                            exit(-1);
      }
      Liste pc=(Liste)malloc(sizeof(Cellule));
      pc->proba=pr;
      pc->voisin=vo;
      pc->suivant=NULL;
      if(i==0){
               pc->suivant=l;
               l=pc;
               }
      else{
           int j;Liste p=l;
           for(j=0;j<i-1;j++)
           p=p->suivant;
           pc->suivant=p->suivant;
           p->suivant=pc;
           }
           return l;
      }                   
            
Place acces(Liste l,int k){
      
      int i;
      Place p;
      if(k<0 || k>=longueur(l)){
                            printf("erreur acces ! ");
                            exit(-1);
                              }   
      if(k==0)
      return l;
      else{
           p=l;
           for(i=0;i<k;i++)
           p=p->suivant;
           return p;
          }
                          }
Place RechercheV(Liste l,Voisin v){
      
      Place p;
      p=l;
      int b=0;
      while(p)
      {
      if(p->voisin == v)
      {
      b=1;
      break;
      }
      p = p->suivant;  
      }
      return b==0 ? NULL:p ;
                                  }
                          
Place succ(Liste l, Place p){
      
      if (p->suivant==NULL){
                            
                            printf("erreur");
                            exit(-1);
                            }
      return p->suivant;
                           }
                           

                                
Voisin contenuV(Liste l,Place p){
        
        if(longueur(l)==0){
                           
                            printf("erreur");
                            exit(-1);
                             
                           }
         if(p == NULL)
                           return 0;
         else
                           return p->voisin;
        
        }       
        
Proba contenuP(Liste l,Place p){
        
        if(longueur(l)==0){
                           
                            printf("erreur liste vide");
                            exit(-1);
                             
                           }
        if(p == NULL)
                           return 0;
        else
                           return p->proba;
        
        }                              
                           
Voisin kemeV( Liste l,int k){                                                                                                                       

      if(k<0 || k>=longueur(l)){
                            printf("erreur");
                            exit(-1);
                              } 
      return contenuV(l,acces(l,k));
                                }    

Proba kemeP( Liste l,int k){                                                                                                                       

      if(k<0 || k>=longueur(l)){
                            printf("erreur");
                            exit(-1);
                              } 
      return contenuP(l,acces(l,k));
                                }    
                                
Liste supprimer(Liste l,int i){
    
    Liste p;
      
      if(i<0||i>longueur(l))
      {
                            printf("eereur");
                            exit(-1);
      }
         
     if(i==0){
               p=l;
               l=l->suivant;
             }              
     else{
          Place q;
          q=acces(l,i-1);
          p=succ(l,q);
          q->suivant=p->suivant;
          
          }
          free(p);
          return l;
                                }      
                                

                                   
Liste inverser_liste(Liste l){  
          
          Place c,p,s;
          p=NULL;
          c=l;
          
          while(c->suivant){
                          s=c->suivant;
                          c->suivant=p;
                          p=c;
                          c=s;                   
                          }
                          c->suivant=p;
                          l=c;
                          return l; 
                             }  
                             
                                                             
void   afficher(Liste l)
{      
   int i;
  
   for (i=0;i<longueur(l);i++)
   {
       if(i==longueur(l)-1)
       printf("proba(%d)= %f  Fin \n",contenuV(l,acces(l,i)),contenuP(l,acces(l,i)));/*,contenuP(l,acces(l,i)));*/
       else
       printf("\nproba(%d)= %f \n",contenuV(l,acces(l,i)),contenuP(l,acces(l,i)));/*,contenuP(l,acces(l,i)));*/
       
   }
  
}

double Max(double a ,double b)
{
	return a>b ? a:b ; 
}    

Liste fusion(Liste l1,Liste l2)
{

if( l1 == NULL )
return l2;

if ( l2 == NULL )
return l1;

Liste lf;
Place p1,p2;
int c = 0;
p1 = l1;
p2 = l2;
lf = liste_vide();

while ( p1 && p2 )
{    
 
if(p1->voisin < p2->voisin)
{
lf = inserer(lf,c++,p1->voisin,p1->proba);
p1 = p1 -> suivant;            
}

else if(p1->voisin > p2->voisin )
{
lf = inserer(lf,c++,p2->voisin,0.0);
p2 = p2 -> suivant;        
}

else if( p1->voisin == p2->voisin )// cas d'egalité
{
lf = inserer(lf,c++,p1->voisin,p1->proba);
p1 = p1 -> suivant; 
p2 = p2 -> suivant;             
}
     
} // fin while

while(p2)
{
lf = inserer(lf,c++,p2->voisin,0.0);   
p2 = p2 -> suivant;
}        
 
while(p1)
{
lf = inserer(lf,c++,p1->voisin,p1->proba);  
p1 = p1-> suivant;
}                     

return lf;
     
} 



Liste MaxSt(Liste l1,Liste l2,int N,int t)
{
 Liste l = liste_vide();
 double s1,s2,s3,s4,s5,s6;
 s6=0;
 int c,lg,count;
 c=0;
 
 s4=0;

  
 l1 = fusion(l1,l2);
 l2 = fusion(l2,l1);

 c=0;

 
 lg = longueur(l1)-1;
 count = lg;
 
 l1 = inverser_liste(l1);
 l2 = inverser_liste(l2);

while( l1 )
{

if(count == lg)
{     
    s1 = l1 -> proba;

    s2 = l2 -> proba;
   
    s5 = Max(s1,s2);
    
    if( fabs(s5)<EPSILON )
    s5 = 0; 
      
         if( s5 != 0) //  ( s5 !=0 )
         {
             l=inserer(l,c,l1->voisin,s5); 
             s4 = s5;
             c++;
         }   
}
else
 {

   
    s1 = s1 + l1->proba;
    s2 = s2 + l2->proba;

    s3 = Max(s1,s2); 
   
    s5 = s3-s4;
   
    
    
    if( fabs(s5) < EPSILON)
    s5 = 0;
  
         if( s5 > EPSILON) //  ( s5 !=0 )
         {
             l=inserer(l,c,l1->voisin,s5); 
             s4 += s5;
             c++;
         }
        
     
   }// fin else
   count--; 
   l1 = l1 -> suivant;
   l2 = l2 -> suivant;
 }   //fin while
 
 return inverser_liste(l); 
}//function 



void afficherV(double*V,int n)
{
int i;
printf("\n affichage : [");
for(i=0;i<n;i++)
  {
   printf("%lf ,",V[i]);
  }
printf("]\n");
}

void  ProblemeMemoire()
{
	printf("Sorry, not enougth memory , \n");
	printf("try to change the size of vectors \n");
	exit(1);
}

double **alloc_mat(int n, double **mat)
{
if(n<0)
       {
       printf("taille non valide");
       exit(-1); 
       }
 int i;
 mat = (double **)malloc(n*sizeof(double));
 
 if(mat == NULL)
 {
  ProblemeMemoire()   ; 
 }
 for( i=0 ; i<n ; i++) 
 {
   mat[i] = (double*)malloc(n*sizeof(double));
   if(mat[i] == NULL)
   ProblemeMemoire();
 }
 //printf("OK! alloc \n");
 return mat;
}

void afficherMatrice(double **mat,int n) 
{ 
    int i,j;
    printf("\n Matrice : \n"); 
    for (i=0;i<n;i++) 
    { 
        for (j=0;j<n;j++) 
        { 
            printf("%f  ",mat[i][j]); 
        } 
        printf("\n" ); 
    } 
} 

double *Prod_Vect_Mat(double *vect,double **mat,int n)
{
 int i, j,c=0;
 double sum;
 double * res;
 res = malloc(n*sizeof(double));
 for (i = 0 ; i < n ; i++) 
 {
  sum = 0;
  
  for (j = 0 ; j < n ; j++) 
  {
    sum += mat[j][i] * vect[j];
  }   
  res[c++] = sum;
 }
return res;       
}

double *Sous_Vect(double *vect1,double *vect2,int n)
{
 int i;
 double * res;
 res = malloc(n*sizeof(double));
 for( i=0 ; i<n ; i++)
 {
    res[i]= vect1[i]-vect2[i]; 
    if( fabs(res[i]) < EPSILON )
    res[i] = 0;
 }
return res;       
}

double *TransformeLtoV(Liste l,int n,double *V)
{
       
int i;


  for(i = 0 ; i<n ; i++)
  {
  V[i] = 0;   
  }    
  
  for(i = 0 ; i<longueur(l) ; i++)
  {
   V[kemeV(l,i)] = kemeP(l,i); 
  }    
  return V;     
}

int RechercheIndice(int j,double *P,int taille)
{
int i;

for(i = 0 ; i<taille ; i++)  
{
if( fabs(P[i] - (double)j) < EPSILON )      
{
return 1;
}
}
return -1;   
   
} 

double MAXZ(double *s,double *w,double *z,double *P ,int taille)
{
int l,c,i;       
double max,so;
double S[taille];
c=0;

for( l=0 ; l<taille ; l++)
{
i = (int)P[l];

   if ( fabs(z[i]) < EPSILON )
   {
    S[c] = 0;
    c++;
    continue; 
   }
   else
   {
    so = (s[i] - w[i])/z[i] ;   
    if( so < EPSILON)
    {
     S[c] = 0;    
    } 
    else
    {
      S[c] = so;
    }
    c++;     
   }     
         
}      

max = S[0];
for( l=1 ; l<taille ; l++)
{
 if( S[l] > max )
 max = S[l];
}   
  
return max;             
}

double *Prod_VectKst1(double *v,int n,double *res)
{
int i;

res[0] = 1;
for(i=1 ; i<n ;i++)
{
res[i] = res[i-1] - v[i-1];     
}           
return res;              
}

double *Prod_VectKst2(double *v,int n)
{
int i;
double *res;
res = (double*)malloc(n*sizeof(double)) ;

res[0] = 0;
for(i=1 ; i<n ;i++)
{
res[i] = res[i-1] - v[i-1];     
}           
return res;              
}
     
void UpperBoundMatrix(char NameOutPut[80],int k)
{
char s1[80],s2[80],s3[80];
FILE *Size,*Rii,*Output1;
int n,n1,n2,m,m1,m2,i,j,c1,c2,t;
double p;

strcpy(s1, NameOutPut);
strcpy(s2, NameOutPut);
strcpy(s3, NameOutPut);


strcat(s1,".sz");
strcat(s2,".Rii");
strcat(s3,".LR");


Size=fopen(s1,"r");
if(Size == NULL)
{
 printf("erreur : file \"%s\" doesn't exist !",s1);
 exit(2);       
}
Rii=fopen(s2,"r");
if(Rii == NULL)
{
 printf("erreur : file \"%s\" doesn't exist !",s2);
 exit(2);       
}
Output1=fopen(s3,"w");


fscanf(Size,"%d %d %d",&m1,&n,&m2);


Liste l1,l2;
l1 = liste_vide();

m=0; //nbre elements non nul

//----------------- 1- v = P[1,*] -----------------------------------------------
//printf("\netape 1 \n");

double *v,*V;
V = (double*)malloc(n*sizeof(double));

fscanf(Rii,"%d %d ",&n1,&n2);
for(j=0 ; j<n2 ; j++)
        {
         fscanf(Rii,"%lf %d",&p,&n1);
         l1 = inserer(l1,j,n1,p);
        }
        
v = TransformeLtoV(l1,n,V);
/*printf("\n v = ");
afficherV(v,n);*/

// ---------------- 2- ri[1] = 0 et ri[N] = 1 --------------------------------------
//printf("etape 2 \n");

double **r;
r = (double **)malloc(k*sizeof(double));
if(r == NULL)
 {
  printf("Stockage R :\n");
  ProblemeMemoire()   ; 
 }
 for( i=0 ; i<k ; i++) 
 {
   r[i] = (double*)malloc(n*sizeof(double));
   if(r[i] == NULL)
   {
   printf("\n Stockage R :\n");
   ProblemeMemoire()   ; 
   }
   r[i][0] = 0;
   r[i][n-1] = 1;
 }


// -------------- 3- w = v x Kst -------------------------------------------------
//printf("etape 3 \n");

double *w,*res;
res = (double*)malloc(n*sizeof(double)) ;
w = Prod_VectKst1(v,n,res);
//afficherV(w,n);

// ---------- 4- Computing the maximum of the rows of P using Vincent's algorithm ----
//printf("etape 4 \n");

Liste lMax = l1;
for(i=1 ; i<n-1 ; i++)
{

fscanf(Rii,"%d %d ",&n1,&n2);

l2 = liste_vide();

        for(j=0 ; j<n2 ; j++)
        {
         fscanf(Rii,"%lf %d",&p,&n1);
         l2 = inserer(l2,j,n1,p);
        }
        //afficher(l1);
        
lMax = MaxSt(lMax,l2,n,i); 
}


// ----------------- 5- c = max - v ---------------------------------------------
//printf("etape 5 \n");
V = (double*)malloc(n*sizeof(double));
double *c;
c = Sous_Vect(TransformeLtoV(lMax,n,V),v,n);
/*printf("\n c = ");
afficherV(c,n);*/

// ---------------- 6- z = c x Kst -------------------------------------------------
//printf("etape 6 \n");

double *z;
z = Prod_VectKst2(c,n);
/*printf("\n z = ");
afficherV(z,n);*/
// --------------- 7- Finding a partition of C into k subsets ----------------------
//printf("etape 7 \n");

double **P;
P = (double **)malloc(k*sizeof(double));
if(P == NULL)
 {
  printf("Stockage P :\n");
  ProblemeMemoire()   ; 
 }
int taille[k];

 for( i=0 ; i<k ; i++) 
 {
   P[i] = (double*)malloc(((n/k)+1)*sizeof(double));
   if(P[i] == NULL)
    {
        printf("Stockage P :\n");
        ProblemeMemoire()   ; 
    }
 }

c1 = 0;

 for( i=0 ; i<n ; i++) 
 {
      c2 = 0;
      if( fabs(z[i]) < EPSILON ) // ~0
      {   
      if( fabs(c[i]) > EPSILON )
      P[c1][c2++] = i;
      
      j = i+1;
      while ( fabs(z[j]) > EPSILON )
      {
      if( fabs(c[j]) > EPSILON )
      P[c1][c2++] = j;
      j++;   
      }
      taille[c1] = c2;  
      c1++;  
      
      if(c1 == k)
      break;  
       
      }   
 }
 if(c1 >= KMAX )
 {
 printf("\n erreur : rang du decoupage trop grand ");
 exit(2);     
 }
 if( c1 != k )
 {
 printf("\n erreur : la valeur maximal de K pour le decoupage de C en subnet dans l'etape 7 de l'algorithme est : %d \n ==> Relancer l'execution avec une valeur de K maximal = %d",c1,c1);
 exit(2);   
 }
 
 
// ------------ 8- Defintion des vecteurs Ci ------------------------------------
//printf("etape 8 \n");

double **C;
C = (double **)malloc(k*sizeof(double));
if(C == NULL)
 {
  printf("Stockage C :\n");
  ProblemeMemoire()   ; 
 }

 for( i=0 ; i<k ; i++) 
 {
   C[i] = (double*)malloc(n*sizeof(double));
   if(C[i] == NULL)
   {
         printf("Stockage C :\n");
         ProblemeMemoire()   ; 
   }
 }
 
for( i=0 ; i<k ; i++) 
 {
    for( j=0 ; j<n ; j++) 
    {
     if(RechercheIndice(j,P[i],taille[i]) != -1)
     {
     C[i][j] = c[j];
     }
     else
     {
     C[i][j] = 0;
     }
    }   
 }
 
 
//----------- 9 - Defintion des vecteurs Ri ------------------------------------
//printf("etape 9 \n");

double *s;
double **h;

V = (double*)malloc(n*sizeof(double));
res = (double*)malloc(n*sizeof(double));
h = (double **)malloc(k*sizeof(double));

if(h == NULL)
 {
  printf("Stockage h :\n");
  ProblemeMemoire()   ; 
 }
for( i=0 ; i<k ; i++) 
 {
   h[i] = (double*)malloc(n*sizeof(double));
   if(h[i] == NULL)
   {
        printf("Stockage h :\n");
        ProblemeMemoire()   ; 
   }
 }
 
 
Rii=fopen(s2,"r");
fscanf(Rii,"%d %d ",&n1,&n2);

for(j=0 ; j<n2 ; j++)
        {
         fscanf(Rii,"%lf %d",&p,&n1);
        }
        
for(j=1 ; j<n-1 ; j++)
{
fscanf(Rii,"%d %d ",&n1,&n2);

l2 = liste_vide();

        for(t=0 ; t<n2 ; t++)
        {
         fscanf(Rii,"%lf %d",&p,&n1);
         l2 = inserer(l2,t,n1,p);
        }
       
        
s = Prod_VectKst1(TransformeLtoV(l2,n,V),n,res);
//afficherV(s,n);

for( i=0 ; i<k ; i++ )
{
h[i][j] = MAXZ(s,w,z,P[i],taille[i]);
r[i][j] = Max(h[i][j],r[i][j-1]);
}
}


// ---------- 10- generer la matrice dans un fichier ----------------------------
//printf("etape 10 \n");

fprintf(Output1,"%d %d \n",k,n);

for( i=0 ; i<n ; i++ )
{
fprintf(Output1,"%lf      ",v[i]);
}
fprintf(Output1,"\n");
for( i=0 ; i<k ; i++ )
{
     for( j=0 ; j<n ; j++ )
     {
     fprintf(Output1,"%lf      ",r[i][j]);
     }
     fprintf(Output1,"\n");
     for( j=0 ; j<n ; j++ )
     {
     fprintf(Output1,"%lf      ",C[i][j]);
     }
     fprintf(Output1,"\n");
}

printf(" Operation reussie ! \n voir la matrice Resultante sous forme Decomposee dans le fichier :  \"%s\"  \n",s3);

fclose(Output1);


}





     
      
      
      
                                                                                                             
