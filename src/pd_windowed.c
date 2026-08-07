
#include "globals.h"
#define BIG_RESISTOR 1.0/EPSILON

int pd_windowed(struct signal *psig,double time, Filedata *pdatain, int *pSTATE)
{

/*This phase detector uses a D type flip-flop phase detector if the phase of the feedback clock
and the divided down input clock is less (in magnitude) than psig->tw. Otherwise, it tristates the
DFF output and sets psig->vs1 and psig->vs2 depending on a direction indicator*/

/*Initialize static variables to track the number of 2pi increments
that phasein and phaseo have undergone. Incremented if phase goes
through 2pi interval*/

/*10-28-99, add tauff and deadzone to better model delay and minimum
pulse width impact*/

int ORIG_STATE = 0, old_filter_select_flag = 0, old_freq_direction = 0;

static long j,m,dj,dm;
static int timer1set, timer2set, filter_select_flag, freq_direction, end_of_simulation_flag;

char *plog_string, log_string[LOGFILE_LINELENGTH + 1];
char value_string[NUMBER_OF_VALUE_STRINGS][LINELENGTH_OF_VALUE_STRING + 1];

static double tend1, sample_value, old_sample_value,sin_deadzone, orig_R1, orig_RS1, orig_RS2;
static double tend2, sample_value2, old_sample_value2, prpoint;
double rphase = 0.0;

/*Initialize pointers*/

plog_string = &log_string[0];


if (psig->phaseo == 0.0) /*Initialize all variables to initial state at start of simulation*/
   {
   j = 0;
   m = 0;
   dj = 0;
   dm = 0;
   timer1set = 0;
   timer2set = 0;
   sample_value = 0.0;
   old_sample_value = 0.0;
   sample_value2 = 0.0;
   old_sample_value2 = 0.0;
   sin_deadzone = sin (2.0 * pi * pdatain->pfd_dff_deadzone_ui);
   snprintf(plog_string,LOGFILE_LINELENGTH,
   "Assuming a deadzone of %1.3e UI with fin = %1.6e Hz.\n",
   pdatain->pfd_dff_deadzone_ui, pdatain->fin);
   print_string_to_log(plog_string,pdatain);
   tend1 = -999.0;
   tend2 = -999.0;
   orig_R1 = pdatain->R1; /*Static variables used to store the original entered values for loop filter resistors*/
   orig_RS1 = pdatain->RS1; /*These are set to high values to effectively tri-state the output*/
   orig_RS2 = pdatain->RS2;
   old_filter_select_flag = 0;
   old_freq_direction = 0;
   prpoint = (pdatain->TSTOP - pdatain->TSTART)/(pdatain->npoints - 0.0);
   end_of_simulation_flag = 0;
   }

if (end_of_simulation_flag != 1)
	{
	old_freq_direction = freq_direction; /*Save frequency direction flag*/
	old_filter_select_flag = filter_select_flag; /*Save filter selection flag*/
	
	/*Subroutine to determine the output of D flip-flop phase detector.
	This routine examines the phase of the two input signals (if required
	for the specific phase detector) and modifies the integer STATE if required.
	For a D flip-flop, a sample is taken if the phase of the VCXO (sig.phaseo)
	crosses 2pi. The routine returns a "1" if a sample was taken and a "0"
	if no sample was taken*/
	
	if (psig->phaseo_delayed/(2.0 * pi) >= j) 
	   {
	   /*Transition occured;*/
	
	   /*Sample input clock and set timer to create a delayed version of psig->vq1*/
	
	   if (timer1set == 0)
	      {
	      if (sin(psig->phasein) > sin_deadzone)
	         sample_value = 1.0;
	      else
	         {
	         if (sin(psig->phasein) < (0.0 - sin_deadzone))
	            sample_value = 0.0;
	         else /*If within deadzone of the DFF, toggle the state*/
	            {
	            if (old_sample_value == 1.0)
	               sample_value = 0.0;
	            else
	               sample_value = 1.0;
	            }
	         }
	      tend1 = time + pdatain->tauff;
	      timer1set = 1;
	      old_sample_value = sample_value;
	      }
	   else
	      {
	      snprintf(plog_string,LOGFILE_LINELENGTH,
			"Error in pd_windowed.c! A second transition at VQ before");
			print_string_to_log(plog_string,pdatain);
			snprintf(plog_string,LOGFILE_LINELENGTH,
			"the held version of VQ was passed to the DFF at time = %2.4e...exiting.\n",time);
			print_string_to_log(plog_string,pdatain);
	      exit(0);
	      }
	   j++;
	   }
	
	   /*Find value of delayed version of the DFF output by seeing if first timer 
	is up*/
	
	if ((time > tend1) && (timer1set == 1)) /*Note minimum value of tauff is set by integration time!*/
	   {
	   psig->vq1 = sample_value;
	   timer1set = 0;
	   }
	   
	/*Before making decisions, make sure that signals are only in valid logic states*/
	
	if ((psig->vq1 != 0.0) && (psig->vq1 != 1.0)) 
	   {
	   snprintf(plog_string,LOGFILE_LINELENGTH,
	   "At time = %e, psig-vq1 = %e in pd_windowed.c. Error should be 1.0 or 0.0!\n",
	   time, psig->vq1);
		print_string_to_log(plog_string,pdatain); 
	   exit(0);
	   }
	   
	/*Sample value of feedback clock to determine direction indicator*/
	if (psig->phasein/(2.0 * pi) >= dj) 
	   {
	   /*Transition occured in input clock*/
	
	   /*Sample feedback clock and set timer to create a delayed version of psig->vq2*/
	
	   if (timer2set == 0)
	      {
	      if (sin(psig->phaseo_delayed) > sin_deadzone)
	         sample_value2 = 1.0;
	      else
	         {
	         if (sin(psig->phaseo_delayed) < (0.0 - sin_deadzone))
	            sample_value2 = 0.0;
	         else /*If within deadzone of the DFF, toggle the state*/
	            {
	            if (old_sample_value2 == 1.0)
	               sample_value2 = 0.0;
	            else
	               sample_value2 = 1.0;
	            }
	         }
	      tend2 = time + pdatain->tauff;
	      timer2set = 1;
	      old_sample_value2 = sample_value2;
	      }
	   else
	      {
	      snprintf(plog_string,LOGFILE_LINELENGTH,
			"Error in pd_windowed.c! A second transition at VQ before\n");
			print_string_to_log(plog_string,pdatain); 
	      snprintf(plog_string,LOGFILE_LINELENGTH,		
			"the held version of VQ was passed to the DFF2 at time = %2.4e...exiting.\n",time);
			print_string_to_log(plog_string,pdatain); 	
	      exit(0);
	      }
	   dj++;
	   }
	
	   /*Find value of delayed version of the DFF2 output by seeing if first timer 
	is up*/
	
	if ((time > tend2) && (timer2set == 1)) /*Note minimum value of tauff is set by integration time!*/
	   {
	   psig->vq2 = sample_value2;
	   /*psig->vq2 is the direction indicator, a value of 1.0 =>slow down
	      feedback clock, a value of 0.0 => speed up feedback clock*/
	   timer2set = 0;
	   }
	   
	/*Determine which signal sources to set based on relative phase between
	input and output clocks - only use routine if time exceeds pdatain->tacq_max*/
	
	rphase = psig->phaseo_delayed - psig->phasein;
	   
	if ((fabs(rphase/(2.0 * pi) - round(rphase/(2.0 * pi))) >=  (pdatain->tw)*pdatain->fin) && (time > pdatain->tacq_max))
	   {
	   filter_select_flag = 1; /*Set filter flag to one =>current source based phase detector*/
	   if (old_filter_select_flag != filter_select_flag)
	   	{
	      snprintf(plog_string,LOGFILE_LINELENGTH,
	      "Windowed phase detector using current source based phase detector at time = %e.\n",time);
	      print_string_to_log(plog_string,pdatain);
	      }
	   if (psig->vq2 > 0.50)
	      {
	      if (old_filter_select_flag != filter_select_flag)
	      	{
	         snprintf(plog_string,LOGFILE_LINELENGTH,
	         "Relative phase is greater than detector window of %s at time = %e.\n",
	         add_units((pdatain->tw)*pdatain->fin,3,"UI",value_string[0]),time);
	         print_string_to_log(plog_string,pdatain);
	         }
	      /*Reduce frequency*/
	      pdatain->R1 = BIG_RESISTOR;
	      pdatain->RS1 = orig_RS1;
	      pdatain->RS2 = orig_RS2;
	      psig->vs1 = 0.0 - pdatain->vs1;
	      psig->vs2 = 0.0 - pdatain->vs2;     
	      freq_direction = 0;
	      
	      }
	   else
	      {
	      if (old_filter_select_flag != filter_select_flag)
	      	{
	         snprintf(plog_string,LOGFILE_LINELENGTH,
	         "Relative phase is less than detector window of %s at time = %e.\n",
	         add_units((pdatain->tw)*pdatain->fin,3,"UI",value_string[0]),time);
	         print_string_to_log(plog_string,pdatain);
	         }
	      /*Increase frequency*/
	      pdatain->R1 = BIG_RESISTOR;
	      pdatain->RS1 = orig_RS1;
	      pdatain->RS2 = orig_RS2;
	      psig->vs1 = 0.0 + pdatain->vs1;
	      psig->vs2 = 0.0 + pdatain->vs2;
	      freq_direction = 1;
	      }
	   }
	else
	   {
	   /*Outside window where current sources are used - set the normal DFF to its active state*/
	   filter_select_flag = 0; /*Set filter flag to zero => DFF phase detector*/
	   if (old_filter_select_flag != filter_select_flag)
	   	{
	      snprintf(plog_string,LOGFILE_LINELENGTH,
	      "Relative phase is within detector window of %s at time = %e.\n",
	      add_units((pdatain->tw)*pdatain->fin,3,"UI",value_string[0]),time);
	      print_string_to_log(plog_string,pdatain);
	      snprintf(plog_string,LOGFILE_LINELENGTH,
	      "Windowed phase detector using DFF based phase detector.\n");
	      print_string_to_log(plog_string,pdatain);
	      }
	   pdatain->R1 = orig_R1;
	   pdatain->RS1 = BIG_RESISTOR;
	   pdatain->RS2 = BIG_RESISTOR;
	   }
	
	/*Set output state */
	
	ORIG_STATE = *pSTATE; 
	
	if ((psig->vq1 == 1.0) && (filter_select_flag == 0)) /*Using DFF based phase detector*/
	   *pSTATE = 1;
	else
	   {
	   if ((psig->vq1 == 0.0) && (filter_select_flag == 0)) /*Using DFF based phase detector*/
	      *pSTATE = 0;
	   else
	      {
	      if ((freq_direction == 1) && (filter_select_flag == 1))/*Using current based phase detector*/
	         *pSTATE = 3;
	      else
	         {
	         if ((freq_direction == 0) && (filter_select_flag == 1))/*Using current based phase detector*/
	            *pSTATE = 3;
	         else
	            {
	            snprintf(plog_string,LOGFILE_LINELENGTH,
	            "An error occured at time %e in pd_windowed.c. psig->vq1\n",time);
	            print_string_to_log(plog_string,pdatain);
	            snprintf(plog_string,LOGFILE_LINELENGTH,
	            "was neither 1.0 nor 0.0 or filter_select_flag was neither 1 nor 0....exiting\n");
	            print_string_to_log(plog_string,pdatain);
	            exit(0);
	            }
	         }
	      }
	   }
	
	/*    prpoint = (pdatain->TSTOP - pdatain->TSTART)/(pdatain->npoints - 0.0); */
	/*    while (time < (pdatain->TSTOP + prpoint)) */
	
	if ((time + pdatain->deltat) >= (pdatain->TSTOP + prpoint))
		{
		/* snprintf(plog_string,LOGFILE_LINELENGTH,
		"Detected end of simulation at time = %1.6e (TSTOP = %1.6e). Restoring values of RS1, RS2\n",
		time,pdatain->TSTOP);
		print_string_to_log(plog_string,pdatain);
		snprintf(plog_string,LOGFILE_LINELENGTH,
		"to: RS1 =  %1.6e, RS2 = %1.6e, R1 = %1.6e\n",
		orig_RS1,orig_RS2,orig_R1);
		print_string_to_log(plog_string,pdatain); */
		pdatain->RS1 = orig_RS1;
	   pdatain->RS2 = orig_RS2;
		pdatain->R1 = orig_R1;
		end_of_simulation_flag = 1;
		}
	}
else
	{
	*pSTATE = ORIG_STATE;
	filter_select_flag = old_filter_select_flag;
	}
	
if ((*pSTATE == ORIG_STATE) && (filter_select_flag == old_filter_select_flag))
	return(0); /*No changes to the state of DFF or filter - no need to re-compute coefficients*/
else
	return(1); /*A change in output state occured as either a different loop type was selected
	or a change in the DFF phase detector occured. Re-compute coefficients*/	                                         
}
