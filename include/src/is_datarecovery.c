
#include "globals.h"

int is_datarecovery(int phase_detector_type)
{
int returnval=0;

/* Returns a one if the phase detector is a data recovery tpre that REQUIRES
a data input file*/

switch (phase_detector_type)

   {   
   case PFD: case DFF: case NONE_VPD_HIGH: case NONE_VPD_LOW: case PFD_CP:case PD_WINDOWED:case PD_EXOR:
      {
      returnval = 0;
      break;
      }
   case HOGGE_CP: case HOGGE: case PD_ALEXANDER:
   	{
      returnval = 1;
      break;
      }
   default:
      {
      printf("Did not recognize phase detector type in is_datarecovery.c. Exiting...\n");
      exit(0);
      }
    }
 return(returnval);
 }