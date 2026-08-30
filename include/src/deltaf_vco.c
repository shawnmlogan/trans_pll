
#include "globals.h"

/*Function to compute output frequency of VCO*/

double deltaf_vco(double vc, int vc_model, double *pvco, int vco_polynomial_degree)

{
double deltaf = 0.0, power = 0.0, *ptemp_pointer;
double c0 = 0.0, c1 = 0.0, c2 = 0.0, c3 = 0.0;

ptemp_pointer = pvco;

if (vc_model == USE_TANH_MODEL)
   {
   c0 = *ptemp_pointer++;
   c1 = *ptemp_pointer++;
   c2 = *ptemp_pointer++;
   c3 = *ptemp_pointer++;
 	deltaf = c0 + c1*tanh(c2*(vc - c3));
   /*deltaf = -0.28282 + 0.413863*tanh(7.903775*(vc-0.433964));*/
   }
else
   {
   if (vc_model == USE_POLYNOMIAL_MODEL)
   	{
	   do 
	      {
	      if (*ptemp_pointer != 0.0)
	         {
	         deltaf = deltaf + (*ptemp_pointer) * pow(vc,power);
	         }
	      power += 1.0;
	      ptemp_pointer++;
	      }     
	   while (power < vco_polynomial_degree);
	   }
	else
		deltaf = 0.0;
   }
return(deltaf);
}
