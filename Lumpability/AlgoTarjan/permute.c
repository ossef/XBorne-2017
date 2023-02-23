#include "struct.h"
#include "io.h"
#include <unistd.h>

int usage (int argc, char *argv[]) 
{
	fprintf (stderr, "usage: %s model name\n", argv[0]);
	fprintf (stderr, "\t INPUT : assumes that <<model.Rii>>, <<model.sz>> and <<model.name.perm>> exist\n");
	fprintf (stderr, "\t         name is a name given by the user to distinguish experiments\n");
	fprintf (stderr,"\t OUTPUT : <<model.name.Rii>>, <<model.name.sz>>\n");
	fprintf (stderr,"\t          if <<model.cd>> exists, the output includes <<model.name.cd>>\n");
	exit (1);
}

int main(int argc, char * argv[])
{		
	if (argc < 3)
	{
		usage (argc, argv);
	}
	sprintf(model, "%s", argv[1]);
	sprintf(name, "%s", argv[2]);	
	sprintf (part, "%s.%s", model, name);

	if (getcwd (cwd, sizeof(cwd)) != NULL) 
	{
		 read_model (); 
	}	  	
	
	read_permutation (); 
	reorder_model (2); 
	reorder_cd ();

	return 0;
}
