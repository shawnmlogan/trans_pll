
#include "globals.h"

int setlow(struct signal *psig,double deadzone,int *pSTATE)
{

/*Initialize static variables to track the number of 2pi increments
that phasein and phaseo have undergone. Incremented if phase goes
through 2pi interval*/

static int j,m = 0;
int pdoutput = 0;

if (j == 0)
   j = 1;
/*Subroutine force output of phase detector to logic low under
all conditions. The routine returns a 1 when is first called and a zero after that .*/

   *pSTATE = 0;
   return(0);
}