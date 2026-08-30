#include "globals.h"

void adjuststring(char *pstring, int LENGTH)
{
int i=0, j=0;
char *ptempstring, tempstring[LINELENGTH+1];
char *pnewstring, newstring[LINELENGTH+1];

/*Initialize pointers*/

ptempstring = &tempstring[0];
pnewstring = &newstring[0];

strcpy(ptempstring,pstring);

for (i=0; (tempstring[i] != '\0') && (i < LENGTH); i++)
   newstring[i] = tempstring[i];
   
if (i < LENGTH)
   {
   for (j = i; j < LENGTH;j++)
      newstring[j] = ' ';
   }

newstring[LENGTH] = '\0';

strcpy(pstring,pnewstring);

return;
}


