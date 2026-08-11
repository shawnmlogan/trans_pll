
#include "globals.h"

void compute(struct coeff *pw,double R1,double R2,double R3,double C1,double C2, 
double RS1, double RS2,int ph)
{
double denom = 0.0, denomC1 = 0.0, denomC2 = 0.0;
/*This function computes the coefficients for a set of differential equations with the following two topologies*/
/*With ph set to 0, it assumes driving source is a current. Otherwise, it assumes driving source is a
voltage. */

/* 
        -------RS1----------------
                                  |
                                  |
        -------R1---------R3-----------------
                                  |          |
                                  |          |
                                 R2         C2
        -------RS2----------------|          |
                                 C1          |
                                  |          |
                                 GND        GND
 */
 
 if (!is_chargepump(ph))
   {
   denom = RS1*(R1*R2 + R3*(R1 + R2)) + R1*R2*R3;
   denomC1 = C1 * denom;
   denomC2 = C2 * denom;
   if ((denomC1 == 0) || (denomC2 == 0.0) || (denom == 0.0))
      {
      printf("Divide by 0.0 in compute_110799!. Exiting...\n");
      printf ("C1 = %e, C2 = %e, denominator = %e.\n", C1,C2,denom);
      exit(0);
      }
       
   /*Coefficients for dvc1/dt*/
   
   pw->c1 = R1*R3/(denomC1); /*Coefficient of vs1*/
   pw->c2 = 1.0/(RS2*C1); /*Coefficient of vs2*/
   pw->c3 = R3*RS1/(denomC1); /*Coefficient of vpd*/
   pw->c4 = R3*R1*RS1/(R2*denomC1) - ((RS2 + R2)/(RS2*R2*C1)); /*Coefficient of vc1*/
   pw->c5 = (1.0/(R2*C1)) - R3*(R1*R2 + RS1*(R1+R2))/(R2*denomC1); /*Coefficient of vc2*/
   
   /*Coefficients for dvc2/dt*/
   
   pw->c6 = R1*R2/(denomC2);/*Coefficient of vs1*/
   pw->c7 = 0.0; /*Coefficient of vs2*/
   pw->c8 = RS1*R2/(denomC2); /*Coefficient of vpd*/
   pw->c9 = RS1*R1/(denomC2); /*Coefficient of vc1*/
   pw->c10 = 0.0 - (R1*R2 + RS1*(R1 + R2))/(denomC2); /*Coefficient of vc2*/
   
   /*Coefficient for computing vc in runge-kutta...adds to state variable vc2*/
   
   pw->c11 = R3*C2;
   
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
