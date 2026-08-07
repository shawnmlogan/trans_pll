
#include "globals.h"

double clocknoise(double amplitude, double phase_val)
{
/*This routine computes a deterministic noise component - unique value for a given phase_val */
/*The noise is computed and returned in unit intervals. The range of noise is 2 * amplitude. The 
units of phase_val are in radians*/

double noise = 0.0;

noise = amplitude*(0.11*sin(3.0*phase_val/10.0 + pi/3)
      + 0.11*sin(5.55*phase_val/10.0) + 0.11*sin(0.1111*phase_val)
      + 0.11*sin(phase_val/19.0-pi/12) + 0.11*sin(phase_val/555.55)
      + 0.11*sin(phase_val/7.77 + pi/10) + 0.11*sin(phase_val/333.33)
      + 0.11 * sin(1.7*phase_val/10.0) + 0.056* sin(0.111*phase_val + pi)
      + 0.054 * sin(phase_val/3.0-pi));

return noise;
}
