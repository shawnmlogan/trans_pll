
#include "globals.h"

int pd_hogge(struct signal *psig,int VIN,int *pSTATE,int *perror)
{
/* This routine takes the state of CLK and VIN (both already determined) */
/* and returns the state of the resulting Hogge phase detector. */
/* It uses doubles psig->vq1 and psig->vq2 to determine the state. */

int posv = 0,negv = 0, error_flag = 0;
int VA = 0,VB = 0,CLK = 0;
static int OLD_CLK;

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
	psig->vq1 = VIN;
   }
if ((posv == 0) && (negv == 1))
	{
	psig->vq2 = psig->vq1;
   }
if ((posv == 1) && (negv == 1))
  {
   printf("Pos and neg edge occurred at same time - exiting\n");
   error_flag = 1;
   }
   
/*Run through combinational logic*/
   
VA = dexor(psig->vq1,(double) VIN);
VB = dexor(psig->vq1,psig->vq2);

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