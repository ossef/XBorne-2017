#include "A2vect.h" // inclure le fichier vect.h dans celuici .c
#define EPSILON 0.0000000001  // pour comparer les flottants

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
   {
   printf("\n %d \n",i);
   ProblemeMemoire();
   }
 }
 //printf("OK! alloc \n");
 return mat;
}

vecteur Alloc(int t)
{
vecteur v;
if(t<0)
       {
       printf("taille non valide");
       exit(-1); 
       }
else
    {
    v.taille=t;
    v.relements=(double*)malloc(sizeof(double)*t);
    v.celements=(double*)malloc(sizeof(double)*t);
    return v;
    }

}

int taille(vecteur v)
{
    return(v.taille);
}


double **Prod_VectT_Vect(double *vect1,double *vect2,int n)
{
 int i,j;
 double **mat = NULL;
 mat = alloc_mat(n,mat);
 
 for( i = 0 ; i < n ; i++)
 {  
      for( j = 0 ; j < n ; j++)
      {
           mat[i][j] = vect1[i]*vect2[j];
      }
 }
return mat;
}

double Prod_Vect_VectT(double *vect1,double *vect2,int n)
{
 int i;
 double s = 0;
 for( i = 0 ; i < n ; i++)
 {  
 s+=vect1[i]*vect2[i];
 }
 return s;    
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

double *Add_Vect(double *vect1,double *vect2,int n)
{
 int i;
 for( i=0 ; i<n ; i++)
 {
    vect1[i]= vect1[i]+vect2[i]; 
 }
return vect1;       
}

double *Prod(double *vect ,double val ,int n)
{
 int i;
 double * res;
 res = malloc(n*sizeof(double));
 for( i=0 ; i<n ; i++)
 {
    res[i]= vect[i]*val; 
 }
return res;  
}

double **Add_Mat(double **mat1,double **mat2,int n)
{
int i,j;  

for (i = 0 ; i < n ; i++) 
 {
  for (j = 0 ; j < n ; j++) 
  {
    mat2[i][j]= mat1[i][j] + mat2[i][j];
  }   
 }
 return mat2;     
}

void afficher(double*V,int n)
{
int i;
printf("\n affichage : [");
for(i=0;i<n;i++)
  {
   printf("%lf ,",V[i]);
  }
printf("]\n");
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

void afficherMatriceInv(double **mat,int n) 
{ 
    int i,j; 
    printf("\n Matrice Inverse : \n");
    for (i=0;i<n;i++) 
    { 
        for (j=n;j<2*n;j++) 
        { 
            printf("%lf ",mat[i][j]); //"% .15E  "
        } 
        printf("\n" ); 
    } 
} 

double *Sequence(int indiceDebut,int time,int n,vecteur *LR,int k,double *v,double *e)
{
 int i,j,t;
 double *s,*s1,*s2;

 s2 = (double*)malloc(n*sizeof(double));
 
 
 //--------- Calcule de la suite Sn --------------------
 
 for (i=0;i<k;i++) 
    {
    if(i == indiceDebut)
         {
         s = LR[i].relements;
        
         for(t=0 ; t<time ;t++)
         {
         s1 = Prod(e,Prod_Vect_VectT(v,s,n) ,n);
         
         for (j=0;j<n;j++)
          s2[j] = 0 ;
         
         for (j=0;j<k;j++)
         {
         s2 = Add_Vect(s2,Prod(LR[j].relements,Prod_Vect_VectT(LR[j].celements,s,n) ,n),n);
         }
         s = Add_Vect(s1,s2,n);
         }
         break;
         }         
    }
    if(i==k)
    {
            printf("erreur indice : vecteur r%d existe pas ! \n",indiceDebut);
            exit(2);
    }
 
    return s;   
}

double **Puissance_M(vecteur*LR,double *e,double*v,int n,int k,int time)
{
 int i,j;
 double *s;
    double **M2 = NULL;
    double **M = NULL ;
 
 M2 = alloc_mat(n,M2);
 
 for (i=0;i<n;i++) 
 {
     for (j=0;j<n;j++) 
     {
         M2[i][j] = 0;
     }
 }
 
 for (i=0;i<k;i++) 
 {
     s = Sequence(i,time,n,LR,k,v,e); // Sequence(int indiceDebut,int time,int n,vecteur *LR,int k,double *v,double *e)
     M2 = Add_Mat(M2,Prod_VectT_Vect(s,LR[i].celements,n),n);
 } 
 
 M = Add_Mat(M2,Prod_VectT_Vect(e,v,n),n);
 return M;
 //afficherMatrice(M,n) ; 
       
}



void Dist_TR_LR(char NameOutPut[80],int time)
{
int i,j,t,n,k;
char s1[80],s2[80],s3[80],s4[80];   
FILE *Input,*Input2,*Output;  
double *v;

//s4 = (char)time;
strcpy(s1, NameOutPut);
strcpy(s2, NameOutPut);
strcpy(s3, NameOutPut);

strcat(s1,".LR");   // fichier en entrée
strcat(s3,".pi_0");   // fichier en entrée
sprintf(s4,".pi_%d",time); 
strcat(s2,s4);  // fichier de sortite


if( fopen(s1,"r") == NULL )
{
 printf("erreur : file \"%s\" doesn't exist !",s1);
 exit(2);       
}

if( fopen(s3,"r") == NULL )
{
 printf("erreur : file \"%s\" must exist !",s3);
 exit(2);       
}

Input=fopen(s1,"r");
Input2=fopen(s3,"r");
Output=fopen(s2,"w");

//------lecture de k et n -------------------------

fscanf(Input,"%d %d",&k,&n);
if(k<=0 || n<=0)
{
  printf("erreur dans \"%s\" : rang ou taille des vecteurs invalide !",s1);
  exit(2);   
}

//------lecture du vecteur v ----------------------

v = (double*)malloc(n*sizeof(double));
for (i=0;i<n;i++) 
    { 
     fscanf(Input,"%lf",&v[i]);            
    }
  
//-------lecture des vecteurs ri et ci ------------

vecteur *LR;
LR = (vecteur*)malloc(k*sizeof(vecteur));
for (i=0;i<k;i++)
    {
    LR[i] = Alloc(n) ;    
    }
for (i=0;i<k;i++) 
    { 
     for (j=0;j<n;j++) 
        { 
            fscanf(Input,"%lf",&(LR[i].relements[j])); 
        } 
       for (j=0;j<n;j++) 
        { 
            fscanf(Input,"%lf",&(LR[i].celements[j]));
        }  
     }
     
//---------------- Distribution transitoire ------------- 

 double *e,*PI0,*PI;

 PI0 = (double*)malloc(n*sizeof(double));
 e = (double*)malloc(n*sizeof(double));
 
 
 fprintf(Output,"0  ");
 for (i=0;i<n;i++)
 {
     e[i] = 1;
     fscanf(Input2,"%lf",&PI0[i]);
     fprintf(Output,"%lf  ",PI0[i]);
 }
  fprintf(Output,"\n");
 
 
 for (t=0;t<time;t++)
 {
     
     PI = Prod_Vect_Mat(PI0,Puissance_M(LR,e,v,n,k,t),n);
     
     fprintf(Output,"%d  ",t+1);
     
     for (i=0;i<n;i++)
     {
         if( fabs(PI[i]) < EPSILON )
           PI[i]=0;
         fprintf(Output,"%lf  ",PI[i]);
     }
     fprintf(Output,"\n");
 }
  

printf(" Operation reussie ! \n voir resultats dans le fichier \"%s\" \n",s2);

fclose(Input);
fclose(Output);   
}






