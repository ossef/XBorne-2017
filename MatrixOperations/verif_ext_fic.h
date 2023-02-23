// Made by qst sept 2003

char *NOM;
char *EXT;
char *NOMBASE;
char *NOMsz;
char *NOMbk;
char *NOMcd;

struct extension
	{
	char RC;
	char ind1;
	char ind2;
	};
struct extension acceptable = {'U','u','u'};
struct extension argument;

int usage_extension(char *s)
{
fprintf(stderr,"Incorrect extension: %s\n",s);
fprintf(stderr,"          File extension must be of the form \".%c%c%c\"\n",
		acceptable.RC,acceptable.ind1,acceptable.ind2);
if (acceptable.RC=='U') fprintf(stderr,"          U means R, C or U\n");
if ((acceptable.ind1=='u') || (acceptable.ind2=='u'))
	fprintf(stderr,"          u means i, d or u\n");
return 0;
}

int verif_ext_fic (char *s, char *ext)
{
char *t;
if (strlen(s) < 4) usage_extension(s);
acceptable.RC = ext[0];
acceptable.ind1 = ext[1];
acceptable.ind2 = ext[2];
NOM = (char *)malloc(strlen(s)+1);
if (NOM == NULL) {perror("malloc NOM");exit(2);}
sprintf(NOM,"%s",s);
t = s+strlen(s)-4;
*t = '\0';
NOMBASE = s;
t++;
EXT = t;

argument.RC = *t;
if ((*t != 'R') && (*t != 'C') && (*t != 'U')) return usage_extension(NOM);
if ((acceptable.RC == 'R') && (*t != 'R')) return usage_extension(NOM);
if ((acceptable.RC == 'C') && (*t != 'C')) return usage_extension(NOM);
t++;
argument.ind1 = *t;
if ((*t != 'i') && (*t != 'd') && (*t != 'u')) return usage_extension(NOM);
if ((acceptable.ind1 == 'i') && (*t != 'i')) return usage_extension(NOM);
if ((acceptable.ind1 == 'd') && (*t != 'd')) return usage_extension(NOM);
t++;
argument.ind2 = *t;
if ((*t != 'i') && (*t != 'd') && (*t != 'u')) return usage_extension(NOM);
if ((acceptable.ind2 == 'i') && (*t != 'i')) return usage_extension(NOM);
if ((acceptable.ind2 == 'd') && (*t != 'd')) return usage_extension(NOM);

NOMsz = (char *)malloc(strlen(NOMBASE)+4);
if (NOMsz == NULL) {perror("malloc NOMsz");exit(2);}
sprintf(NOMsz,"%s.sz",s);

NOMbk = (char *)malloc(strlen(NOMBASE)+4);
if (NOMbk == NULL) {perror("malloc NOMbk");exit(2);}
sprintf(NOMbk,"%s.bk",s);

NOMcd = (char *)malloc(strlen(NOMBASE)+4);
if (NOMcd == NULL) {perror("malloc NOMcd");exit(2);}
sprintf(NOMcd,"%s.cd",s);
return 1;
}
