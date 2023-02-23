//****************************************
//  LMSUB algorithm
//  Version : row computation
//  created by : A.B.
//****************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LmsubRow.h"

typedef struct {
  long ind;
  double val;
} ElementD;

typedef struct {
  long nbEl;
  ElementD *vElement;
} VectD;

char *sName;
char *sNameOut;

long size;    //number of states 
long nBlocks; //number of blocks

long *lastInBlock; //the last state in a block

//******************
//Error messages
//******************

void  usage(){
  printf("Usage : limsub matrix\nUses matrix.Rii, matrix.sz and matrix.bk\n");
  exit(1);
}

void  noFile(const char* pSFileName){
  printf("File not found: %s \n", pSFileName);
  exit(2);
}

void  memoryProblem(){
  printf("Sorry, not enough memory.\n");
  exit(3);
}

inline void notIrd(){
  #if (STOP==1)
    printf("Error : not compatible with the ireducibility constraints\n");
    exit(4);
  #endif
}

void readErr(long line){
  printf("Error in line %ld.\n", line);
  exit(5);
}

void init(){

  FILE *pFile;
  char *sTemp;
  
  long bk;  
  long garbage;

  if (!(sTemp=(char *)malloc((strlen(sName)+5)*sizeof(char)))) memoryProblem();

  //Reading the name.sz file to obtain the size of the matrix (size)
  strcpy(sTemp, sName);
  strcat(sTemp, ".sz");
  if ((pFile = fopen(sTemp,"r"))==NULL) noFile(sTemp);
  printf("Reading %s ...\n", sTemp);
  fscanf(pFile, "%ld\n%ld", &garbage, &size);
  printf("Lines: %ld\n", size);
  fclose(pFile);
  
  //Reading the block description file
  strcpy(sTemp, sName);
  strcat(sTemp, ".bk");
  if ((pFile = fopen(sTemp,"r"))==NULL) noFile(sTemp);
  printf("Reading %s ...\n", sTemp);

  fscanf(pFile, "%ld", &nBlocks);
  printf("Blocks: %ld\n", nBlocks);
  if (!(lastInBlock=(long *)malloc(nBlocks*sizeof(long)))) memoryProblem();
  for (bk=0; bk<nBlocks; bk++){
    fscanf(pFile, "%ld", &(lastInBlock[bk]));
  }

  fclose(pFile);  
  free(sTemp);
}

void clean(){
  
  free(lastInBlock);
}

//DEBUG only
void displayVect(double *vect, long n){
  long i;
  
  for(i=0; i<n; i++) printf("%lf ", vect[i]);
  printf("\n");
}

//Debug only
void displayVectD(VectD pv){
  long i;

  printf("%ld ", pv.nbEl);
  for (i=0; i<pv.nbEl; i++){
    printf("%.15e %ld\t", pv.vElement[i].val, pv.vElement[i].ind);
  }
  printf("\n");

}

inline void InsertElement(VectD *pv, long ind, double val){
  (*pv).vElement[(*pv).nbEl].ind = ind;
  (*pv).vElement[(*pv).nbEl].val = val;
  (*pv).nbEl++;
}

void updateSum(FILE *pFile, VectD *pvOld, VectD *pvNew, long line){

  long r, nbEl, ind, i;
  double val, sum=0;

  long currBlockNew=nBlocks-1;
  long currEl=0;

  (*pvNew).nbEl=0;

  fscanf(pFile, "%ld", &r);
  if (r!=line) readErr(line);
  
  fscanf(pFile, "%ld", &nbEl);

  for (i=0; i<nbEl; i++){
    fscanf(pFile, "%lf", &val);
    fscanf(pFile, "%ld", &ind);
    
	//trouver le bloc correspondant pour l'element qu'on vient de lire
    while (currBlockNew > 0 && lastInBlock[currBlockNew-1]>=ind) currBlockNew--;
	
	//traiter les elements dans l'ancienne liste qui ont des indices plus grands ou egaux que le bloc actuel
    while (currEl < (*pvOld).nbEl && (*pvOld).vElement[currEl].ind >= currBlockNew) {
      if ((*pvOld).vElement[currEl].val > sum + epsNull) {
		InsertElement(pvNew, (*pvOld).vElement[currEl].ind, (*pvOld).vElement[currEl].val);
      }
      currEl++;
    }   
    sum+=val;

    //si on deja le bloc actuel dans notre liste
    if ((*pvNew).nbEl>0 && (*pvNew).vElement[(*pvNew).nbEl-1].ind == currBlockNew){
      if ((*pvNew).vElement[(*pvNew).nbEl-1].val + epsNull < sum) {
        (*pvNew).vElement[(*pvNew).nbEl-1].val = sum;
      }
    }
    else { //on a pas encore le bloc actuel dans notre liste
      if ((*pvNew).nbEl==0 || (*pvNew).vElement[(*pvNew).nbEl-1].val + epsNull < sum) {
		InsertElement(pvNew, currBlockNew, sum);
      }
      
    }
  }
     
}


