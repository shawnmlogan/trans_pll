
#include "globals.h"

void compute_single_pole_filter_coefficients(struct coeff *pw,double R1,double R2,double R3,double C1,double C2,double RS1,double RS2,int ph)
{
double alpha = 0.0;
/*This function computes the coefficients for a set of differential equations with the following two topologies*/
/*With ph set to 0, it assumes driving source is a current. Otherwise, it assumes driving source is a
voltage. */

/* 

        -------R1------------------
                                  |
                                 R2
                                  |
        -------RS2----------------|
                                 C1          
                                  |
                                 GND
 */
 
/*

(vs2 - vc1)/RS2 + (vc - vc1)/R2 + (0 - C1*(dvc1/dt)) = 0;	[2]

(vpd - vc)/R1 + (vc1 - vc)/R2 = 0;				[3]



Solve for vc from [3]:
---------------------

vpd/R1 + vc1/R2 - vc*(1/R1 + 1/R2) = 0;

vc = [vpd/R1 + vc1/R2]/[1/R1 + 1/R2];			[4]
 
Solve for dvc1/dt from [2] and [4]
----------------------------------

vs2/RS2 + vc/R2 - vc1*(1/RS2 + 1/R2) - C1*(dvc1/dt) = 0;

C1*(dvc1/dt) = vs2/RS2 - vc1*(1/RS2 + 1/R2) + (1/R2)*[vpd/R1 + vc1/R2]/[1/R1 + 1/R2];

C1*(dvc1/dt) = vs2/RS2 + vc1*((1/R2)*(1/R2)/[1/R1 + 1/R2] - 1/RS2 - 1/R2) + vpd*(1/R2)*(1/R1)/[1/R1 + 1/R2];

Define alpha:

alpha =[1/R1 + 1/R2];

C1*(dvc1/dt) = vs2/RS2 + vc1*((1/R2)*(1/R2)/alpha - 1/RS2 - 1/R2) + vpd*(1/R2)*(1/R1)/alpha;


*/

if (!is_chargepump(ph))
   {
   alpha = (1.0/R1 + 1.0/R2);
   if ((C1 == 0) || (alpha == 0))
      {
      printf("Divide by 0.0 in compute.c!. Exiting...\n");
      printf ("C1 = %e, alpha = %e.\n", C1,alpha);
      exit(0);
      }
    
   /*Coefficients for dvc1/dt*/
   
   pw->c1 = 0.0; /*Coefficient of vs1*/
   pw->c2 = 1.0/(C1*RS2); /*Coefficient of vs2*/   
   pw->c3 = 1.0/(C1*R1*R2*alpha); /*Coefficient of vpd*/
   pw->c4 = 1.0/(C1*R2*R2*alpha) - 1.0/(C1*RS2) - 1.0/(C1*R2); /*Coefficient of vc1*/
   pw->c5 = 0.0; /*Coefficient of vc2*/
   
   /*Coefficients for dvc2/dt*/
   
   pw->c6 = 0.0;/*Coefficient of vs1*/
   pw->c7 = 0.0; /*Coefficient of vs2*/
   pw->c8 = 0.0; /*Coefficient of vpd*/
   pw->c9 = 0.0; /*Coefficient of vc1*/
   pw->c10 = 0.0; /*Coefficient of vc2*/
   	
   /*Coefficients for computing vc in runge-kutta*/
 
   pw->c11 = 1.0/(R2*alpha); /*Coefficient of vc1*/
   pw->c12 = 0.0; /*Coefficient of vc2*/
   pw->c13 = 1.0/(R1*alpha); /*Coefficient of vpd*/
   
   }
else
   {

/*Charge pump - no VCO bandwidth*/
/* 

        ----------------------------------
                    |                    |
                    |                    |
                   R2                    R3
                    |                    |
                   C1                    C2
                    |                    |
                   GND                  GND
                   
 */
    printf ("Need new coefficients!...exiting\n");
    exit(0);
 
   /*pw->c1 = R3/(C1*(R2 + R3));
   pw->c2 = 0.0 - 1.0/((R2 + R3)*C1);
   pw->c3 = 1.0/((R2 + R3)*C1);
   pw->c4 = R2/(C2*(R2 + R3));
   pw->c5 = 1.0/((R2 + R3)*C2);
   pw->c6 = 0.0 - 1.0/((R2 + R3)*C2);
   pw->c7 = R2*C1;*/
   }

return;
}
