#include "globals.h"

void print_limiting_deltat_set_flag(int deltat_set_flag, Filedata *pdatain)
{
char *plog_string,log_string[LOGFILE_LINELENGTH + 1];

plog_string = &log_string[0];

switch (deltat_set_flag)
	{
	case 0:
		break;
	case 1:
	   snprintf(plog_string,LOGFILE_LINELENGTH,
	   "Value of deltat set by period of VCO.\n");
	   print_string_to_log(plog_string,pdatain);
	   break;
	case 2:
	   snprintf(plog_string,LOGFILE_LINELENGTH,
	   "Value of deltat set by period of jitter frequency.\n");
	   print_string_to_log(plog_string,pdatain);
	   break;
	case 3:
	   snprintf(plog_string,LOGFILE_LINELENGTH,
	   "Value of deltat set by the number of points in output file.\n");
	   print_string_to_log(plog_string,pdatain);
	   break;
	case 4:
	   snprintf(plog_string,LOGFILE_LINELENGTH,
	   "Value of deltat set by the value of input variable tauh.\n");
	   print_string_to_log(plog_string,pdatain);
	   break;
	case 5:
	   snprintf(plog_string,LOGFILE_LINELENGTH,
	   "Value of deltat set by the value of input variable tauff.\n");
	   print_string_to_log(plog_string,pdatain);
	   break;
	case 6:
	   snprintf(plog_string,LOGFILE_LINELENGTH,
	   "Value of deltat set by the width of phase detector deadzone.\n");
	   print_string_to_log(plog_string,pdatain);
	   break;
	case 7:
	   snprintf(plog_string,LOGFILE_LINELENGTH,
	   "Value of deltat set by feedback clock delay time.\n");
	   print_string_to_log(plog_string,pdatain);
	   break;
	default:
		break;
	}
}
