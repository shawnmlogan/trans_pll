
#include "globals.h"

int inv(int A)
{
/*Computes the logical inverse of input integer, returns int output*/
int out;

if (A == 1)
   out = 0;
else
   {
   if (A == 0)
      out = 1;
   else
      {
      printf("Error in logic inverse function inv.c!\n");
      printf("Input value \"%d\" was not a 1 or a 0..exiting\n",A);
      exit(0);
      }
   }

return(out);
}