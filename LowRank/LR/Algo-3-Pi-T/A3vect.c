#include "A3vect.h" // inclure le fichier vect.h dans celuici .c
#define EPSILON 0.0000000001 // precision du calcule

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

double *Prod_Mat_Vect(double *vect,double **mat,int n)
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
            sum += mat[i][j] * vect[j];
        }
        res[c++] = sum;
    }
    return res;
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

double **Soust_Mat(double **mat1,double **mat2,int n)
{
    int i,j;
    
    for (i = 0 ; i < n ; i++)
    {
        for (j = 0 ; j < n ; j++)
        {
            mat1[i][j]= mat1[i][j] - mat2[i][j];
            
            if( fabs(mat1[i][j]) < EPSILON )
                mat1[i][j] = 0;
        }
    }
    return mat1;
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

double **prod_mat(double **mat1,double **mat2, int n)
{
    int i,j,k,b1,b2;
    b1 = 0;
    b2 = 0;
    double s;
    double **mat=NULL;
    
    for (i = 0 ; i < n ; i++)
    {
        for (j = 0 ; j < n ; j++)
        {
            if(mat1[i][j]!=0)
            {
                b1 = 1;
                break;
            }
        }
        if(b1 == 1)
            break;
    }
    
    for (i = 0 ; i < n ; i++)
    {
        for (j = 0 ; j < n ; j++)
        {
            if(mat2[i][j]!=0)
            {
                b2 = 1;
                break;
            }
        }
        if(b2 == 1)
            break;
    }
    
    if(b1 == 0 )
    {
        return mat1;
    }
    if(b2 == 0 )
    {
        return mat2;
    }
    
    mat = alloc_mat(n,mat);
    
    for (i = 0 ; i < n ; i++)
    {
        for (j = 0 ; j < n ; j++)
        {
            s=0;
            for (k = 0 ; k < n ; k++)
            {
                s+= mat1[i][k] * mat2[k][j];
            }
            
            mat[i][j] = s;
        }
        
    }
    
    return mat;
    
}

double **Puissance_A(double **A, int k,int p)
{
    
    if( p == 0 )
    {
        printf("erreur : la puissance de la matrice doit etre > 0 ");
        exit(2);
    }
    
    double **z;
    z = creerMatriceId(k);
    
    while ( p>0 )
    {
        if( p%2 == 0 )
        {
            A = prod_mat(A,A,k);
            p = p/2;
        }
        else
        {
            z = prod_mat(A,z,k);
            p--;
        }
    }
    
    return z;
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
    double **mat=NULL;
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
    double ** NewMat = NULL;
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

double *Sequence(int indiceDebut,int time,int n,int k,vecteur *LR,double *V,double **A,double *e)
{
    int i,it,j,t,boolean;
    double *s,*s1,*s2 ,*G;
    double **B,**Inv;
    
    s2 = (double*)malloc(n*sizeof(double));
    
    
    //--------- Calcule de la suite Sn --------------------
    
    for (i=0;i<k;i++)
    {
        if(i == indiceDebut)
        {
            s = LR[i].relements;
            
            for(t=0 ; t<time ;t++)
            {
                B = Puissance_A(A,k,t+1); // etape 3 : calcule de B(n) = A^n où n : represente l'instant t
                Inv = alloc_mat(k,Inv); // etape 4 : calcule de G(n) = (I-A)^-1 x (I-B(n)) x V
                boolean = MethodeGauss(Soust_Mat(creerMatriceId(k),A,k),k);
                
                if(boolean == 1)
                {
                    int c;
                    
                    for (it=0;it<k;it++)
                    {
                        for ( c=0,j=k ; j<2*k ; j++)
                        {
                            Inv[it][c++] = NewMat[it][j];
                        }
                    }
                    G = Prod_Vect_Mat(V,prod_mat(Inv,Soust_Mat(creerMatriceId(k),B,k), k),k);
                }
                else
                {
                    printf("\n Matrice (Id - A) est non inversible ! \n ");
                    exit(2);
                }
                
                s1 = Prod(e,G[i],n);
                
                for (j=0;j<n;j++)
                    s2[j] = 0 ;
                
                for (j=0;j<k;j++)
                {
                    s2 = Add_Vect(s2,Prod(LR[j].relements,B[i][j],n),n);
                }
                s = Add_Vect(s1,s2,n);
            } //fin for t
            break;
        } // fin if
    } //fin for i
    if(i==k)
    {
        printf("erreur indice : vecteur r%d existe pas ! \n",indiceDebut);
        exit(2);
    }
    
    return s;   // etape 5 : calcule de S(n) ok !
}

double **Puissance_M(vecteur *LR,double **A,double *V,double *e,double *v,int n,int k,int time)
{
    int i,j;
    double *s;
    double **M2=NULL;
    double **M=NULL;
    
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
        s = Sequence(i,time,n,k,LR,V,A,e);
        M2 = Add_Mat(M2,Prod_VectT_Vect(s,LR[i].celements,n),n);
    }
    
    M = Add_Mat(M2,Prod_VectT_Vect(e,v,n),n);
    return M;
    
    
}

void Dist_TR_LR(char NameOutPut[80],int time)
{
    int i,j,t,k,n;
    char s1[80],s2[80],s3[80],s4[80];
    FILE *Input,*Input2,*Output;
    double *e,*v,*V;
    double **A=NULL;
    
    
    strcpy(s1, NameOutPut);
    strcpy(s2, NameOutPut);
    strcpy(s3, NameOutPut);
    
    
    strcat(s1,".LR");   // fichier en entrée
    strcat(s3,".pi_0");  // fichier en entrée
    sprintf(s4,".pi_%d",time);
    strcat(s2,s4);  // fichier de sortite
    
    if( fopen(s1,"r") == NULL )
    {
        printf("erreur : file \"%s\" must exist !",s1);
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
    
    //------lecture du vecteur v ------------------------------------------------
    
    v = (double*)malloc(n*sizeof(double));
    e = (double*)malloc(n*sizeof(double));
    
    for (i=0;i<n;i++)
    {
        fscanf(Input,"%lf",&v[i]);
        e[i] = 1;
    }
    
    //-------lecture des vecteurs ri et ci --------------------------------------
    
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
    
    //---------  1- Computing vector V by V(i) = v*Tri --------------------------  
    
    V = (double*)malloc(k*sizeof(double));
    for(i=0 ; i<k ; i++)
    {
        V[i] = Prod_Vect_VectT(v,LR[i].relements,n);              
    }
    
    
    //-------- 2- Computing matrix A by A(i,l) = cl*Tri -------------------------
    
    A = alloc_mat(k,A);
    for(i=0 ; i<k ; i++)
    {
        for(j=0 ; j<k ; j++)
        {
            A[i][j] = Prod_Vect_VectT(LR[i].celements,LR[j].relements,n);
        }
    } 
    //afficherMatrice(A,k);
    
    //---------------- etape 3 , 4 et 5 à voir dans la méthode "Sequence" qui sera appelé dans la méthode "Puissance_M" ---------
    
    //---------------- etape final : Distribution Transitoire ----------------------
    
    double *PI,*PI0;
    
    PI0 = (double*)malloc(n*sizeof(double));
    
    fprintf(Output,"0  ");
    for (i=0;i<n;i++)
    {
        fscanf(Input2,"%lf",&PI0[i]); 
        fprintf(Output,"%lf  ",PI0[i]);
    }
    fprintf(Output,"\n");
    
    
    for (t=0;t<time;t++)
    {
        
        PI = Prod_Vect_Mat(PI0,Puissance_M(LR,A,V,e,v,n,k,t),n);
        
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






