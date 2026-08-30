
#define CASH_KARP
#include "globals.h"

int rkstep5(double tinit,double tfinal,struct coeff *pw,struct signal *psig, double fo,\
int vc_model, double *pvco,int degree_of_polynomial,\
double vmax, double vmin,double errmax,int iter)

{

/*This function computes the output signals due to a two
pole system with time constants in the structure w using runge-kutta
embedded algorithm to provide fifth order runge-kutta integration
and step size adjustment.

psig = pointer to structure containing vpd,vc1,vc2,vc,phasein,phaseo
tinit = time at start of iteration
tfinal = time at which new estimate of output voltage is required
fo = nominal frequency of VCXO in loop
errmax = maximum value of phase change
iter = maximum of number of timesetp reductions allowed before error
message is given and routine exits*/

double h,deltat,error,time,newstep,maxerror,dvc1,dvc2;
double k1,k2,k3,k4,k5,k6;
double l1,l2,l3,l4,l5,l6;
double j1,j2,j3,j4,j5,j6;
double temp = 0.0;
int i, status = 0, reducestep;
/*static int count = 0;*/
struct signal signew,signewb;
char signal[80];

/*printf("Entered rkv1...tinit = %4.2e, tfinal =%4.2e\n",tinit,tfinal);*/
/*Compute initial derivatives*/

dvc1 = f1(pw,psig->vs1,psig->vs2,psig->vpd,psig->vc1,psig->vc2);
dvc2 = f2(pw,psig->vs1,psig->vs2,psig->vpd,psig->vc1,psig->vc2);

/*Define initial step size (maximum) and stop time*/

time = tinit;
deltat = tfinal - tinit;

/*Start integration with h = deltat*/

h = deltat;

/*initialize loop counter i*/

i = 1;
/*printf("Before while loop, psig->vpd = %2.2e psig->vc1 %2.2e psig->vc2=%2.2e\n",
psig->vpd,psig->vc1,psig->vc2);*/

/*Perform integration until timestep causes time to exceed tfinal*/

while (time != tfinal )
   {
/*printf("In while loop, time = %2.2e, time + h = %2.2e, tfinal = %2.2e\n",
time,time+h,tfinal);*/
/*printf("In while loop,psig->vpd = %2.2e psig->vc1=%2.2e psig->vc2=%2.2e\n",
psig->vpd,psig->vc1,psig->vc2);*/
   k1 = h*f1(pw,psig->vs1,psig->vs2,psig->vpd,psig->vc1,psig->vc2);
   l1 = h*f2(pw,psig->vs1,psig->vs2,psig->vpd,psig->vc1,psig->vc2);
   k2 = h*f1(pw,psig->vs1,psig->vs2,psig->vpd,psig->vc1 + (b21*k1),psig->vc2 + (b21*l1));
   l2 = h*f2(pw,psig->vs1,psig->vs2,psig->vpd,psig->vc1 + (b21*k1),psig->vc2 + (b21*l1));
   k3 = h*f1(pw,psig->vs1,psig->vs2,psig->vpd,psig->vc1 + b31*k1 + b32*k2,
   psig->vc2 + b31*l1 + b32*l2);
   l3 = h*f2(pw,psig->vs1,psig->vs2,psig->vpd,psig->vc1 + b31*k1 + b32*k2,
   psig->vc2 + b31*l1 + b32*l2);
   k4 = h*f1(pw,psig->vs1,psig->vs2,psig->vpd,psig->vc1 + b41*k1 + b42*k2 + b43*k3,
   psig->vc2 + b41*l1 + b42*l2 + b43*l3);
   l4 = h*f2(pw,psig->vs1,psig->vs2,psig->vpd,psig->vc1 + b41*k1 + b42*k2 + b43*k3,
   psig->vc2 + b41*l1 + b42*l2 + b43*l3);
   k5 = h*f1(pw,psig->vs1,psig->vs2,psig->vpd,psig->vc1 + b51*k1 + b52*k2 + b53*k3 + b54*k4,
   psig->vc2 + b51*l1 + b52*l2 + b53*l3 + b54*l4);
   l5 = h*f2(pw,psig->vs1,psig->vs2,psig->vpd,psig->vc1 + b51*k1 + b52*k2 + b53*k3 + b54*k4,
   psig->vc2 + b51*l1 + b52*l2 + b53*l3 + b54*l4);
   k6 = h*f1(pw,psig->vs1,psig->vs2,psig->vpd,psig->vc1 + b61*k1 + b62*k2 + b63*k3 + b64*k4
   + b65*k5,psig->vc2 + b61*l1 + b62*l2 + b63*l3 + b64*l4 + b65*l5);
   l6 = h*f2(pw,psig->vs1,psig->vs2,psig->vpd,psig->vc1 + b61*k1 + b62*k2 + b63*k3 + b64*k4
   + b65*k5,psig->vc2 + b61*l1 + b62*l2 + b63*l3 + b64*l4 + b65*l5);
   /*printf("Value of freq = %6.4e\n",fo*(1 + deltaf(psig->vc2)));*/

   j1 = j2 = j3 = j4 = j5 = j6 = (h * 2.0 * pi * fo*(1 +   deltaf_vco(psig->vco,vc_model,pvco,degree_of_polynomial)));

/*Compute next value and error*/


   signew.vc1 = psig->vc1 + c1*k1 + c2*k2 + c3*k3 + c4*k4 + c5*k5 + c6*k6;
   signewb.vc1 = psig->vc1 + s1*k1 + s2*k2 + s3*k3 + s4*k4 + s5*k5 + s6*k6;
   signew.vc2 = psig->vc2 + c1*l1 + c2*l2 + c3*l3 + c4*l4 + c5*l5 + c6*l6;
   signewb.vc2 = psig->vc2 + s1*l1 + s2*l2 + s3*l3 + s4*l4 + s5*l5 + s6*l6;
   signew.phaseo = psig->phaseo + c1*j1 + c2*j2 + c3*j3 + c4*j4 + c5*j5 + c6*j6;
   signewb.phaseo = psig->phaseo + s1*j1 + s2*j2 + s3*j3 + s4*j4 + s5*j5 + s6*j6;
   
   /*Verify that errors are less than permissible errors*/
   
   reducestep = 0;
   newstep = h;
    
   if (fabs(signew.vc1 - signewb.vc1) > fabs(errmax + fabs(h*dvc1)))
      {
      reducestep = 1;
      error = fabs(signew.vc1 - signewb.vc1);
      maxerror = fabs(errmax + fabs(h*dvc1));
      newstep = h * pow(maxerror/error, 0.25);
      /*printf("case 1, error = %4.2e maxerror = %4.2e\n",error,maxerror);*/
      }
   if (fabs(signew.vc2 - signewb.vc2) > fabs(errmax + fabs(h*dvc2)))
      {
      reducestep = 1;
      error = fabs(signew.vc2 - signewb.vc2);
      maxerror = fabs(errmax + fabs(h*dvc2));
      /*printf("case 2, error = %4.2e maxerror = %4.2e\n",error,maxerror);*/
      if (h * pow(maxerror/error, 0.25) < newstep)
         {
         newstep = h * pow(maxerror/error, 0.25);
         }
      }
   if (fabs(signew.phaseo - signewb.phaseo) > (2.0*pi/10.0))
      {
      reducestep = 1;
      error = fabs(signew.phaseo - signewb.phaseo);
      maxerror = 2*pi/10;
      /*printf("iter = %d, h = %4.2e, j6 = %4.2e\n, freq =%6.4e",iter,h,j6,
      fo*(1 + deltaf(psig->vc2)));
      printf("case 3, error = %4.2e maxerror = %4.2e\n",error,maxerror);*/
      if (h * pow(maxerror/error, 0.25) < newstep)
         {
         newstep = h * pow(maxerror/error, 0.25);
         }
      }

   /*If must reduce timestep, verify that iter has not been exceeded*/
   
   if (reducestep == 1)
      {
      	
      /*Verify that number of times timestep has been reduced is less than
      iter - if it has terminate execution with return status 1 and print
      error message*/
	
      if (i >= iter ) 
         {
         printf("Iteration limit exceeded at time %2.4e, execution stopped!\n",
         time);
         printf("Reduced timestep %d times and maximum error was still too large.\n",
         iter);
         status=1;
         return(status);
         }	
      /*Reduce timestep to appropriate value depending on measured error,
      do not allow stepsize to be reduced more than a factor of 10*/
      
      /*printf("Step size reduced from deltat = %2.4e",h);*/
      
	  if (newstep < h/10.0)
	     h = h/10.0;
	  else
	     h = newstep;
	     
     /* printf(" to %2.4e seconds\n",h);*/
  
      i++;
      }
	
      else
      {

      /*If no error, reset i=1, advance time by increment h*/

      if ((time + h > tfinal) && (time != tfinal))
         {
         h = tfinal - time;
         time = tfinal;
         }
      else
         {
         time = time + h;
         }
      i = 1;
      
      /*Assign new values to state variables*/

      psig->vc1 = signew.vc1;
      psig->vc2 = signew.vc2;
      psig->phaseo = signew.phaseo;
      /*printf("No error in rkv2, psig->vc1 = %4.2e psig->vc2 = %4.2e",
      psig->vc1,psig->vc2);
      printf(" at time = %4.2e\n",time);*/

      /*Compute value of vc*/
      psig->vc = psig->vc2 + (pw->c11)*f2(pw,psig->vs1,psig->vs2,psig->vpd,psig->vc1,psig->vc2);      
/*if (count % 1000 == 0)
   printf("In rkstep5, sig->vc = %.3f.\n",psig->vc);*/

      /*Limit signal value at vc to vcmax and vcmin*/

      if (psig->vc > vmax)
         psig->vc = vmax;
      if (psig->vc < vmin)
         psig->vc = vmin;
      if (psig->vc1 > vmax)
         psig->vc1 = vmax;
      if (psig->vc1 < vmin)
         psig->vc1 = vmin;

      if (psig->vc2 > vmax)
         psig->vc2 = vmax;
      if (psig->vc2 < vmin)
         psig->vc2 = vmin;

      }
		

   }
/*printf("Exiting rkv1.c with status = %d at time = %4.2e\n",status,time);*/	
/*count++;*/
return(status);

}




