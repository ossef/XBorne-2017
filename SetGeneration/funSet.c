
/*---------------------------------------------*/
/*                                             */
/*partie de Code specifique a  chaque probleme */
/*                                             */
/*---------------------------------------------*/

/* 2 files en tandem, uniformisee avec perte si debordement */


/* taux des evenements */ 

#define l1  1.0		
#define l2  2.0		
#define m1  2.0		
#define m2  5.0		
#define p12  0.6		

/* codage des evenements */
/* 1 : loop */
/* 2 : arrivee file 1 */
/* 3 : arrivee file 2 */
/* 4 : service file 1 et routage 1->2 */
/* 5 : service file 1 et depart  */
/* 6 : service file 2 et depart  */

void InitEtendue()
{
    Min[0] = 0;
    Max[0] = 9;
    Min[1] = 0;
    Max[1] = 9;
}


void EtatInitial(E)
int *E;
{
    /*donne a E la valeur de l'etat racine de l'arbre de generation*/
    E[0] = 0;
    E[1] = 0;
}



double InfProbabilite(indexevt, E)
int indexevt;
int *E;
{
    /*
     retourne la probabilite d'apparition de l'evenement indexevt
     */
    double p1;
    double da = (l1 + l2 + m1 + m2)*2;
    switch (indexevt) {
        case 1:
            p1 = 0.5;
            break;
        case 2:
            p1 = l1/(2*da);
            break;
        case 3:
            p1 = l2/da;
            break;
        case 4:
            p1 = m1*p12/(2*da);
            break;
        case 5:
            p1 = m1*(1-p12)/da;
            break;
        case 6:
            p1 = m2/da;
            break;
    }
    return(p1);
}

double SupProbabilite(indexevt, E)
int indexevt;
int *E;
{
    /*
     retourne la probabilite d'apparition de l'evenement indexevt
     */
    double p1;
    double da = (l1 + l2 + m1 + m2)*2;
    switch (indexevt) {
        case 1:
            p1 = 0.6;
            break;
        case 2:
            p1 = l1/da;
            break;
        case 3:
            p1 = l2/da;
            break;
        case 4:
            p1 = m1*p12/da;
            break;
        case 5:
            p1 = m1*(1-p12)/da;
            break;
        case 6:
            p1 = m2/da;
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
    F[1] = E[1];
    switch (indexevt) {
        case 1: break;
        case 2:     if (E[0]<Max[0]) {F[0]++;
        }
            break;
        case 3:  if (E[1]<Max[1]) { F[1]++;}
            break;
        case 4:  if (E[0]>Min[0]) { 
            F[0]--; 
            if (E[1]<Max[1]) {
            F[1]++;
        }
        }
            break;
        case 5:  if (E[0]>Min[0]) { F[0]--;}
            break;
        case 6:  if (E[1]>Min[1]) { F[1]--;}
            break;
    }
}



void InitParticuliere()
{
    
}
