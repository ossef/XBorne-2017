#include "struct.h"
#include "io.h"
#include "tree_v.h"
#include "sort_v.h"
#include <unistd.h>

int usage (int argc, char * argv[]) 
{
	fprintf (stderr, "usage: %s model name num [epsilon]\n", argv[0]);
	fprintf (stderr, "\t INPUT : assumes that <<model.Rii>>, <<model.sz>> <<model.part>> exist\n");
	fprintf (stderr, "\t         name is a name given by the user to distinguish experiments\n");
	fprintf (stderr, "\t 	     num is the version of Tarjan's lumpability algorithm\n");
	fprintf (stderr, "\t         \t num = 1 : binary tree\n");
	fprintf (stderr, "\t         \t num = 2 : ascending order\n");
	fprintf (stderr, "\t         \t num = 3 : descending order\n");
	fprintf (stderr, "\t         epsilon refers to Tarjan's quasi-lumpability epsilon\n");
	fprintf (stderr, "\t OUTPUT : <<model.tarjan.name.part>> that contains the final partition\n");
	fprintf (stderr, "\t          <<model.tarjan.name.epsi>> that contains the value of epsilon (0 if no epsilon is given)\n");	
	exit (1);
}

void verify()
{
	int i, j, b, c;
	for (b = 0; b < nb; b ++)	
	{		
		for (c = 0; c < nb; c ++)	
		{			 
			printf ("%d:%d", b, c);	
			for (i = 0; i < n; i ++)	
			{
				w[i] = 0;
				for (j = 0; j < mkv_sz[i]; j ++)	
				{
					if (bl[i] == b && bl[mkv_suc[i][j]] == c)
					{
						w[i] += mkv_w[i][j];
					}					
				}
				if (bl[i] == b)
				{
					printf (" %d %.8lf ", i, w[i]);
				}
			}
			printf ("\n");	
		}		
	}
}

int main (int argc, char * argv[])
{
	int i;	
	if (argc < 4)
	{
		usage (argc, argv);
	}
	sprintf (model, "%s", argv[1]);
	sprintf (name, "%s", argv[2]);
	sprintf (part, "%s", argv[1]);
	if (getcwd (cwd, sizeof(cwd)) != NULL) 
	{
		read_model (); 
	}
	//create_partition ();  	
	read_partition (); 		
	version = atoi (argv[3]); 
	if (argc == 5) 
	{
		eps = atof (argv[4]);	
	} 
	U_or_V = malloc (n * sizeof(struct bits));
	w = malloc (n * sizeof(double));
	spl = malloc (n * sizeof(struct bits));
	for (i = 0; i < n; i ++)
	{
		U_or_V[i].b = 0;
		w[i] = 0;
		spl[i].b = 0;		
	} 
	if (version == 1) 
	{
		tree_version (argc);
	}
	else if (version == 2 || version == 3)
	{		
		num = malloc (n * sizeof(int)); 
		sort_version ();
	}	
	else
	{
		printf("Only 3 versions of the algorithm are allowed\n");
		exit (1);
	}
	write_partition (argc); 
	//verify();
	return 0;
}
