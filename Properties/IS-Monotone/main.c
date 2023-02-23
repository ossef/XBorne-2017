#include "CListe.h"
#include "CListe.c"



int main(int argc, char *argv[])
{
  
  if (argc != 2  )
  {
   printf("\nUsage : main <FileName> \n");
   printf("FileName.Rii and FileName.sz must exist before. \n");
   return 0;
  }
  
  
  
  Monotonie(argv[1]);

  
 
  return 0;
}


