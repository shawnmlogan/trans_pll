
#include "globals.h"

double f1(struct coeff *pw, double vs1, double vs2, double vpd, double vc1, double vc2)

{
double f;

f = (pw->c1)*vs1 + (pw->c2)*vs2 + (pw->c3)*vpd + (pw->c4)*vc1 + (pw->c5)*vc2;

return(f);

}
