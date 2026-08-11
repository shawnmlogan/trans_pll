
#include "globals.h"

int pd_exor_with_delay(struct signal *psig,int CLKIN,double time,Filedata *pdatain,int *pSTATE)
{
/* This routine takes the state of psig->clk and psig->vco_clk */
/* and returns the state of the resulting exclusive-or based phase detector. */
/* It returns the result to *pSTATE. If the relative time of the two clocks are less */
/* the eadzone, it sets *pSTATE to 3 - a high-impedance state.*/

char *plog_string,log_string[LOGFILE_LINELENGTH + 1];
int ORIG_STATE,error_flag = 0;
double rphase_ui = 0.0;

static int timer1set;
static double tend1,sample1_value, old_sample1_value;

plog_string = &log_string[0];

if (time == 0.0) /*Initialize all variables to initial state at start of simulation*/
   {
   timer1set = 0;
   sample1_value = 0.0;
   old_sample1_value = 0.0;
   tend1 = -999.0;
   }

/*Subroutine to determine the output of Hogge phase detector*/

ORIG_STATE = *pSTATE;
rphase_ui = (psig->phaseo_delayed - psig->phasein)/(2.0 * pi);

if (fabs(rphase_ui) >= pdatain->pfd_dff_deadzone_ui)
	{	 
	/*Run through combinational logic*/
   
	if ((dexor(psig->clk,CLKIN) != old_sample1_value) && (timer1set != 1))
		{
		if (dexor(psig->clk,CLKIN) == 1)
			sample1_value = 1;
		else
			sample1_value = 0;
		tend1 = time + pdatain->tauh;
	   timer1set = 1;
	   old_sample1_value = sample1_value;
	   }
		   
	   /*Find value of delayed version of the EXOR outputs by seeing if either timer 
	is up*/
	
	if ((time > tend1) && (timer1set == 1)) /*Note minimum value of tauh is set by integration time!*/
	   {
	   *pSTATE = sample1_value;
	   timer1set = 0;
	   }
	}
else	
	*pSTATE = 3;

if (*pSTATE == ORIG_STATE)
	return(0); /*No changes to the state - no need to re-compute coefficients*/
else
	return(1); /*A change in output state occurred. Re-compute coefficients*/	                                         

}