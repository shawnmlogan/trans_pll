# include "globals.h"

void find_moving_averages(struct signal *psig,struct filedata *pdatain,int history_length,long int loopcount,Stats *psig_stats,double *pvc_history,double *prphase_ui_history,double *pdeltaf_vco_history)

{
int flag_partial_sum = 0, j = 0;
long int i;

char *plog_string,log_string[LOGFILE_LINELENGTH + 1];
char *plog_string_temp,log_string_temp[LOGFILE_LINELENGTH + 1];
char value_string[NUMBER_OF_VALUE_STRINGS][LINELENGTH_OF_VALUE_STRING + 1];

double sum_vc_history = 0.0, vc_running_average = 0.0;
double sumsq_vc_residuals = 0.0, vc_history_min = 1e12, vc_history_max = -1e12,vc_history_pp = 0.0;

double sum_rphase_ui_history = 0.0, rphase_ui_running_average = 0.0;
double sumsq_rphase_ui_residuals = 0.0, sigma_rphase_ui = 0.0;
double rphase_ui_history_min = 1e12, rphase_ui_history_max = -1e12,rphase_ui_history_pp = 0.0;

double sum_deltaf_vco_history = 0.0, relative_phase_slope_average = 0.0;
double sumsq_deltaf_vco_residuals = 0.0, sigma_relative_phase_slope_average = 0.0;

plog_string = &log_string[0];
plog_string_temp = &log_string_temp[0];
/* printf("Entered find_moving_averages, loopcount = %ld, history_length = %d.\n",loopcount,history_length); */
/* Find running average of control voltage, delta vco frequency, and relative phase*/

if (loopcount > (long int) history_length)
	{
	sum_vc_history = 0.0;
	sum_rphase_ui_history = 0.0;
	sum_deltaf_vco_history = 0.0;
	vc_history_max = -1e12;
	vc_history_min = 1e12;
	rphase_ui_history_max = -1e12;
	rphase_ui_history_min = 1e12;
	for(i = 0; i < history_length; i++)
		{
		sum_vc_history = sum_vc_history + pvc_history[i];
		if (pvc_history[i] > vc_history_max)
			vc_history_max = pvc_history[i];						
		if (pvc_history[i] < vc_history_min)
			vc_history_min = pvc_history[i];
		if (prphase_ui_history[i] > rphase_ui_history_max)
			rphase_ui_history_max = prphase_ui_history[i];						
		if (prphase_ui_history[i] < rphase_ui_history_min)
			rphase_ui_history_min = prphase_ui_history[i];
		sum_rphase_ui_history = sum_rphase_ui_history + prphase_ui_history[i];
		sum_deltaf_vco_history = sum_deltaf_vco_history + pdeltaf_vco_history[i];
		}
	psig_stats->vc_running_average = sum_vc_history/(double) history_length;
	if ((psig_stats->vc_running_average > (1.0 + EPSILON)*pdatain->vmax) || (psig_stats->vc_running_average < (1.0 + EPSILON)*pdatain->vmin))
		{
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"In find_moving_averages, psig_stats->vc_running_average = %1.12e at loopcount = %ld history_length = %d\n",
		psig_stats->vc_running_average,loopcount,history_length);
		print_string_to_log(plog_string,pdatain);
		
		if (psig_stats->vc_running_average > (1.0 + EPSILON)*pdatain->vmax)
			{
			snprintf(plog_string,LOGFILE_LINELENGTH,
			"This exceeds the maximum allowed voltage of %1.12e...exiting\n",
			pdatain->vmax);
			print_string_to_log(plog_string,pdatain);
			}
		else
			{
			snprintf(plog_string,LOGFILE_LINELENGTH,
			"This is less than the minimum allowed voltage of %1.12e...exiting\n",
			pdatain->vmin);
			print_string_to_log(plog_string,pdatain);
			}
		snprintf(plog_string,LOGFILE_LINELENGTH,"j,pvc_history[j]\n");
		print_string_to_log(plog_string,pdatain);
		for (j = 0; j < history_length; j++)
			{
			snprintf(plog_string,LOGFILE_LINELENGTH,"%d,%1.12e\n",j,pvc_history[j]);
			print_string_to_log(plog_string,pdatain);
			}
		exit(0);		
		}
	/* printf("vc_history_min = %.4e, vc_history_max= %1.4e, psig_stats->vc_running_average = %1.4e.\n",vc_history_min,vc_history_max,psig_stats->vc_running_average); */
	psig_stats->vc_history_pp = vc_history_max - vc_history_min;
	psig_stats->rphase_ui_running_average = sum_rphase_ui_history/(double) history_length;
	/* printf("rphase_ui_history_min = %1.4e rphase_ui_history_max = %1.4e.\n",
	rphase_ui_history_min,rphase_ui_history_max); */
	psig_stats->rphase_ui_history_pp = rphase_ui_history_max - rphase_ui_history_min;
	psig_stats->relative_phase_slope_average = sum_deltaf_vco_history/(double) history_length;
	sumsq_vc_residuals = 0.0;
	sumsq_rphase_ui_residuals = 0.0;
	sumsq_deltaf_vco_residuals = 0.0;
	for(i = 0; i < history_length; i++)
		{
		sumsq_vc_residuals = sumsq_vc_residuals + pow((pvc_history[i] - vc_running_average),2.0);
		sumsq_rphase_ui_residuals = sumsq_rphase_ui_residuals + pow((prphase_ui_history[i] - rphase_ui_running_average),2.0);
		sumsq_deltaf_vco_residuals = sumsq_deltaf_vco_residuals + pow((pdeltaf_vco_history[i] - relative_phase_slope_average),2.0);
		}
	psig_stats->sigma_vc = pow(sumsq_vc_residuals/(double) history_length,0.50);
	psig_stats->sigma_rphase_ui = pow(sumsq_rphase_ui_residuals/(double) history_length,0.50);
	psig_stats->sigma_relative_phase_slope_average = pow(sumsq_deltaf_vco_residuals/(double) history_length,0.50);
	}
