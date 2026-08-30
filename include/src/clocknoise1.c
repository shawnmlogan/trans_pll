
#include "globals.h"

double clocknoise1(double amplitude, double phase_val)
{
/*This routine computes a deterministic noise component - unique value for a given phase_val */
/*The noise is computed and returned in unit intervals. The range of noise is 2 * amplitude. The 
units of phase_val are in radians*/

double noise = 0.0;

/*noise = amplitude*(0.11*sin(3.0*phase_val/100.0 + pi/3)
      + 0.11*sin(5.55*phase_val/100.0) + 0.11*sin(0.1111*phase_val)
      + 0.11*sin(phase_val/190.0-pi/12) + 0.11*sin(phase_val/5550.55)
      + 0.11*sin(phase_val/70.77 + pi/10) + 0.11*sin(phase_val/3330.33)
      + 0.11 * sin(1.7*phase_val/100.0) + 0.56* sin(0.0111*phase_val + pi/3.0)
      + 0.056 * sin(phase_val/30.0-pi));*/
      noise = amplitude*(sin(phase_val/25.0 + pi/3));

return noise;
}
