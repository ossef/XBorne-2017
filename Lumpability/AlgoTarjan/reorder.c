#include "struct.h"
#include "io.h"
#include <unistd.h>

int usage (int argc, char *argv[]) 
{
	fprintf (stderr, "usage: %s model method name\n", argv[0]);
	fprintf (stderr, "\t INPUT : assumes that <<model.Rii>>, <<model.sz>>, <<model.method.name.part>> exist\n");
	fprintf (stderr, "\t         method is the name of the method used to get the partition e.g. tarjan\n");
	fprintf (stderr, "\t         name is a name given by the user to distinguish experiments\n");
	fprintf (stderr,"\t OUTPUT : the initial model reordered <<model.reorder.name.Rii>>\n");
	fprintf (stderr,"\t          <<model.reorder.name.sz>>\n");
	fprintf (stderr,"\t          the partition file reordered <<model.reorder.name.bk>>\n");
 	fprintf (stderr,"\t          the permutation corresponding to this reordering <<model.reorder.name.perm>>\n");
	exit (1);
}

int main(int argc, char * argv[])
{			
	if (argc < 4)
	{
		usage (argc, argv);
	}
	sprintf(model, "%s", argv[1]);
	sprintf(method, "%s", argv[2]);
	sprintf(name, "%s", argv[3]);

	if (getcwd (cwd, sizeof(cwd)) != NULL) 
	{
		read_model (); 
	}	  	
	
	sprintf (part, "%s.%s.%s", model, method, name);
	read_partition (); 
	reorder_partition (); 
	reorder_model (1);	
	write_permutation ();

	return 0;
}
