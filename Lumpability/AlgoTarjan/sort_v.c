#include "struct.h"

void bsort_double (double * x, int * y, int j)
{
	int f, k, t;
	double d;
	do
    	{
        	f = 0;  
        	for (k = 0; k < j-1; k++)
        	{
        		if (dif (x[k+1], x[k]))          
            		{
				f = 1; 
				d = x[k]; x[k] = x[k+1]; x[k+1] = d;
				t = y[k]; y[k] = y[k+1]; y[k+1] = t;
			}	                 		                
		}
    	}
    	while (f == 1);      
}

int partition (int b)
{	
	int j, k = 0, l, c, max; 
	for (j = 0; j < n; j ++)
	{
		if (bl[j] == b)
		{
			k ++;
		}
	}
	int * i = malloc (k * sizeof(int));
	double * y = malloc (k * sizeof(double));
	k = 0;
	for (j = 0; j < n; j ++)
	{
		if (bl[j] == b) 
		{
			y[k] = w[j];
			i[k ++] = j;			
		}		
	} 	
	bsort_double (y, i, k); 
	j = 0;
	c = b;
	max = -1;
	while (j < k)
	{
		l = j+1;
		num[b] = 1;		
		bl[i[j]] = b;	
		U_or_V[b].b = 0; 	 	
		while (l < k && dif_eq (y[j], y[l]) && dif_eps (y[l], y[j], eps))
		{
			num[b] ++;	
			bl[i[l ++]] = b;
		}
		if (num[b] > max) 
		{
			c = b;
			max = num[b];			
		}	
		if (l == k) break;
		j = l;
		nbU ++;				
		b = nb ++; 					
	}	
	return c;
}

void sort_version ()
{
	int i, j, b, nc;
	while (nbU > 0)
	{		
		srand (time (NULL));
		while (1)
		{
  			b = rand () % nb;
			if (U_or_V[b].b == 0)
			{
				nbU --;	
				U_or_V[b].b = 1;
				break;
			}
		}  		
		for (i = 0; i < n; i ++)
		{
			for (j = 0; j < mkv_sz[i]; j ++)
			{
				if (bl[mkv_suc[i][j]] == b)
				{
					w[i] += mkv_w[i][j]; 
					if (w[i] > 0) 
					{
						spl[bl[i]].b = 1;
					}						
				}
			}
		}	
		nc = nb; 	
		for (b = 0; b < nc; b ++)	
		{	
			if (spl[b].b == 1 && U_or_V[b].b == 0)
			{	
				partition (b);					
				spl[b].b = 0;			
			}			
		}
		nc = nb; 	
		for (b = 0; b < nc; b ++)
		{
			if (spl[b].b == 1 && U_or_V[b].b == 1)
			{					
				U_or_V[partition(b)].b = 1; 	       
				spl[b].b = 0;											
			} 			
		}			
		for (i = 0; i < n; i ++)
		{	
			w[i] = 0;
		}			
	}   
}
