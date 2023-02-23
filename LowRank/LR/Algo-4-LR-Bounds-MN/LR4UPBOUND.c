#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "A4CListe.c"
#include <time.h>


int main(int argc, char *argv[])
{
  
  if (argc != 4  || atoi(argv[3]) <= 0 )
  {
   printf("\nUsage : main -f <FileName>  <K> \n");
   printf("FileName.Rii and FileName.sz must exist before. \n");
   printf("<K> : est le rang de la Decomposition de la matrice en sortie \n");
   return 0;
  }
  
  
  clock_t t1, t2;
  
  t1 = clock();
  UpperBoundMatrix(argv[2],atoi(argv[3]));
  t2 = clock();
  printf("\ntemps en seconde : %lf\n",(float)(t2-t1)/CLOCKS_PER_SEC );

  
	
  return 0;
}


