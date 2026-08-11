
#include "globals.h"

int vsample(int *pvdata, Filedata *pdatain, double vdata_phase0, double phasein, int *perror, int N, int *pdeltaphase, double *pphase_val)
{

/* This function takes the pointer to an N element vector whose first
element is occurs at phase of (w0*time). Each increment of 2pi in
phase corresponds to the next element in the vector. The function
returns the value corresponding to the input value of phase 
"samplephase". If the input "samplephase" requires a value which
exceeds the available data in the vector of length N, it sets
error to -1 indicating that the phase value corresponds to
a value prior to the value in the current vector. 
If the value of "samplephase" exceeds w*time + k0*2*PI, it
sets the value of error to +1 indicating the sample is
outside the positive range of the input vector set.
Otherwise, it sets error to 0 indicating the sample is within the 
range of the vector set.*/

double phase_val;
int return_val = 0, deltaphase = 0, CLKIN = 0;
static int  LAST_CLKIN;
int flag_hi = 0, neg_transition = 0, pos_transition = 0;
unsigned int s;


phase_val = (phasein - vdata_phase0 - pdatain->phase0) / (2.0 * pi);
phase_val = (phasein - vdata_phase0) / (2.0 * pi);

*pphase_val = phase_val;
deltaphase = (int) floor(phase_val + 0.5);
*pdeltaphase = deltaphase;


if (deltaphase < 0)
   {
   printf("At sampled phase %2.6e, the desired sampling point ",phasein);
   printf("was less than the\n");
   printf("available data vector, program exits! deltaphase = %d \n",deltaphase);
   printf("deltaphase = %2.6e (normalized to 2pi radians).\n",phase_val);
   *perror = -1;
   }
else
   {
   if (deltaphase > N - 1)
      {
      printf("At sampled phase %2.6e, the desired sampling point was",phasein);
      printf(" greater than the\n");
      printf("available data vector, program exits! deltaphase = %d \n",deltaphase);
      *perror = 1;
      }
   else
      {
      return_val = pvdata[deltaphase];
      *perror = 0;
      }
   }

return return_val;
}