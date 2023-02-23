#include "A6vect.h" // inclure le fichier vect.h dans celuici .c
#define EPSILON 0.0000000001


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
           if( fabs(mat[i][j]) < EPSILON )
           mat[i][j] = 0;
      }
 }
return mat;
}

double **Add_Mat(double **mat1,double **mat2,int n)
{
int i,j;  

for (i = 0 ; i < n ; i++) 
 {
  for (j = 0 ; j < n ; j++) 
  {
    mat2[i][j]= mat1[i][j] + mat2[i][j];
    
    if( mat2[i][j]  < EPSILON )
    mat2[i][j] = 0;
    
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



int taille_creu(double *vect , int n)
{
  int i,c = 0;
  
  for (i=0;i<n;i++)
  {
  if( fabs(vect[i]) > EPSILON )
  c++;
  }
  
return c;      
}

void LR_TO_Rii(char NameOutPut[80])
{
int i,j,k,n;
char s1[80],s2[80],s3[80],s4[80];   
FILE *Input,*Output1,*Output2,*Output3;  
double *v,*e;


strcpy(s1, NameOutPut);
strcpy(s2, NameOutPut);
strcpy(s3, NameOutPut);
strcpy(s4, NameOutPut);

strcat(s1,".LR");   // fichier en entrée
strcat(s2,".Rii");  // fichier en sortie 1
strcat(s3,".sz");  // fichier en sortie 2 
strcat(s4,".Rid"); // fichier en sortie 3


if( fopen(s1,"r") == NULL )
{
 printf("erreur : file \"%s\" doesn't exist !",s1);
 exit(2);       
}

Input=fopen(s1,"r");
Output1=fopen(s2,"w");
Output2=fopen(s3,"w");
Output3=fopen(s4,"w");

//------lecture de k et n -------------------------

fscanf(Input,"%d %d",&k,&n);
if(k<=0 || n<=0)
{
  printf("erreur dans \"%s\" : rang ou taille des vecteurs invalide !",s1);
  exit(2);   
}

//------lecture du vecteur v ----------------------

v = (double*)malloc(n*sizeof(double));
e = (double*)malloc(n*sizeof(double));

for (i=0;i<n;i++) 
    { 
     fscanf(Input,"%lf",&v[i]);   
     e[i] = 1;         
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
 //--------------- Création de la Matrice M -------------
 
double **M;
M = Prod_VectT_Vect(e,v,n);

for (i=0;i<k;i++)
{
M = Add_Mat(M,Prod_VectT_Vect(LR[i].relements,LR[i].celements,n),n);   
}


//------------ Transformation en Rii --------------------

int d,c = 0;

for (i=0;i<n;i++) 
    { 
    fprintf(Output1,"%12d",i);
    fprintf(Output3,"%12d",i);
    d = taille_creu(M[i],n);
    fprintf(Output1,"%12d",d);
    fprintf(Output3,"%12d",d);
    c+= d;
    
     for (j=0;j<n;j++) 
        { 
        if( fabs( M[i][j] ) > EPSILON )
        fprintf(Output1," %lf%12d",M[i][j],j); // "% .15E%12d"
        }
        
     for (j=n-1;j>=0;j--) 
        { 
        if( fabs( M[i][j] ) > EPSILON )
        fprintf(Output3," %lf%12d",M[i][j],j); // "% .15E%12d"
        }  
        
    fprintf(Output1,"\n");
    fprintf(Output3,"\n");
    }

fprintf(Output2,"%12d\n",c); // #element non null
fprintf(Output2,"%12d\n",n); // dimension
fprintf(Output2,"%12d\n",1);
     

printf(" Operation reussie ! \n voir resultats dans les fichiers :  \"%s\" \"%s\" \"%s\" \n",s2,s4,s3);

fclose(Input);
fclose(Output1);   
fclose(Output2);  
fclose(Output3);
}






