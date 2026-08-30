#include "globals.h"

Boolean check_double_positive_non_zero(double input_double,char *pdouble_name,Boolean *pcorrected_status)
{
Boolean check_result = FALSE;

if (input_double > 0.0)
   check_result = TRUE;
else
	{
	check_result = FALSE;
	if (input_double == 0.0)
		printf("Detected value for %s is identically zero. Choose a value for %s greater than zero.\n",pdouble_name,pdouble_name);
	else
		printf("Detected value for %s is negative. Choose a value for %s greater than zero.\n",pdouble_name,pdouble_name);
	}

*pcorrected_status = check_result;

return check_result;
}