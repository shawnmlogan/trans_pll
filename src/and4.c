
#include "globals.h"

int and4(int A, int B, int C, int D)
{
/*Computes AND function on 4  integers, returns int output*/
int out;

if ((A == 1) && (B == 1) && (C == 1) && (D  == 1))
   {
   out = 1;
   }
else
   out = 0;

return(out);
}