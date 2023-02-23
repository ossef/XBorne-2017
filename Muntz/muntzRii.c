#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int i, j, k, n, m, z;
FILE *f, *f1, *f2, * g;
char p_in[250], u_out[100], cwd[100]; 
double a[250][250], p2[250][250], aux[250][250], u[250], p[250], s, o, l;
double min_p[250], max_p[250];	 

FILE * open_file (char * name, char * mode)
{
	FILE * f = fopen (name, mode);
	if (f == NULL)
	{
		printf ("Can't open file %s\n", name);
		exit (1);
	}
	return f;
}

int scan_int (FILE * f)
{
	int i, ret;
	ret = fscanf (f, "%d", & i);
	if (ret != 1) 
	{
		exit (1);
	}
	return i;
}

double scan_double (FILE * f)
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

void init_matrix ()
{
	for (i = 0; i <= m; i ++)
	{			
		for (j = 0; j <= m; j ++)
		{
			aux[i][j] = p2[i][j];
		}
	}
}

void restore_column (int j)
{
	for (k = 0; k <= m; k ++)
	{	
		aux[k][j] = p2[k][j];		
	}
}

void set_min_p ()
{
	for (k = 0; k <= m; k ++)
	{	
		min_p[k] = 2; 		
	}
}

void set_max_p ()
{
	for (k = 0; k <= m; k ++)
	{	
		max_p[k] = -1; 		
	}
}

void read_chain ()
{
	int n1, n2;
	double p;
	for (i = 0; i <= m; i ++)
	{	
		fscanf(f2,"%d %d ",&n1,&n2);
				
		for (j = 0; j < n2; j ++)
		{
			fscanf(f2,"%lf %d",&p,&n1);
			p2[i][n1] = p;
		}
	}
	
}

void verify()
{
	o = 0;
	for (i = 0; i <= m; i ++)
	{					
		l = 0; 
		for (j = 0; j <= m; j ++)
		{
			l += (p[j] * aux[j][i]);
		}
		printf ("%.4lf ~ %.4lf\n", p[i], l);
		o += p[i];
	}
	printf("sum = %lf\n", o);
}

void gth ()
{
	for (i = 0; i <= m; i ++)
	{			
		for (j = 0; j <= m; j ++)
		{
			a[i][j] = aux[j][i] - ((i == j) ? 1 : 0);
		}
	}				
	for (n = 0; n < m; n ++)
	{
		s = 0;
		for (k = n+1; k <= m; k ++)
		{
			s += a[k][n];
		}
		for (j = n+1; j <= m; j ++)
		{	
			a[n][j] = a[n][j] / s;								
			for (i = n+1; i <= m; i ++)
			{
				if (i != j)
				{
					a[i][j] += (a[i][n] * a[n][j]);
				}
			}
		}
	}
	u[m] = 1;
	for (n = m-1; n >= 0; n --)
	{
		u[n] = 0;
		for (j = n+1; j <= m; j ++)
		{
			u[n] += (a[n][j] * u[j]);
		}			
	}	
	s = 0;
	for (j = 0; j <= m; j ++)
	{
		s += u[j];
	} 	
	for (i = 0; i <= m; i ++)
	{		
		p[i] = u[i] / s;
		fprintf (g, "%.4lf\t", p[i]); 
	}
	fprintf(g, "\n");
}

int main (int argc, char * argv[])
{	
	char s1[250],s2[250];
	int m1;
	
	if (argc != 4) 
	{
		printf("\nUsage : %s -f <FileName> <out> \n",argv[0]);
		printf("FileName.Rii and FileName.sz must exist before. \n");
		printf("out : nom du fichier en sortie \n");
		exit(1);
	}
	
	strcpy(p_in, argv[2]);
	strcpy(s1, p_in);
    strcpy(s2, p_in);
    
    strcat(s1,".sz"); //fichier d'entrée
    strcat(s2,".Rii"); //fichier d'entréee
    
	f1 = open_file (s1, "r");
	f2 = open_file (s2, "r");
	
	fscanf(f1,"%d %d ",&m1,&m);
	m--;
	read_chain();
	init_matrix ();
	set_min_p ();
	set_max_p ();
	g = open_file (argv[3], "w");
	for (z = 0; z <= m; z ++)
	{
		//if (z != 1)
		//{
		for (i = 0; i <= m; i ++)
		{
			s = 0;		
			for (j = 0; j <= m; j ++)
			{
				if (j != z)
				{
					s += p2[i][j];
				}
			}
			aux[i][z] = 1 - s;		
		}

		for (i = 0; i <= m; i ++)
		{
			for (j = 0; j <= m; j ++)
			{
				printf ("%.2lf ", aux[i][j]);
			}
			printf("\n");
		}

		gth (); printf("\n"); //verify ();
		for (i = 0; i <= m; i ++)
		{
			if (min_p[i] > p[i]) min_p[i] = p[i];
			if (max_p[i] < p[i]) max_p[i] = p[i];
		}
		restore_column (z);	
		//}	
	}
	printf("min = ");
	for (i = 0; i <= m; i ++)
	{		
		printf ("%.4lf\t", min_p[i]); 
	}
	printf("\nmax = ");
	for (i = 0; i <= m; i ++)
	{		
		printf ("%.4lf\t", max_p[i]); 
	}
	printf("\n");
	
	return 0;
}	

