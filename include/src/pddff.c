
#include "globals.h"

int pddff(struct signal *psig,double time, Filedata *pdatain, int *pSTATE)
{

/*Initialize static variables to track the number of 2pi increments
that phasein and phaseo have undergone. Incremented if phase goes
through 2pi interval*/

/*10-28-99, add tauff and deadzone to better model delay and minimum
pulse width impact*/

int ORIG_STATE = 0;

char *plog_string,log_string[LOGFILE_LINELENGTH + 1];

static long j,m;
static int timer1set;
static double tend1,sample_value, old_sample_value,sin_deadzone;

plog_string = &log_string[0];

if (time == 0.0) /*Initialize all variables to initial state at start of simulation*/
   {
   j = 0;
   m = 0;
   timer1set = 0;
   sample_value = 0.0;
   old_sample_value = 0.0;
   sin_deadzone = sin (2.0 * pi * pdatain->pfd_dff_deadzone_ui);
   snprintf(plog_string,LOGFILE_LINELENGTH,
   "Assuming a deadzone of %1.3e UI with fin = %1.6e Hz.\n",
   pdatain->pfd_dff_deadzone_ui, pdatain->fin);
   print_string_to_log(plog_string,pdatain);
   tend1 = -999.0;
   }

/*Subroutine to determine the output of D flip-flop phase detector.
This routine examines the phase of the two input signals (if required
for the specific phase detector) and modifies the integer STATE if required.
For a D flip-flop, a sample is taken if the phase of the VCXO (sig.phaseo)
crosses 2pi. The routine returns a "1" if a sample was taken and a "0"
if no sample was taken*/

if (psig->phaseo/(2.0 * pi) >= j) 
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
      printf("Error in pddff.c! A second transition at VQ before\n\
the held version of VQ was passed to the DFF at time = %2.4e...exiting.\n",time);
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
   printf("At time = %e, psig-vq1 = %e in pddff.c. Error should be 1.0 or 0.0!\n",
   time, psig->vq1);
   exit(0);
   }
    
/*Set output state */

ORIG_STATE = *pSTATE; 

if (psig->vq1 == 1.0)
   *pSTATE = 1;
else
   {
   if (psig->vq1 == 0.0)
      *pSTATE = 0;
   else
      {
      printf("An error occured at time %e in pddff.c. psig->vq1\n",time);
      printf("was neither 1.0 nor 0.0.\n");
      exit(0);
      }
   }
   
if (*pSTATE == ORIG_STATE)
   return(0);
else 
   return(1); /*A change in output state occured*/
}
