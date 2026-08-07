#define MAIN
#include "globals.h"

int main(int argc, char **argv)
{

/*Define variables*/

int i, j, k = 1, l, p, q, init,STATE,*pSTATE,pd_event, vdata[NUMBER_OF_SAMPLES_STORED], *pvdata, k0 = 0.0;
int check_data_failed_flag = 0, *perror, error = 0, error_free_flag = 0, last_error_free_flag = 0;
int jj_flag = 0;
int loop = 0; /*counter for jitter frequency*/
int m = 0, n = 0;
int CLKIN = 0, LAST_CLKIN = 0, VIN = 0, VOUT = 0, LAST_VIN = 0, LAST_VOUT = 0;
int CLKIN_rising_edge_flag = 0, CLKIN_falling_edge_flag = 0, vin_rising_edge_flag = 0;
int timer1set = 0, *ptimer1set, p_flag = 0;
int lock_detect_flag = 0, lost_lock_detect_flag = 0, N_lock_detect;
int num_control_voltage_points = 100, plot_normalized_freq = PLOT_NORMALIZED_FREQ;
int history_length;
int flag_stoptime = 0, flag_add_phase_or_freq_step = 0;
int deltaphase = 0;
int ll;
/* Dynamic arrays to store sampled values of VIN and VOUT (data recovery only)*/
int *pvin_sampled, *pvout_sampled;
int deltat_set_flag = 0;

unsigned int seed;
static int num = 0;

long int filtered_noise_counter = 0, unfiltered_noise_counter = 0, time_point_counter = 0;
long int lock_detect_counter = 0, loopcount = 0, last_loopcount = 0, numperiods = 0.0, last_numperiods = 0.0;
long int pphaseo_sample_buffer_size = 0, jt_samples_buffer_size = 0;
long int ii, jj = 0, number_deltat_to_skip = 1;
long int number_of_output_samples = 0, pdelay = -1, last_pdelay = -1;
long int error_free_sample_size = 0, number_of_blocked_transitions = 0;
long int num_vout_rising_edges = 0;
long int vco_clk_pos_edge_counter = 0,vco_clk_neg_edge_counter = 0;
long int clk_pos_edge_counter = 0,clk_neg_edge_counter = 0;

char *pcontrol_voltage_filename,control_voltage_filename[LINELENGTH+1];
char *plog_string,log_string[LOGFILE_LINELENGTH + 1];
char *plog_string_temp,log_string_temp[LOGFILE_LINELENGTH + 1];
char *plog_filename, log_filename[FILENAME_LINELENGTH + 1];
char ffooter[SUFFIX1_LINELENGTH + 1], *pffooter;
char fnameout0[FILENAME_LINELENGTH + 1], *pfnameout0;
char fnameout1[FILENAME_LINELENGTH + 1], *pfnameout1,fnameout2[FILENAME_LINELENGTH + 1], *pfnameout2;
char input_filename[FILENAME_LINELENGTH + 1],*pinput_filename, phdetect[LINELENGTH + 1];
char output_filename_jt[FILENAME_LINELENGTH + 1], *poutput_filename_jt;
char debug_hogge_alexander_filename[FILENAME_LINELENGTH + 1],*pdebug_filename;
char *ptitle_string, title_string[TITLE_LINELENGTH + 1];
char pd[LINELENGTH+1],deltat[LINELENGTH+1],alexander_pd_threshold_ui[LINELENGTH+1];
char *pvc_history_filename, vc_history_filename[FILENAME_LINELENGTH + 1];
char *pfilename_vin_vout_sampled,filename_vin_vout_sampled[FILENAME_LINELENGTH+1];
char *pfilename_sampled_noise, filename_sampled_noise[FILENAME_LINELENGTH+1];

char *pcsv_line,csv_line[CSV_FILE_LINELENGTH + 1];
char *pcsv_line_temp,csv_line_temp[CSV_FILE_LINELENGTH + 1];
char *pfileprompt="Enter filename to read data from:";
char noise_type_string[LINELENGTH+1];

char fileout_system_command_line[LINELENGTH+1], *pfileout_system_command_line;
char *psystem_command_line_1, system_command_line_1[COMMAND_LINELENGTH + 1];
char *psystem_command_line_2, system_command_line_2[COMMAND_LINELENGTH + 1];
char *pphase_units="radians";
char *ptimestamp,timestamp[LINELENGTH + 1];
char *plock_time_string,lock_time_string[LINELENGTH + 1];
char *plost_lock_time_string,lost_lock_time_string[LINELENGTH + 1];
char value_string[NUMBER_OF_VALUE_STRINGS][LINELENGTH_OF_VALUE_STRING + 1];
char *poctave = "octave",*poctave_path,octave_path[LINELENGTH + 1];
char *pgnuplot = "gnuplot",*pgnuplot_path,gnuplot_path[LINELENGTH + 1];
char plot_preference[LINELENGTH + 1];
char *psample_filename, sample_filename[LINELENGTH + 1];
char *pcsv = ".csv",*ptxt = ".txt",*pblank = "", *pquestion_mark = "\?";
char *pno_named_file = "no_named_file", *ppost_processed = "post_processed";

double input_data_sample_number = 1.0, output_data_sample_number = 1.0;
double RZ=10.0E12;/*Impedance for tri-state*/
double deadzone = 0.0; /*Not used in current program*/
double vc_polynomial[MAX_VCO_POLYNOMIAL],jitter_freq[NUMBER_OF_JITTER_FREQ];
double **pjitter_transfer_array; /* Dynamic memory array for storing jitter transfer and lock time data */
double *pjitter_transfer_gain, *pjitter_transfer_phase; /* Dynamic memory arrays for gain and phase data */
double time,time0,fo,freq,freqp,prpoint,prtime,fj = 0.0;
double vdata_phase0 = 0.0;
double initjitter_time = 0.0;
double phase_step = 0.0;
double freq_step = 0.0;
double tsample,errmax = 100.0;
double phasein_ui = 0.0, phaseout_ui = 0.0, rphase_ui = 0.0, rphase = 0.0;
double tau=EPSILON,vhpf = 0,vsat = 0.0;
double noise = 0.0, filtered_noise = 0.0, filtered_noise_max = -999.0, filtered_noise_min = 999.0;
double unfiltered_noise_min = 999.0, unfiltered_noise_max = -999.0;
double rphase_ui_min = +999.0, rphase_ui_max = -999.0, tau_noise_bandwidth_sec;
double phaseout_delayed_ui = 0.0;
double freq_ssc_ppm = 0.0, ssc_slope = 0.0, ssc_phase = 0.0;
double unity_time_constant = 1.0;
double phase_max = 1.0/EPSILON, phase_min = -1.0/EPSILON;
double lock_time = 0.0,lock_detect_time = 0.0;
double lost_lock_time = 0.0,lost_lock_detect_time = 0.0;
double tend1 = 0.0;
double *read_pointer,*write_pointer;
double max_error_percent_vcenter_voltage = 0.0001;
double *pphaseo_sample; /* Dynamic memory array to store phaseo to allow for delay in feedback loop */
double *pvc_history, *prphase_ui_history, *pdeltaf_vco_history; /* Dynamic memory arrays to store running average data */
double last_phasein = 0.0, last_phaseo_delayed = 0.0;
double ber = NEVER_DETECTED_ERROR_FREE, last_ber = NEVER_DETECTED_ERROR_FREE;
double phase_val = 0.0;
double CLKIN_rising_edge_time = 0.0, CLKIN_falling_edge_time = 0.0, min_CLKIN_pulse_width = 0.0;
double on_time_sec = 0.0, min_on_time_sec = 1.0/EPSILON, max_on_time_sec = -1.0/EPSILON;
double time_vin_rising_edge = 0.0, vin_vout_rising_edge_delay = 0.0, sum_vin_vout_rising_edge_delay = 0.0;
double max_vin_vout_rising_edge_delay = -1.0/EPSILON, min_vin_vout_rising_edge_delay = 1.0/EPSILON;
double *ptime_jt_samples; /* Dynamic memory array to store time values for transfer function analysis */
double *pphasein_jt_samples; /* Dynamic memory array to store phasein for transfer function analysis */
double *pphaseo_jt_samples; /* Dynamic memory array to store phaseo for transfer function analysis */
double *pphaseo_delayed_jt_samples; /* Dynamic memory array to store phaseo_delayed for transfer function analysis */
double last_pd_event_time = 0.0;
double unity_divider = 1.0;

/*Define structure for data inputs and coefficients*/

struct signal sig,*psig;
struct coeff w,*pw;
Stats *psig_stats, sig_stats;

/*Define structure containing input data*/

Filedata *pdatain,datain = {.enable_plot_flag = 1, .file_entry_flag = 0};

FILE *fpw1,*fpw2, *fpw3, *fpw4, *fpw5, *fpw6, *fp_pvc, *fpw_input_data, *fpw_output_data, *fpw100, *fpw200;

clock_t tic,toc;

tic = clock();

/*Initialize pointers*/

pw = &w;
psig = &sig;
psig_stats = &sig_stats;
plog_string = &log_string[0];
plog_string_temp = &log_string_temp[0];
plog_filename = &log_filename[0];
ptimestamp = &timestamp[0];
pdatain = &datain;
pdatain->pvc_model_string = &pdatain->vc_model_string[0];
pdatain->pvc_polynomial = &vc_polynomial[0];
pdatain->pjitter_freq = &jitter_freq[0];
pdatain->ppd = &pd[0];
pdatain->pdeltat = &deltat[0];
pdatain->plock_detect_delta_vc_limit_percent_string = &pdatain->lock_detect_delta_vc_limit_percent_string[0];
pdatain->plock_detect_rphase_pp_limit_ui_string = &pdatain->lock_detect_rphase_pp_limit_ui_string[0];
pdatain->plock_detect_rphase_pp_loss_lock_limit_ui_string = &pdatain->lock_detect_rphase_pp_loss_lock_limit_ui_string[0];
pdatain->pfile_format = &pdatain->file_format[0];
pdatain->p_plot_preference = &plot_preference[0];
pdatain->pinput_filename = &pdatain->input_filename[0];
pdatain->poutput_base_filename = &pdatain->output_base_filename[0];
pdatain->pdatafilein = &pdatain->datafilein[0];
pdatain->ptimestamp = &pdatain->timestamp[0];
pdatain->plog_filename = &pdatain->log_filename[0];
pinput_filename = &input_filename[0];
pfnameout0 = &fnameout0[0];
pfnameout1 = &fnameout1[0];
pfnameout2 = &fnameout2[0];
poutput_filename_jt = &output_filename_jt[0];
pdebug_filename = &debug_hogge_alexander_filename[0];
pdatain->palexander_pd_threshold_ui = &alexander_pd_threshold_ui[0];
pdatain->pnoise_type = &noise_type_string[0];
pffooter = &ffooter[0];
pvdata = &vdata[0];
pSTATE = &STATE;
perror = &error;
pcsv_line = &csv_line[0];
pcsv_line_temp = &csv_line_temp[0];
ptitle_string = &title_string[0];
pfileout_system_command_line = &fileout_system_command_line[0];
psystem_command_line_1 = &system_command_line_1[0];
psystem_command_line_2 = &system_command_line_2[0];
pcontrol_voltage_filename = &control_voltage_filename[0];
poctave_path = &octave_path[0];
pgnuplot_path = &gnuplot_path[0];
ptimer1set = &timer1set;
plock_time_string = &lock_time_string[0];
plost_lock_time_string = &lost_lock_time_string[0];
pvc_history_filename = &vc_history_filename[0];
pfilename_vin_vout_sampled = &filename_vin_vout_sampled[0];
pfilename_sampled_noise = &filename_sampled_noise[0];
psample_filename = &sample_filename[0];

snprintf(psample_filename,LINELENGTH,"trans_pll_v%.2f_input_params.txt",VERSION_NUMBER);

/*Define pi */

pi = 2.0 * asin(1.0);

/* Find timestamp to append to filenames */

find_timestamp(ptimestamp,LINELENGTH);
strncpy(pdatain->ptimestamp,ptimestamp,LINELENGTH);

/*Prompt for input file from user*/

/*Command line check*/

if (argc != 2)
   {
   printf("\ntrans_pll v%.2f %s\n\n",VERSION_NUMBER,VERSION_DATE);
   printf("Usage: trans_pll <filename with trans_pll inputs>\n\ntrans_pll ? creates sample input file\n\n");
   exit(0);
   }
else
   {
   if (strcmp(argv[1],pquestion_mark) == 0)
   	{
   	create_sample_file(psample_filename);
   	printf("Created sample file \"%s\".\nModify file with your input parameter values.\n",
   	psample_filename);
   	exit(0);
   	}
   else
	   {
	   pdatain->file_entry_flag = 1;
	   if (filecheck(argv[1],pdatain) != EXIT_SUCCESS)
	      {
	      printf("Exiting...try again with valid filename\n");
	      exit(0);
	      }
 	      
	   /*Initialize input data structure to allow for correct error checking*/

		initdata(pdatain);

	   if (pop_data(argv[1],pdatain) != EXIT_SUCCESS)
	   	{
	   	snprintf(plog_string,LOGFILE_LINELENGTH,
	   	"Reading input file failed...type \"trans_pll %s\" to create sample input file.\n",
	   	pquestion_mark);
	   	create_sample_file(psample_filename);
	   	strncpy(plog_string_temp,plog_string,LOGFILE_LINELENGTH);
			}
		else
			strncpy(plog_string_temp,pblank,LOGFILE_LINELENGTH);

	   if(check_data(pdatain) != EXIT_SUCCESS)
	   	{
	   	printf("%s\n",plog_string);
	   	check_data_failed_flag = 1;
	   	}
		else
			strncpy(plog_string_temp,pblank,LOGFILE_LINELENGTH);
	   }
   }

if (check_data_failed_flag == 1)
	exit(0);

/* Create log filename */

replace_string(pdatain->pinput_filename,plog_filename,ptxt,pblank,FILENAME_LINELENGTH);
snprintf(pdatain->plog_filename,FILENAME_LINELENGTH,"%s_%s.log",plog_filename,ptimestamp);
strncpy(plog_filename,pdatain->plog_filename,FILENAME_LINELENGTH);

snprintf(plog_string,LOGFILE_LINELENGTH,"\ntrans_pll v%.2f %s\n\n%s",
VERSION_NUMBER,VERSION_DATE,plog_string_temp);
print_string_to_log(plog_string,pdatain);
	
/* Provide warning regarding saving very large SAVE_DEBUG_CDR_PLL_SIGNALS file size if option enabled */
		
#ifdef SAVE_DEBUG_CDR_PLL_SIGNALS
	snprintf(plog_string,LOGFILE_LINELENGTH,
	"\nWARNING: Option to save large CDR phase detector debugging file is enabled!\n\n");
	print_string_to_log(plog_string,pdatain);
#endif

#ifdef SAVE_FULL_OUTPUT_FILE
	snprintf(plog_string,LOGFILE_LINELENGTH,
	"\nWARNING: Option to save full output file is enabled!\n\n");
	print_string_to_log(plog_string,pdatain);
#endif

snprintf(plog_string,LOGFILE_LINELENGTH,"Input filename: %s\n",pdatain->pinput_filename);
print_string_to_log(plog_string,pdatain);

/* Create filename for VCO characteristic */

sprintf(pcontrol_voltage_filename, "delta_f_vs_vc_%s.csv",ptimestamp);

/*Set center of array to be sampling point nominally to allow jitter amplitude to span both sides of data*/

k0 = floor(NUMBER_OF_SAMPLES_STORED/2.0);

/* printf("Number of samples stored in vector vdata is %d.\n",NUMBER_OF_SAMPLES_STORED);
printf("Nominal sampling location is placed after %d clock cycles.\n",k0); */

/*Initialize random number sequence*/

seed = (unsigned int) clock();
srand(seed);

/*Initialize input signal running average statistics*/

init_sig_stats(psig_stats,pdatain);

/* Disable plotting if neither octave nor gnuplot are available */

if (pdatain->plot_outputs == 1)
	{
	if ((check_executable(poctave,poctave_path)) == 0)
		pdatain->octave = 0;
	else
		pdatain->octave = 1;
	if ((check_executable(pgnuplot,pgnuplot_path)) == 0)
		pdatain->gnuplot = 0;
	else
		pdatain->gnuplot = 1;
	if ((pdatain->octave == 0) && (pdatain->gnuplot == 0))
		{
		snprintf(plog_string,LOGFILE_LINELENGTH,"Did not locate executable for octave nor gnuplot.\n");
		print_string_to_log(plog_string,pdatain);
		snprintf(plog_string,LOGFILE_LINELENGTH,"Disabling all plots as neither octave nor gnuplot was not found in path.\n");
		print_string_to_log(plog_string,pdatain);
		pdatain->plot_outputs = 0;
		}
	}
	
/*Define tau for noise bandlimiting filter (check_data() verified pdatain->noise_bandwidth_Hz > 0.0)*/

tau_noise_bandwidth_sec = 1.0/(2.0*pi*pdatain->noise_bandwidth_Hz);

/*Compute slope of SSC characteristic - just verified that all SSC entries are entered and have correct range*/

ssc_slope = 2.0*(pdatain->freq_ssc)*(pdatain->max_pos_ssc_ppm+pdatain->max_neg_ssc_ppm);

/* Allocate memory for jitter transfer gain, phase, and jitter transfer array */

if ((pjitter_transfer_gain = (double *) calloc(pdatain->number_of_jitter_freq,sizeof(double))) == NULL || (pjitter_transfer_phase = (double *) calloc(pdatain->number_of_jitter_freq,sizeof(double))) == NULL || (pjitter_transfer_array = (double **) calloc(pdatain->number_of_jitter_freq,sizeof(double *))) == NULL)
	{
	snprintf(plog_string,LOGFILE_LINELENGTH,
	"Memory allocation failure for pjitter_transfer_gain,pjitter_transfer_phase,\n");
	print_string_to_log(plog_string,pdatain);
	snprintf(plog_string,LOGFILE_LINELENGTH,
	"or pjitter_transfer_array in main.c\n");
	print_string_to_log(plog_string,pdatain);
	exit(0);
	}

for (i = 0; i < pdatain->number_of_jitter_freq; i++)
	{
	if ((pjitter_transfer_array[i] = (double *) calloc(5,sizeof(double))) == NULL)
		{
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"Memory allocation failure for pjitter_transfer_array[%d] in main.c\n",i);
		print_string_to_log(plog_string,pdatain);
		exit(0);
		}
	}

