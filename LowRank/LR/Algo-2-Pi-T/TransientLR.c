#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "A2vect.c"
#include <time.h>

int main(int argc, char *argv[])
{
  
  if (argc != 4 || atoi(argv[3])<=0 )
  {
   printf("\nUsage : main -f <ModelName> <time>\n\n");
   printf("    <time> : doit etre >0 ,instant (discret) ,DTMC at time ... \n\n");
   printf("files \"ModelName.LR\" and \"ModelName.pi_0\" must exists before \n");
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
  Dist_TR_LR(argv[2],atoi(argv[3]));
  t2 = clock();
  printf("\ntemps en seconde : %lf\n",(float)(t2-t1)/CLOCKS_PER_SEC );
  
  return 0;
}
