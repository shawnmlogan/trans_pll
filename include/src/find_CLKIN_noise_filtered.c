#include "globals.h"

int find_CLKIN_noise_filtered(struct signal *psig, Filedata *pdatain,double time,double *pCLKIN_rising_edge_time,int *pCLKIN_rising_edge_flag,double *pCLKIN_falling_edge_time,int *pCLKIN_falling_edge_flag,int LAST_CLKIN,double min_pulse_width_sec,double *pon_time_sec,long int *pnumber_of_blocked_transitions)
{

int CLKIN;
char *plog_string,log_string[LOGFILE_LINELENGTH + 1];
char value_string[NUMBER_OF_VALUE_STRINGS][LINELENGTH_OF_VALUE_STRING + 1];

/*Initialize pointers*/

plog_string = &log_string[0];

if (sin(psig->phasein) >= 0.0)
   CLKIN = 1;
else
   CLKIN = 0;

if ((CLKIN == 1) && (LAST_CLKIN == 0))
	{
	*pCLKIN_rising_edge_time = time;
	*pCLKIN_rising_edge_flag = 1;
	}
if ((CLKIN == 0) && (LAST_CLKIN == 1) && (*pCLKIN_rising_edge_flag == 1))
	{
	if ((time - *pCLKIN_rising_edge_time) < min_pulse_width_sec)
		{	
		#ifdef DEBUG_CLKIN_DUTY_CYCLE_VERBOSE
			snprintf(plog_string,LOGFILE_LINELENGTH,
			"Blocking CLKIN transition from 1 to 0 at time %s,CLKIN_falling_edge_time - CLKIN_rising_edge_time was %s.\n",
			add_units(time,8,"s",value_string[0]),
			add_units(time - *pCLKIN_rising_edge_time,4,"s",value_string[1]));				
			print_string_to_log(plog_string,pdatain);
		#endif	
		CLKIN = LAST_CLKIN;
		(*pnumber_of_blocked_transitions)++;
		}
	else
		{
		*pCLKIN_falling_edge_time = time;
		*pon_time_sec = (*pCLKIN_falling_edge_time - *pCLKIN_rising_edge_time);
		*pCLKIN_rising_edge_flag = 0;
		}
	}
 
if ((CLKIN == 0) && (LAST_CLKIN == 1))
	{
	*pCLKIN_falling_edge_time = time;
	*pCLKIN_falling_edge_flag = 1;
	}
if ((CLKIN == 1) && (LAST_CLKIN == 0) && (*pCLKIN_falling_edge_flag == 1))
	{
	if ((time - *pCLKIN_falling_edge_time) < min_pulse_width_sec)
		{
		#ifdef DEBUG_CLKIN_DUTY_CYCLE_VERBOSE
			snprintf(plog_string,LOGFILE_LINELENGTH,
			"Blocking CLKIN transition from 0 to 1 at time %s,CLKIN_rising_edge_time - CLKIN_falling_edge_time was %s.\n",
			add_units(time,8,"s",value_string[0]),
			add_units(time - *pCLKIN_falling_edge_time,4,"s",value_string[1]));				
			print_string_to_log(plog_string,pdatain);
		#endif
		CLKIN = LAST_CLKIN;
		(*pnumber_of_blocked_transitions)++;
		}
	else
		{
		*pCLKIN_rising_edge_time = time;
		*pon_time_sec = (1.0/pdatain->fin) - (*pCLKIN_rising_edge_time - *pCLKIN_falling_edge_time);
		*pCLKIN_falling_edge_flag = 0;
		}
	}
   
return CLKIN;
}
