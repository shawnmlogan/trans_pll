
#include "globals.h"

int is_chargepump(int phase_detector_type)
{
int returnval=0;

/* Returns a one if the phase detector uses a charge pump*/

switch (phase_detector_type)

   {   
   case PFD: case DFF: case NONE_VPD_HIGH: case NONE_VPD_LOW: case HOGGE: case PD_EXOR:
   case PD_WINDOWED: case PD_ALEXANDER:
      {
      returnval = 0;
      break;
      }
   case HOGGE_CP: case PFD_CP:
      {
      returnval = 1;
      break;
      }
   default:
      {
      printf("Did not recognize phase detector type in is_chargepump.c. Exiting...\n");
      exit(0);
      }
    }
 return(returnval);
 }
