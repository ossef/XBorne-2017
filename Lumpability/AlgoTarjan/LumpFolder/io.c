#include "struct.h"
#include <string.h>
#include <math.h>

inline FILE * open_file (char * name, char * mode)
{
	FILE * f = fopen (name, mode);
	if (f == NULL)
	{
		printf ("Can't open file %s\n", name);
		exit (1);
	}
	return f;
}

inline int scan_int (FILE * f)
{
	int i, ret;
	ret = fscanf (f, "%d", & i);
	if (ret != 1) 
	{
		exit (1);
	}
	return i;
}

inline double scan_double (FILE * f)
{
	int ret;
	double i;
	ret = fscanf (f, "%lf", & i);
	if (ret != 1) 
	{
		exit (1);
	}
	return i;
}

void create_partition (void)
{
	int i;
	char prt[250]; 
	FILE * f;
   	sprintf (prt, "%s/%s.part", cwd, model); 
	f = open_file (prt, "w"); 	
	fprintf (f,"2\n0 1 0\n1 %d\t", n-1); 				//create sophisticated initial partition
	for (i = 1; i < n; i ++)
	{
		fprintf (f, "%d ", i);
	}
	fprintf (f, "\n");
	fclose (f);
}

void read_model (void)
{
	int i, j;
	char sz[250], Rii[250];
	FILE * f, * g;
	sprintf (Rii, "%s/%s.Rii", cwd, model); 
	sprintf (sz, "%s/%s.sz", cwd, model);
	f = open_file (Rii, "r");
	g = open_file (sz, "r");			
	i = scan_int (g);	
	n = scan_int (g);						//read the size of the model (in the second line)
	fclose(g); 		
	mkv_w = malloc (n * sizeof (double *));				//read the Markov Chain	
	mkv_suc = malloc (n * sizeof (int *));
	mkv_sz = malloc (n * sizeof (int));				
	for (i = 0; i < n; i ++)
	{				
		j = scan_int (f); 					//read the number of i's successors
		if (i != j) 
		{
			printf ("The elements in file %s are not in the right order\n", Rii);
			exit (1);
		}
		mkv_sz[i] = scan_int (f); 		
		mkv_w[i] = malloc (mkv_sz[i] * sizeof (double));	//read i's successors	 
		mkv_suc[i] = malloc (mkv_sz[i] * sizeof (int));
		for (j = 0; j < mkv_sz[i]; j ++)
		{
			mkv_w[i][j] = scan_double (f); 
			mkv_suc[i][j] = scan_int (f); 
		} 
	}
	fclose(f); 
}

void read_partition ()
{
	int b, c, i, j, m;
	char prt[250];
	FILE * f;
	sprintf (prt, "%s/%s.part", cwd, part); 
	f = open_file (prt, "r"); 
	nb = scan_int (f); 	
	nbU = nb;
	bl = malloc (n * sizeof(int)); 
	for (b = 0; b < nb; b ++)
	{	
		c = scan_int (f); 
		if (b != c) 
		{
			printf ("The blocks in file %s are not in the right order\n", prt);
			exit (1);
		}
		m = scan_int (f); 
		for (i = 0; i < m; i ++)
		{
			j = scan_int (f); 
			bl[j] = b;
		} 
	}
	fclose (f);
}

void write_partition (int argc)
{
	int i, b, num;
	char prt[250], epsi[250];
	FILE * f, * g;
	sprintf (prt, "%s/%s.tarjan.%s.part", cwd, model, name); 
	sprintf (epsi, "%s/%s.tarjan.%s.epsi", cwd, model, name); 
	f = open_file (prt, "w");
	g = open_file (epsi, "w");	
	fprintf (f, "%d\n", nb);
	for (b = 0; b < nb; b ++)
	{	
		fprintf (f, "%d ", b);
		num = 0;
		for (i = 0; i < n; i ++)
		{
			if (bl[i] == b && U_or_V[b].b == 1) 
			{				
				num ++;				
			}
		}
		fprintf (f, "%d\t", num);
		for (i = 0; i < n; i ++)
		{
			if (bl[i] == b && U_or_V[b].b == 1) 
			{				
				fprintf(f, "%d ", i);
			}
		}	
		fprintf (f, "\n");						
	}
	fprintf (g, "%lf\n", (argc == 5) ? eps : 0);
	fclose (f); fclose (g);	
}

