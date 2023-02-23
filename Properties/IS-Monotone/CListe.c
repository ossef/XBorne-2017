
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
      
      if(k<0){
                            printf("erreur acces ! ");
                            exit(-1);
                              }   
      if(k>=longueur(l))
      return NULL;
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
      p = l;
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
                           
                            printf("dans contenuP erreur liste vide");
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

      if(k<0){
                            printf("erreur");
                            exit(-1);
                              } 
      return contenuP(l,acces(l,k));
                                }    
                                
Liste supprimer(Liste l,int i){
      
      int j;
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
       printf("\nproba(%d)= %f  Fin \n",contenuV(l,acces(l,i)),contenuP(l,acces(l,i)));/*,contenuP(l,acces(l,i)));*/
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
 double max,s1,s2,s3,s4,s5,s6,tmp1,tmp2;
 s6=0;
 int i,j,c,lg,count;
 c=0;
 
 s4=0;

  
 l1 = fusion(l1,l2);
 l2 = fusion(l2,l1);

 c=0;
 //afficher(l1);
 
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
    
    if( fabs((float)s5 - (float)l1->proba) <EPSILON2 )
    {
    printf("Erreur1 :Ligne %d et voisin %d :s5 = %f et proba = %f et %e \nLa matrice est non monotone :( \n",t,l1 ->voisin,s5,l1->proba,s5 - l1->proba);
    exit(-1);  
    }
      
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
    
    if( fabs((float)s5 - (float)l1->proba) <EPSILON2 )
    {
    printf("Erreur2 :Ligne %d et voisin %d s5 = %f et proba = %f et %e \nLa matrice est non monotone :( \n",t,l1->voisin,s5,l1->proba,s5 - l1->proba);
    exit(-1);  
    }
  
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



void Monotonie(char NameOutPut[80])
{
char s1[80],s2[80];
FILE *Size,*Rii;
int n,n1,n2,m,m1,m2,i,j,s;
double p;

strcpy(s1, NameOutPut);
strcpy(s2, NameOutPut);


strcat(s1,".sz");
strcat(s2,".Rii");



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



fscanf(Size,"%d %d %d",&m1,&n,&m2);


Liste l1,l2;
l1 = liste_vide();
l2 = liste_vide();
m=0; 


//printf("ligne 0 :\n");
fscanf(Rii,"%d %d ",&n1,&n2);
//printf(" s = %d , degree = %d \n",n1,n2);


for(j=0 ; j<n2 ; j++)
        {
         fscanf(Rii,"%lf %d",&p,&n1);
         l2 = inserer(l2,j,n1,p);
         l1 = inserer(l1,j,n1,p);
        }
//printf("\n avant Maxst \n");
l1 = MaxSt(l1,l2,n,0);
//printf("\n apres Maxst \n");

     
for(i=1 ; i<n ; i++)
{
fscanf(Rii,"%d %d ",&n1,&n2);


l1 = liste_vide();

        for(j=0 ; j<n2 ; j++)
        {
         fscanf(Rii,"%lf %d",&p,&n1);
         l1 = inserer(l1,j,n1,p);
        }

     
l2 = MaxSt(l1,l2,n,i); 

}



printf("La matrice est monotone :) \n");

}





     
      
      
      
                                                                                                             
