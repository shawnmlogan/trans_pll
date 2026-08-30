
#include "globals.h"

void dff(int D,int clock, int old_clock, int reset, int *Q, int *QB)
{

/*Computes positive edge dff. Asynchronous reset with reset is 1. */

if ((old_clock == 0) && (clock == 1) && (reset != 1)) /*Detects positive edge of clock*/
   {
   *Q = D;
   *QB = inv(D);
   }

if (reset == 1)
   {
   *Q = 0;
   *QB = 1;
   }
return;