int member_in_bl (int b)
{
	int i;
	for (i = 0; i < n; i ++)	
	{
		if (bl[i] == b)
		{
			return i;
		}
	}
	printf ("No element found in block %d\n", b);
	exit (1);
}

int compute_weight(int i, int b, int c)
{
	int j;
	int connected = 0;
	agr_w[b][agr_sz[b]] = 0;			
	for (j = 0; j < mkv_sz[i]; j ++)	
	{
		if (bl[mkv_suc[i][j]] == c)
		{
			agr_w[b][agr_sz[b]] += mkv_w[i][j];	
			agr_suc[b][agr_sz[b]] = c;
			connected = 1;
		}		
	}
	return connected;
}

void write_aggregation (double e)
{
	int i, b, c, connected, size_e, agr_trans;
	char lump[250], eu[250], el[250], dif[250], sz[250], sz_eu[250], sz_el[250], sz_dif[250];
	FILE * f, * g, * h, * l, * m, * k;
	agr_w = malloc (nb * sizeof(double *));
	agr_w_min = malloc (nb * sizeof(double *));
	agr_w_max = malloc (nb * sizeof(double *));
	agr_suc = malloc (nb * sizeof(int *));
	agr_sz = malloc (nb * sizeof(int));	
	for (b = 0; b < nb; b ++)
	{			
		agr_w[b] = malloc (nb * sizeof(double));
		agr_w_min[b] = malloc (nb * sizeof(double));
		agr_w_max[b] = malloc (nb * sizeof(double));
		agr_suc[b] = malloc (nb * sizeof(int));
		agr_sz[b] = 0;			
	}	
	for (b = 0; b < nb; b ++)	
	{		
		for (c = 0; c < nb; c ++)	
		{
			if (eq(e, 0))
			{
				i = member_in_bl (b);
				if (compute_weight(i, b, c) == 1) 
				{
					agr_sz[b] ++;
				}	
			}
			else
			{		
				connected = 0; 
				i = member_in_bl (b);		
				if (compute_weight(i, b, c) == 1)
				{
					agr_w_min[b][agr_sz[b]] = agr_w[b][agr_sz[b]];
					agr_w_max[b][agr_sz[b]] = agr_w[b][agr_sz[b]];
					connected = 1;
				}
				for (i = i+1; i < n; i ++)	
				{					
					if (bl[i] == b && compute_weight (i, b, c) == 1)
					{
						if (inf (agr_w[b][agr_sz[b]], agr_w_min[b][agr_sz[b]])) 
						{
							agr_w_min[b][agr_sz[b]] = agr_w[b][agr_sz[b]];
						}
						if (inf (agr_w_max[b][agr_sz[b]], agr_w[b][agr_sz[b]])) 
						{
							agr_w_max[b][agr_sz[b]] = agr_w[b][agr_sz[b]];	
						}						
					}						
				}
				if (connected == 1) 
				{
					agr_sz[b] ++;
				}
			}			
		}						
	}		
	agr_trans = 0;
	for (b = 0; b < nb; b ++)	
	{
		agr_trans += agr_sz[b];		
	}	
	if (eq(e, 0))
	{	
		sprintf (lump, "%s/%s.%s.%s.lump.Rii", cwd, model, method, name); 
		f = open_file (lump, "w");
		fprintf(f, "%d\n", nb);
		for (b = 0; b < nb; b ++)	
		{
			fprintf(f, "%d %d", b, agr_sz[b]);
			for (c = 0; c < agr_sz[b]; c ++)
			{
				fprintf (f, "\t%lf %d", agr_w[b][c], agr_suc[b][c]);			
			}
			fprintf(f, "\n");
		}		
		sprintf (sz, "%s/%s.%s.%s.lump.sz", cwd, model, method, name); 
		h = open_file (sz, "w");		
		fprintf(h, "%d\n", agr_trans);
		fprintf(h, "%d\n", nb);
		fprintf(h, "%d\n", (int) ceil (log2 (nb)));
		fclose(f); fclose(h);
	}
	else
	{	
		sprintf (eu, "%s/%s.%s.%s.lump.EU.Rii", cwd, model, method, name); 
		sprintf (sz_eu, "%s/%s.%s.%s.lump.EU.sz", cwd, model, method, name); 
		f = open_file (eu, "w");				
		h = open_file (sz_eu, "w");		
		fprintf(h, "%d\n", agr_trans);
		fprintf(h, "%d\n", nb);
		fprintf(h, "%d\n", (int) ceil (log2 (nb)));				
		sprintf (el, "%s/%s.%s.%s.lump.EL.Rii", cwd, model, method, name); 
		sprintf (sz_el, "%s/%s.%s.%s.lump.EL.sz", cwd, model, method, name); 
		g = open_file (el, "w");		
		m = open_file (sz_el, "w");		
		fprintf(m, "%d\n", agr_trans);
		fprintf(m, "%d\n", nb);
		fprintf(m, "%d\n", (int) ceil (log2 (nb)));				
		sprintf (dif, "%s/%s.%s.%s.lump.EU-L.Rii", cwd, model, method, name); 
		sprintf (sz_dif, "%s/%s.%s.%s.lump.EU-L.sz", cwd, model, method, name); 
		l = open_file (dif, "w");
		k = open_file (sz_dif, "w");	
		fprintf(f, "%d\n", nb);
		fprintf(g, "%d\n", nb);
		fprintf(l, "%d\n", nb);
		for (b = 0; b < nb; b ++)	
		{
			size_e = 0;
			fprintf(f, "%d %d", b, agr_sz[b]);
			fprintf(g, "%d %d", b, agr_sz[b]);
			for (c = 0; c < agr_sz[b]; c ++)
			{
				fprintf (f, "\t%.8lf %d", agr_w_max[b][c], agr_suc[b][c]);
				fprintf (g, "\t%.8lf %d", agr_w_min[b][c], agr_suc[b][c]);
				if (! eq (agr_w_max[b][c] - agr_w_min[b][c], 0.0)) 
				{
					size_e ++;					
				}
				else
				{
					agr_trans --;
				}
			}	
			fprintf(l, "%d %d", b, size_e);
			for (c = 0; c < agr_sz[b]; c ++)
			{	
				if (! eq (agr_w_max[b][c] - agr_w_min[b][c], 0.0)) 
				{					
					fprintf (l, "\t%.8lf %d", agr_w_max[b][c] - agr_w_min[b][c], agr_suc[b][c]);			
				}
			}
			fprintf(f, "\n"); fprintf(g, "\n"); fprintf(l, "\n");
		}
		fprintf(k, "%d\n", agr_trans);
		fprintf(k, "%d\n", nb);
		fprintf(k, "%d\n", (int) ceil (log2 (nb)));		
		fclose(h); fclose(m); fclose(f); fclose(g); fclose(l); fclose(k);
	}
}