else
	{
	flag_partial_sum = 1;
	sum_vc_history = 0.0;
	sum_rphase_ui_history = 0.0;
	sum_deltaf_vco_history = 0.0;
	vc_history_max = -1e12;
	vc_history_min = 1e12;
	rphase_ui_history_max = -1e12;
	rphase_ui_history_min = 1e12;
	for(i = 0; i < loopcount; i++)
		{
		sum_vc_history = sum_vc_history + pvc_history[i];
		if (pvc_history[i] > vc_history_max)
			vc_history_max = pvc_history[i];						
		if (pvc_history[i] < vc_history_min)
			vc_history_min = pvc_history[i];
		if (prphase_ui_history[i] > rphase_ui_history_max)
			rphase_ui_history_max = prphase_ui_history[i];						
		if (prphase_ui_history[i] < rphase_ui_history_min)
			rphase_ui_history_min = prphase_ui_history[i];
		sum_rphase_ui_history = sum_rphase_ui_history + prphase_ui_history[i];
		sum_deltaf_vco_history = sum_deltaf_vco_history + pdeltaf_vco_history[i];
		}
	psig_stats->vc_running_average = sum_vc_history/(double) loopcount;
	psig_stats->vc_history_pp = vc_history_max - vc_history_min;
	psig_stats->rphase_ui_running_average = sum_rphase_ui_history/(double) loopcount;
	/* printf("rphase_ui_history_min = %1.4e rphase_ui_history_max = %1.4e.\n",
	rphase_ui_history_min,rphase_ui_history_max); */
	psig_stats->rphase_ui_history_pp = rphase_ui_history_max - rphase_ui_history_min;
	psig_stats->relative_phase_slope_average = sum_deltaf_vco_history/(double) loopcount;
	sumsq_vc_residuals = 0.0;
	sumsq_rphase_ui_residuals = 0.0;
	sumsq_deltaf_vco_residuals = 0.0;
	for(i = 0; i < (loopcount - 1); i++)
		{
		sumsq_vc_residuals = sumsq_vc_residuals + pow((pvc_history[i] - vc_running_average),2.0);
		sumsq_rphase_ui_residuals = sumsq_rphase_ui_residuals + pow((prphase_ui_history[i] - rphase_ui_running_average),2.0);
		sumsq_deltaf_vco_residuals = sumsq_deltaf_vco_residuals + pow((pdeltaf_vco_history[i] - relative_phase_slope_average),2.0);
		}
	psig_stats->sigma_vc = pow(sumsq_vc_residuals/(double) loopcount,0.50);
	psig_stats->sigma_rphase_ui = pow(sumsq_rphase_ui_residuals/(double) loopcount,0.50);
	psig_stats->sigma_relative_phase_slope_average = pow(sumsq_deltaf_vco_residuals/(double) loopcount,0.50);
	}
/* printf("Exiting find_moving_averages: flag_partial_sum = %d and loopcount = %ld.\n",flag_partial_sum,loopcount); */
}
