#include "globals.h"

int find_CLKIN(struct signal *psig, Filedata *pdatain)
{
int CLKIN;

if (sin(psig->phasein) >= 0.0)
/* if (sin(psig->phasein - pdatain->phase0) >= 0.0) */
   CLKIN = 1;
else
   CLKIN = 0;
   
return CLKIN;
}
