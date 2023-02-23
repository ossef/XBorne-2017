#include "struct.h"

btree * cr_btr (int i)
{
	btree * b = malloc (sizeof(btree));
	b->i = i;	
	b->num = 1;
	b->l = b->r = NULL;
	b->min = b->max = w[i]; 
	U_or_V[bl[i]].b = 0;	
	return b;
}

btree * ins_btr (btree * b, int i)
{
	if (b == NULL) 
	{
		b = cr_btr (i); 
	} 
	else if (inf (w[i], w[b->i]))
	{	
		if (b->l == NULL)
		{
			nbU ++;	
			bl[i] = nb ++;
			b->l = cr_btr (i);							
		}
		else 	
		{
			b->l = ins_btr (b->l, i);
		}			
	} 
	else if (inf (w[b->i], w[i]))
	{	
		if (b->r == NULL)
		{
			nbU ++;	
			bl[i] = nb ++;
			b->r = cr_btr (i);							
		}			
		else 
		{
			b->r = ins_btr (b->r, i);	
		}	
	}
	else if (eq (w[i], w[b->i]))	 
	{		
		b->num ++;	
		bl[i] = bl[b->i];		
	}
	return b;
}

btree * ins_btr_eps (btree * b, int i)
{
	if (b == NULL) 
	{		 
		b = cr_btr (i); 		
	}
	else if (inf_eq (b->max, w[i] + eps) && inf (w[i], b->min + eps))
	{			
		b->num ++; 
		bl[i] = bl[b->i]; 
		if (inf (w[i], b->min)) 
		{
			b->min = w[i];	
		}			
		if (inf (b->max, w[i])) 
		{
			b->max = w[i];		
		}
	}
	else if (inf (w[i] + eps, b->max))
	{	 
		if (b->l == NULL)
		{
			nbU ++;	
			bl[i] = nb ++;
			b->l = cr_btr (i);										
		}
		else 	
		{
			b->l = ins_btr_eps (b->l, i); 
		}			
	}
	else if (inf_eq (b->min + eps, w[i]))
	{	
		if (b->r == NULL)
		{
			nbU ++;	
			bl[i] = nb ++;
			b->r = cr_btr (i); 						
		}			
		else 
		{
			b->r = ins_btr_eps (b->r, i); 
		}	
	}	
	return b;
}

void mod_btr (btree * b, int * max, int * i)
{
	if (b != NULL)
	{
		if (b->num > * max) 
		{
			* i  = b->i;	
			* max  = b->num;			
		}	
		mod_btr (b->l, max, i);
		mod_btr (b->r, max, i);
	}	
}

void dealloc (btree * b)
{
	if (b != NULL)
	{
		dealloc (b->l);
		dealloc (b->r);		
		free (b); 
	}
}

void tree_version (int argc)
{
	int i, j, b, nc;
	btree * btr;
	while (nbU > 0)
	{	
		srand (time(NULL)); 
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
					if (eq (w[i], 0) ? 0 : w[i] > 0) 
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
				btr = NULL; 
				for (i = 0; i < n; i ++)
				{
					if (bl[i] == b)
					{
						btr = (argc == 4) ? ins_btr (btr, i) : ins_btr_eps (btr, i);	
					}	
				}
				dealloc (btr);
				spl[b].b = 0;					
			}			
		}
		nc = nb; 
		for (b = 0; b < nc; b ++)
		{
			if (spl[b].b == 1 && U_or_V[b].b == 1)
			{	
				btr = NULL; 				
				for (i = 0; i < n; i ++)
				{							
					if (bl[i] == b)
					{
						btr = (argc == 4) ? ins_btr (btr, i) : ins_btr_eps (btr, i);	
					}
				}		
				if (btr != NULL)			
				{	
					int max = 0, k = -1; 
					mod_btr (btr, & max, & k); 
					U_or_V[bl[k]].b = 1; 									        
				}	
				dealloc (btr); 
				spl[b].b = 0;											
			} 			
		}		
		for (i = 0; i < n; i ++)
		{	
			w[i] = 0;
		}	
	} 
}
