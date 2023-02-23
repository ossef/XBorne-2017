/* Check if all the states are reachable  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#define Title 250
#define DEBUG 1

/* The model state */

typedef int Etat[NEt];
static int Min[NEt], Max[NEt];

#include "fun.c"

void  usage()
{
    printf("usage : AllReachable -f filename  \n");
	printf("filename.sz and fun.c must exist before \n");
	exit(1);
}

int main(argc, argv)
int argc;
char *argv[];
{
    int NSommets, NArcs;
    int i, j, k, b, nb;
    FILE *pf1 ;
    char s1[Title];
    
	if (argc!=3) usage();
        --argc;
	++argv;
	if (**argv != '-') usage();
        switch (*++*argv) {
            case 'f' :  /* get the model name */
            {++argv;
                strcpy(s1,*argv);
                ++argv;
                /* add .sz, and .cd */
                strcat(s1,".sz");
                /* does it exist ? */
                if ((pf1=fopen(s1,"r"))==NULL)
                {
                    usage();printf("Problem with the .sz file\n");
                }
                fclose(pf1);
                break;
            }
            default	 : usage();
        }
    
    /*     Get the sizes in filename.sz   */
    
    pf1=fopen(s1,"r");
    fscanf(pf1,"%d\n", &NArcs);
    fscanf(pf1,"%d\n", &NSommets);
    fclose(pf1);
    printf("Checking the size of the model\n");
    
    InitEtendue();
    
    i = Max[0] - Min[0];
    for (j = 1; j < NEt; j++)
    {
        i = i * (Max[j] - Min[j] + 1) + Max[j] - Min[j];
    }
    i++;
    
    if (i==NSommets) {
        printf("All the states are reachable..\n");
    }
    else
    {
        printf("Some states are not reachable, real states: %d, product space: %d \n", NSommets, i);
    }
    exit(0);
}



