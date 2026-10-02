
#include "globals.h"

void compute(struct coeff *pw,double R1,double R2,double R3,double C1,double C2, 
double RS1, double RS2,int ph)
{

if (select_second_order_filter_coefficents(R2,R3,C1,C2) == TRUE)
	compute_two_pole_filter_coefficients(pw,R1,R2,R3,C1,C2,RS1,RS2,ph);
else
	compute_single_pole_filter_coefficients(pw,R1,R2,R3,C1,C2,RS1,RS2,ph);

return;

}
