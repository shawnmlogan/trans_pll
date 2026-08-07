#include "globals.h"

void print_string_to_log(char *pstring, Filedata *pdatain)
{
static int i;

FILE *fpw1;


printf("%s",pstring);
if (i == 0)
	fpw1 = fopen(pdatain->plog_filename,"w");
else
	fpw1 = fopen(pdatain->plog_filename,"a");

fprintf(fpw1,"%s",pstring);
fclose(fpw1);
i++;

}