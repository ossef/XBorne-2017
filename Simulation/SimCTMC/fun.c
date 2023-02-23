
/* 1 file M/M/1/B */

#define ARRIVAL 1
#define SERVICE 2
#define Buffersize 100

void InitEtendue()
{
    Min[0] = 0;
    Max[0] = Buffersize;
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
        case ARRIVAL:
            p1 = l1*1.0;
            break;
        case SERVICE:
            p1 = mu*1.0;
            break;
    }
    return(p1);
}

void Equation(E, indexevt, F, R)
int *E;
int indexevt;
int *F, *R;
{
    F[0] = E[0];
    switch (indexevt) {
        case ARRIVAL: if (E[0]<Max[0]) { F[0]++;}
            break;
        case SERVICE: if (E[0]>Min[0]) { F[0]--;}
            break;
    }
}

void InitParticuliere()
{    
}
