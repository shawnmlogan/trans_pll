# include "globals.h"

void detect_lock(struct signal *psig,struct filedata *pdatain,long int lock_detect_counter,double time,Stats *psig_stats,int *plock_detect_flag, double *plock_time,int *plost_lock_detect_flag, double *plost_lock_time)

{
int i;

char *plog_string,log_string[LOGFILE_LINELENGTH + 1];
char *plog_string_temp,log_string_temp[LOGFILE_LINELENGTH + 1];
char value_string[NUMBER_OF_VALUE_STRINGS][LINELENGTH_OF_VALUE_STRING + 1];

double fo;
double vc_diff_percent_limit = pdatain->lock_detect_delta_vc_limit_percent;
double relative_phase_slope_average_ppm_limit = RELATIVE_PHASE_SLOPE_AVERAGE_PPM_LIMIT;
double rphase_ui_running_average_limit = RPHASE_UI_HISTORY_LIMIT;
double rphase_ui_running_average_loss_lock_limit = RPHASE_UI_HISTORY_LOSS_LOCK_LIMIT;
double rphase_ui_history_pp_limit = pdatain->lock_detect_rphase_pp_limit_ui;
double rphase_ui_history_pp_loss_lock_limit = pdatain->lock_detect_rphase_pp_loss_lock_limit_ui;
double locked_rphase_ui_running_average = 0.0;

plog_string = &log_string[0];
plog_string_temp = &log_string_temp[0];

/* printf("Entered detect_lock, lock_detect_counter = %ld.\n",lock_detect_counter); */
#ifdef DEBUG_LOCK_DETECT_VERBOSE
	if (lock_detect_counter == 0)
		{
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"\nParameter status at time %s with lock_detect_counter = %ld, *plock_detect_flag = %d....\n",add_units(time,4,"s",value_string[0]),lock_detect_counter,*plock_detect_flag);
		print_string_to_log(plog_string,pdatain);	
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"Center voltage check: psig_stats->vcenter_voltage = %.3f\npsig_stats->vc_running_average = %.3f, %% difference is %.1f. (should be less than %s)\n",
		psig_stats->vcenter_voltage,psig_stats->vc_running_average,100.0*fabs((psig_stats->vcenter_voltage - psig_stats->vc_running_average)/psig_stats->vcenter_voltage),add_units(vc_diff_percent_limit,1,"%",value_string[0]));
		print_string_to_log(plog_string,pdatain);	
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"Lock detect flag must be 0: *plock_detect_flag == %d.\n",*plock_detect_flag);
		print_string_to_log(plog_string,pdatain);	
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"fabs(psig_stats->relative_phase_slope_average) = %.1f ppm (must be less than %s\n",1e6*fabs(psig_stats->relative_phase_slope_average),add_units(relative_phase_slope_average_ppm_limit,1,"ppm",value_string[0]));
		print_string_to_log(plog_string,pdatain);
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"psig_stats->rphase_ui_history_pp = %.4f UIpp (must be less than %s)\n",psig_stats->rphase_ui_history_pp,add_units(rphase_ui_history_pp_limit,2,"UIpp",value_string[0]));
		print_string_to_log(plog_string,pdatain);
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"psig_stats->rphase_ui_running_average = %.4f UIpp (must be less than %s)\n",psig_stats->rphase_ui_running_average,add_units(rphase_ui_running_average_limit,2,"UI",value_string[0]));
		print_string_to_log(plog_string,pdatain);
		}
#endif
	
/* Find lock detect flag based on vc_running_average and rphase*/

#ifdef DEBUG_LOCK_DETECT_VERBOSE
	snprintf(plog_string,LOGFILE_LINELENGTH,
	"fabs((psig_stats->vcenter_voltage - psig_stats->vc_running_average)/psig_stats->vcenter_voltage) = %1.12e.\n",
	fabs((psig_stats->vcenter_voltage - psig_stats->vc_running_average)/psig_stats->vcenter_voltage));
	print_string_to_log(plog_string,pdatain);
	
	snprintf(plog_string,LOGFILE_LINELENGTH,
	"(*plock_detect_flag= %d.\n",*plock_detect_flag);
	print_string_to_log(plog_string,pdatain);	
	
	snprintf(plog_string,LOGFILE_LINELENGTH,
	"1e6*fabs(psig_stats->relative_phase_slope_average) = %1.12e.\n",
	1e6*fabs(psig_stats->relative_phase_slope_average));
	print_string_to_log(plog_string,pdatain);	
	
	snprintf(plog_string,LOGFILE_LINELENGTH,
	"psig_stats->rphase_ui_history_pp = %1.12e.\n",psig_stats->rphase_ui_history_pp);
	print_string_to_log(plog_string,pdatain);	
