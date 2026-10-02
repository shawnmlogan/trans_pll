#include "globals.h"

Boolean select_second_order_filter_coefficents(double R2,double R3,double C1,double C2)
{
Boolean check_result = FALSE;
char *plog_string,log_string[LOGFILE_LINELENGTH + 1];

plog_string = &log_string[0];

if ((C2 > 1e-16) && (R3 < 10e6))
   check_result = TRUE;
else
	check_result = FALSE;

return check_result;
}