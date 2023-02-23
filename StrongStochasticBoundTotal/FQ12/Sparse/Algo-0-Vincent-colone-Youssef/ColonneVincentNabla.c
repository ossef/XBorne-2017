#include <stdio.h>
#include <stdlib.h>
#include "YB0CListe.h"
#include "YB0CListe.c"
#include <time.h>



int main(int argc, char *argv[])
{
    int vi;
    float epsilon;
    
  if (argc != 4  )
  {
   printf("\nUsage : main <FileName> colonne epsilon \n");
   printf("FileName.Rii and FileName.sz must exist before. \n");
   return 0;
  }
  
  clock_t t1, t2;
  
    vi= atoi(argv[2]);
    epsilon = atof(argv[3]);
    
  t1 = clock();
  UpperBoundMatrix(argv[1], vi, epsilon);
  t2 = clock();
  printf("\ntemps en seconde : %Lf\n",(float)(t2-t1)/CLOCKS_PER_SEC );
    
  system("PAUSE");	
  return 0;
}