void reorder_partition (void)
{	
	int b, c, i, j, m, l;
	char prt[250], bk[250];
	FILE * f, * g;
	sprintf (prt, "%s/%s.%s.%s.part", cwd, model, method, name); 
	sprintf (bk, "%s/%s.reorder.%s.bk", cwd, model, name); 
	f = open_file (prt, "r");	
	g = open_file (bk, "w"); 
	nb = scan_int (f); 	
	fprintf (g, "%d\n", nb);	
	bl = malloc (n * sizeof(int)); 
	rdr = malloc (n * sizeof(int)); 
	inv = malloc (n * sizeof(int));  
	l = 0;	
	nbU = nb;
	for (b = 0; b < nb; b ++)
	{	
		c = scan_int (f); 
		if (b != c) 
		{
			printf ("The blocks in file %s are not in the right order\n", prt);
			exit (1);
		}
		fprintf (g, "%d ", c);		
		m = scan_int (f); 		
		fprintf (g, "%d\t", m);
		for (i = 0; i < m; i ++)
		{
			j = scan_int (f); 
			if (i == 0) fprintf (g, "%d ", l);
			inv[l] = j;
			rdr[j] = l ++;
		} 
		fprintf (g, "\n");
	}
	fclose (f); fclose (g);
}

void bsort_int (int * x, double * y, int j)
{
	int f, k, t;
	double d;
        do
        {
        	f = 0;  
        	for (k = 0; k < j-1; k++)
        	{
			if (x[k+1] < x[k])          
            		{
				f = 1; 
				d = y[k]; y[k] = y[k+1]; y[k+1] = d;
				t = x[k]; x[k] = x[k+1]; x[k+1] = t;				                 		                
           		 }
        	}
    	}
        while (f == 1);      
}