for (loop=0; loop < pdatain->number_of_jitter_freq; loop++)
   {
   timer1set = 0;
   lock_detect_flag = 0;
   fj = jitter_freq[loop];
   if ((fj != 0.0) && (pdatain->jitter_amp != 0.0))
      {
      snprintf(plog_string,LOGFILE_LINELENGTH,"Applied jitter frequency = %s\n",add_units(fj,2,"Hz",value_string[0]));
      print_string_to_log(plog_string,pdatain);
      if ((loop == 0) && (pdatain->number_of_jitter_freq > 1) && (fj != 0.0) && (pdatain->jitter_amp != 0.0))
      	{
      	#ifndef SAVE_JITTER_TRANSFER_TRANS_PLL_OUTPUT_FILES
      		snprintf(plog_string,LOGFILE_LINELENGTH,
      		"Note: Individual output data files for each jitter frequency are\nnow removed after jitter analysis completes.\n");
         	print_string_to_log(plog_string,pdatain);
         #endif
      	}

      pdatain->TSTOP = pdatain->TSTART + pdatain->TSTART_jt + 1.50*((double) NUMBER_OF_TRANSFER_FUNCTION_JITTER_CYCLES/fj);
   
	   if ((fj - floor(fj)) != 0.0)
	      {
	      if (fj < 1e-06)
	         {
	         snprintf(plog_string,LOGFILE_LINELENGTH,"Low jitter frequency will not allow unique output file name.\n");
	         print_string_to_log(plog_string,pdatain);	
	         exit(0);
	         }
	      else
	      	snprintf(pffooter,SUFFIX1_LINELENGTH,"%.0fm.csv",fj/1e-03);
	      }
	   else
      	snprintf(pffooter,SUFFIX1_LINELENGTH,"%.0f.csv",fj);
      snprintf(pfnameout1,FILENAME_LINELENGTH,
      "%s_%s_%s",pdatain->poutput_base_filename,ptimestamp,pffooter);
      }
   else
   	{
      snprintf(pffooter,SUFFIX1_LINELENGTH,".csv");
      snprintf(pfnameout1,FILENAME_LINELENGTH,
      "%s_%s%s",pdatain->poutput_base_filename,ptimestamp,pffooter);
      }
      
   snprintf(plog_string,LOGFILE_LINELENGTH,"Output filename: %s\n",pfnameout1);
   print_string_to_log(plog_string,pdatain);
   snprintf(pfnameout0,FILENAME_LINELENGTH,"x_%s_%s_%s",
   pdatain->poutput_base_filename,ptimestamp,pffooter);

   snprintf(pfnameout2,FILENAME_LINELENGTH,"%s_%s_%s_%s",
   ppost_processed,pdatain->poutput_base_filename,ptimestamp,pffooter);
   
   /*Post-processed file no longer saved by default...comment out prompt*/
	
	#ifdef SAVE_POST_PROCESSED_FILE   
   	snprintf(plog_string,LOGFILE_LINELENGTH,"Completed reading, output file(post-processed) is %s\n",pfnameout2);
   	print_string_to_log(plog_string,pdatain);
	#endif
	
   fpw1 = fopen(pfnameout1,"w");
   if (fpw1 == NULL)
      {
	   snprintf(plog_string,LOGFILE_LINELENGTH,"Error opening filename \"%s\"....exiting\n",fnameout1);
	   print_string_to_log(plog_string,pdatain);
	   exit(0);
	   }
	else
		{
	   fclose(fpw1);
	   }
	   
	if (loop > 1)
		init_sig_stats(psig_stats,pdatain);

   /* Find center voltage and plot control voltage characteristic on first iteration (loop = 0)*/
   
   if (loop == 0)
   	{
	   if	(find_center_voltage(pdatain,psig_stats,max_error_percent_vcenter_voltage) != EXIT_SUCCESS)
	   	{
	   	snprintf(plog_string,LOGFILE_LINELENGTH,"Check VCO output frequency - control voltage function.\n");
	   	print_string_to_log(plog_string,pdatain);
	   	snprintf(plog_string,LOGFILE_LINELENGTH,"Did not find a voltage within its range\n");
	   	print_string_to_log(plog_string,pdatain);
	   	snprintf(plog_string,LOGFILE_LINELENGTH,"that produces the desired nominal frequency of %s.\n",
	   	add_units(pdatain->fo,4,"Hz",value_string[0]));
	   	print_string_to_log(plog_string,pdatain);
			compute_control_voltage_characteristic(pdatain,psig_stats,
			num_control_voltage_points,pcontrol_voltage_filename,ptimestamp,pdatain->plot_outputs,plot_normalized_freq);
	   	}
	   else
	   	compute_control_voltage_characteristic(pdatain,psig_stats,num_control_voltage_points,
			pcontrol_voltage_filename,ptimestamp,pdatain->plot_outputs,plot_normalized_freq);
		}

   /*Compute coefficients for differential equations*/

   compute(pw,pdatain->R1,pdatain->R2,pdatain->R3,pdatain->C1,pdatain->C2,
   pdatain->RS1,pdatain->RS2,pdatain->pd);      
   prpoint = (pdatain->TSTOP - pdatain->TSTART)/(pdatain->npoints - 1.0);

   /* If time increment causes a phase increment of larger than MIN_DELTAT_RELATIVE_VCO_PERIOD_IN_UI UI
   reduce value of deltat to MIN_DELTAT_RELATIVE_VCO_PERIOD_IN_UI UI */

   if (pdatain->fo*pdatain->deltat > 0.99*MIN_DELTAT_RELATIVE_VCO_PERIOD_IN_UI)
      {
      pdatain->deltat = MIN_DELTAT_RELATIVE_VCO_PERIOD_IN_UI * (1.0/pdatain->fo);
      deltat_set_flag = 1;
      }
   if (fj != 0.0)
      {
      if (pdatain->deltat > (MIN_DELTAT_RELATIVE_VCO_PERIOD_IN_UI * (1/fj)))
      	{
         pdatain->deltat = MIN_DELTAT_RELATIVE_VCO_PERIOD_IN_UI * (1/fj);
         deltat_set_flag = 2;
         }
      }
   if (pdatain->deltat >= prpoint)
      {
      pdatain->deltat = prpoint;
      deltat_set_flag = 3;
      }
   if ((pdatain->pd == PFD) || (pdatain->pd == PFD_CP))
   	{
   	if ((pdatain->deltat > pdatain->tauh) && (pdatain->tauh != 0.0))
   		{
			pdatain->deltat = pdatain->tauh;
			deltat_set_flag = 4;
			}
   	if ((pdatain->deltat > pdatain->taucp_min) && (pdatain->taucp_min != 0.0))
   		{
			pdatain->deltat = pdatain->taucp_min;
			deltat_set_flag = 4;
			}
   	if ((pdatain->deltat > pdatain->tauff) && (pdatain->tauff != 0.0))
   		{
			pdatain->deltat = pdatain->tauff;
			deltat_set_flag = 5;
			}
   	if ((pdatain->deltat > pdatain->pfd_dff_deadzone_ui) && (pdatain->pfd_dff_deadzone_ui != 0.0))
   		{
			pdatain->deltat = pdatain->pfd_dff_deadzone_ui * (1.0/pdatain->fin);
			deltat_set_flag = 6;
			}
		snprintf(plog_string,LOGFILE_LINELENGTH,"Phase-frequency detector deadzone = %s\n",
		add_units(pdatain->pfd_dff_deadzone_ui,2,"UI",value_string[0]));
		print_string_to_log(plog_string,pdatain);
		}
   if ((pdatain->pd == DFF) && (pdatain->deltat > pdatain->tauff) && (pdatain->tauff != 0.0))
   	{
		pdatain->deltat = pdatain->tauff;
		deltat_set_flag = 5;
		}		
	if ((pdatain->D_delay > 0.0) && (pdatain->deltat > pdatain->D_delay/10.0))
		{
		pdatain->deltat = pdatain->D_delay/10.0;
		deltat_set_flag = 5;
		}

	if (pdatain->pd == PD_ALEXANDER)
		{		
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"Alexander phase detector threshold = %s\n",
		add_units(pdatain->alexander_pd_threshold_ui,2,"UI",value_string[0]));
		print_string_to_log(plog_string,pdatain);
		}
		
   /* Allocate memory for jitter transfer analysis based on NUMBER_OF_TRANSFER_FUNCTION_JITTER_CYCLES cycles of jitter */
   /* frequency and pdatain->deltat */
   
   if ((fj != 0.0) && (pdatain->jitter_amp != 0.0))
   	{
		jt_samples_buffer_size = (long int) pow(2.0,floor(log10(( NUMBER_OF_TRANSFER_FUNCTION_JITTER_CYCLES/fj)/pdatain->deltat)/log10(2.0)));
		
		#ifdef DEBUG_JITTER_TRANSFER_ARRAY_SIZE
			snprintf(plog_string,LOGFILE_LINELENGTH,
			"Updated (new, binary) value of jt_samples_buffer_size is %ld.\n",
			jt_samples_buffer_size);
			print_string_to_log(plog_string,pdatain);
		#endif

		if ( jt_samples_buffer_size > (long int) MAX_NUMBER_OF_TRANSFER_FUNCTION_SAMPLES)
			{
			
			#ifdef DEBUG_JITTER_TRANSFER_ARRAY_SIZE		
				snprintf(plog_string,LOGFILE_LINELENGTH,"jt_samples_buffer_size of %ld exceeds %.0f, limiting to %.0f.\n",
				jt_samples_buffer_size,MAX_NUMBER_OF_TRANSFER_FUNCTION_SAMPLES,
				MAX_NUMBER_OF_TRANSFER_FUNCTION_SAMPLES);
				print_string_to_log(plog_string,pdatain);
			#endif
			
			number_deltat_to_skip = (long int) floor(floor((double) NUMBER_OF_TRANSFER_FUNCTION_JITTER_CYCLES/(fj*pdatain->deltat))/((long int) MAX_NUMBER_OF_TRANSFER_FUNCTION_SAMPLES));
			jt_samples_buffer_size = (long int) MAX_NUMBER_OF_TRANSFER_FUNCTION_SAMPLES;
			}
		else
			number_deltat_to_skip = 1;
					
		if (((ptime_jt_samples = (double *) calloc(jt_samples_buffer_size + 1,sizeof(double))) == NULL) || ((pphasein_jt_samples = (double *) calloc(jt_samples_buffer_size + 1,sizeof(double))) == NULL) || ((pphaseo_jt_samples = (double *) calloc(jt_samples_buffer_size + 1,sizeof(double))) == NULL) || ((pphaseo_delayed_jt_samples = (double *) calloc(jt_samples_buffer_size + 1,sizeof(double))) == NULL))
			{
			printf("Error allocating memory to ptime_jt_samples, pphasein_jt_samples, pphaseo_jt_samples, or pphaseo_delayed_jt_samples in main.c!\n");
			exit(0);
			}
		#ifdef DEBUG_JITTER_TRANSFER_ARRAY_SIZE
			snprintf(plog_string,LOGFILE_LINELENGTH,
			"Value of jt_samples_buffer_size is %ld, number_deltat_to_skip = %ld.\n",
			jt_samples_buffer_size,number_deltat_to_skip);
			print_string_to_log(plog_string,pdatain);
		#endif
		
		for( jj = 0; jj < jt_samples_buffer_size + 1; jj++)
			{
			ptime_jt_samples[jj] = 0.0;
			pphasein_jt_samples[jj] = 0.0;
			pphaseo_jt_samples[jj] = 0.0;
			pphaseo_delayed_jt_samples[jj] = 0.0;
			}
		
		jj = 0; /*reset index for dynamic memory arrays */
		}

	/* Set value of minimum CLKIN width as noise filter to prevent short CLKIN pulses due to additive noise*/
	
	min_CLKIN_pulse_width = (0.05/pdatain->fin);
	
	print_limiting_deltat_set_flag(deltat_set_flag,pdatain);
	
   snprintf(plog_string,LOGFILE_LINELENGTH,"Value of deltat is %2.2e\n",pdatain->deltat);
   print_string_to_log(plog_string,pdatain);
   snprintf(plog_string,LOGFILE_LINELENGTH,"Data will be printed every %s.\n",add_units(prpoint,4,"s",value_string[0]));
   print_string_to_log(plog_string,pdatain);
   printstruct(pfnameout1,pdatain);
   
	if (pdatain->D_delay > 0.0)
		{
		pphaseo_sample_buffer_size = (long int) ceil(pdatain->D_delay/pdatain->deltat);
		if ((pphaseo_sample = (double *) calloc(pphaseo_sample_buffer_size,sizeof(double))) == NULL)
			{
			printf("Error allocating memory to pphaseo_sample in main.c!\n");
			exit(0);
			}
		for (ii = 0; ii < pphaseo_sample_buffer_size; ii++)
			pphaseo_sample[ii] = pdatain->phase0;
		}
   
   /* Define simulation time window for averaging */
   /* and allocate memory for history arrays */
   
   if (pdatain->lock_detect_tau != 0.0)
   	{
	   if ((int) floor(pdatain->lock_detect_tau/pdatain->deltat + 0.50) > MAX_HISTORY_LENGTH)
	   	{
	   	N_lock_detect = MAX_HISTORY_LENGTH;
	   	snprintf(plog_string,LOGFILE_LINELENGTH,
	   	"Sample signals every %d time points to determine lock status (%s)\n",
	   	N_lock_detect,add_units(((double) N_lock_detect*pdatain->deltat),3,"s",value_string[0]));
	   	print_string_to_log(plog_string,pdatain);
	   	history_length = MAX_HISTORY_LENGTH;
	   	}
	   else
	   	{
	   	N_lock_detect = (int) floor(pdatain->lock_detect_tau/pdatain->deltat + 0.50);
	   	snprintf(plog_string,LOGFILE_LINELENGTH,
	   	"Sample signals every %d time points to determine lock status (%s)\n",
	   	N_lock_detect,add_units(((double) N_lock_detect*pdatain->deltat),3,"s",value_string[0]));
	   	print_string_to_log(plog_string,pdatain);
	  	 	history_length = N_lock_detect;
	  	 	}
		snprintf(plog_string,LOGFILE_LINELENGTH,
	   "Lock detect thresholds:\n\tControl voltage tolerance about center voltage for phase-lock: %s\n",
	   add_units(pdatain->lock_detect_delta_vc_limit_percent,1,"%",value_string[0]));
	   print_string_to_log(plog_string,pdatain);
		snprintf(plog_string,LOGFILE_LINELENGTH,
	   "\tRelative phase tolerance for phase-lock: %s\n",
	   add_units(pdatain->lock_detect_rphase_pp_limit_ui,1,"UI",value_string[0]));
	   print_string_to_log(plog_string,pdatain);
		snprintf(plog_string,LOGFILE_LINELENGTH,
	   "\tRelative phase threshold to declare loss of phase-lock: %s\n",
	   add_units(pdatain->lock_detect_rphase_pp_loss_lock_limit_ui,1,"UI",value_string[0]));
	   print_string_to_log(plog_string,pdatain);
		}
	else
	   history_length = NOMINAL_HISTORY_LENGTH;

	pvc_history = (double *) calloc(history_length,sizeof(double));
	prphase_ui_history = (double *) calloc(history_length,sizeof(double));
	pdeltaf_vco_history = (double *) calloc(history_length,sizeof(double));
	
	if((pvc_history == NULL) || (prphase_ui_history == NULL) || (pdeltaf_vco_history == NULL))
		{
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"Memory allocation failure for pvc_history, prphase_ui_history, or pdeltaf_vco_history in main.c\n");
		print_string_to_log(plog_string,pdatain);
		exit(0);
		}

   /*Set counter to determine number of printed points (time_point_counter), initialize user notification counter k*/

   time_point_counter = 0;
   loopcount=1;
   k = 0;

   /*Initialize time*/

   time = 0.0;
   last_pd_event_time = 0.0;
   tsample = (1/freq)*(pdatain->phase0)/(2.0*pi);
   
   /* Initialize lock detect flag, lock detect counter, and lock time */
   
   lock_detect_flag = 0;
   lost_lock_detect_flag = 0;
   lock_detect_counter = 0;
   lock_time = 0.0;
   lost_lock_time = 0.0;

   /*Initialize pd_event to 0*/

   pd_event = 0;
   
   /* Initialize CLKIN on time and its maximum and minimum values */
   
   on_time_sec = 0.50/(pdatain->fin);
   min_on_time_sec = 1.0/EPSILON;
   max_on_time_sec = -1.0/EPSILON;
   CLKIN_rising_edge_flag = 0;
   CLKIN_rising_edge_time = 0.0;
   CLKIN_falling_edge_time = 0.0;
   CLKIN_falling_edge_flag = 0;
   number_of_blocked_transitions = 0;
   
   /* initialize VIN/VOUT delay time variables */
   
   if (is_datarecovery(pdatain->pd))
   	{
	   LAST_VIN = 0;
	   vin_rising_edge_flag = 0;
	   LAST_VOUT = 0;
	   vin_vout_rising_edge_delay = 0.0;
	   min_vin_vout_rising_edge_delay = 1.0/EPSILON;
	   max_vin_vout_rising_edge_delay = -1.0/EPSILON;
		sum_vin_vout_rising_edge_delay = 0.0;
		num_vout_rising_edges = 0;
		error_free_flag = 0;
		}
	
   /*Initialize phase of input and VCXO*/

   sig.phasein = pdatain->phase0;
   sig.phaseo = 0.0;
   vdata_phase0 = psig->phasein - 2.0*pi*((double) k0);
   sig.phaseo_delayed = 0.0;
   vco_clk_pos_edge_counter = 0;vco_clk_neg_edge_counter = 0;
   clk_pos_edge_counter = 0;clk_neg_edge_counter = 0;
   last_phasein = sig.phasein;
   last_phaseo_delayed = sig.phaseo_delayed;
   sig.phaseoq = sig.phaseo - pi/2.0;
   sig.phaseoq = sig.phaseo_delayed - pi/2.0;
   rphase = sig.phasein - sig.phaseo_delayed;
   sig.clk = 0;
   sig.clkq = 0;
   sig.vco_clk = 0;

   /*Initialize all voltages*/

   if (is_chargepump(pdatain->pd))
      sig.vpd = pdatain->logic_lo_pdsignal + pdatain->logic_hi_pdsignal;
   else
      sig.vpd = pdatain->logic_lo_pdsignal;
   sig.vs1 = sig.vs2 = 0.0;
   sig.vq1 = sig.vq2 = 0.0;
   sig.vq3 = sig.vq4 = 0.0;
  
   /*Let user know version of vco transfer function is being used*/
   
   if (pdatain->vc_model == USE_TANH_MODEL)
   	{
      snprintf(plog_string,LOGFILE_LINELENGTH,"Using tanh approximation to VCO transfer curve.\n");
      print_string_to_log(plog_string,pdatain);
      }
   else
      {
      snprintf(plog_string,LOGFILE_LINELENGTH,"Using polynomial of degree %d to approximate VCO transfer curve.\n",
      pdatain->vco_polynomial_degree-1);
      print_string_to_log(plog_string,pdatain);
      }
   
   /*Compute initial divided VCXO frequency at input to phase detector*/
   
   fo = pdatain->fo/pdatain->D;
   freq = fo*(1 + deltaf_vco(psig->vco,pdatain->vc_model,
   pdatain->pvc_polynomial,pdatain->vco_polynomial_degree));
   freqp = (freq - fo)/fo;
   
   /*If initvc is greater than zero, set phase detector to STATE = 1,
   otherwise set to STATE = 0*/

   if (pdatain->initvc > 0.0)
      {
      STATE = 1;
      }
   else
      STATE = 0;
   sig.vc1 = pdatain->initvc;
   sig.vc2 = pdatain->initvc;
   sig.vc = pdatain->initvc;
   sig.vco = sig.vc2;
   time0 = 0.0 - k0 * (1.0/fo);
  
  /*Initialize data array vdata[] and numperiods if using data recovery phase detector*/

   if (is_datarecovery(pdatain->pd))
   	{
		fpw2 = fopen(pdatain->pdatafilein, "r");
		first_sample(pvdata,fpw2,pdatain->pdatafilein,k0,NUMBER_OF_SAMPLES_STORED);
		numperiods = 0.0;
     
	/* Allocate memory for arrays to store sampled VIN and VOUT values for BER check */
	
		number_of_output_samples = 2 * pdatain->datafilein_number_of_lines;
		
		if ((pvin_sampled = (int *) calloc(pdatain->datafilein_number_of_lines,sizeof(int))) == NULL || (pvout_sampled = (int *) calloc(number_of_output_samples,sizeof(int))) == NULL)
			{
			snprintf(plog_string,LOGFILE_LINELENGTH,
			"Memory allocation failure for pvin_sampled or pvout_sampled in main.c\n");
			print_string_to_log(plog_string,pdatain);
			exit(0);
			}
     }

   /*Print header to file fpw1*/

   fpw1 = fopen(pfnameout1,"a"); 
	fprintf(fpw1,"Time(s),vpd(V),vs1(V),vs2(V),vc1(V),vc(V),phasein(UI),phaseo(UI),");
	fprintf(fpw1,"phaseo_delayed(UI),rphase(UI),vc2(V),VIN,CLK,VCO_CLK,CLKIN,");
	fprintf(fpw1,"freq (ppm from fin),VOUT\n");
           
   /*Start loop*/

   /*start = clock();*/
   flag_stoptime = 0;
   flag_add_phase_or_freq_step = 0;
   phase_step = 0.0;

   while (time < (pdatain->TSTOP + prpoint))
      {

	   /*printf("Time is %2.4e\n",time);*/
	   /*Compute relative phase of two waveforms - account for phase0*/
	
      if (time > pdatain->TSTART_jt)
			{
			sig.phasein = (2.0 * pi * pdatain->fin * time) + (pdatain->jitter_amp*pi*sin(2.0*pi*fj*(time-initjitter_time))) + pdatain->phase0;
          }
      else
			{
	   	sig.phasein = (2.0 * pi * pdatain->fin * time) +  pdatain->phase0;
			initjitter_time = time;
			}

      if (time > pdatain->phase_freq_step_time_in_sec) 
         {
         freq_step = (pdatain->freq_step_ppm * 1.0e-06) * (pdatain->fin) * 2.0 * pi * (time - pdatain->phase_freq_step_time_in_sec);
         sig.phasein = sig.phasein + freq_step;
         phase_step = (pdatain->phase_step_in_sec) * (pdatain->fin) * 2.0 * pi;
         sig.phasein = sig.phasein + phase_step;
         if (flag_add_phase_or_freq_step == 0)
            {
            if (fabs(pdatain->freq_step_ppm) > 0.0 )
            	{
            	snprintf(plog_string,LOGFILE_LINELENGTH,
            	"Added frequency step of %1.2e ppm or %4.4e Hz at time = %2.6e.\n",
            	pdatain->freq_step_ppm, (pdatain->freq_step_ppm)*(pdatain->fin)*1e-06,time);
            	print_string_to_log(plog_string,pdatain);
            	}
            if (fabs(pdatain->phase_step_in_sec) > 0.0 )
            	{
            	snprintf(plog_string,LOGFILE_LINELENGTH,
            	"Added phase step of %1.4e sec at time = %2.6e.\n",
            	pdatain->phase_step_in_sec,time);
            	print_string_to_log(plog_string,pdatain);
            	}
            flag_add_phase_or_freq_step = 1;
            }
         }
      else
         {
         sig.phasein = sig.phasein;
         }
         
         /*Add SSC to phasein if time exceeds ssc start time*/         
         
     if (time > pdatain->ssc_start_time_in_sec) 
         {
         if (pdatain->freq_ssc*fmod((time-pdatain->ssc_start_time_in_sec),1.0/(pdatain->freq_ssc)) < 0.50)
            {
            freq_ssc_ppm = 0.0 - pdatain->max_neg_ssc_ppm + ssc_slope*fmod((time-pdatain->ssc_start_time_in_sec),1.0/pdatain->freq_ssc);
            }
         else
            {
            freq_ssc_ppm  = (pdatain->max_pos_ssc_ppm-ssc_slope*(fmod((time-pdatain->ssc_start_time_in_sec),1.0/pdatain->freq_ssc)-0.5*(1.0/pdatain->freq_ssc)));
            }
       if(rkstep1(time-pdatain->ssc_start_time_in_sec,time-pdatain->ssc_start_time_in_sec+pdatain->deltat,unity_time_constant,freq_ssc_ppm,&ssc_phase,phase_max,phase_min,errmax,MAX_ITERATIONS) == 1)
            {
            snprintf(plog_string,LOGFILE_LINELENGTH,"Error in rsktep1!\n");
            print_string_to_log(plog_string,pdatain);
            exit(EXIT_FAILURE);
            }
         sig.phasein = sig.phasein + 2.0*pi*pdatain->fin*1e-06*ssc_phase;
         }
      else
         {
         sig.phasein = sig.phasein;
         }
         
	   /*Add random phase noise of maximum magnitude "noise_amp_pp" UIpp to fin*/
	   
	   if (pdatain->noise_amp != 0.0)
	      {
	      switch (pdatain->noise_type)
	         {
	         case GAUSSIAN_NOISE:
	            noise = random_gaussian_clocknoise(pdatain->noise_amp,0.0,seed);
	            break;
	         case UNIFORM_NOISE:
	         	noise = random_uniform_clocknoise(pdatain->noise_amp,0.0,seed);
	            break;
	         default:
	            snprintf(plog_string,LOGFILE_LINELENGTH,"In main.c, did NOT recognize noise type! Exiting...\n");
	            print_string_to_log(plog_string,pdatain);
	            exit(0);
	         }
	      
	      if(rkstep1(time,time+pdatain->deltat,tau_noise_bandwidth_sec,noise,&filtered_noise,10.0,
         -10.0,errmax,MAX_ITERATIONS) == 1)
            {
            snprintf(plog_string,LOGFILE_LINELENGTH,
            "Error in runge-kutta routine for noise, program exits!\n");
            print_string_to_log(plog_string,pdatain);
            time = pdatain->TSTOP;
            } 
	      if (noise > unfiltered_noise_max)
	         unfiltered_noise_max = noise;
	      if (noise < unfiltered_noise_min)
	         unfiltered_noise_min = noise;
	      if (fabs(noise) > 0.50)
	      	unfiltered_noise_counter++;
	      
	      if (filtered_noise > filtered_noise_max)
	         filtered_noise_max = filtered_noise;
	      if (filtered_noise < filtered_noise_min)
	         filtered_noise_min = filtered_noise;
	      if (fabs(filtered_noise) > 0.50)
	      	filtered_noise_counter++;

	      #ifdef SAVE_NOISE_SAMPLES_TO_FILE
				if (time == 0.0)
					{
					sprintf(pfilename_sampled_noise,"sample_unfiltered_filtered_%s_%s_noise_%s.csv",
					add_units_underscore(pdatain->noise_amp,1,"UI",value_string[0]),
					add_units_underscore(pdatain->noise_bandwidth_Hz,1,"Hz",value_string[1]),
					ptimestamp);
					fpw200 = fopen(pfilename_sampled_noise, "w");
					fprintf(fpw200,"Timepoint,Unfiltered noise (UI),Filtered noise (UI)\n");
					fprintf(fpw200,"%ld,%1.8e,%1.8e\n",loopcount,noise,filtered_noise);
					}
				else
					fprintf(fpw200,"%ld,%1.8e,%1.8e\n",loopcount,noise,filtered_noise);
				if (time >= (pdatain->TSTOP - EPSILON))
					fclose(fpw200);
			#endif
					
	      sig.phasein = sig.phasein + 2.0*pi*filtered_noise;
	      }

		rphase = sig.phaseo_delayed - sig.phasein;
	  
		CLKIN = find_CLKIN_noise_filtered(psig,pdatain,time,&CLKIN_rising_edge_time,&CLKIN_rising_edge_flag,
		&CLKIN_falling_edge_time,&CLKIN_falling_edge_flag,LAST_CLKIN,min_CLKIN_pulse_width,&on_time_sec,
		&number_of_blocked_transitions);

		if (numperiods > 1)
			{
			if (on_time_sec > max_on_time_sec)
				max_on_time_sec = on_time_sec;
			if (on_time_sec < min_on_time_sec)
				min_on_time_sec = on_time_sec;
			}
  			
		if ((CLKIN == 1) && (LAST_CLKIN == 0))
         numperiods++;

		if ((time > (pdatain->TSTART_jt + 1/fj)) && (fj != 0.0))
			{
			phasein_ui = sig.phasein/(2.0 * pi);
			phaseout_ui = sig.phaseo/(2.0 * pi);
			phaseout_delayed_ui = sig.phaseo_delayed/(2.0 * pi);
			rphase_ui = phaseout_delayed_ui - phasein_ui;
			if (rphase_ui > rphase_ui_max)
				rphase_ui_max = rphase_ui;
			if (rphase_ui < rphase_ui_min)
            rphase_ui_min = rphase_ui;
         }

		/* Delay phaseo by divider delay */
		
		if (pdatain->D_delay > 0.0)
			{
			psig->phaseo_delayed = pphaseo_sample[(loopcount - 1) % pphaseo_sample_buffer_size];
			pphaseo_sample[(loopcount - 1) % pphaseo_sample_buffer_size] = psig->phaseo;
			}
		else
			psig->phaseo_delayed = psig->phaseo;
 	  
      sig.vco_clk = divided_clockstate(sig.vco_clk,&vco_clk_pos_edge_counter,
      &vco_clk_neg_edge_counter,0.0,sig.phaseo,pdatain->vco_clk_dutycycle,
      pdatain->D,pdatain);
      if (pdatain->D != 1.0)
      	{
      	sig.clk = divided_clockstate(sig.clk,&clk_pos_edge_counter,
      	&clk_neg_edge_counter,0.0,sig.phaseo_delayed,
      	pdatain->clk_dutycycle,unity_divider,pdatain); 
      	}
      else
      	{
      	sig.clk = divided_clockstate(sig.clk,&clk_pos_edge_counter,
			&clk_neg_edge_counter,0.0,sig.phaseo_delayed,
			pdatain->vco_clk_dutycycle,unity_divider,pdatain); 
      	}
      		
      switch (pdatain->pd)
         {
         case PFD: case PFD_CP:
            if (pfd_with_delay(psig,time,pdatain->tauh,pdatain->tauff,
            pdatain->taucp_min,pdatain->pfd_dff_deadzone_ui,pSTATE) == 1)
            	{
            	if ((time - last_pd_event_time) > 0.999*pdatain->taucp_min)
            		{
               	pd_event = 1;
               	last_pd_event_time = time;
               	}
               }
            break;
         case DFF:
            if (pddff(psig,time,pdatain,pSTATE) == 1)
               pd_event = 1;
            break;
         case PD_WINDOWED:
            if (pd_windowed(psig,time,pdatain,pSTATE) == 1)
               pd_event = 1;
            break;
         case PD_EXOR:
            if (pd_exor_with_delay(psig,CLKIN,time,pdatain,pSTATE) == 1)
               pd_event = 1;
            break;
         case NONE_VPD_HIGH:
            if (sethigh(psig,deadzone*freq,pSTATE) == 1)
               pd_event = 1;
            break;
         case NONE_VPD_LOW:
            if (setlow(psig,deadzone*freq,pSTATE) == 1)
               pd_event = 1;
            break;
         case HOGGE:
                                      
            if ((CLKIN == 1) && (LAST_CLKIN == 0))
            	{          	
            	/*Advance data in data vector if necessary*/          	
            	next_sample(pvdata,fpw2,pdatain->pdatafilein,NUMBER_OF_SAMPLES_STORED);
            	VIN = pvdata[k0];
            	}
            	
            if (pd_hogge_with_delay(psig,time,pdatain,VIN,pSTATE,perror) == EXIT_SUCCESS)
               {
               pd_event = 1;
               VOUT = psig->vq2;
               }
            else
               {
               snprintf(plog_string,LOGFILE_LINELENGTH,
               "Error occured in pdhogge at time = %2.6e...exiting\n",time);
               print_string_to_log(plog_string,pdatain);
               snprintf(plog_string,LOGFILE_LINELENGTH,"Error code was %d.\n",*perror);
               print_string_to_log(plog_string,pdatain);
               exit(0);
               }
            break;
         case PD_ALEXANDER:
                                      
            if ((CLKIN == 1) && (LAST_CLKIN == 0))
            	{          	
            	/*Advance data in data vector if necessary*/          	
            	next_sample(pvdata,fpw2,pdatain->pdatafilein,NUMBER_OF_SAMPLES_STORED);
            	VIN = pvdata[k0];
            	}
                                             
            if (pd_alexander_with_delay(psig,pdatain,time,VIN,pSTATE,perror) == EXIT_SUCCESS)
               {
               pd_event = 1;
               VOUT = psig->vq3;
               }
            else
               {
               snprintf(plog_string,LOGFILE_LINELENGTH,
               "Error occured in pd_alexander() at time = %2.6e...exiting\n",time);
               print_string_to_log(plog_string,pdatain);
               snprintf(plog_string,LOGFILE_LINELENGTH,"Error code was %d.\n",*perror);
               print_string_to_log(plog_string,pdatain);
               exit(0);
               }
         	break;
         default:
            snprintf(plog_string,LOGFILE_LINELENGTH,
            "In main.c, did NOT recognize phase detector type! Exiting...\n");
            print_string_to_log(plog_string,pdatain);
            exit(0);
         }

      if (pd_event == 1)
         {       
         switch (STATE)
            {
            case 0:
               compute(pw,pdatain->R1,pdatain->R2,pdatain->R3,pdatain->C1,pdatain->C2,
               pdatain->RS1,pdatain->RS2,pdatain->pd);
               if (pdatain->pd != PD_ALEXANDER)
               	sig.vpd = pdatain->logic_lo_pdsignal;
               else
                	vpd_amp_alexander(psig,pdatain,pdatain->alexander_pd_threshold_ui,
                	pdatain->logic_lo_pdsignal);
               pd_event = 0;
               break;
            case 1:
               compute(pw,pdatain->R1,pdatain->R2,pdatain->R3,pdatain->C1,pdatain->C2,
               pdatain->RS1,pdatain->RS2,pdatain->pd);
               if (pdatain->pd != PD_ALEXANDER)
               	sig.vpd = pdatain->logic_hi_pdsignal;
               else
               	vpd_amp_alexander(psig,pdatain,pdatain->alexander_pd_threshold_ui,
               	pdatain->logic_hi_pdsignal);
               pd_event = 0;
               break;
            case 3:
               /*High-impedance state*/
               if (!is_chargepump(pdatain->pd))
                  {
	               if (pdatain->pd != PD_ALEXANDER)
	               	sig.vpd = (pdatain->logic_lo_pdsignal + pdatain->logic_hi_pdsignal)/2.0;
						else
							vpd_amp_alexander(psig,pdatain,pdatain->alexander_pd_threshold_ui,
							(pdatain->logic_lo_pdsignal + pdatain->logic_hi_pdsignal)/2.0);
                  compute(pw,RZ,pdatain->R2,pdatain->R3,pdatain->C1,pdatain->C2,
                  pdatain->RS1,pdatain->RS2,pdatain->pd);               
                  }
               else
                  {
                  compute(pw,pdatain->R1,pdatain->R2,pdatain->R3,pdatain->C1,pdatain->C2,
                  pdatain->RS1,pdatain->RS2,pdatain->pd);
                  sig.vpd = pdatain->logic_lo_pdsignal + pdatain->logic_hi_pdsignal;
                  }
               pd_event = 0;
               break;
            default:
               {
               snprintf(plog_string,LOGFILE_LINELENGTH,"Ended up in default state!! STATE = %d.\n", STATE);
               print_string_to_log(plog_string,pdatain);
               exit(0);
               }
            }
         } 
              
      /*Notify user of progress*/
   
      if (fabs(time - (k * pdatain->TSTOP/NUMBER_OF_PROGRESS_POINTS)) < pdatain->deltat)
         {
         if (loopcount > 1)
         	{
				find_moving_averages(psig,pdatain,history_length,loopcount,
				psig_stats,pvc_history,prphase_ui_history,pdeltaf_vco_history);
				} 		     
         snprintf(plog_string,LOGFILE_LINELENGTH,"Time = %s %3.0f %% complete, Vc = %s\n",
         add_units(time,3,"s",value_string[0]),100*time/pdatain->TSTOP,
         add_units(sig.vc,3,"V",value_string[1]));
         print_string_to_log(plog_string,pdatain);
			snprintf(plog_string,LOGFILE_LINELENGTH,"\tAverages: vc = %s (deltaV = %s), relative phase = %s (%s).\n",
			add_units(psig_stats->vc_running_average,3,"V",value_string[2]),
			add_units(100.0*(psig_stats->vc_running_average - psig_stats->vcenter_voltage)/psig_stats->vcenter_voltage,2,"%",value_string[3]),
			add_units(psig_stats->rphase_ui_running_average,3,"UI",value_string[4]),
			add_units(psig_stats->rphase_ui_history_pp,3,"UIpp",value_string[5]));
			print_string_to_log(plog_string,pdatain);
         k++;
         }
      
		if (pdatain->lock_detect_tau != 0.0)
			{
			lock_detect_time = ((double) lock_detect_counter)*((double) N_lock_detect)*pdatain->deltat + pdatain->TSTART;
			if (((lock_detect_time - time) < pdatain->deltat) && (fabs(lock_detect_time - time) < fabs(lock_detect_time - (time + pdatain->deltat))))
				{
				find_moving_averages(psig,pdatain,history_length,loopcount,psig_stats,
				pvc_history,prphase_ui_history,pdeltaf_vco_history);	
				
				#ifdef DEBUG_LOCK_DETECT
					if (time == 0.0)
						{
						fpw6 = fopen("debug_lock_status.csv","w");
						fprintf(fpw6,"Lock status time (sec),loopcount,lock_detect_counter,lock_detect_flag,");
						fprintf(fpw6,"sig_stats.vcenter_voltage,sig_stats.vc_running_average,sig_stats.vc_history_pp,");
						fprintf(fpw6,"sig_stats.sigma_vc,sig_stats.rphase_ui_running_average,sig_stats.rphase_ui_history_pp,");
						fprintf(fpw6,"sig_stats.sigma_rphase_ui,sig_stats.relative_phase_slope_average,sig_stats.sigma_relative_phase_slope_average\n");
						}
					fprintf(fpw6,"%1.12e,%ld,%ld,%d,",time,loopcount,lock_detect_counter,lock_detect_flag);			
					fprintf(fpw6,"%1.12e,%1.12e,%1.12e,",
					psig_stats->vcenter_voltage,psig_stats->vc_running_average,psig_stats->vc_history_pp);
					fprintf(fpw6,"%1.12e,%1.12e,%1.12e,",
					psig_stats->sigma_vc,psig_stats->rphase_ui_running_average,
					psig_stats->rphase_ui_history_pp);
					fprintf(fpw6,"%1.12e,%1.12e,%1.12e\n",
					psig_stats->sigma_rphase_ui,psig_stats->relative_phase_slope_average,
					psig_stats->sigma_relative_phase_slope_average);
					
				#endif

				detect_lock(psig,pdatain,lock_detect_counter,time,psig_stats,&lock_detect_flag,&lock_time,
				&lost_lock_detect_flag,&lost_lock_time);
				lock_detect_counter++;
				}
			}
		#ifdef SAVE_DEBUG_CDR_PLL_SIGNALS
			if (is_datarecovery(pdatain->pd))
				{
				if (time == 0.0)
   				{
					snprintf(pdebug_filename,FILENAME_LINELENGTH,
					"debug_cdr_signals.csv");
					fpw5 = fopen(pdebug_filename,"w");
					fprintf(fpw5,"Time,CLKIN,VIN,sig.clk,sig.vco_clk,vpd (normalized),STATE,rphase_ui,sig.phasein,VOUT\n");
					fprintf(fpw5,"%1.12e,%d,%d,%d,%d,%1.12e,%d,%1.12e,%1.12e,%d\n",
					time,CLKIN,VIN,psig->clk,psig->vco_clk,
					normalize_vpd(pdatain,psig->vpd),STATE,
					(sig.phaseo_delayed - sig.phasein)/(2.0*pi),
					sig.phasein,VOUT);
					}
				else
					{
					fprintf(fpw5,"%1.12e,%d,%d,%d,%d,%1.12e,%d,%1.12e,%1.12e,%d\n",
					time,CLKIN,VIN,psig->clk,psig->vco_clk,
					normalize_vpd(pdatain,psig->vpd),STATE,
					(sig.phaseo_delayed - sig.phasein)/(2.0*pi),
					sig.phasein,VOUT);
					}
				}
			else
				{
				if (time == 0.0)
   				{
					snprintf(pdebug_filename,FILENAME_LINELENGTH,
					"debug_pll_signals.csv");
					fpw5 = fopen(pdebug_filename,"w");
					fprintf(fpw5,"Time,CLKIN,sig.clk,vpd (normalized),STATE,sig.phasein,rphase_ui\n");
					fprintf(fpw5,"%1.12e,%d,%d,%1.12e,%d,%1.12e,%1.12e\n",
					time,CLKIN,psig->clk,normalize_vpd(pdatain,psig->vpd),STATE,
					sig.phasein,(sig.phaseo_delayed - sig.phasein)/(2.0*pi));
					}
				else
					{
					fprintf(fpw5,"%1.12e,%d,%d,%1.12e,%d,%1.12e,%1.12e\n",
					time,CLKIN,psig->clk,normalize_vpd(pdatain,psig->vpd),STATE,
					sig.phasein,(sig.phaseo_delayed - sig.phasein)/(2.0*pi));
					}
				}
			if (time > (pdatain->TSTOP - pdatain->deltat/2.0))
				fclose(fpw5);
		#endif
		
		if ((pdatain->number_of_jitter_freq > 0) && (fj != 0.0) && (pdatain->jitter_amp != 0.0) && (time >= (pdatain->TSTOP + prpoint - NUMBER_OF_TRANSFER_FUNCTION_JITTER_CYCLES/fj)) && (loopcount % number_deltat_to_skip == 0))
			{
			if (jj == 0)
				{
				snprintf(plog_string,LOGFILE_LINELENGTH,"Entered loop to write jt arrays at time = %1.8e\n",time);
				print_string_to_log(plog_string,pdatain);
				}
			if (jj > jt_samples_buffer_size) 
				{
				if (jj_flag == 0)
					{
					snprintf(plog_string,LOGFILE_LINELENGTH,
					"jj = %ld and is equal to maximum jitter transfer array",
					jj);
					print_string_to_log(plog_string,pdatain);
					snprintf(plog_string,LOGFILE_LINELENGTH,
					" size of (jt_samples_buffer_size + 1) at time = %1.8e\n",
					time);
					print_string_to_log(plog_string,pdatain);
					snprintf(plog_string,LOGFILE_LINELENGTH,
					"No longer collecting samples.\n");
					print_string_to_log(plog_string,pdatain);
					}
				jj_flag = 1;
				}
			else
				{
				pphasein_jt_samples[jj] = (sig.phasein - (2.0 * pi * pdatain->fin * time))/(2.0 * pi) - pdatain->phase0;
				pphaseo_jt_samples[jj] = sig.phaseo/(2.0 * pi) -  fo * time;
				pphaseo_delayed_jt_samples[jj] = sig.phaseo_delayed/(2.0 * pi) -  fo * time;
				ptime_jt_samples[jj] = time;
				jj++;
				}
			}
	
	   /*Print data to file if time > TSTART and time is close to printstep*/
	   
      #ifndef SAVE_FULL_OUTPUT_FILE
      	prtime = ((double) time_point_counter)*prpoint + pdatain->TSTART;
      #endif
    
      #ifdef SAVE_FULL_OUTPUT_FILE
      	prtime = ((double) time_point_counter)*pdatain->deltat + pdatain->TSTART;      	
      #endif

      /*if (fabs(prtime - time) < pdatain->deltat/2.0 )*/
      
      if ((fabs(prtime - time) < pdatain->deltat) && (fabs(prtime - time) < fabs(prtime - (time + pdatain->deltat))))
         {
			phasein_ui = sig.phasein/(2.0 * pi);
			phaseout_ui = sig.phaseo/(2.0 * pi);
			phaseout_delayed_ui = sig.phaseo_delayed/(2.0 * pi);
			rphase_ui = phaseout_delayed_ui - phasein_ui;
			rphase = sig.phaseo_delayed - sig.phasein;
			phasein_ui = (sig.phasein - (2.0 * pi * pdatain->fin * time))/(2.0 * pi);
			phaseout_ui = sig.phaseo/(2.0 * pi) -  fo * time;
			phaseout_delayed_ui = sig.phaseo_delayed/(2.0 * pi) -  fo * time;
			
			snprintf(pcsv_line,CSV_FILE_LINELENGTH,
			"%1.12e,%1.12e,%1.12e,%1.12e,%1.12e,%1.12e,%1.12e,%1.12e,%1.12e,%1.12e,%1.12e,%d,%d,%d",
			time,sig.vpd,sig.vs1,sig.vs2,
			sig.vc1,sig.vc,phasein_ui,phaseout_ui,
			phaseout_delayed_ui,rphase_ui,sig.vc2,VIN,sig.clk,sig.vco_clk);
			if (strlen(pcsv_line) == CSV_FILE_LINELENGTH)
				{
				snprintf(plog_string,LOGFILE_LINELENGTH,
				"Output line will exceed %d characters! Reduce linelength to less than %d characters.\n",
				CSV_FILE_LINELENGTH,CSV_FILE_LINELENGTH);
				exit(0);
				}
			else
				{
				strncpy(pcsv_line_temp,pcsv_line,CSV_FILE_LINELENGTH);
				snprintf(pcsv_line,LOGFILE_LINELENGTH,"%s,%d,%1.12e,%d",
				pcsv_line_temp,CLKIN,freqp/1e-06,VOUT);
				if (strlen(pcsv_line) == CSV_FILE_LINELENGTH)
					{
					snprintf(plog_string,LOGFILE_LINELENGTH,
					"Output line will exceed %d characters! Reduce linelength to less than %d characters.\n",
					CSV_FILE_LINELENGTH,CSV_FILE_LINELENGTH + 1);
					print_string_to_log(plog_string,pdatain);
					exit(0);
					}
				fprintf(fpw1, "%s\n", pcsv_line);
				}
         ++time_point_counter;
      	}
      
      if(rkstep5(time,time+pdatain->deltat,pw,psig,fo,pdatain->vc_model,pdatain->pvc_polynomial,
      pdatain->vco_polynomial_degree,pdatain->vmax,pdatain->vmin,errmax,MAX_ITERATIONS) == 1)
         {
         snprintf(plog_string,LOGFILE_LINELENGTH,"Error in runge-kutta routine, program exits!\n");
         print_string_to_log(plog_string,pdatain);
         time = pdatain->TSTOP;
         }
      
      /*Add effect of VCO pole with time constant pdatain->tau. Add as isolated pole as often some stages exist between
            loop filter and varactor (which sets pole)*/
            
     if(rkstep1(time,time+pdatain->deltat,pdatain->vco_tau,psig->vc2,&(sig.vco),pdatain->logic_hi_pdsignal,
      pdatain->logic_lo_pdsignal,errmax,MAX_ITERATIONS) == 1)
         {
         snprintf(plog_string,LOGFILE_LINELENGTH,"Error in runge-kutta routine for vco, program exits!\n");
         print_string_to_log(plog_string,pdatain);
         time = pdatain->TSTOP;
         } 
            
      sig.phaseoq = sig.phaseo - pdatain->quadphase; /*Create quadrature version of output clock phase*/
      time = time + pdatain->deltat;

	/* Since frequency changes almost instantaneously when varactor
	capacitance changes (but amplitude of signal lags by 2L1/R) eliminate
	the time delay due to resonator response on frequency*/
		
      freqp = deltaf_vco(sig.vco,pdatain->vc_model,pdatain->pvc_polynomial,pdatain->vco_polynomial_degree);
      freq = (1 + freqp)*fo;
      pvc_history[(loopcount - 1) % history_length] = psig->vc;
      prphase_ui_history[(loopcount - 1) % history_length] = (psig->phaseo_delayed - psig->phasein)/(2.0 * pi);
      /* pdeltaf_vco_history[(loopcount - 1) % history_length] = deltaf_vco(psig->vco,
      pdatain->vc_model,pdatain->pvc_polynomial,pdatain->vco_polynomial_degree); */
      if ((psig->phasein - last_phasein) != 0.0)
      	{
      	/* pdeltaf_vco_history[(loopcount - 1) % history_length] = ((psig->phaseo_delayed - last_phaseo_delayed) - (psig->phasein - last_phasein))/(psig->phasein - last_phasein); */
      	pdeltaf_vco_history[(loopcount - 1) % history_length] = freqp;
      	}
      else
      	{
      	/* pdeltaf_vco_history[(loopcount - 1) % history_length] = 0.0; */
      	pdeltaf_vco_history[(loopcount - 1) % history_length] = freqp;
      	}
      last_phaseo_delayed = psig->phaseo_delayed;
      last_phasein = psig->phasein;
      if ((lock_detect_flag == 1) && is_datarecovery(pdatain->pd))
     		{
     		if (DETECT_AND_PRINT_BER == TRUE)
     			{
      		save_vin_vout_and_compare_ber(pvin_sampled,pvout_sampled,lock_detect_flag,
      		CLKIN,VIN,pdatain->datafilein_number_of_lines,psig->clk,
      		VOUT,number_of_output_samples,time,&pdelay,&error_free_flag,
      		&ber,&error_free_sample_size,pdatain);
      		#ifdef DEBUG_SAVE_VIN_VOUT_AND_COMPARE
	      		if ((lock_detect_flag == 1) && (numperiods % 50 == 0))
	      			{
	      			sprintf(pfilename_vin_vout_sampled,"vin_vout_sampled_noise_amp_%s_%ld.csv",
	      			add_units_underscore(pdatain->noise_amp,1,"UI",value_string[0]),numperiods);
	      			fpw100 = fopen(pfilename_vin_vout_sampled, "w");
	      			fprintf(fpw100,"k,numperiods,vin,vout,lock_detect_flag,error_free_flag\n");
	      			for (ll = 0; ll < number_of_output_samples; ll++)
	      				{
	      				if (ll < pdatain->datafilein_number_of_lines)
	      					fprintf(fpw100,"%d,%ld,%d,%d,%d,%d\n",ll,numperiods,pvin_sampled[ll],pvout_sampled[ll],
	      					lock_detect_flag,error_free_flag);
	      				else
	      					fprintf(fpw100,"%d,%ld,NaN,%d,%d,%d\n",ll,numperiods,pvout_sampled[ll],
	      					lock_detect_flag,error_free_flag);
	      				}
	      			fclose(fpw100);
	      			}
	      	#endif
	      	if (error_free_flag == 1)
	      		{
		      	if ((VOUT == 1) && (LAST_VOUT == 0) && (vin_rising_edge_flag == 1))
		      		{
		      		vin_vout_rising_edge_delay = time - time_vin_rising_edge;
		      		sum_vin_vout_rising_edge_delay = sum_vin_vout_rising_edge_delay + vin_vout_rising_edge_delay;
		      		num_vout_rising_edges++;
		      		vin_rising_edge_flag = 0;
		      		if (vin_vout_rising_edge_delay > max_vin_vout_rising_edge_delay)
		      			max_vin_vout_rising_edge_delay = vin_vout_rising_edge_delay;
		      		if (vin_vout_rising_edge_delay < min_vin_vout_rising_edge_delay)
		      			min_vin_vout_rising_edge_delay = vin_vout_rising_edge_delay;
		      		}
		      	if ((VIN == 1) && (LAST_VIN == 0))
		      		{
		      		time_vin_rising_edge = time;
		      		vin_rising_edge_flag = 1;
		      		}
	      		}
	      	else
	      		{
					LAST_VIN = 0;
				   vin_rising_edge_flag = 0;
				   LAST_VOUT = 0;
				   vin_vout_rising_edge_delay = 0.0;
				   min_vin_vout_rising_edge_delay = 1.0/EPSILON;
				   max_vin_vout_rising_edge_delay = -1.0/EPSILON;
					sum_vin_vout_rising_edge_delay = 0.0;
					num_vout_rising_edges = 0;
					}     	
     			}
     		else
     			{
      		save_vin_vout_and_compare(pvin_sampled,pvout_sampled,lock_detect_flag,
      		CLKIN,VIN,pdatain->datafilein_number_of_lines,psig->clk,VOUT,
      		number_of_output_samples,time,&pdelay,&error_free_flag,pdatain);
      		}
      	if ((pdelay != last_pdelay) && (lock_detect_flag == 1) && (pdelay != -1))
      		{
      		last_pdelay = pdelay;
      		last_loopcount = loopcount;
      		}
      	}
      loopcount++;
      LAST_CLKIN = CLKIN;
      LAST_VIN = VIN;
      LAST_VOUT = VOUT;
      last_error_free_flag = error_free_flag;
	   }
	   fclose(fpw1);
	   free(pvc_history);
		free(prphase_ui_history);
		free(pdeltaf_vco_history);
		if (is_datarecovery(pdatain->pd))
			{
			free(pvin_sampled);
			free(pvout_sampled);
			}
   	
   	#ifdef DEBUG_DIVIDED_CLOCKSTATE
   		snprintf(plog_string,LOGFILE_LINELENGTH,
			"vco_clk_pos_edge_counter = %ld, vco_clk_neg_edge_counter = %ld\n",
			vco_clk_pos_edge_counter,vco_clk_neg_edge_counter);
			print_string_to_log(plog_string,pdatain); 
   		snprintf(plog_string,LOGFILE_LINELENGTH,
			"clk_pos_edge_counter = %ld, clk_neg_edge_counter = %ld\n",
			clk_pos_edge_counter,clk_neg_edge_counter);
			print_string_to_log(plog_string,pdatain);
	   #endif
		   		
		#ifdef DEBUG_LOCK_DETECT
			snprintf(plog_string,LOGFILE_LINELENGTH,
			"Lock detect debug file is \"debug_lock_status.csv\".\n");
			print_string_to_log(plog_string,pdatain); 
	   	fclose(fpw6);
	   #endif
		
		#ifdef DEBUG_CLKIN_DUTY_CYCLE
			snprintf(plog_string,LOGFILE_LINELENGTH,
			"Maximum CLKIN duty cycle = %s (± %s), Minimum CLKIN duty cycle = %s (± %s) (difference of %s)\n",
			add_units(100.0*max_on_time_sec*pdatain->fin,2,"%",value_string[0]),
			add_units(100.0*pdatain->deltat*pdatain->fin,1,"%",value_string[1]),
			add_units(100.0*min_on_time_sec*pdatain->fin,2,"%",value_string[2]),
			add_units(100.0*pdatain->deltat*pdatain->fin,1,"%",value_string[3]),
			add_units(100.0*(max_on_time_sec - min_on_time_sec)*pdatain->fin,1,"%",value_string[4]));
			print_string_to_log(plog_string,pdatain);
			if (number_of_blocked_transitions > 0)
				{
				snprintf(plog_string,LOGFILE_LINELENGTH,
				"Number of blocked CLKIN transitions (clock glitches) due to noise = %ld.\n",
				number_of_blocked_transitions);
				print_string_to_log(plog_string,pdatain);
				}
			if (is_datarecovery(pdatain->pd) && (error_free_flag == 1))
				{
				snprintf(plog_string,LOGFILE_LINELENGTH,
				"Maximum delay between VOUT and VIN = %s (± %s)\nMinimum delay between VOUT and VIN = %s (± %s) (difference of %s)\n",
				add_units(max_vin_vout_rising_edge_delay*pdatain->fin,2,"UI",value_string[0]),
				add_units(pdatain->deltat*pdatain->fin,2,"UI",value_string[1]),
				add_units(min_vin_vout_rising_edge_delay*pdatain->fin,2,"UI",value_string[2]),
				add_units(pdatain->deltat*pdatain->fin,2,"UI",value_string[3]),
				add_units((max_vin_vout_rising_edge_delay - min_vin_vout_rising_edge_delay)*pdatain->fin,1,"UI",value_string[4]));
				print_string_to_log(plog_string,pdatain);
				snprintf(plog_string,LOGFILE_LINELENGTH,
				"Average delay between VOUT and VIN = %s (± %s) or %s (± %s)\n",
				add_units(sum_vin_vout_rising_edge_delay/((double) num_vout_rising_edges)*pdatain->fin,2,"UI",value_string[0]),
				add_units(pdatain->deltat*pdatain->fin,2,"UI",value_string[1]),
				add_units(sum_vin_vout_rising_edge_delay/((double) num_vout_rising_edges),2,"s",value_string[2]),
				add_units(pdatain->deltat,2,"s",value_string[3]));
				print_string_to_log(plog_string,pdatain);
				}
		#endif
			   
      if (is_datarecovery(pdatain->pd))
      	{
         fclose(fpw2);
         if ((DETECT_AND_PRINT_BER == TRUE) && (lock_detect_flag != 0.0))
		   	{
		   	if (ber != NEVER_DETECTED_ERROR_FREE)
		   		{
		   		snprintf(plog_string,LOGFILE_LINELENGTH,
		   		"Done! Last BER was %1.4e based on %ld samples.\n",ber,error_free_sample_size);
		   		}
		   	else
		   		{
		   		snprintf(plog_string,LOGFILE_LINELENGTH,
		   		"Done! Did not detect error free operation after lock detect (%.0f samples).\n",floor((pdatain->TSTOP - lock_time)*fo));
		   		}
	   		print_string_to_log(plog_string,pdatain);
	   		}
	   	}

	   if (lock_detect_flag == 0)
	   	{
	   	if (pdatain->lock_detect_tau != 0.0)
	   		{
	   		snprintf(plog_string,LOGFILE_LINELENGTH,"Done! Did NOT detect phase lock by end of simulation.\n");
	   		print_string_to_log(plog_string,pdatain);
	   		}
	   	else
	   		sprintf(plock_time_string,"Lock detect feature was not active.");
	   	}
	   else
	   	{
	   	if (lock_detect_flag == 1)
	   		{
	   		snprintf(plog_string,LOGFILE_LINELENGTH,
	   		"Done! Last phase-lock event observed at about %s.\n", add_units(lock_time,4,"s",value_string[0]));
	   		print_string_to_log(plog_string,pdatain);
	   		sprintf(plock_time_string,"%1.12e",lock_time);
	   		}
	   	}
	   	
	   if ((pdatain->noise_amp != 0.0) && (lock_detect_flag == 1) && (pdatain->lock_detect_tau != 0.0))
	      {
	      snprintf(plog_string,LOGFILE_LINELENGTH,"Input Data: filtered noise_max = %.4f UI, filtered noise_min = %.4f UI, or %.4f UIpp.\n",
	      filtered_noise_max,filtered_noise_min,filtered_noise_max - filtered_noise_min);
	      print_string_to_log(plog_string,pdatain);
	      snprintf(plog_string,LOGFILE_LINELENGTH,"The magnitude of the filtered noise exceeded 0.50 UI a total of %ld times.\n",
	      filtered_noise_counter);
	      print_string_to_log(plog_string,pdatain);

	      snprintf(plog_string,LOGFILE_LINELENGTH,"Input Data: unfiltered noise_max = %.4f UI, unfiltered noise_min = %.4f UI, or %.4f UIpp.\n",
	      unfiltered_noise_max,unfiltered_noise_min,unfiltered_noise_max - unfiltered_noise_min);
	      print_string_to_log(plog_string,pdatain);
	      snprintf(plog_string,LOGFILE_LINELENGTH,"The magnitude of the unfiltered noise exceeded 0.50 UI a total of %ld times.\n",
	      unfiltered_noise_counter);
	      print_string_to_log(plog_string,pdatain);

	      }
		if ((fj != 0.0) && (pdatain->jitter_amp != 0.0))
			{
		   #ifdef USE_LOCK_DETECT_FLAG_TO_FILTER_RESULTS
		   	if ((fj != 0.0) && (lock_detect_flag == 1)) 
		   #endif
		   #ifndef USE_LOCK_DETECT_FLAG_TO_FILTER_RESULTS
		   	if (fj != 0.0)
		   #endif   
				{             	
				snprintf(plog_string,LOGFILE_LINELENGTH,
				"Value of rphase_ui_min = %s.\n",add_units(rphase_ui_min,3,"UI",value_string[0]));
				print_string_to_log(plog_string,pdatain);
				snprintf(plog_string,LOGFILE_LINELENGTH,
				"Value of rphase_ui_max = %s.\n",add_units(rphase_ui_max,3,"UI",value_string[0]));
				print_string_to_log(plog_string,pdatain);
				snprintf(plog_string,LOGFILE_LINELENGTH,"Value of pp jitter is = %s or %s.\n",
				add_units(rphase_ui_max - rphase_ui_min,3,"UI",value_string[0]),
				add_units((rphase_ui_max - rphase_ui_min)/pdatain->fin,4,"s",value_string[1]));
				print_string_to_log(plog_string,pdatain);

				if (jj > jt_samples_buffer_size + 1)
					{
					snprintf(plog_string,LOGFILE_LINELENGTH,
					"The number of jitter transfer data points, jj = %ld, exceeds the jitter transfer array size of jt_samples_buffer_size + 1 = %ld (array margin = %ld). Exiting...\n",
					jj,jt_samples_buffer_size + 1,jt_samples_buffer_size + 1 - jj);
					print_string_to_log(plog_string,pdatain);
					exit(0);
					}
					
				#ifdef DEBUG_JITTER_TRANSFER_ARRAY_SIZE
					snprintf(plog_string,LOGFILE_LINELENGTH,
					"The total number of jitter transfer data points is %ld, jitter array size  = %ld (array margin = %ld).\n",
					jj,jt_samples_buffer_size + 1,
					jt_samples_buffer_size + 1 - jj);
					print_string_to_log(plog_string,pdatain);
				#endif
				ffind_fourier_transfer_function(ptime_jt_samples,pphasein_jt_samples,pphaseo_jt_samples,
				pphaseo_delayed_jt_samples,jj,fj,pdatain,
				&(pjitter_transfer_gain[loop]),&(pjitter_transfer_phase[loop]),pfnameout2);
				pjitter_transfer_array[loop][0] = fj;
				pjitter_transfer_array[loop][1] = pjitter_transfer_gain[loop];
				pjitter_transfer_array[loop][2] = pjitter_transfer_phase[loop];
				if ((lock_detect_flag == 1) && (lock_time < ptime_jt_samples[0]))
            	pjitter_transfer_array[loop][3] = lock_time;
         	else
            	pjitter_transfer_array[loop][3] = 0;
         	pjitter_transfer_array[loop][4] = pdatain->TSTOP;
        	
				#ifndef SAVE_POST_PROCESSED_FILE
         		remove(pfnameout2);
        		 #endif
        		 
         	#ifndef SAVE_JITTER_TRANSFER_TRANS_PLL_OUTPUT_FILES
         		if (pdatain->number_of_jitter_freq > 1)
            		remove(pfnameout1); 
      			#endif
      			}
      	   /* Free jitter tolerance sample arrays */
	      	
   			if ((fj != 0.0) && (pdatain->jitter_amp != 0.0))
   				{
   				free(ptime_jt_samples);
   				free(pphasein_jt_samples);
   				free(pphaseo_jt_samples);
   				free(pphaseo_delayed_jt_samples);
   				}
	      	}
	      	
	      /* Write interim jitter transfer data to output file */

			#ifdef USE_LOCK_DETECT_FLAG_TO_FILTER_RESULTS
	   		if ((fj != 0.0) && (pdatain->jitter_amp != 0.0) && (loop >= 1) && (lock_detect_flag == 1) && (pdatain->lock_detect_tau != 0.0))
	   	#endif
      	#ifndef USE_LOCK_DETECT_FLAG_TO_FILTER_RESULTS
	   		if ((fj != 0.0) && (pdatain->jitter_amp != 0.0) && (loop >= 1))
	   	#endif
         	{
	         /* Sort by frequency */   
	      	sort_array_by_column_number(pjitter_transfer_array,loop,5,1);
	      	/* Unwrap phase in column 3 of frequency sorted array */
				/* unwrap_phase(pjitter_transfer_array,pdatain->number_of_jitter_freq,5,3,"nowrap"); */
				unwrap_phase(pjitter_transfer_array,pdatain->number_of_jitter_freq,5,3,pphase_units);
				sprintf(poutput_filename_jt,
				"%s_jitter_transfer_%s.csv",
				pdatain->poutput_base_filename,ptimestamp);
				fpw3 = fopen(poutput_filename_jt,"w");
				fprintf(fpw3,"Frequency (Hz),Gain (dB),Phase (degrees),Lock time (s),TSTOP (s)\n");
	      	for (i = 0; i < loop;i++)
	      		{
	      		for(j = 0; j < 5;j++)
	      			{
	      			if( j < 2)
	      				fprintf(fpw3,"%1.12e,",pjitter_transfer_array[i][j]);
	      			else
	      				{
	      				if (j == 2)
	      					{
	      					if(strcmp(pphase_units,"radians") == 0)
	      						fprintf(fpw3,"%1.12e,",pjitter_transfer_array[i][j]*(180.0/pi));
	      					else
	      				   	fprintf(fpw3,"%1.12e,",pjitter_transfer_array[i][j]);
	      				   }
	      				else
	      					{
	      					if (j == 3)
	      						{
	      						if (pjitter_transfer_array[i][j] == 0.0)
	      							{
	      							if (pdatain->lock_detect_tau != 0.0)
	      								{
	      								fprintf(fpw3,"No lock detected after %s,",
	      								add_units(pjitter_transfer_array[i][j + 1],4,"s",value_string[0]));
	      								}
	      							else
	      								fprintf(fpw3,"Lock detect feature was not active,");
	      							}
	      						else
	      							fprintf(fpw3,"%1.12e,",pjitter_transfer_array[i][j]);
	      						}
	      					else
	      						{
	      						if (j == 4)
	      							fprintf(fpw3,"%1.12e\n",pjitter_transfer_array[i][j]);
	      						}
	      					}
	      				}
	      			}
	      		}
	      	fclose(fpw3);
      	}
      	
		/* End of write interim jitter transfer results to file */
		}
	            
      #ifdef USE_LOCK_DETECT_FLAG_TO_FILTER_RESULTS
	   	if ((fj != 0.0) && (pdatain->jitter_amp != 0.0) && (loop == pdatain->number_of_jitter_freq - 1) && (lock_detect_flag == 1) && (pdatain->lock_detect_tau != 0.0)))
	   #endif
      #ifndef USE_LOCK_DETECT_FLAG_TO_FILTER_RESULTS
	   	if ((fj != 0.0) && (pdatain->jitter_amp != 0.0) && (loop == pdatain->number_of_jitter_freq - 0))
	   #endif
         {

         /* Sort by frequency */   
      	sort_array_by_column_number(pjitter_transfer_array,pdatain->number_of_jitter_freq,5,1);
      	/* Unwrap phase in column 3 of frequency sorted array */
			/* unwrap_phase(pjitter_transfer_array,pdatain->number_of_jitter_freq,5,3,"nowrap"); */
			unwrap_phase(pjitter_transfer_array,pdatain->number_of_jitter_freq,5,3,pphase_units);
   		sprintf(poutput_filename_jt,"%s_jitter_transfer_%s.csv",pdatain->poutput_base_filename,ptimestamp);
			fpw3 = fopen(poutput_filename_jt,"w");
			fprintf(fpw3,"Frequency (Hz),Gain (dB),Phase (degrees),Lock time (s),TSTOP (s)\n");
      	for (i = 0; i < pdatain->number_of_jitter_freq;i++)
      		{
      		for(j = 0; j < 5;j++)
      			{
      			if( j < 2)
      				fprintf(fpw3,"%1.12e,",pjitter_transfer_array[i][j]);
      			else
      				{
      				if (j == 2)
      					{
      					if(strcmp(pphase_units,"radians") == 0)
      						fprintf(fpw3,"%1.12e,",pjitter_transfer_array[i][j]*(180.0/pi));
      					else
      				   	fprintf(fpw3,"%1.12e,",pjitter_transfer_array[i][j]);
      				   }
      				else
      					{
      					if (j == 3)
      						{
      						if (pjitter_transfer_array[i][j] == 0.0)
      							{
      							if (pdatain->lock_detect_tau != 0.0)
      								{
      								fprintf(fpw3,"No lock event detected during jitter transfer analysis period,");
      								}
      							else
      								fprintf(fpw3,"Lock detect feature was not active,");
      							}
      						else
      							fprintf(fpw3,"%1.12e,",pjitter_transfer_array[i][j]);
      						}
      					else
      						{
      						if (j == 4)
      							fprintf(fpw3,"%1.12e\n",pjitter_transfer_array[i][j]);
      						}
      					}
      				}
      			}
      		}
      	fclose(fpw3);
      	free(pjitter_transfer_gain);
      	free(pjitter_transfer_phase);
			for (i = 0; i < pdatain->number_of_jitter_freq; i++)

      	snprintf(plog_string,LOGFILE_LINELENGTH,"Jitter transfer data in file \"%s\".\n",poutput_filename_jt);
      	print_string_to_log(plog_string,pdatain);
      	if ((pdatain->number_of_jitter_freq > 2) && (pdatain->plot_outputs != 0))
      		{
      			if (pdatain->plot_preference == OCTAVE)
						{
						if (pdatain->pd != PD_WINDOWED)
							{
				      	sprintf(ptitle_string,
				      	"%s VCO, Kvco = %s, divider = %.0f, divider delay = %s, RZERO = %s, R2 = %s\nC1 = %s, C2 = %s, Icp = %s, Icn = %s (Ro = %s), leakage current = %s",
				      	add_units(pdatain->fo,2,"Hz",value_string[0]),
				      	add_units(*(pdatain->pvc_polynomial + 1)*pdatain->fo,2,"Hz/V",value_string[1]),
				      	pdatain->D,add_units(pdatain->D_delay,1,"s",value_string[2]),
				      	add_units(pdatain->R2,1,"ohms",value_string[3]),
				      	add_units(pdatain->R3,1,"ohms",value_string[4]),
				      	add_units(pdatain->C1,2,"f",value_string[5]),
				      	add_units(pdatain->C2,2,"f",value_string[6]),
				      	add_units(pdatain->logic_hi_pdsignal/pdatain->R1,1,"A",value_string[7]),
				      	add_units(pdatain->logic_lo_pdsignal/pdatain->R1,1,"A",value_string[8]),
				      	add_units(pdatain->R1,1,"ohms",value_string[9]),
				      	add_units((psig_stats->vc_running_average - pdatain->vs1)/pdatain->RS1 + (psig_stats->vc_running_average - pdatain->vs2)/pdatain->RS2,2,"A",value_string[10]));
				      	}
				      else
				      	{
				      	sprintf(ptitle_string,
				      	"%s VCO, Kvco = %s, divider = %.0f, divider delay = %s, RZERO = %s, R2 = %s\nC1 = %s, C2 = %s, Icp = %s, Icn = %s (Ro = %s), leakage current = %s",
				      	add_units(pdatain->fo,2,"Hz",value_string[0]),
				      	add_units(*(pdatain->pvc_polynomial + 1)*pdatain->fo,2,"Hz/V",value_string[1]),
				      	pdatain->D,add_units(pdatain->D_delay,1,"s",value_string[2]),
				      	add_units(pdatain->R2,1,"ohms",value_string[3]),
				      	add_units(pdatain->R3,1,"ohms",value_string[4]),
				      	add_units(pdatain->C1,2,"f",value_string[5]),
				      	add_units(pdatain->C2,2,"f",value_string[6]),
				      	add_units(pdatain->logic_hi_pdsignal/pdatain->R1,1,"A",value_string[7]),
				      	add_units(pdatain->logic_lo_pdsignal/pdatain->R1,1,"A",value_string[8]),
				      	add_units(pdatain->R1,1,"ohms",value_string[9]),
				      	add_units((psig_stats->vc_running_average - pdatain->vs1)/pdatain->RS1 + (psig_stats->vc_running_average - pdatain->vs2)/pdatain->RS2,2,"A",value_string[10]));
							snprintf(ptitle_string,TITLE_LINELENGTH,
							"%sSlew rate detector: IS1 = %s (RS1 = %s), IS2 = %s (RS2 = %s)",
							ptitle_string,
							add_units(pdatain->vs1/pdatain->RS1,2,"A",value_string[11]),
							add_units(pdatain->RS1,2,"ohms",value_string[12]),
							add_units(pdatain->vs2/pdatain->RS2,2,"A",value_string[13]),
							add_units(pdatain->RS2,2,"ohms",value_string[14]));
				      	}
			      	sprintf(psystem_command_line_2,
			      	"%s/plotting_routines/octave/plot_jitter_transfer.m %s \"%s\" \"%s\"\n",
			      	PLOTTING_ROUTINES_DIR,poutput_filename_jt,ptitle_string,ptimestamp);
						sprintf(pfileout_system_command_line,
						"%s_jitter_transfer_system_octave_command_line_%s.txt",pdatain->poutput_base_filename,ptimestamp);
						}
      			if (pdatain->plot_preference == GNUPLOT)
						{
						if (pdatain->pd != PD_WINDOWED)
							{
				      	sprintf(ptitle_string,
				      	"{/:Bold %s VCO, Kvco = %s, divider = %.0f, divider delay = %s, RZERO = %s, R2 = %s}\n{/:Bold C1 = %s, C2 = %s, Icp = %s, Icn = %s (Ro = %s), leakage current = %s}",add_units(pdatain->fo,2,"Hz",value_string[0]),
				      	add_units(*(pdatain->pvc_polynomial + 1)*pdatain->fo,2,"Hz/V",value_string[1]),pdatain->D,
				      	add_units(pdatain->D_delay,1,"s",value_string[2]), add_units(pdatain->R2,1,"ohms",value_string[3]),
				      	add_units(pdatain->R3,1,"ohms",value_string[4]),
				      	add_units(pdatain->C1,2,"f",value_string[5]),
				      	add_units(pdatain->C2,2,"f",value_string[6]),
				      	add_units(pdatain->logic_hi_pdsignal/pdatain->R1,1,"A",value_string[7]),
				      	add_units(pdatain->logic_lo_pdsignal/pdatain->R1,1,"A",value_string[8]),
				      	add_units(pdatain->R1,1,"ohms",value_string[9]),
				      	add_units((psig_stats->vc_running_average - pdatain->vs1)/pdatain->RS1 + (psig_stats->vc_running_average - pdatain->vs2)/pdatain->RS2,2,"A",value_string[10]));
				      	}
				      else
				      	{
				      	sprintf(ptitle_string,
				      	"{/:Bold %s VCO, Kvco = %s, divider = %.0f, divider delay = %s, RZERO = %s, R2 = %s}\n{/:Bold C1 = %s, C2 = %s, Icp = %s, Icn = %s (Ro = %s), leakage current = %s}",add_units(pdatain->fo,2,"Hz",value_string[0]),
				      	add_units(*(pdatain->pvc_polynomial + 1)*pdatain->fo,2,"Hz/V",value_string[1]),pdatain->D,
				      	add_units(pdatain->D_delay,1,"s",value_string[2]), add_units(pdatain->R2,1,"ohms",value_string[3]),
				      	add_units(pdatain->R3,1,"ohms",value_string[4]),
				      	add_units(pdatain->C1,2,"f",value_string[5]),
				      	add_units(pdatain->C2,2,"f",value_string[6]),
				      	add_units(pdatain->logic_hi_pdsignal/pdatain->R1,1,"A",value_string[7]),
				      	add_units(pdatain->logic_lo_pdsignal/pdatain->R1,1,"A",value_string[8]),
				      	add_units(pdatain->R1,1,"ohms",value_string[9]),
				      	add_units((psig_stats->vc_running_average - pdatain->vs1)/pdatain->RS1 + (psig_stats->vc_running_average - pdatain->vs2)/pdatain->RS2,2,"A",value_string[10]));
				      	snprintf(ptitle_string,TITLE_LINELENGTH,
							"%s\n{/:Bold Slew rate detector: IS1 = %s (RS1 = %s), IS2 = %s (RS2 = %s)}",
							ptitle_string,
							add_units(pdatain->vs1/pdatain->RS1,2,"A",value_string[11]),
							add_units(pdatain->RS1,2,"ohms",value_string[12]),
							add_units(pdatain->vs2/pdatain->RS2,2,"A",value_string[13]),
							add_units(pdatain->RS2,2,"ohms",value_string[14]));
				      	}
			      	sprintf(psystem_command_line_2,
			      	"gnuplot -e \"input_filename = \'%s\'; jitter_transfer_plot_title = \'%s\'; RZERO = %1.12e; C1 = %1.12e; timestamp = \'%s\'; PLOTTING_ROUTINES_DIR = \'%s\';\" %s/plotting_routines/gnuplot/plot_jitter_transfer.gnu\n",
			      	poutput_filename_jt,ptitle_string,pdatain->R2,pdatain->C1,ptimestamp,
			      	PLOTTING_ROUTINES_DIR,PLOTTING_ROUTINES_DIR);
						sprintf(pfileout_system_command_line,
						"%s_jitter_transfer_gnuplot_command_line_%s.txt",pdatain->poutput_base_filename,ptimestamp);
						}
				if (PRINT_GNUPLOT_OR_OCTAVE_COMMAND == TRUE)
					{
					fpw4 = fopen(pfileout_system_command_line,"w");
					fprintf(fpw4,"%s\n",psystem_command_line_2);
					fclose(fpw4);
					}
	      	system(psystem_command_line_2);
	      	}
      	}
	   num++; /*Increment number of times through function to add to filename*/

