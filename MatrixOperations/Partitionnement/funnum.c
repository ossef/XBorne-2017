#define BUFFERSIZE 3

int NumerodeBlock(et, j)
int  *et;
int j;
{
  long k;
  k=et[NEt-1]*(1+BUFFERSIZE)+et[NEt-2];
  return(k);
}

