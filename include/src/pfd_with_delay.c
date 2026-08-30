
#include "globals.h"

int pfd_with_delay(struct signal *psig,double time,double tauh,double tauff,double taucp_min,double phase_dz_ui,int *pSTATE)
{

/*Initialize static variables to track the number of 2pi increments
that phasein and phaseo_delayed have undergone. Incremented if phase goes
through 2pi interval*/

int ORIG_STATE = 0;
double rphase_ui = 0.0;
static int j,m,r,v, timer1set, timer2set, timer3set;
static double tend1, tend2, tend3;

if (psig->phaseo == 0.0) /*Initialize all variables to initial state at start of simulation*/
   {
   j = 1;
   m = 1;
   r = 0;
   v = 0;
   timer1set = 0;
   timer2set = 0;
   timer3set = 0;
   tend1 = -999.0;
   tend2 = -999.0;
   tend3 = -999.0;
   }
/*Subroutine to determine the output of PFD phase detector.
This routine examines the phase of the two input signals (if required
for the specific phase detector) and sets two DFF's to the required state.
A delay psig->tauh occurs between the time the output of both flip-flops
goes to a logic high and when the two flip-flops are reset.
For a PFD, the output state may change if the R or V inputs undergoes a 
positive transition. A positive edge transition is defined as when a given
input phase undergoes an integral number of 2pi radians.The routine returns
a "1" if a sample was taken and a "0" if no sample was taken*/
/*Signals psiq-vq1 and psig-vq2 are the outputs of the UP and DOWN flip-flops
respectively*/

/*Save initial state - only want to change vpd if a change in state occurs to
	minimize processing time*/
	
ORIG_STATE = *pSTATE; 
rphase_ui = (psig->phaseo_delayed - psig->phasein)/(2.0 * pi);

if (fabs(rphase_ui) >= phase_dz_ui)
	{	
	if ((psig->phaseo_delayed/(2.0 * pi) >= j) && (psig->vq1 == 0.0))
	   {
	   /*Transition occured;*/
	   /*Set timer to create a delayed version of psig->vq1(vu)*/
	
	   if (timer1set == 0)
	      {
	      tend1 = time + tauff;
	      timer1set = 1;
	      }
	   else
	      {
	      /*printf("Error in pfd()! A second transition at VQ1 before\n\
	the held version of VQ1 was passed to FF1 at time = %2.4e...exiting.\n",time);
	      exit(0);*/
	      }
	   j++;
	   }
	   /*Find value of delayed version of FF1 output by seeing if first timer is up*/
	
	if ((time > tend1) && (timer1set == 1)) /*Note minimum value of tauff is set by integration time!*/
	   {
	   psig->vq1 = 1.0;
	   timer1set = 0;
	   }
	   
	if ((psig->phasein/(2.0 * pi) >= m) && (psig->vq2 == 0.0))
	   {
	   /*Transition occured*/
	   /*Set timer to create a delayed version of psig->vq2*/
	
	   if (timer2set == 0)
	      {
	      tend2 = time + tauff;
	      timer2set = 1;
	      }
	   else
	      {
	      /*printf("Error in pfd()! A second transition at VQ2 before\n\
	the held version of VQ2 was passed to FF1 at time = %2.4e...exiting.\n",time);
	      exit(0);*/
	      }
	   m++;
	   }
	   /*Find value of delayed version of FF2 output by seeing if first timer is up*/
	
	if ((time > tend2) && (timer2set == 1)) /*Note minimum value of tauff is set by integration time!*/
	   {
	   psig->vq2 = 1.0;
	   timer2set = 0;
	   }
	/*As PFD cannot produce an output whose width is arbitrarily small, verify that if both timers are
	set the pulse width exceeds a minimum PFD width. If the time does not, turn off both timers*/
	
	/*if ((timer1set == 1) && (timer2set == 1) && ((tend2 - tend1) > 0.0) && ((tend2 - tend1) < taucp_min) )*/
	if ((timer1set == 1) && (timer2set == 1) && (fabs(tend2 - tend1) < taucp_min) )
	/*if ((timer1set == 1) && (timer2set == 1) && ((tend2 - tend1) > 0.0) && ((tend2 - tend1) < 5e-12) )*/
	   {
	   /*printf("tend2 - tend1 = %1.3e ps; setting cp timers to 0 since less than %.1f ps.\n",(tend2 - tend1)/1e-12,taucp_min/1e-12);*/
	   timer1set = 0;
	   timer2set = 0;
	   }
	
	/*Before making decisions, make sure that signals are only in valid logic states*/
	
	if ((psig->vq1 != 0.0) && (psig->vq1 != 1.0)) 
	   {
	   printf("At time = %e, psig-vq1 = %e in pfd(). Error should be 1.0 or 0.0!\n",
	   time, psig->vq1);
	   exit(0);
	   }
	if ((psig->vq2 != 0.0) && (psig->vq2 != 1.0)) 
	   {
	   printf("At time = %e, psig-vq2 = %e in pfd(). Error should be 1.0 or 0.0!\n",
	   time, psig->vq2);
	   exit(0);
	   }
	    
	/*Determine if a there is  a reset condition to both flip-flops*/ 
	
	if ((psig->vq1 == 1.0) && (psig->vq2  == 1.0))
	   {
	   if (timer3set == 0)
	      {
	      tend3 = time + tauh;
	      timer3set = 1; 
	      }
	   else
	      {
	       /*Do nothing as we're waiting for first reset to clear*/
	      }
	   }
	if ((time >= tend3) && (timer3set == 1)) /*Note minimum value of tauh is integration time*/
	   {
	   /*Timer is up for AND gate and prop delay to reset flip-flops, reset the output of the two flip flops*/
	   psig->vq1 = 0.0;
	   psig->vq2 = 0.0;
	   timer3set = 0;
	   }

	/*Set output state depending on VUP, VDOWN*/	
	
	if ((psig->vq1 == 1.0) && (psig->vq2 == 0.0))
	   *pSTATE = 0;
	else
	   {
	   if ((psig->vq1 == 0.0) && (psig->vq2 == 1.0))
	      *pSTATE = 1;
	   else
	      *pSTATE = 3;
	   }
   }
else
   *pSTATE = 3;
   
if (*pSTATE == ORIG_STATE)
   return(0);
else 
   return(1); /*A change in output state occured*/
}
