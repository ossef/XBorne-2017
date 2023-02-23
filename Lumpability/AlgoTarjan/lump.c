#include "struct.h"
#include "io.h"
#include <unistd.h>

int usage (int argc, char *argv[]) 
{
	fprintf (stderr, "usage: %s model method name\n", argv[0]);
	fprintf (stderr, "\t INPUT : assumes that <<model.Rii>>, <<model.sz>>, <<model.method.name.part>>, <<model.method.name.epsi>> exist\n");
	fprintf (stderr, "\t         method is the name of the method used to get the partition e.g. tarjan\n");
	fprintf (stderr, "\t         name is a name given by the user to distinguish experiments\n");
	fprintf (stderr,"\t OUTPUT : if epsilon in file <<model.method.name.epsi>> is zero, the output is\n");
 	fprintf (stderr,"\t 		<<model.method.name.lump.Rii>>, <<model.method.name.lump.sz>>\n");
	fprintf (stderr,"\t          if epsilon is non zero, the output is\n");
	fprintf (stderr,"\t             the lower (upper) matrix <<<model.method.name.lump.EL.Rii>> (<<model.method.name.lump.EU.Rii>>)\n");
	fprintf (stderr,"\t             the size matrix <<model.method.name.lump.EL.sz>> (<<model.method.name.lump.EU.sz>>)\n");
	fprintf (stderr,"\t             the delta between upper and lower matrix <<model.method.name.lump.EU-L.Rii>>\n");
 	fprintf (stderr,"\t             the size matrix <<model.method.name.lump.EU-L.sz>>\n");
	exit (1);
}

int main (int argc, char * argv[])
{		
	int ret;
	double e;
	char ep[250];
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

	//read epsilon	
	sprintf (ep, "%s/%s.%s.%s.epsi", cwd, model, method, name); 
	FILE * f = fopen (ep, "r");
	ret = fscanf (f, "%lf", & e); 
	if (ret != 1) exit(0);

	write_aggregation (e);
	return 0;
}
