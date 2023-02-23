#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "A5CListe.c"
#include <time.h>


int main(int argc, char *argv[])
{
  
  if (argc != 4 || atoi(argv[3]) <= 0 )
  {
   printf("\n Usage : main -f <FileName> <K> \n");
   printf("-FileName.Rii and FileName.sz and FileName.V.U.Rii and FileName.V.U.sz \n must exist before. \n");
   printf("-FileName.V.U.Rii and FileName.V.U.sz peuvent etre obtenue grace \n a l'algorithme de Vincent (voir dossier Algo-0)\n");
   printf("-<K> : est le rang de la Decomposition de la matrice en sortie \n");
   return 0;
  }
  
  clock_t t1, t2;
  
  t1 = clock();
  UpperBoundMatrix(argv[2],atoi(argv[3]));
  t2 = clock();
  printf("\ntemps en seconde : %lf\n",(float)(t2-t1)/CLOCKS_PER_SEC );

  return 0;
}


