
#include "globals.h"

int pd_alexander(struct signal *psig,Filedata *pdatain,double time,int VIN,int *pSTATE,int *perror)
{
/* This routine takes the state of CLK and VIN (both already determined) */
/* and returns the state of the resulting Alexander phase detector. */
/* It uses doubles psig->vq1, psig->vq2, psig->vq3, and psig->vq4 and */
/* two exor gates to determine the state. */

int posv = 0, negv = 0, error_flag = 0;
int VA = 0, VB = 0, CLK = 0;
static int OLD_CLK;

char *plog_string,log_string[LOGFILE_LINELENGTH + 1];
char value_string[NUMBER_OF_VALUE_STRINGS][LINELENGTH_OF_VALUE_STRING + 1];

plog_string = &log_string[0];

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
   psig->vq3 = psig->vq1; /* DFF 3 */
   psig->vq1 = VIN; /* DFF 1 */
   psig->vq4 = psig->vq2; /* DFF 4 */
   }
if ((posv == 0) && (negv == 1))
  {
   psig->vq2 = VIN; /* DFF 2 */
   }
if ((posv == 1) && (negv == 1))
  {
   snprintf(plog_string,LOGFILE_LINELENGTH,
   "Pos and neg edge occurred simultaneously at %s in pd_alexander() - error!\n",
	add_units(time,8,"s",value_string[0]));
   print_string_to_log(plog_string,pdatain);
   error_flag = 1;
   }
/*Run through combinational logic*/
   
VB = dexor(psig->vq1,psig->vq4);
VA = dexor(psig->vq3,psig->vq4);

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