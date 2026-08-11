
#include "globals.h"

int dexor(double A, double B)
{

/*Computes exor function on two doubles, returns int output*/

int out, Aint, Bint;

if ((A < 0.0 - EPSILON) || (B < 0.0 - EPSILON))
   {
   printf("In dexor.c, input was less than zero...exiting.\n");
   printf("Input A = %2.4e, Input B = %2.4e.\n",A,B);
   exit(0);
   }

/*Convert the doubles to integers*/

A = A + 0.50;
B = B + 0.50;

Aint = (int) floor(A);
Bint = (int) floor(B);

/*Compute the exor function*/

if ((Aint + Bint) == 1)
   {
   out = 1;
   }
else
   out = 0;

return(out);
}
