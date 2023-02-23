#include "A5CListe.h"
#define EPSILON 0.0000000001 // 10^-10



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

double **alloc_mat(int n,int k, double **mat)
{
    if(n<0)
    {
        printf("taille non valide");
        exit(-1);
    }
    int i;
    mat = (double **)malloc(k*sizeof(double));
    
    if(mat == NULL)
    {
        ProblemeMemoire()   ;
    }
    for( i=0 ; i<k ; i++)
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

double MAXZ(double *s,double *w,double *z,int debut,int fin)
{
    int l,c;
    double max,so;
    double S[fin-debut+1];
    c=0;
    
    for( l=debut ; l<fin ; l++)
    {
        
        if ( fabs(z[l]) < EPSILON )
        {
            S[c] = 0;
            c++;
            continue;
        }
        else
        {
            so = (s[l] - w[l])/z[l] ;
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
    for( l=1 ; l<c ; l++)
    {
        if( S[l] > max )
            max = S[l];
    }
    
    return max;
}

/* int rdtsc()
{
    __asm__ __volatile__("rdtsc");
}
*/

int *Generer(int n,int k)
{
    int i,r;
    int *l;
    
    if( k>=n )
    {
        printf("Erreur n = %d et k = %d : n doit etre strictement superieur a k,\npour pouvoir diviser en k+1 elements distinct ordonnee",n,k);
        exit(2);
    }
    
    l = (int*)malloc((k+1)*sizeof(int));
    srand(SEED);
    
    l[0] = 0;
    l[k] = n-1;
    i = 1;
    
    while(i<k)
    {
        r = 1 + rand()%((n-1)-(k-i));
        
        if(i>1)
        {
            if(r > l[i-1])
            {
                l[i] = r;
                i++;
            }
        }
        else
        {
            l[i] = r;
            i++;
        }
        
    }
    return l;
}

int Recherche(int val,int *tab,int n)
{
    int i;
    for(i=0 ; i<n ;i++)
    {
        if(tab[i] == val)
            return 1;
    }
    return -1;
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
    char s1[80],s2[80],s3[80],s4[80],s5[80];
    FILE *Size,*Rii,*Qii,*SizeQ,*Output1;
    int n,n1,n2,m,m1,m2,i,j,c,itj;
    double p;
    
    strcpy(s1, NameOutPut);
    strcpy(s2, NameOutPut);
    strcpy(s3, NameOutPut);
    strcpy(s4, NameOutPut);
    strcpy(s5, NameOutPut);
    
    
    
    strcat(s1,".sz");
    strcat(s2,".Rii");
    strcat(s3,".LR");
    strcat(s4,".V.U.Rii");
    strcat(s5,".V.U.sz");
    
    
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
    SizeQ=fopen(s5,"r");
    if(SizeQ == NULL)
    {
        printf("erreur : file \"%s\" doesn't exist !",s5);
        exit(2);
    }
    Qii=fopen(s4,"r");
    if(Qii == NULL)
    {
        printf("erreur : file \"%s\" doesn't exist !",s4);
        exit(2);
    }
    Output1=fopen(s3,"w");
    
    
    fscanf(SizeQ,"%d %d %d",&m1,&n,&m2);
    
    
    Liste l1;
    l1 = liste_vide();
    
    m=0;
    
    //------------------- 1- generer aleatoirement k+1 lignes li ----------------------
    printf("etape 1 \n");
    int *l;
    /*l = (int*)malloc((k+1)*sizeof(int));
     l[0] = 0;
     l[1] = 2;
     l[2] = 5;*/
    
    l = Generer(n,k);
    printf("\n l = [ ");
    for (i = 0 ; i<k ; i++)
        printf("%d ,",l[i]);
    printf("%d ]\n",l[k]);
    
    
    //----------------- 2- calcule de vi = Q[li,*] ------------------------------------
    printf("etape 2 \n");
    
    double **v=NULL;
    double *V,*res;
    
    res = (double*)malloc(n*sizeof(double));
    c=0;
    v = alloc_mat(n,k,v);
    
    for( i=0 ; i<n ; i++)
    {
        
        fscanf(Qii,"%d %d ",&n1,&n2);
        
        if( Recherche(i,l,k+1) == 1 )
        {
            
            l1 = liste_vide();
            
            for(j=0 ; j<n2 ; j++)
            {
                
                fscanf(Qii,"%lf %d",&p,&n1);
                l1 = inserer(l1,j,n1,p);
            }
            V = (double*)malloc(n*sizeof(double));
            v[c++] = TransformeLtoV(l1,n,V);
        }
        else
        {
            for(j=0 ; j<n2 ; j++)
            {
                fscanf(Qii,"%lf %d",&p,&n1);
            }
        }
        
    }
    
    
    //-------------------- 3-calcul de ci et ri -------------------------------------------
    
    //------------------- 3-1 calcule de c  -------------------------------------
    printf("etape 3 - 1 \n");
    
    double *s,*ci,*z,*w,*FirstRow;
    double **h = NULL;
    double **r = NULL;
    
    r = alloc_mat(n,k,r);
    h = alloc_mat(n,k,h);
    
    
    fscanf(Rii,"%d %d ",&n1,&n2);
    l1 = liste_vide();
    for(j=0 ; j<n2 ; j++)
    {
        fscanf(Rii,"%lf %d",&p,&n1);
        l1 = inserer(l1,j,n1,p);
    }
    V = (double*)malloc(n*sizeof(double));
    FirstRow = TransformeLtoV(l1,n,V);
    
    fprintf(Output1,"%d %d \n",k,n);
    for(j=0 ; j<n ; j++)
        fprintf(Output1,"%lf    ",FirstRow[j]);
    fprintf(Output1,"\n");
    
    for(i=0 ; i<k ; i++)
    {
        
        Rii=fopen(s2,"r");
        
        //--- calcule de c ----
        
        ci = Sous_Vect(v[i+1],v[i],n);
        
        //--- calcule de z ----
        
        z = Prod_VectKst2(ci,n);
        
        //--- calcule de w ----
        res = (double*)malloc(n*sizeof(double));
        w = Prod_VectKst1(v[i],n,res);
        
        //------------------- 3-2  calcule de r -------------------------------------
        printf("etape 3 - 2 \n");
        //------ la borne  j<=l[i] -----
        
        for(j=0 ; j<=l[i] ;j++)
        {
            r[i][j] = 0;
            fscanf(Rii,"%d %d ",&n1,&n2);
            for(itj=0 ; itj<n2 ; itj++)
            {
                fscanf(Rii,"%lf %d",&p,&n1);
            }
        }
        
        
        //------ le milieu de r -----
        V = (double*)malloc(n*sizeof(double));
        res = (double*)malloc(n*sizeof(double));
        for(j=l[i]+1; j<l[i+1] ;j++)
        {
            fscanf(Rii,"%d %d ",&n1,&n2);
            l1 = liste_vide();
            for(itj=0 ; itj<n2 ; itj++)
            {
                fscanf(Rii,"%lf %d",&p,&n1);
                l1 = inserer(l1,itj,n1,p);
            }
            
            s = Prod_VectKst1(TransformeLtoV(l1,n,V),n,res);
            h[i][j] = MAXZ(s,w,z,l[i]+1,l[i+1]);
            r[i][j] = Max(h[i][j],r[i][j-1]);
        }
        
        //------ la borne  j>=l[i+1] -----
        
        for(j=l[i+1] ; j<n ;j++)
        {
            r[i][j] = 1;
            fscanf(Rii,"%d %d ",&n1,&n2);
            for(itj=0 ; itj<n2 ; itj++)
            {
                fscanf(Rii,"%lf %d",&p,&n1);
            }
        }
        
        
        //---- remplissage du ficher ".LR" --------------
        for (j=0 ; j<n ;j++)
            fprintf(Output1,"%lf    ",r[i][j]);
        fprintf(Output1,"\n");
        for (j=0 ; j<n ;j++)
            fprintf(Output1,"%lf    ",ci[j]);
        fprintf(Output1,"\n");
        
    }
    
    
    
    
    printf(" Operation reussie ! \n voir la matrice Resultante sous forme Decomposee dans le fichier :  \"%s\"  \n",s3);
    
    fclose(Output1);
    
    
}










