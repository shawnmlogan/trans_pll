#include "globals.h"

/* save_vin_vout_and_compare(loopcount,CLKIN,VIN,psig->clk,VOUT,time,&pdelay); */

void save_vin_vout_and_compare(int* pvin_sampled,int *pvout_sampled,int lock_detect_flag,int CLKIN,int VIN,long int number_of_input_samples,int CLKOUT,int VOUT,long int number_of_output_samples, double time,long int *pdelay, int *perror_free_flag,Filedata *pdatain)
{

int k = 0;
static int LAST_CLKIN, LAST_CLKOUT, last_pdelay;

static long int number_of_input_clock_cycles, number_of_output_clock_cycles;
static long int last_cycle_check;

long int i, j, delay = 0, sum = 0, min_sum = number_of_input_samples;

char *poutput_filename, output_filename[FILENAME_LINELENGTH + 1];
char *plog_string,log_string[LOGFILE_LINELENGTH + 1];
char value_string[NUMBER_OF_VALUE_STRINGS][LINELENGTH_OF_VALUE_STRING + 1];

double bit_error_rate;
static double last_bit_error_rate = 1.0;

FILE *fpw1;

poutput_filename = &output_filename[0];
plog_string = &log_string[0];
	
if ((LAST_CLKOUT == 1) && (CLKOUT == 0))
	{
	for (i = 0; i < number_of_output_samples; i++)
		{
		if ( i != (number_of_output_samples - 1))
			pvout_sampled[i] = pvout_sampled[i + 1];
		else
			pvout_sampled[i] = VOUT;
		}
	number_of_output_clock_cycles++;
	if ((number_of_output_clock_cycles > number_of_output_samples) && (lock_detect_flag == 1) && (*perror_free_flag == 0))
		{
		for (i = 0; i < (number_of_output_samples - number_of_input_samples); i++)
			{
			sum = 0;
			for (j = 0; j < number_of_input_samples; j++)
				{
				sum = sum + abs(pvin_sampled[j] - pvout_sampled[i + j]);
				if (sum != 0)
					j = number_of_input_samples;
				}
			if (sum == 0)
				{
				*pdelay = i;
				#ifdef DEBUG_SAVE_VIN_VOUT_AND_COMPARE
					if (i != last_pdelay)
						{
						snprintf(poutput_filename,FILENAME_LINELENGTH,
						"sampled_data_check_input_clocks_%ld_output_clocks_%ld.csv",
						number_of_input_clock_cycles,number_of_output_clock_cycles);
						fpw1 = fopen(poutput_filename,"w");
						for (k = 0; k < number_of_output_samples; k++)
							{
							if (k < number_of_input_samples)
								fprintf(fpw1,"%ld,%d,%d\n",*pdelay,pvin_sampled[k],pvout_sampled[k]);
							else
								fprintf(fpw1,"%ld,%d\n",*pdelay,pvout_sampled[k]);
							}
						fclose(fpw1);
						}
				#endif
				last_pdelay = *pdelay;
				i = number_of_output_samples - number_of_input_samples;
				}
			}
		last_cycle_check = number_of_output_clock_cycles;
		}
	else
		{
		if ((number_of_output_clock_cycles > number_of_output_samples) && (lock_detect_flag == 1) && (*perror_free_flag == 1))
			{
			/* printf("Skipping full data check...\n"); */
			/* Just check last bit to verify it is the same */
			if (abs(pvout_sampled[number_of_output_samples - 1] - pvin_sampled[number_of_input_samples - 1 - *pdelay]) != 0)
				{
				snprintf(plog_string,LOGFILE_LINELENGTH,
				"Detected an error...at time %s, re-synchronizing...\n",
				add_units(time,8,"s",value_string[0]));
				print_string_to_log(plog_string,pdatain);				
				*perror_free_flag = 0;
				}
			else
				*perror_free_flag = 1;
			}
		}
	}

if ((LAST_CLKIN == 1) && (CLKIN == 0))
	{
	for (i = 0; i < number_of_input_samples; i++)
		{
		if ( i != (number_of_input_samples - 1))
			pvin_sampled[i] = pvin_sampled[i + 1];
		else
			pvin_sampled[i] = VIN;
		}
	number_of_input_clock_cycles++;
	}

LAST_CLKIN = CLKIN;
LAST_CLKOUT = CLKOUT;
}
