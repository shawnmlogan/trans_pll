#define EXIT_CHARS
#include "globals.h"

Boolean check_for_quit_characters(char *pline)
{
Boolean flag_found_exitchar = FALSE;
char *ptempline;
int i = 0;

if ((ptempline = (char *) calloc(strlen(pline) + 1,sizeof(char))) == NULL)
	{
	printf("Error allocating memory for ptempfile in check_for_quit_characters()..exiting\n");
	exit(0);
	}

strcpy(ptempline,pline);
remove_whitespace(ptempline);

for(i=0;i<num_exitchars;i++)
	{
	if (ptempline[0] == exitchars[i])
		flag_found_exitchar = TRUE;
	}
	
free(ptempline);

return flag_found_exitchar;
}
