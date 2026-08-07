
#include "globals.h"

int pd_alexander_with_delay(struct signal *psig,Filedata *pdatain,double time,int VIN,int *pSTATE,int *perror)
{
/* This routine takes the state of CLK and VIN (both already determined) */
/* and returns the state of the resulting Alexander phase detector. */
/* It uses doubles psig->vq1, psig->vq2, psig->vq3, and psig->vq4 and */
/* two exor gates to determine the state. */

int posv = 0, negv = 0, error_flag = 0;
int CLK = 0;

static int VA, VB;
static int OLD_CLK;
static int timer1set, timer2set, timer3set, timer4set, timer5set, timer6set;
static double tend1,sample1_value;
static double tend2,sample2_value;
static double tend3,sample3_value;
static double tend4,sample4_value;
static double tend5,sample5_value, old_sample5_value;
static double tend6,sample6_value, old_sample6_value;

char *plog_string,log_string[LOGFILE_LINELENGTH + 1];
char value_string[NUMBER_OF_VALUE_STRINGS][LINELENGTH_OF_VALUE_STRING + 1];

plog_string = &log_string[0];

if (time == 0.0) /*Initialize all variables to initial state at start of simulation*/
   {
   timer1set = 0;
   timer2set = 0;
   timer3set = 0;
   timer4set = 0;
   timer5set = 0;
   timer6set = 0;
   sample1_value = 0.0;
   sample2_value = 0.0;
   sample3_value = 0.0;
   sample4_value = 0.0;
   sample5_value = 0.0;
   old_sample5_value = 0.0;
   sample6_value = 0.0;
   old_sample6_value = 0.0;
   tend1 = tend2 = tend3 = tend4 = tend5 = tend6 = -999.0;
   }

/*Subroutine to determine the output of Alexander phase detector*/

CLK = psig->clk;

if ((CLK == 1) && (OLD_CLK == 0))
   {
   posv = 1;
   }
else 
   {
   if ((CLK == 0) && (OLD_CLK == 1))
      {
      negv = 1;
      }
   }
   
/*See if an edge occured and clock in data into four DFF's*/

if ((posv == 1) && (negv == 0))
	{
	sample3_value = psig->vq1; /* DFF 3 */	
	tend3 = time + pdatain->tauff;
	timer3set = 1;

	sample1_value = VIN; /* DFF 1 */
	tend1 = time + pdatain->tauff;
   timer1set = 1;
   
	sample4_value = psig->vq2; /* DFF 4 */
	tend4 = time + pdatain->tauff;
	timer4set = 1;
   }
if ((posv == 0) && (negv == 1))
	{
	sample2_value = VIN; /* DFF 2 */
	tend2 = time + pdatain->tauff;
   timer2set = 1;
   }
if ((posv == 1) && (negv == 1))
  {
   snprintf(plog_string,LOGFILE_LINELENGTH,
   "Pos and neg edge occurred simultaneously at %s in pd_alexander() - error!\n",
	add_units(time,8,"s",value_string[0]));
   print_string_to_log(plog_string,pdatain);
   error_flag = 1;
   }
   
   /*Find values of delayed version of the DFF outputs by seeing if any timer 
is up*/

if ((time >= tend3) && (timer3set == 1))
   {
   psig->vq3 = sample3_value;
   timer3set = 0;
   }
if ((time >= tend1) && (timer1set == 1)) /*Note minimum value of tauff is set by integration time!*/
   {
   psig->vq1 = sample1_value;
   timer1set = 0;
   }
if ((time >= tend4) && (timer4set == 1))
	{
   psig->vq4 = sample4_value;
   timer4set = 0;
   }
if ((time >= tend2) && (timer2set == 1))
   {
   psig->vq2 = sample2_value;
   timer2set = 0;
   }

/*Run through combinational logic*/

if ((dexor(psig->vq1,psig->vq4) != old_sample5_value) && (timer5set != 1))
	{
	sample5_value = dexor(psig->vq1,psig->vq4);
	tend5 = time + pdatain->tauh;
   timer5set = 1;
   old_sample5_value = sample5_value;
   }
if ((dexor(psig->vq3,psig->vq4) != old_sample6_value) && (timer6set != 1))
	{
	sample6_value = dexor(psig->vq3,psig->vq4);
	tend6 = time + pdatain->tauh;
   timer6set = 1;
   old_sample6_value = sample6_value;
   }

   /*Find value of delayed version of the EXOR outputs by seeing if either timer 
is up*/

 if ((time >= tend5) && (timer5set == 1))
   {
   VB = sample5_value;
   timer5set = 0;
   }
if ((time >= tend6) && (timer6set == 1))
   {
   VA = sample6_value;
   timer6set = 0;
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

if (error_flag != 0)
	return EXIT_FAILURE;
else
	return EXIT_SUCCESS;
}