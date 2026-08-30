
#include "globals.h"

double f2(struct coeff *pw, double vs1, double vs2, double vpd, double vc1, double vc2)

{
double f;

f = (pw->c6)*vs1 + (pw->c8)*vpd + (pw->c9)*vc1 + (pw->c10)*vc2;

return(f);

}