void reorder_model (int op)
{
	int i, j, k;
	char rii[250], sz[250], rdr_sz[250]; 
	int * x;
	double * y;
	FILE * f, * g, * h;
	sprintf (rii, "%s/%s.%s%s.Rii", cwd, model, (op == 1) ? "reorder." : "", name); 
	g = open_file (rii, "w");	 
	for (i = 0; i < n; i ++)
	{
		fprintf (g, "%d %d\t", i, mkv_sz[inv[i]]);
		x = malloc (mkv_sz[inv[i]] * sizeof(int));
		y = malloc (mkv_sz[inv[i]] * sizeof(double));
		for (j = 0; j < mkv_sz[inv[i]]; j ++)
		{
			 x[j] = rdr[mkv_suc[inv[i]][j]];
			 y[j] = mkv_w[inv[i]][j];	
		}
		bsort_int (x, y, mkv_sz[inv[i]]);
		for (j = 0; j < mkv_sz[inv[i]]; j ++)
		{
			 fprintf (g, "%f %d   ", y[j], x[j]);	
		}
		fprintf (g, "\n");
		free(x); free (y);
	}
	sprintf (sz, "%s/%s.sz", cwd, model);
	sprintf (rdr_sz, "%s/%s.%s%s.sz", cwd, model, (op == 1) ? "reorder." : "", name); 
	f = open_file (sz, "r");
	h = open_file (rdr_sz, "w");
	i = scan_int (f);
	fprintf (h, "%d\n", i);
	j = scan_int (f);
	fprintf (h, "%d\n", j);
	k = scan_int (f);
	fprintf (h, "%d\n", k);
	fclose(f); fclose (g); fclose(h); 
}

void read_permutation (void)
{
	char p[250];
	int i, j;
	FILE * f;	
	sprintf (p, "%s/%s.%s.perm", cwd, model, name); 
	f = open_file (p, "r");	
	rdr = malloc (n * sizeof (int));
	inv = malloc (n * sizeof (int));
	for (i = 0; i < n; i ++)
	{
		i = scan_int (f); 		
		j = scan_int (f); 		
		rdr[i] = j;
		inv[j] = i;		
	}
	fclose(f);
}

void write_permutation (void)
{
	int i;
	char p[250];
	FILE * f;
	sprintf (p, "%s/%s.reorder.%s.perm", cwd, model, name); 
	f = open_file (p, "w");		
	for (i = 0; i < n; i ++)
	{
		fprintf (f, "%d %d\n", i, rdr[i]);
	}
	fclose (f);
}

void reorder_cd (void)
{
	int i, j, k;
	char cd[250], pe[250];
	FILE * f, * g;
	sprintf (cd, "%s/%s.cd", cwd, model); 
	sprintf (pe, "%s/%s.%s.cd", cwd, model, name);
	f = open_file (cd, "r");	 
	g = open_file (pe, "w"); 	
	bit = malloc (n * sizeof (struct bits *)); 		
	for (i = 0; i < n; i ++)
	{
		bit[i] = malloc ((int) ceil (log2 (n)) * sizeof (struct bits)); 
		k = scan_int (f); 
		if (k != i)
		{
			printf ("The elements in file %s are in the wrong order\n", cd);
			exit (1);
		} 				
		for (j = 0; j < (int) ceil (log2 (n)); j++)
		{
			k = scan_int (f); 		
			bit[i][j].b = k;
		}
	}		
	for (i = 0; i < n; i ++)
	{
		fprintf (g, "%d\t", i); 
		for (j = 0; j < (int) ceil (log2 (n)); j++)
		{
			fprintf(g, "%d\t", bit[inv[i]][j].b);
		}
		fprintf (g, "\n"); 
	}
	fclose (f); fclose (g);
}
