
#include "globals.h"

/*Function to compute output frequency of VCO - updated 5/26/99 to add tanh function
To use tanh function, set the first coefficient of polynomial to USE_TANH_FUNCTION with no other coefficients*/

double deltaf_vco_ratio(double vc, double ratio, double *pvco, int vco_polynomial_degree)

{
double deltaf = 0.0, power = 0.0, *ptemp_pointer;

ptemp_pointer = pvco;

if ((*pvco == USE_TANH_FUNCTION) && (vco_polynomial_degree == 1))
   {
   /*Case 1deltaf = 0.005*(vc-2.0) + 0.01 + 0.11*tanh(5.0*(vc-2.0));*/
   /*Case 2 -tanh2*/
   /*deltaf = 0.01 + 0.12*tanh(5.0*(vc-2.0));*/
   /*Case 3 -tanh3*/
   deltaf = 0.01 + 0.12*tanh(8.0*(vc-2.0));
   }
else
   {
   do 
      {
      if (*ptemp_pointer != 0.0)
         {
         deltaf = deltaf + (*ptemp_pointer) * pow(vc,power) * ratio;
         }
      power += 1.0;
      ptemp_pointer++;
      }     
   while (power < vco_polynomial_degree);
   }
return(deltaf);
}
