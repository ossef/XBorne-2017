#include "vect.h" // inclure le fichier vect.h dans celuici .c
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
   ProblemeMemoire();
 }
 //printf("OK! alloc \n");
 return mat;
}

double **alloc_matInv(int n, double **mat)
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
  printf("Stockage Matrice Inverse : \n");
  ProblemeMemoire() ; 
 }
 
 for( i=0 ; i<n ; i++) 
 {
   mat[i] = (double*)malloc(2*n*sizeof(double));
   if(mat[i] == NULL)
   {
   printf("Stockage Matrice Inverse : \n");
   ProblemeMemoire();  
   }
   
 }
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

double *Prod_Vect_MatInv(double *vect,double **mat,int n)
{
 int i, j,c=0;
 double sum;
 double * res;
 res = malloc(n*sizeof(double));
 for (i = n ; i < 2*n ; i++) 
 {
  sum = 0;
  
  for (j = 0 ; j < n ; j++) 
  {
    sum += mat[j][i] * vect[j];
    
    
  }  
  if( fabs(sum) < EPSILON )
      res[c++] = 0; 
  else
      res[c++] = sum;
 }
return res;       
}

double *Add_Vect(double *vect1,double *vect2,int n)
{
 int i;
 double * res;
 res = malloc(n*sizeof(double));
 for( i=0 ; i<n ; i++)
 {
    res[i]= vect1[i]+vect2[i]; 
 }
return res;       
}

double *Prod(double *vect ,double val ,int n)
{
 int i;
 double * res;
 res = malloc(n*sizeof(double));
 for( i=0 ; i<n ; i++)
 {
    res[i]= vect[i]*val; 
    if( fabs(res[i]) < EPSILON )
           res[i]=0;
    
 }
return res;  
}

double **Soust_Mat(double **mat1,double **mat2,int n)
{
int i,j;  
double **res=NULL;
res = alloc_mat(n,res);

for (i = 0 ; i < n ; i++) 
 {
  for (j = 0 ; j < n ; j++) 
  {
    res[i][j]= mat1[i][j] - mat2[i][j];
    if( fabs(res[i][j]) < EPSILON )
           res[i][j]=0;
  }   
 }
 return res;     
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


double **creerMatriceId(int n) 
{ 
    int i,j; 
    double **mat=NULL ;
    mat = alloc_mat(n,mat);
    for (i=0;i<n;i++) 
    { 
        for (j=0;j<n;j++) 
        { 
            if (i==j) 
            { 
                mat[i][j] = 1; 
            } 
            else 
            { 
                mat[i][j] = 0; 
            } 
        } 
    } 
    return mat;
} 


double **definirNouvelleMatrice(double **mat1,double **matId,int n) 
{ 
    int i,j; 
    double ** NewMat = NULL ;
    NewMat = alloc_matInv(n,NewMat);
    
    for (i=0;i<n;i++) 
    { 
        for (j=0;j<2*n;j++) 
        { 
            if (j<n) 
            { 
                NewMat[i][j] = mat1[i][j]; 
            } 
            else 
            { 
                NewMat[i][j] = matId[i][j-n]; 
            } 
        } 
    } 
    return NewMat;
} 


int MethodeGauss(double ** mat1,int n)
{
    int inversible = 1;
    int k,i,colonne,colonnebis;
    double var,var1;
    k=0;
    double **matId;
    matId = creerMatriceId(n);
    NewMat = definirNouvelleMatrice(mat1,matId,n);
    
    while((inversible == 1)&&(k<n)) 
    {
            if (NewMat[k][k] != 0)
            {
                var = NewMat[k][k];
                for (colonne=0;colonne<2*n;colonne++)
                {
                    NewMat[k][colonne] = NewMat[k][colonne]/var;  //Normalisation de la ligne contenant l'élément diagonal
                }
                for (i=0;i<n;i++)
                {
                    if (i != k)
                    {
                        var1=NewMat[i][k];
                        for (colonnebis=0;colonnebis<2*n;colonnebis++)
                        {
                            NewMat[i][colonnebis] = NewMat[i][colonnebis] - NewMat[k][colonnebis]*var1;
                        }
                    }
                }
                k++;
            }
            else // diagonale contient un element null
            {
                inversible = 0 ;
            }
    }
    return inversible;
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

void Dist_Stat_LR(char NameOutPut[80])
{
int i,j,n,k,boolean;
char s1[80],s2[80];   
FILE *Input,*Output;  
double *v,*V,*E,*PI;
double **A= NULL;


strcpy(s1, NameOutPut);
strcpy(s2, NameOutPut);

strcat(s1,".LR");   // fichier en entrée
strcat(s2,".pi");  // fichier de sortie

if( fopen(s1,"r") == NULL )
{
 printf("erreur : file \"%s\" doesn't exist !",s1);
 exit(2);       
}

Input=fopen(s1,"r");
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
     
//---------  1- Computing vector V by V(i) = v*Tri-----------    

V = (double*)malloc(k*sizeof(double));
for(i=0 ; i<k ; i++)
{
V[i] = Prod_Vect_VectT(v,LR[i].relements,n);              
}


//-------- 2- Computing matrix A by A(i,l) = cl*Tri ----------

A = alloc_mat(k,A);
for(i=0 ; i<k ; i++)
{
  for(j=0 ; j<k ; j++)
  {
   A[i][j] = Prod_Vect_VectT(LR[i].celements,LR[j].relements,n);
  }
} 


//------- 3- Computing Vector E by E = V(Id-A)^-1 -------------

E = (double*)malloc(k*sizeof(double));
boolean = MethodeGauss(Soust_Mat(creerMatriceId(k),A,k),k);
if(boolean == 1)
{
E = Prod_Vect_MatInv(V,NewMat,k);
}
else
{
printf("\n Matrice (Id - A) est non inversible ! \n ");
exit(2);
}


//------ 4- Finaly Computing the steady-state distribution Pi ---

PI = (double*)malloc(n*sizeof(double));
for(i=0 ; i<n ; i++)
{
PI[i] = 0;
}
for (i=0;i<k;i++)
{
PI = Add_Vect(PI,Prod(LR[i].celements,E[i],n),n);
}
PI = Add_Vect(v,PI,n);
for (i=0;i<n;i++)
{
if( fabs(PI[i]) < EPSILON )
           PI[i]=0;
fprintf(Output,"%lf \n",PI[i]);
}

printf(" Operation reussie ! \n voir resultats dans le fichier \"%s\" \n",s2);

fclose(Input);
fclose(Output);   
}






