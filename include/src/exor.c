
#include "globals.h"

int exor(int A, int B)
{
/*Computes exor function on two unsigned integers, returns unsigned output*/
int out;

if ((A + B) == 1)
   {
   out = 1;
   }
else
   out = 0;

return(out);
}