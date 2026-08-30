
#include "globals.h"

int pd_hogge_with_delay(struct signal *psig,double time,Filedata *pdatain,int VIN,int *pSTATE,int *perror)
{
/* This routine takes the state of CLK and VIN (both already determined) */
/* and returns the state of the resulting Hogge phase detector. */
/* It uses doubles psig->vq1 and psig->vq2 to determine the state. */

char *plog_string,log_string[LOGFILE_LINELENGTH + 1];
int posv = 0,negv = 0, error_flag = 0;
int CLK = 0;

static int VA, VB;
static int OLD_CLK;
static int timer1set, timer2set, timer3set, timer4set;
static double tend1,sample1_value, old_sample1_value;
static double tend2,sample2_value, old_sample2_value;
static double tend3,sample3_value, old_sample3_value;
static double tend4,sample4_value, old_sample4_value;

plog_string = &log_string[0];

if (time == 0.0) /*Initialize all variables to initial state at start of simulation*/
   {
   timer1set = 0;
   timer2set = 0;
   timer3set = 0;
   timer4set = 0;
   sample1_value = 0.0;
   old_sample1_value = 0.0;
   sample2_value = 0.0;
   old_sample2_value = 0.0;
   sample3_value = 0.0;
   old_sample3_value = 0.0;
   sample4_value = 0.0;
   old_sample4_value = 0.0;
   tend1 = tend2 = tend3 = tend4 = -999.0;
   }

/*Subroutine to determine the output of Hogge phase detector*/

CLK = psig->clk;

if ((CLK == 1) && (OLD_CLK == 0))
   {
   posv = 1;
   negv = 0;
   }
else 
   {
   if ((CLK == 0) && (OLD_CLK == 1))
      {
      posv = 0;
      negv = 1;
      }
   }
   
/*See if an edge occurred and clock in data into one of two DFF's*/

if ((posv == 1) && (negv == 0))
	{
	sample1_value = VIN;
	tend1 = time + pdatain->tauff;
   timer1set = 1;
   old_sample1_value = sample1_value;
	/* psig->vq1 = VIN; */
   }
if ((posv == 0) && (negv == 1))
	{
	sample2_value = psig->vq1;
	tend2 = time + pdatain->tauff;
   timer2set = 1;
   old_sample2_value = sample2_value;
	/* psig->vq2 = psig->vq1; */
   }
if ((posv == 1) && (negv == 1))
	{
	snprintf(plog_string,LOGFILE_LINELENGTH,
   "Pos and neg edge occurred at same time - exiting\n");
   print_string_to_log(plog_string,pdatain);
   error_flag = 1;
   }

   /*Find value of delayed version of the DFF outputs by seeing if either timer 
is up*/

if ((time > tend1) && (timer1set == 1)) /*Note minimum value of tauff is set by integration time!*/
   {
   psig->vq1 = sample1_value;
   timer1set = 0;
   }
if ((time > tend2) && (timer2set == 1)) /*Note minimum value of tauff is set by integration time!*/
   {
   psig->vq2 = sample2_value;
   timer2set = 0;
   }
 
/*Run through combinational logic*/
   
/* VA = dexor(psig->vq1,(double) VIN);
VB = dexor(psig->vq1,psig->vq2); */


if ((dexor(psig->vq1,(double) VIN) != old_sample3_value) && (timer3set != 1))
	{
	sample3_value = dexor(psig->vq1,(double) VIN);
	tend3 = time + pdatain->tauh;
   timer3set = 1;
   old_sample3_value = sample3_value;
   }

if ((dexor(psig->vq1,psig->vq2) != old_sample4_value) && (timer4set != 1))
	{
	sample4_value = dexor(psig->vq1,psig->vq2);
	tend4 = time + pdatain->tauh;
   timer4set = 1;
   old_sample4_value = sample4_value;
   }
   
   /*Find value of delayed version of the EXOR outputs by seeing if either timer 
is up*/

if ((time > tend3) && (timer3set == 1)) /*Note minimum value of tauh is set by integration time!*/
   {
   VA = sample3_value;
   timer3set = 0;
   }
if ((time > tend4) && (timer4set == 1)) /*Note minimum value of tauh is set by integration time!*/
   {
   VB = sample4_value;
   timer4set = 0;
   }

/*Set output state depending on VA, VB*/

if ((VA == 1) && (VB == 0))
   *pSTATE = 1;
else
   {
   if ((VA == 0) && (VB == 1))
      *pSTATE = 0;
   else
      *pSTATE = 3;
   }

OLD_CLK = CLK;

if (error_flag != 1)
	return EXIT_SUCCESS;
else
	return EXIT_FAILURE;
}
