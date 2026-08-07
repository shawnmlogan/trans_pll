
#include "globals.h"

int check_data(Filedata *pdatastruct)
{
Boolean loop_filter_element_status = TRUE;
char *pdefault = "default",*ptempstring, tempstring[LINELENGTH+1], *pmissing_entry = "no_entry";
char *pvalue_string, value_string[LINELENGTH_OF_VALUE_STRING + 1];
int i = 0;
double datarate, df, *ptemp_pointer;
double ave_input_signal = 0.0, min_input_signal = 0.0, max_input_signal = 0.0;
long int number_of_lines = 0;
FILE *fpw1;

/*Checks data for validity in structure for pll analysis*/

/*Initialize pointers*/

ptempstring = &tempstring[0];
pvalue_string = &value_string[0];

/*Phase detector option*/

if (strcmp(pdatastruct->ppd,"pfd") == 0)
   pdatastruct->pd = PFD;
if (strcmp(pdatastruct->ppd,"dff") == 0)
   pdatastruct->pd = DFF;
if (strcmp(pdatastruct->ppd,"exor") == 0)
   pdatastruct->pd = PD_EXOR;
if (strcmp(pdatastruct->ppd,"none_vpd=1") == 0)
   pdatastruct->pd = NONE_VPD_HIGH;
if (strcmp(pdatastruct->ppd,"none_vpd=0") == 0)
   pdatastruct->pd = NONE_VPD_LOW;
if (strcmp(pdatastruct->ppd,"hogge") == 0)
   pdatastruct->pd = HOGGE;
if (strcmp(pdatastruct->ppd,"hogge_cp") == 0)
   pdatastruct->pd = HOGGE_CP;
if (strcmp(pdatastruct->ppd,"pfd_cp") == 0)
   pdatastruct->pd = PFD_CP;
if (strcmp(pdatastruct->ppd,"pd_windowed") == 0)
   pdatastruct->pd = PD_WINDOWED;
if (strcmp(pdatastruct->ppd,"alexander") == 0)
   pdatastruct->pd = PD_ALEXANDER;
if ((pdatastruct->pd < 0) || (pdatastruct->pd > 9))
   {
   printf("\nERROR:Did not recognize phase detector\noption in check_data.c \"%s\"!", pdatastruct->ppd);
   return EXIT_FAILURE;
   }

/*Control voltage model options*/

if ((strcmp(pdatastruct->pvc_model_string,"polynomial") == 0) || (strcmp(pdatastruct->pvc_model_string,"poly") == 0))
   pdatastruct->vc_model = USE_POLYNOMIAL_MODEL;
if (strcmp(pdatastruct->pvc_model_string,"tanh") == 0)
   pdatastruct->vc_model = USE_TANH_MODEL;
if ((pdatastruct->vc_model != 0) && (pdatastruct->vc_model != 1))
   {
   printf("\nERROR:Did not recognize VCO control voltage model\noption \"%s\" in check_data.c", pdatastruct->pvc_model_string);
   return EXIT_FAILURE;
   }
if ((pdatastruct->vc_model == 1) && (pdatastruct->vco_polynomial_degree != 4))
   {
   printf("\nERROR:tanh control voltage model requires 4 coefficients,\nand only %d were detected in check_data.c", pdatastruct->vco_polynomial_degree);
   return EXIT_FAILURE;
   }
  
/*For data recovery phase detectors, check to make sure input data file can be opened and that fo/D is nominally fin*/

datarate = pdatastruct->fo/pdatastruct->D;
df = (pdatastruct->fin - datarate)/pdatastruct->fin;

if (is_datarecovery(pdatastruct->pd) && (fabs(df) > EPSILON))
   {
   printf("\nNOTE:In data recovery application with this phase detector\n");
   printf("the data frequency is generally equal to the clock frequency!");
   }

if (is_datarecovery(pdatastruct->pd))
   {
   if ((fpw1=fopen(pdatastruct->pdatafilein, "r")) == NULL)
      {
      printf("\nERROR:Can't find file \"%s\" with input data pattern!\n",pdatastruct->pdatafilein);
      return EXIT_FAILURE;
      }
   else
   	{
   	if (find_stats_column_one_of_file(pdatastruct->pdatafilein, &ave_input_signal,&min_input_signal,&max_input_signal,&number_of_lines) != EXIT_SUCCESS)
   		{
			printf("\nERROR:find_stats_column_one_of_file() failed in check_data.c\n");
			return EXIT_FAILURE;
   		}
   	else
   		{
   		if (min_input_signal < 0.0)
   			{
   			printf("Input data file \"%s\" contains an entry less than 0! Please check input file.\n",pdatastruct->pdatafilein);
   			return EXIT_FAILURE;
   			}
   		if (max_input_signal > 1.0)
   			{
   			printf("Input data file \"%s\" contains an entry greater than 1! Please check input file.\n",pdatastruct->pdatafilein);
   			return EXIT_FAILURE;
   			}
   		if (number_of_lines < MAX_NUMBER_OF_INPUT_SAMPLES_STORED)
   			pdatastruct->datafilein_number_of_lines = number_of_lines;
   		else
     			pdatastruct->datafilein_number_of_lines = MAX_NUMBER_OF_INPUT_SAMPLES_STORED; 		
   		}
   	}
   }

if (pdatastruct->D != 1.0)
	{
	if (pdatastruct->clk_dutycycle == INIT_VAL)
		{
		printf("Check input file, did not detect a value for clk_dutycyle.\n");
		printf("Enter a feedback clock duty cycle greater than 0.0 and less than 1.0. Exiting...\n");
		return EXIT_FAILURE;
		}
	if ((pdatastruct->clk_dutycycle <= 0.0) || (pdatastruct->clk_dutycycle >= 1.0))
		{
		printf("Input file contains a feedback clock duty cycle of %.2f. Please check input file.\n",
		pdatastruct->clk_dutycycle);
		printf("Enter a feedback clock duty cycle greater than 0.0 and less than 1.0. Exiting...");
		return EXIT_FAILURE;
		}
	}
else
	{
	if (pdatastruct->vco_clk_dutycycle == INIT_VAL)
		{
		printf("Check input file, did not detect a value for vco_clock_duty_cycle(fo).\n");
		printf("Enter a value between 0.0 and 1.0 for duty cycle of VCO clock! Exiting...\n");
		return EXIT_FAILURE;
		}
	/* if (pdatastruct->vco_clkq_dutycycle == INIT_VAL)
		{
		printf("Check input file, did not detect a value for vco_clkq_dutycyle.\n");
		printf("Enter a value between 0.0 and 1.0 for duty cycle of VCO clock! Exiting...\n");
		return EXIT_FAILURE;
		} */
	}
if ((pdatastruct->vco_clk_dutycycle <= 0.0) || (pdatastruct->vco_clk_dutycycle >= 1.0))
	{
	printf("Input file contains a VCO clock duty cycle of %.2f. Please check input file.\n",
	pdatastruct->vco_clk_dutycycle);
	printf("Enter a VCO clock duty cycle greater than 0.0 and less than 1.0. Exiting...");
	return EXIT_FAILURE;
	}

/* if ((pdatastruct->vco_clkq_dutycycle <= 0.0) || (pdatastruct->vco_clkq_dutycycle >= 1.0))
	{
	printf("Input file contains a VCO quadrature clock duty cycle of %.2f. Please check input file.\n",
	pdatastruct->vco_clk_dutycycle);
	printf("Enter a VCO quadrature clock duty cycle greater than 0.0 and less than 1.0. Exiting...");
	return EXIT_FAILURE;
	} */
	
/*Convert phase0 from degrees to radians*/

pdatastruct->phase0 = (pi/180.0)*pdatastruct->phase0;

/*Verify that a jitter amplitude was entered*/

if (pdatastruct->jitter_amp == INIT_VAL)
	{
	printf("Check input file, did not detect a value for jitter_amp(UIpp). Please check input file.\n");
	printf("Enter a reasonable value for jitter amplitude (< %.1f UI)! Exiting...\n",MAX_JITTER_AMP_UI);
	return EXIT_FAILURE;
	}
	
/*Verify that the input jitter amplitude is reasonable*/

if (fabs(pdatastruct->jitter_amp) > MAX_JITTER_AMP_UI)
	{
	printf("Enter a lower value (< %.1f UI) for input jitter amplitude! Exiting...\n",MAX_JITTER_AMP_UI);
	return EXIT_FAILURE;
	}

/*Verify an entry was made jitter_freq and TSTOP is set to include 4 periods of jitter frequency*/

if ((pdatastruct->number_of_jitter_freq == INIT_VAL) && (pdatastruct->jitter_amp != 0.0))
	{
   printf("Did not detect a value for jitter frequency in input file.\n");
   printf("Verify one or more entry exists for \"jitter_freqs(Hz)\". Exiting...\n");
   return EXIT_FAILURE;
   }
else
	{
	if (pdatastruct->jitter_amp != 0.0)
		{
		
		/* Verify all input frequencies are greater than zero */
		
		for (i = 0; i<pdatastruct->number_of_jitter_freq ; i++)
			{
			if(pdatastruct->pjitter_freq[i] < 0.0)
				{
				printf("Detected a value for jitter frequency of less than 0 in input file (%.2e). Please check input file.\n",pdatastruct->pjitter_freq[i]);
	   		printf("Enter positive values for all values of \"jitter_freqs(Hz)\". Exiting...\n");
	   		return EXIT_FAILURE;
	   		}
	   		ptemp_pointer++;
	   	}
			
	   /*Verify that TSTART_jt > TSTART*/
	
	   if ( (pdatastruct->TSTART_jt <= pdatastruct->TSTART) && (pdatastruct->number_of_jitter_freq != 0.0))
	      {
	      printf("Jitter analysis time must exceed TSTART in jitter tolerance simulations! Exiting...\n");
	      return EXIT_FAILURE;
	      }
   
	   /*Alert user a non-zero amplitude was set and jitter frequency is non-zero*/
	
	   if ( (pdatastruct->jitter_amp == 0.0) && (pdatastruct->pjitter_freq[0] != 0.0))
	      {
	      printf("Warning: It appears a non-zero jitter frequency was entered, but the jitter amplitude is set to zero.\n");
	      }
	
	   /*Alert user if a jitter frequency of 0 Hz was entered in list of more than 1 jitter frequencies*/
	
	   if ( pdatastruct->number_of_jitter_freq > 1)
	      {
	      for (i = 0; i < pdatastruct->number_of_jitter_freq; i++)
	      	{
	      	if (pdatastruct->pjitter_freq[i] == 0.0)
	      		{
	      		printf("A jitter frequency of 0 Hz was entered in a jitter transfer analysis! Exiitng...\n");
	      		return EXIT_FAILURE;
	      		}
	      	}
	      }
	   }
	else
		{
		pdatastruct->number_of_jitter_freq = 1;
		pdatastruct->pjitter_freq[0] = 0.0;
		}
   }
   
/*Read deltat - if text "default" set to MIN_DELTAT_RELATIVE_VCO_PERIOD_IN_UI of fo frequency*/

if (strcmp(pdatastruct->pdeltat,pdefault) == 0)
   pdatastruct->deltat = MIN_DELTAT_RELATIVE_VCO_PERIOD_IN_UI * (1/(pdatastruct->fo));
else
	{
	if (strcmp(pdatastruct->pdeltat,pmissing_entry) == 0)
		{
		printf("Input file was missing a value for the time step.\n");
		printf("Check input file for parameter \"deltat(sec)\".\n");
		return EXIT_FAILURE;
		}
	else
   	pdatastruct->deltat = atof(pdatastruct->pdeltat);
   }

/*If data recovery phase detector - require that sample input data more than once per cycle for meaningful results. Verify that
at least taking 4 samples every data sample*/

if (is_datarecovery(pdatastruct->pd))
   {
   if (pdatastruct->deltat > 1.0/(10.0*pdatastruct->fin))
      {
      pdatastruct->deltat = 1.0/(10.0*pdatastruct->fin);
      printf("\n\n******NOTE*************************************************************\n");    
      printf("Value of deltat will not assure at least 10 samples per data bit\n");
      printf("... assigning deltat to %2.2e seconds.\n",pdatastruct->deltat);
      }
      
   /* If data recovery phase detector is an Alexander phase detector- define and verify */
	/* entered value for phase detector offset is positive and between 0 and 1.0 */
	
	if (pdatastruct->pd == PD_ALEXANDER)
		{
		if (strcmp(pdatastruct->palexander_pd_threshold_ui,pdefault) == 0)
   		pdatastruct->alexander_pd_threshold_ui = 0.0;
		else
			{
			if (strcmp(pdatastruct->palexander_pd_threshold_ui,pmissing_entry) == 0)
				{
				printf("Input file was missing a value for the Alexander phase detector offset.\n");
				printf("Check input file for parameter \"alexander_phase_detector_phase_threshold(UI)\".\n");
				return EXIT_FAILURE;
				}
			else
				{
				pdatastruct->alexander_pd_threshold_ui = atof(pdatastruct->palexander_pd_threshold_ui);
				if ((pdatastruct->alexander_pd_threshold_ui < 0.0) || (pdatastruct->alexander_pd_threshold_ui > 0.49))
					{
					printf("Enter a value for Alexander phase detector offset\n");
					printf("in UI greater than 0.0 and less than 0.50.\n");
					}
				}
			}
		} 
   }

if ((pdatastruct->pd == PFD_CP) || (pdatastruct->pd == PFD))
   {
   if (pdatastruct->tauff < 0.0)
      {
      printf("Enter a value greater than 0.0 for tauff for pfd.\n");
      return EXIT_FAILURE;
      }
   if (pdatastruct->tauh < 0.0)
      {
      printf("Enter a value greater than 0.0 for tauh for pfd.\n");
      return EXIT_FAILURE;
      }
   if (pdatastruct->taucp_min < 0.0)
      {
      printf("Enter a value greater than 0.0 for taucp_min for pfd.\n");
      return EXIT_FAILURE;
      }
   if ((pdatastruct->pfd_dff_deadzone_ui < 0.0) || (pdatastruct->pfd_dff_deadzone_ui >= 0.5))
      {
      printf("Enter a value greater than or equal to 0.0 UI and less than 0.50 UI for pfd_deadzone.\n");
      return EXIT_FAILURE;
      }
   }
/*Verify that entered reasonable numbers for DFF delays*/

if ((pdatastruct->pd == DFF) || (pdatastruct->pd == PD_WINDOWED) || (pdatastruct->pd == PFD_CP) || (pdatastruct->pd == PFD) || (pdatastruct->pd == PD_EXOR))
   {
   if (pdatastruct->tauff < 0.0)
      {
      printf("Enter a value greater than 0.0 for tauff.\n");
      return EXIT_FAILURE;
      }
   }

/*Verify that entered divider delay greater than zero*/

if (pdatastruct->D_delay < 0.0)
	{
	printf("Enter a value greater than 0.0 for divider delay.\n");
	return EXIT_FAILURE;
	}
  
if (pdatastruct->pd == PD_WINDOWED)
   {
   if (pdatastruct->tacq_max == 0.0)
      {
      printf("Entered a zero value for maximum acquisition time with\n");
      printf("windowed phase detector...this will require a long time to run!\n");
      return EXIT_FAILURE;
      }
   if (pdatastruct->tw < 0.0)
      {
      printf("Enter a value greater than 0.0 for time window in windowed phase detector.\n");
      return EXIT_FAILURE;
      }
   if (pdatastruct->vs1 < pdatastruct->vmin)
      {
      printf("Enter a value greater than %.3f for vs1 in windowed phase detector.\n",
      pdatastruct->vmin);
      return EXIT_FAILURE;
      }
   if (pdatastruct->vs2 < pdatastruct->vmin)
      {
      printf("Enter a value greater than %.3f for vs2 in windowed phase detector.\n",
      pdatastruct->vmin);
      return EXIT_FAILURE;
      }
   if (pdatastruct->vs1 < 0.0)
      {
      printf("Enter a value greater than 0.0 for vs1 in windowed phase detector.\n");
      return EXIT_FAILURE;
      }
   if (pdatastruct->vs2 < 0.0)
      {
      printf("Enter a value greater than 0.0 for vs2 in windowed phase detector.\n");
      return EXIT_FAILURE;
      }
   if ((pdatastruct->vs1 == pdatastruct->vmin) && (pdatastruct->vs2 == pdatastruct->vmin))
      {
      printf("Enter a value greater than %.3f for vs1 or vs2 (or both) in windowed phase detector.\n"
      ,pdatastruct->vmin);
      return EXIT_FAILURE;
      }
   }
   
/*Verify noise type option and noise bandwidth is greater than zero*/

if (pdatastruct->noise_amp > 0.0)
	{
	if (strcmp(pdatastruct->pnoise_type,"gaussian") == 0)
	   pdatastruct->noise_type = GAUSSIAN_NOISE;
	if (strcmp(pdatastruct->pnoise_type,"uniform") == 0)
	   pdatastruct->noise_type = UNIFORM_NOISE;
	if ((pdatastruct->noise_type < 0) || (pdatastruct->noise_type > 1))
	   {
	   printf("\nERROR:Did not recognize noise_type\noption in check_data.c \"%s\"!", pdatastruct->pnoise_type);
	   return EXIT_FAILURE;
	   }
	else
		{
		if (pdatastruct->noise_bandwidth_Hz < 0.0)
			{
	   	printf("\nERROR:Noise bandwidth must be greater than 0 Hz.\n");
	   	return EXIT_FAILURE;
	   	}
	   }
	}

/*Verify that number of points requested in analysis is greater than 1 - otherwise computes prpoints incorrectly*/

if (pdatastruct->npoints < 2)
   {
   printf("At least 2 points are required in analysis - changing npoints to 2...");
   pdatastruct->npoints = 2.0;
   }
strcpy(ptempstring,pdatastruct->pfile_format);
for (i = 0; tempstring[i] !='\0'; i++)
   tempstring[i] = tolower(tempstring[i]);
if (strcmp(ptempstring,"csv") == 0)
   pdatastruct->ff = 0;
else
   pdatastruct->ff = 0;

/*Verify that phase_freq_step_time_in_sec >= 0.0*/

if ((pdatastruct->phase_freq_step_time_in_sec < 0.0) || (pdatastruct->phase_freq_step_time_in_sec == INIT_VAL))
   {
   printf("Enter a non-negative start time (parameter \"phase_freq_step_time(sec)\") for the application of a phase/freq step ...\n");
   printf("pdatastruct->phase_freq_step_time_in_sec = %1.4e.\n",pdatastruct->phase_freq_step_time_in_sec);
   return EXIT_FAILURE;
   }

/*Verify that phase_step is < 1.0 UI*/

if ((fabs((pdatastruct->phase_step_in_sec)*(pdatastruct->fin)) >= 1.0) || (pdatastruct->phase_step_in_sec == INIT_VAL))
   {
   printf("Enter a phase step (parameter \"phase_step(sec)\") that is less than 1 UI or %.2f ps.\n", 1e12/pdatastruct->fin);
   printf("pdatastruct->phase_step_in_sec = %1.4e.\n",pdatastruct->phase_step_in_sec);
   return EXIT_FAILURE;
   }

/*Verify that freq_step_ppm is < 10,000 ppm*/

if ((fabs(pdatastruct->freq_step_ppm) >= 1e4) || (pdatastruct->freq_step_ppm == INIT_VAL))
   {
   printf("Enter a frequency step (parameter \"freq_step(ppm)\") that is less than 10,000 ppm.\n");
   printf("pdatastruct->freq_step_ppm = %1.4e.\n",pdatastruct->freq_step_ppm);
   return EXIT_FAILURE;
   }

/*Verify that phase_step and freq_step are 0 when jitter transfer analysis is selected */

if (((fabs(pdatastruct->phase_step_in_sec) > 0.0) || (fabs(pdatastruct->freq_step_ppm) > 0.0)) && (pdatastruct->jitter_amp > 0.0))
   {
   printf( "pdatastruct->jitter_amp = %.3f.\n",pdatastruct->jitter_amp);
   printf("Set phase and frequency steps to zero when performing a jitter transfer analysis\n");
   return EXIT_FAILURE;
   }

/*Verify that ssc_start_time_in_sec >= 0.0*/

if ((pdatastruct->ssc_start_time_in_sec < 0.0) || (pdatastruct->ssc_start_time_in_sec == INIT_VAL))
   {
   printf("Enter a non-negative start time for the application of SSC (parameter \"ssc_start_time_in(sec)\")\n");
   printf("pdatastruct->ssc_start_time_in_sec = %1.4e.\n",pdatastruct->ssc_start_time_in_sec);
   return EXIT_FAILURE;
   }

/*Verify that ssc excursions are 0 when jitter transfer analysis is selected */

if (((fabs(pdatastruct->max_pos_ssc_ppm) > 0.0) || (fabs(pdatastruct->max_neg_ssc_ppm) > 0.0)) && (pdatastruct->jitter_amp > 0.0))
   {
   printf("Set maximum positive and negative SSC modulations to zero when performing a jitter transfer analysis.\n");
   return EXIT_FAILURE;
   }

/*Verify that SSC maximum positive excursion is < 50,000 ppm*/

if ((pdatastruct->max_pos_ssc_ppm >= 5e4) || (pdatastruct->max_pos_ssc_ppm < 0.0) || (pdatastruct->max_pos_ssc_ppm == INIT_VAL))
   {
   printf("Enter a maximum positive SSC excursion (parameter \"max_pos_ssc(ppm)\") that is greater than 0 and less than 50,000 ppm\n");
   printf("Read pdatastruct->max_pos_ssc_ppm = %1.4e.\n",pdatastruct->max_pos_ssc_ppm);
   return EXIT_FAILURE;
   }

/*Verify that SSC maximum negative excursion is < 50,000 ppm*/

if ((pdatastruct->max_neg_ssc_ppm >= 5e4) || (pdatastruct->max_neg_ssc_ppm < 0.0) || (pdatastruct->max_neg_ssc_ppm == INIT_VAL))
   {
   printf("Enter a maximum negative SSC excursion (parameter \"max_neg_ssc(ppm)\") that is greater than 0 and less than 50,000 ppm\n");
   printf("Read pdatastruct->max_neg_ssc_ppm = %1.4e.\n",pdatastruct->max_neg_ssc_ppm);
   return EXIT_FAILURE;
   }

/*Verify that VCO bandwidth is entered and greater than zero*/

if (pdatastruct->vco_tau <= 0.0)
   {
   printf("A VCO modulation bandwidth of less than zero was entered...\n");
   return EXIT_FAILURE;
   }
if ((2.0*M_PI)*(pdatastruct->vco_tau) < 1.0/((pdatastruct->D)*(pdatastruct->fo)))
   {
   printf("A VCO modulation bandwidth of greater than the VCO frequency was entered...\n");
   printf("(2.0*M_PI)*(pdatastruct->vco_tau)= %.4e, 1/(pdatastruct->D)*(pdatastruct->fo) = %.4e.\n",
   (2.0*M_PI)*(pdatastruct->vco_tau),1.0/((pdatastruct->D)*(pdatastruct->fo)));
   return EXIT_FAILURE;
   }

/* Verify that lock detect time constant is greater than zero and the specified timestep*/
/* lock detect time constant of 0 indicates no lock detector*/

if (pdatastruct->lock_detect_tau != INIT_VAL)
	{
	if (pdatastruct->lock_detect_tau < 0.0)
	   {
	   printf("A lock detector time constant of less than zero was entered...\n");
	   return EXIT_FAILURE;
	   }
	else
		{
		if ((pdatastruct->lock_detect_tau <= pdatastruct->deltat) && (pdatastruct->lock_detect_tau != 0.0))
		   {
		   printf("A lock detector time constant of less or equal to the time step %s was entered...\n",add_units(pdatastruct->deltat,3,"s",pvalue_string));
		   return EXIT_FAILURE;
		   }
		}
	}
else
	pdatastruct->lock_detect_tau = 0.0;

if (pdatastruct->lock_detect_tau > 0.0)
	{
	if ((strcmp(pdatastruct->plock_detect_delta_vc_limit_percent_string,pdefault) == 0) || (strcmp(pdatastruct->plock_detect_delta_vc_limit_percent_string,pmissing_entry) == 0))
   	pdatastruct->lock_detect_delta_vc_limit_percent = LOCK_DETECT_VC_DIFF_PERCENT_LIMIT;
   else
   	{
   	if (atof(pdatastruct->plock_detect_delta_vc_limit_percent_string) < 0.0) 
   		{
			printf("Enter value for lock_detect_delta_vc_limit(%%) greater than 0.0\n");
			printf("Check input file for parameter \"lock_detect_delta_vc_limit(%%)\".\n");
			return EXIT_FAILURE;
			}
		else
			pdatastruct->lock_detect_delta_vc_limit_percent = atof(pdatastruct->plock_detect_delta_vc_limit_percent_string);
		}
	if ((strcmp(pdatastruct->plock_detect_rphase_pp_limit_ui_string,pdefault) == 0) || (strcmp(pdatastruct->plock_detect_rphase_pp_limit_ui_string,pmissing_entry) == 0))
   	pdatastruct->lock_detect_rphase_pp_limit_ui = RPHASE_UI_HISTORY_PP_LIMIT;
   else
   	{
   	if (atof(pdatastruct->plock_detect_rphase_pp_limit_ui_string) < 0.0) 
   		{
			printf("Enter value for lock_detect_rphase_pp_limit(UIpp) greater than 0.0\n");
			printf("Check input file for parameter \"lock_detect_rphase_pp_limit(UIpp)\".\n");
			return EXIT_FAILURE;
			}
		else
			pdatastruct->lock_detect_rphase_pp_limit_ui = atof(pdatastruct->plock_detect_rphase_pp_limit_ui_string);
		}
	if ((strcmp(pdatastruct->plock_detect_rphase_pp_loss_lock_limit_ui_string,pdefault) == 0) || (strcmp(pdatastruct->plock_detect_rphase_pp_loss_lock_limit_ui_string,pmissing_entry) == 0))
   	pdatastruct->lock_detect_rphase_pp_loss_lock_limit_ui = RPHASE_UI_HISTORY_PP_LOSS_LOCK_LIMIT;
   else
   	{
   	if (atof(pdatastruct->plock_detect_rphase_pp_loss_lock_limit_ui_string) < 0.0) 
   		{
			printf("Enter value for lock_detect_rphase_pp_loss_lock_limit(UIpp) greater than 0.0\n");
			printf("Check input file for parameter \"lock_detect_rphase_pp_loss_lock_limit(UIpp)\".\n");
			return EXIT_FAILURE;
			}
		else
			{
		  	if (atof(pdatastruct->plock_detect_rphase_pp_loss_lock_limit_ui_string) < pdatastruct->lock_detect_rphase_pp_limit_ui) 
   			{
				printf("Enter value for lock_detect_rphase_pp_loss_lock_limit(UIpp) greater than\n");
				printf("value for lock_detect_rphase_pp_limit(UIpp) (%.3f)\n",
				pdatastruct->lock_detect_rphase_pp_limit_ui);
				printf("Check input file for parameter \"lock_detect_rphase_pp_loss_lock_limit(UIpp)\".\n");
				return EXIT_FAILURE;
				}
			else
				pdatastruct->lock_detect_rphase_pp_loss_lock_limit_ui = atof(pdatastruct->plock_detect_rphase_pp_loss_lock_limit_ui_string);
			}
		}
	}
	
/*Verify values for all loop filter components are greater than zero*/

if ((check_double_positive_non_zero(pdatastruct->R1,"R1",&loop_filter_element_status) == FALSE) \
|| (check_double_positive_non_zero(pdatastruct->R2,"R2",&loop_filter_element_status) == FALSE) \
|| (check_double_positive_non_zero(pdatastruct->R3,"R3",&loop_filter_element_status) == FALSE) \
|| (check_double_positive_non_zero(pdatastruct->RS1,"RS1",&loop_filter_element_status) == FALSE) \
|| (check_double_positive_non_zero(pdatastruct->RS2,"RS2",&loop_filter_element_status) == FALSE) \
|| (check_double_positive_non_zero(pdatastruct->C1,"C1",&loop_filter_element_status) == FALSE) \
|| (check_double_positive_non_zero(pdatastruct->C2,"C2",&loop_filter_element_status) == FALSE))
	return EXIT_FAILURE;


/*Verify that plotting tool preference is valid and assign value*/

if (strcmp(pdatastruct->p_plot_preference,"octave") == 0)
   pdatastruct->plot_preference = OCTAVE;
if (strcmp(pdatastruct->p_plot_preference,"gnuplot") == 0)
   pdatastruct->plot_preference = GNUPLOT;
if ((pdatastruct->plot_preference < 0) || (pdatastruct->plot_preference > 1))
   {
   printf("\nERROR:Did not recognize plotting tool preference\noption in check_data.c \"%s\"!", pdatastruct->p_plot_preference);
   return EXIT_FAILURE;
   }


return EXIT_SUCCESS;

}
