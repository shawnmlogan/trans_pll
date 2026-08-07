
#include "globals.h"

int clockstate1(int CLKIN, double phase0, double phase,double duty_cycle)
{
static long j;
static double m;
int pos_edge = 0, neg_edge = 0, return_val = -1;

if (phase == phase0) /*Initialize all variables to initial state at start of simulation*/
   {
   j = 1;
   m = 1.0;
   }

if (phase/(2.0 * pi) >= j)
   {
   pos_edge = 1;
   j++;
   }
else 
   {
   if (phase/(2.0 * pi) >= (m + duty_cycle))
      {
      neg_edge = 1;
      m++;
      }
   }

if ((pos_edge == 0) && (neg_edge == 0))
   return_val = CLKIN;
else
   {
   if ((pos_edge == 1) && (neg_edge == 0))
     return_val = 1;
   else
      {
      if ((pos_edge == 0) && (neg_edge == 1))
         return_val = 0;
      else
         {        
         if ((pos_edge == 1) && (neg_edge == 1))
            {
             printf("Error in clock function, positive and negative edges at same time!\n");
             exit(0);
             }
          }
       }
    }
return return_val;
}