void limsubRow(){

  FILE *pFile, *pFileOut;
  
  long block, line, i, currBlock;
  long first=0, last;
  
  long nbEl;
  
  char *sTemp, *sTempOut;
  
  long nbTrans=0;
  
  VectD *pvNew, *pvOld, *pvTemp; 
  
  double sum;
  
  if (!(sTemp=(char *)malloc((strlen(sName)+5)*sizeof(char)))) memoryProblem();
  if (!(sTempOut=(char *)malloc((strlen(sNameOut)+5)*sizeof(char)))) memoryProblem();

  //Opening the name.Rid file
  strcpy(sTemp, sName);
  strcat(sTemp, ".Rid");
  if ((pFile = fopen(sTemp,"r"))==NULL) noFile(sTemp);
  printf("Opening %s ...\n", sTemp);
  
  //Opening the nameOut.Rii file
  strcpy(sTempOut, sNameOut);
  strcat(sTempOut, ".Rii");
  if ((pFileOut = fopen(sTempOut,"w"))==NULL) noFile(sTempOut);
  printf("Opening %s ...\n", sTempOut);

  if (!(pvNew=(VectD *)malloc(sizeof(VectD)))) memoryProblem();
  if (!(pvOld=(VectD *)malloc(sizeof(VectD)))) memoryProblem();
  if (!((*pvNew).vElement=(ElementD *)malloc(nBlocks*sizeof(ElementD)))) memoryProblem();
  if (!((*pvOld).vElement=(ElementD *)malloc(nBlocks*sizeof(ElementD)))) memoryProblem();

  (*pvNew).nbEl=0;
  
  for (block=0; block<nBlocks; block++){
    //monotonicity if MON==1
	#if (MON==0)
	  (*pvNew).nbEl=0;
	#endif
	
	if(block>0) first=lastInBlock[block-1]+1;
        last=lastInBlock[block];
	
	for (line=first; line<=last; line++){    
	  pvTemp=pvOld; pvOld=pvNew; pvNew=pvTemp;
	  updateSum(pFile, pvOld, pvNew, line);	  
	}
    
    //Ireducibility constraints
	#if (IRD==1)
	  pvTemp=pvOld; pvOld=pvNew; pvNew=pvTemp;
	  
	  //On copie les elements dans le triangle (strictement) superieur 
	  (*pvNew).nbEl=0;
	  for(i=0; i<(*pvOld).nbEl && (*pvOld).vElement[i].ind > block; i++){
		InsertElement(pvNew, (*pvOld).vElement[i].ind, (*pvOld).vElement[i].val);
	  }
	  
	  //On ajoute (eventuellement) l'element de la surdiagonale 
	  if (i==0) {
		if(block+1 < nBlocks){
		  sum=EPSILON;
		  InsertElement(pvNew, block+1, sum);
		}
		else sum=0;
	  }
	  else sum=(*pvNew).vElement[(*pvNew).nbEl-1].val;
	  
	  //L'element de la diagonale
	  if(i<(*pvOld).nbEl && (*pvOld).vElement[i].ind == block){
	    if((*pvOld).vElement[i].val>sum+epsNull){
		  InsertElement(pvNew, (*pvOld).vElement[i].ind, (*pvOld).vElement[i].val);
		  sum=(*pvNew).vElement[(*pvNew).nbEl-1].val;
		}
	    i++;
	  }
	  
	  //La sous-diagonale (on DOIT avoir un element)
	  if(i<(*pvOld).nbEl && (*pvOld).vElement[i].ind == block-1){
	    if((*pvOld).vElement[i].val>sum+epsNull){
		  InsertElement(pvNew, (*pvOld).vElement[i].ind, (*pvOld).vElement[i].val);
		  sum=(*pvNew).vElement[(*pvNew).nbEl-1].val;
		}
		i++;
	  }
	  if(block>0){
	    if((*pvNew).nbEl==0 || (*pvNew).vElement[(*pvNew).nbEl-1].ind!=block-1){
		  if(sum+epsNull>1.0) notIrd();
		  else {
		    sum+=(1.0-sum)*EPSILON;
		    InsertElement(pvNew, block-1, sum);
		  }
		}
	  }
	  
	  //On copie les elements dans le triangle (strictement) inferieur si ils sont > sum 
	  for( ; i<(*pvOld).nbEl; i++){
	    if((*pvOld).vElement[i].val>sum+epsNull){
		  InsertElement(pvNew, (*pvOld).vElement[i].ind, (*pvOld).vElement[i].val);
		}
	  }
	#endif

	//Write the line to the file
	fprintf(pFileOut, "%ld %ld", block, (*pvNew).nbEl);
	
	nbTrans+=(*pvNew).nbEl;
       	 
	for (i=(*pvNew).nbEl-1; i>0; i--) 
          fprintf(pFileOut, " %.12e %ld", (*pvNew).vElement[i].val - (*pvNew).vElement[i-1].val, (*pvNew).vElement[i].ind);
	fprintf(pFileOut, " %.12e %ld\n", (*pvNew).vElement[0].val, (*pvNew).vElement[0].ind); 
  }
  
  fclose(pFile);
  fclose(pFileOut);

  
  //Writing the nameOut.sz file
  strcpy(sTempOut, sNameOut);
  strcat(sTempOut, ".sz");
  
  if ((pFileOut = fopen(sTempOut,"w"))==NULL) noFile(sTempOut);
  printf("Writing %s ...\n", sTempOut);
  fprintf(pFileOut, "%ld\n%ld\n%ld\n", nbTrans, nBlocks, 1);
  
  fclose(pFileOut);
  
  free(sTemp);
  free(sTempOut);
  
  free((*pvNew).vElement);
  free(pvNew);
  
  free((*pvOld).vElement);
  free(pvOld);
}

int main(int argc, char* argv[]){

  if (argc != 3) usage();
  sName=argv[1];
  sNameOut=argv[2];
  init();
  limsubRow();
  clean();
  return 0;
}
