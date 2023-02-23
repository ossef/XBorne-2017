
/*---------------------------------------------*/
/*                                             */
/*partie de Code specifique a  chaque probleme */
/*                                             */
/*---------------------------------------------*/

/* 2 files en tandem, uniformisee avec perte si debordement */


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
  Max[0] = 3;
  Min[1] = 0;
  Max[1] = 3;
}


void EtatInitial(E)
int *E;
{
  /*donne a E la valeur de l'etat racine de l'arbre de generation*/
  E[0] = 0;
  E[1] = 0;
}


void InitLabels()
{
    Label[0] = 'l';
    Label[1] = 'a';
    Label[2] = 'A';
    Label[3] = 'r';
    Label[4] = 'd';
    Label[5] = 'D';
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
   case 4:  if (E[0]>Min[0]) { F[0]--; if (E[1]<Max[1]) {
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
