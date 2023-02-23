
void InitEtendue()
{
  Min[0] = 0;
  Max[0] = 100;
}


void InitTabou()
{
    int k;
    Etat E;
      
    E[0]=8;
    k=Etat2Int(E);
    TabTabou[0]=k-1;
    
    E[0]=10;
    k=Etat2Int(E);
    TabTabou[1]=k-1;
}

void InitOblig()
{
     TabOblig[0][0] = 0;
     TabOblig[1][0] = 1;
}

int imax(i,j)
int i,j;
{
 if (i<j) {return(j);}
 else {return(i);}
}

int imin(i,j)
int i,j;
{
 if (i<j) {return(i);}
 else {return(j);}
}


double Probabilite(inx, E)
int   inx;
int *E;
{
  /*
  retourne la probabilite d'apparition de l'evenement indexevt
  */
     double p1;
     switch (inx) {
  case 1:
    p1 = 0.4;
    break;
  case 2:
    p1 = 0.2;
    break;
      case 3:
    p1 = 0.1;
    break;
      case 4:
    p1 = 0.18;
    break;
      case 5:
    p1 = 0.12;
    break;
  }
  return(p1);
}

void Equation(E, indexevt, F, R)
int *E;
int  indexevt;
int *F, *R;
{
  switch (indexevt) {
  case 1: /* depart de 10 */ 
    F[0] = imax(Min[0],E[0]-10);
    break;
  case 2: /* depart de 1 */ 
    F[0] = imax(Min[0],E[0]-1);
    break;
  case 3:/* rien */ 
    F[0] = E[0];
    break;
  case 4: /* arrive de 1 */ 
    F[0] = imin(Max[0],E[0]+1);
    break;
  case 5: /* arrive de 2 */ 
    F[0] = imin(Max[0],E[0]+2);
    break;
  }
}


  
void InitParticuliere()
{
    
}
