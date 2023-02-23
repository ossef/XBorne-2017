#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "A6vect.c"


int main(int argc, char *argv[])
{
  
  if (argc != 3 )
  {
   printf("\nUsage : main -f <ModelName> \n");
   printf("file \"ModelName.LR\" must exist before, the file contain 2K+2 rows \n(first row to define k and n ,2k+1 rows of the decomposition) \n");
   printf("format du fichier \"ModelName.LR\" :\n");
   printf("k n \n");
   printf("une ligne pour le vecteur \"v\" \n");
   printf("une ligne pour le vecteur \"r1\" \n");
   printf("une ligne pour le vecteur \"c1\" \n");
   printf("...                        \"ri\"   \n");
   printf("...                         \"ci\" \n");
   
   
   return 0;
  }
  
  clock_t t1, t2;
  
  t1 = clock();
  LR_TO_Rii(argv[2]);
  t2 = clock();
  printf("\ntemps en seconde : %lf\n",(float)(t2-t1)/CLOCKS_PER_SEC );
  

  return 0;
}
