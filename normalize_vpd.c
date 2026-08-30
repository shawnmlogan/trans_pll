#include "globals.h"

double normalize_vpd(Filedata *pdatain,double vpd)
{
char *plog_string,log_string[LOGFILE_LINELENGTH + 1];
double vpd_normalized;

plog_string = &log_string[0];

if (vpd == pdatain->logic_lo_pdsignal)
	{
	if (pdatain->logic_lo_pdsignal != 0.0)
		vpd_normalized = vpd/fabs(pdatain->logic_lo_pdsignal);
	else
		vpd_normalized = vpd;
	}
else
	{
	if (vpd == pdatain->logic_hi_pdsignal)
		{
		if (pdatain->logic_hi_pdsignal != 0.0)
			vpd_normalized = vpd/fabs(pdatain->logic_hi_pdsignal);
		else
			vpd_normalized = vpd;
		}
	else
		{
		if ((vpd == pdatain->logic_lo_pdsignal + pdatain->logic_hi_pdsignal) && (pdatain->logic_lo_pdsignal == 0.0 - pdatain->logic_hi_pdsignal) && (pdatain->logic_lo_pdsignal != 0.0) && (pdatain->logic_hi_pdsignal != 0.0))
			vpd_normalized = 0.0;
		else
			{
			snprintf(plog_string,LOGFILE_LINELENGTH,
			"vpd has the undefined value of %1.6e in function normalize_vpd()!! Exiting...\n", vpd);
         print_string_to_log(plog_string,pdatain);
         exit(0);
         }
      }
   }

return vpd_normalized;
}

	