
/* 1 file  DTMC */
/* codage des evenements */
/* e1 : no arrivals 
 e2 = 1 arrival
 e3 = burst 
 depart  et ensuite arrivée */

void InitEtendue()
{
    Min[0] = 0;
    Max[0] = 50;
}


void EtatInitial(E)
int *E;
{
    E[0] = 0;
}

double Probabilite(indexevt, E)
int indexevt;
int *E;
{
    double p1;
    switch (indexevt) {
        case 1:
            p1 = l1;
            break;
        case 2:
            p1 = l2;
            break;
        case 3:
            p1 = l3;
            break;
    }
    return(p1);
}


void Equation(E, indexevt, F, R)
int *E;
int indexevt;
int *F, *R;
{
    /*ecriture de l'equation d'evolution, transformation de E en F grace
     a l'evenemnt indexevt, mesure de la recompense R sur cette transition*/
    F[0] = E[0];
    switch (indexevt) {
        case 1: if (E[0]>Min[0]) { F[0]--;}
            break;
        case 2: if (E[0]==Min[0]) { F[0]=1;}
            break;
        case 3:  if (E[0]>Min[0]) { F[0]--;}
            F[0]+=burst;
            if (E[0]>Max[0]) { F[0]=Max[0];}
            break;
    }
}



void InitParticuliere()
{
    
}
