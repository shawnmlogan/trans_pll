#include "globals.h"

double max(double *px, long int N)
{
long int i;
double max_value = 0.0 - 1.0/EPSILON;

for(i=0;i < N;i++)
   {
   if(px[i] > max_value)
      max_value = px[i];
   }
return max_value;
}