/* Only produce transient plot if there is no applied jitter frequency and the jitter amplitude is 0 */

#ifdef USE_LOCK_DETECT_FLAG_TO_FILTER_RESULTS
	if ((pdatain->number_of_jitter_freq < 2) && (pdatain->jitter_amp == 0.0) && (lock_detect_flag == 1) && (pdatain->plot_outputs != 0))
#endif
#ifndef USE_LOCK_DETECT_FLAG_TO_FILTER_RESULTS
	if ((pdatain->number_of_jitter_freq < 2) && (pdatain->plot_outputs != 0))
#endif
	{
	if (pdatain->plot_preference == OCTAVE)
		{
		if (pdatain->pd != PD_WINDOWED)
			{
			snprintf(ptitle_string,TITLE_LINELENGTH,
			"%s VCO, Kvco = %s, divider = %.0f, divider delay = %s\nRZERO = %s, R2 = %s, C1 = %s, C2 = %s\nIcp = %s, Icn = %s (R1 = %s), leakage current = %s",
			add_units(pdatain->fo,2,"Hz",value_string[0]),
			add_units(*(pdatain->pvc_polynomial + 1)*pdatain->fo,2,"Hz/V",value_string[1]),pdatain->D,
			add_units(pdatain->D_delay,1,"s",value_string[2]),
			add_units(pdatain->R2,2,"ohms",value_string[3]),
			add_units(pdatain->R3,1,"ohms",value_string[4]),
			add_units(pdatain->C1,2,"f",value_string[5]),
			add_units(pdatain->C2,2,"f",value_string[6]),
			add_units(pdatain->logic_hi_pdsignal/pdatain->R1,1,"A",value_string[7]),
			add_units(pdatain->logic_lo_pdsignal/pdatain->R1,1,"A",value_string[8]),
			add_units(pdatain->R1,2,"ohms",value_string[9]),
			add_units((psig_stats->vc_running_average - pdatain->vs1)/pdatain->RS1 + (psig_stats->vc_running_average - pdatain->vs2)/pdatain->RS2,2,"A",value_string[10]));
			}
		else
			{
			snprintf(ptitle_string,TITLE_LINELENGTH,
			"%s VCO, Kvco = %s, divider = %.0f, divider delay = %s\nDFF loop filter: RZERO = %s, R2 = %s, C1 = %s, C2 = %s\nIcp = %s, Icn = %s (R1 = %s), leakage current = %s\n",
			add_units(pdatain->fo,2,"Hz",value_string[0]),
			add_units(*(pdatain->pvc_polynomial + 1)*pdatain->fo,2,"Hz/V",value_string[1]),pdatain->D,
			add_units(pdatain->D_delay,1,"s",value_string[2]),
			add_units(pdatain->R2,2,"ohms",value_string[3]),
			add_units(pdatain->R3,1,"ohms",value_string[4]),
			add_units(pdatain->C1,2,"f",value_string[5]),
			add_units(pdatain->C2,2,"f",value_string[6]),
			add_units(pdatain->logic_hi_pdsignal/pdatain->R1,1,"A",value_string[7]),
			add_units(pdatain->logic_lo_pdsignal/pdatain->R1,1,"A",value_string[8]),
			add_units(pdatain->R1,2,"ohms",value_string[9]),
			add_units((psig_stats->vc_running_average - pdatain->vs1)/pdatain->RS1 + (psig_stats->vc_running_average - pdatain->vs2)/pdatain->RS2,2,"A",value_string[10]));
			snprintf(ptitle_string,TITLE_LINELENGTH,
			"%sSlew rate detector: IS1 = %s (RS1 = %s), IS2 = %s (RS2 = %s)",
			ptitle_string,
			add_units(pdatain->vs1/pdatain->RS1,2,"A",value_string[11]),
			add_units(pdatain->RS1,2,"ohms",value_string[12]),
			add_units(pdatain->vs2/pdatain->RS2,2,"A",value_string[13]),
			add_units(pdatain->RS2,2,"ohms",value_string[14]));
			}
		}
	if (pdatain->plot_preference == GNUPLOT)
		{
		if (pdatain->pd != PD_WINDOWED)
			{
			snprintf(ptitle_string,TITLE_LINELENGTH,
			"{/:Bold %s VCO, Kvco = %s, divider = %.0f, divider delay = %s}\n{/:Bold RZERO = %s, R2 = %s, C1 = %s, C2 = %s}\n{/:Bold Icp = %s, Icn = %s (R1 = %s), leakage current = %s}",
			add_units(pdatain->fo,2,"Hz",value_string[0]),
			add_units(*(pdatain->pvc_polynomial + 1)*pdatain->fo,2,"Hz/V",value_string[1]),
			pdatain->D,add_units(pdatain->D_delay,1,"s",value_string[2]),
			add_units(pdatain->R2,2,"ohms",value_string[3]),
			add_units(pdatain->R3,1,"ohms",value_string[4]),
			add_units(pdatain->C1,2,"f",value_string[5]),
			add_units(pdatain->C2,2,"f",value_string[6]),
			add_units(pdatain->logic_hi_pdsignal/pdatain->R1,1,"A",value_string[7]),
			add_units(pdatain->logic_lo_pdsignal/pdatain->R1,1,"A",value_string[8]),
			add_units(pdatain->R1,2,"ohms",value_string[9]),
			add_units((psig_stats->vc_running_average - pdatain->vs1)/pdatain->RS1 + (psig_stats->vc_running_average - pdatain->vs2)/pdatain->RS2,2,"A",value_string[10]));
			}
		else
			{
			snprintf(ptitle_string,TITLE_LINELENGTH,
			"{/:Bold %s VCO, Kvco = %s, divider = %.0f, divider delay = %s}\n{/:Bold RZERO = %s, R2 = %s, C1 = %s, C2 = %s}\n{/:Bold Icp = %s, Icn = %s (R1 = %s), leakage current = %s}",
			add_units(pdatain->fo,2,"Hz",value_string[0]),
			add_units(*(pdatain->pvc_polynomial + 1)*pdatain->fo,2,"Hz/V",value_string[1]),
			pdatain->D,add_units(pdatain->D_delay,1,"s",value_string[2]),
			add_units(pdatain->R2,2,"ohms",value_string[3]),
			add_units(pdatain->R3,1,"ohms",value_string[4]),
			add_units(pdatain->C1,2,"f",value_string[5]),
			add_units(pdatain->C2,2,"f",value_string[6]),
			add_units(pdatain->logic_hi_pdsignal/pdatain->R1,1,"A",value_string[7]),
			add_units(pdatain->logic_lo_pdsignal/pdatain->R1,1,"A",value_string[8]),
			add_units(pdatain->R1,2,"ohms",value_string[9]),
			add_units((psig_stats->vc_running_average - pdatain->vs1)/pdatain->RS1 + (psig_stats->vc_running_average - pdatain->vs2)/pdatain->RS2,2,"A",value_string[10]));
			snprintf(ptitle_string,TITLE_LINELENGTH,
			"%s\n{/:Bold Slew rate detector: IS1 = %s (RS1 = %s), IS2 = %s (RS2 = %s)}",
			ptitle_string,
			add_units(pdatain->vs1/pdatain->RS1,2,"A",value_string[11]),
			add_units(pdatain->RS1,2,"ohms",value_string[12]),
			add_units(pdatain->vs2/pdatain->RS2,2,"A",value_string[13]),
			add_units(pdatain->RS2,2,"ohms",value_string[14]));
			/* snprintf(plog_string,LOGFILE_LINELENGTH,
			"After sim, printed plot title: RS1 =  %1.6e, RS2 = %1.6e, R1 = %1.6e\n",
			pdatain->RS1,pdatain->RS2,pdatain->R1);
			print_string_to_log(plog_string,pdatain); */
			}
		}
	if (pdatain->noise_amp > 0.0) 
		{
		switch (pdatain->noise_type)
			{
			case GAUSSIAN_NOISE:
				if (pdatain->plot_preference == OCTAVE)
					{
					sprintf(ptitle_string,
					"%s\nGaussian noise sigma = %s, bandwidth = %s",
					ptitle_string,add_units(pdatain->noise_amp,2,"UI",value_string[0]),
					add_units(pdatain->noise_bandwidth_Hz,1,"Hz",value_string[1]));
					}
				if (pdatain->plot_preference == GNUPLOT)
					{
					sprintf(ptitle_string,"%s\n{/:Bold Gaussian noise sigma = %s, bandwidth = %s}",
					ptitle_string,add_units(pdatain->noise_amp,2,"UI",value_string[0]),
					add_units(pdatain->noise_bandwidth_Hz,1,"Hz",value_string[1]));
					}			
				break;
			case UNIFORM_NOISE:
				if (pdatain->plot_preference == OCTAVE)
					{
					sprintf(ptitle_string,"%s\nUniform noise range = %s, bandwidth = %s",
					ptitle_string,add_units(2.0*pdatain->noise_amp,2,"UI",value_string[0]),
					add_units(pdatain->noise_bandwidth_Hz,1,"Hz",value_string[1]));
					}
				if (pdatain->plot_preference == GNUPLOT)
					{
					sprintf(ptitle_string,"%s\n{/:Bold Uniform noise range = %s, bandwidth = %s}",
					ptitle_string,add_units(2.0*pdatain->noise_amp,2,"UI",value_string[0]),
					add_units(pdatain->noise_bandwidth_Hz,1,"Hz",value_string[1]));
					}
				break;
			default:
				snprintf(plog_string,LOGFILE_LINELENGTH,"In main.c, did NOT recognize noise type! Exiting...\n");
				print_string_to_log(plog_string,pdatain);
				exit(0);
			}
		}
	if  ((pdatain->octave == 1) && (pdatain->plot_preference == OCTAVE))
		{
		snprintf(psystem_command_line_1,COMMAND_LINELENGTH,
		"%s/plotting_routines/octave/plot_transient_data.m %s \"%s\" \"%s\" \"%s\"\n",
		PLOTTING_ROUTINES_DIR,pfnameout1,ptitle_string,ptimestamp,plock_time_string);
		snprintf(psystem_command_line_2,COMMAND_LINELENGTH,
		"%s_transient_octave_system_command_line_%s.txt",pdatain->poutput_base_filename,ptimestamp);
		}
	if  ((pdatain->gnuplot == 1) && (pdatain->plot_preference == GNUPLOT))
		{
		snprintf(psystem_command_line_1,COMMAND_LINELENGTH,
		"gnuplot -e \"input_filename = \'%s\'; trans_pll_plot_title = \'%s\'; timestamp = \'%s\'; lock_time_sec = %1.12e; lost_lock_time_sec = %1.12e; PLOTTING_ROUTINES_DIR = \'%s\';\" %s/plotting_routines/gnuplot/script.gp\n",
		pfnameout1,ptitle_string,ptimestamp,lock_time,lost_lock_time,PLOTTING_ROUTINES_DIR,PLOTTING_ROUTINES_DIR);
		snprintf(psystem_command_line_2,COMMAND_LINELENGTH,"%s_transient_gnuplot_command_line_%s.txt",
		pdatain->poutput_base_filename,ptimestamp);
		}
	if (PRINT_GNUPLOT_OR_OCTAVE_COMMAND == TRUE)
		{
		fpw4 = fopen(psystem_command_line_2,"w");
		fprintf(fpw4,"%s\n",psystem_command_line_1);
		fclose(fpw4);
		}
	system(psystem_command_line_1);
	}

if (pdatain->D_delay > 0.0)
	free(pphaseo_sample);

toc = clock();
snprintf(plog_string,LOGFILE_LINELENGTH,"Elapsed time: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
print_string_to_log(plog_string,pdatain);

}