#endif

if( (fabs((psig_stats->vcenter_voltage - psig_stats->vc_running_average)/psig_stats->vcenter_voltage) < vc_diff_percent_limit/100.0) && (*plock_detect_flag == 0) && ((psig_stats->rphase_ui_running_average < rphase_ui_running_average_limit) || (psig_stats->rphase_ui_history_pp < rphase_ui_history_pp_limit)))
	{
	snprintf(plog_string,LOGFILE_LINELENGTH,
	"Lock detected at time %s.\n",add_units(time,4,"s",value_string[0]));
	print_string_to_log(plog_string,pdatain);
	*plock_time = time;
	*plock_detect_flag = 1;
	locked_rphase_ui_running_average = psig_stats->rphase_ui_running_average;
	
	#ifdef DEBUG_LOCK_DETECT
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"Running average vc = %s, difference from center voltage = %s\nMaximum limit for lock: %s\n",
		add_units(psig_stats->vc_running_average,3,"V",value_string[0]),
		add_units(100.0*fabs((psig_stats->vcenter_voltage - psig_stats->vc_running_average)/psig_stats->vcenter_voltage),2,"%",value_string[1]),
		add_units(vc_diff_percent_limit,2,"%",value_string[2]));
		print_string_to_log(plog_string,pdatain);
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"Running average delta_f_vco = %s\nMaximum limit for lock: %s\n",add_units(1e6*fabs(psig_stats->relative_phase_slope_average),
		2,"ppm",value_string[0]),add_units(relative_phase_slope_average_ppm_limit,2,"ppm",value_string[1]));
		print_string_to_log(plog_string,pdatain);
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"rphase_ui_running_average  = %s\nMaximum limit for lock: %s\n",
		add_units(psig_stats->rphase_ui_running_average,2,"UI",value_string[0]),
		add_units(rphase_ui_running_average_limit,2,"UI",value_string[1]));
		print_string_to_log(plog_string,pdatain);
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"rphase_ui_history_pp  = %s\nMaximum limit for lock: %s\n",
		add_units(psig_stats->rphase_ui_history_pp,2,"UI",value_string[0]),
		add_units(rphase_ui_history_pp_limit,2,"UI",value_string[1]));
		print_string_to_log(plog_string,pdatain);

	#endif
	
	}
else
	{
	if (((psig_stats->rphase_ui_running_average > rphase_ui_running_average_loss_lock_limit) || (psig_stats->rphase_ui_history_pp > rphase_ui_history_pp_loss_lock_limit)) && (*plock_detect_flag == 1))
		{
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"Lock lost at time %s.\n",add_units(time,4,"s",value_string[0]));
		print_string_to_log(plog_string,pdatain);
		*plock_detect_flag = 0;
		*plost_lock_detect_flag = 1;
		*plost_lock_time = time;
		
		#ifdef DEBUG_LOCK_DETECT
			snprintf(plog_string,LOGFILE_LINELENGTH,
			"vc = %s, sigma_vc = %s, vc_history_pp = %s, rphase_ui_running_average = %s, sigma_rphase_ui = %s, ",
			add_units(psig_stats->vc_running_average,3,"V",value_string[0]),
			add_units(psig_stats->sigma_vc,3,"V",value_string[1]),
			add_units(psig_stats->vc_history_pp,3,"V",value_string[2]),
			add_units(psig_stats->rphase_ui_running_average,3,"UI",value_string[3]),
			add_units(psig_stats->sigma_rphase_ui,3,"UI",value_string[4]));
			print_string_to_log(plog_string,pdatain);
			snprintf(plog_string,LOGFILE_LINELENGTH,
			"relative_phase_slope_average = %.1f ppm, sigma_relative_phase_slope_average  = %.1f ppm, rphase_ui_history_pp = %s\n",
			1e6*psig_stats->relative_phase_slope_average,1e6*psig_stats->sigma_relative_phase_slope_average,
			add_units(psig_stats->rphase_ui_history_pp,3,"UI",value_string[0]));
			print_string_to_log(plog_string,pdatain);
		#endif
		
		}
	}
}