
#include "globals.h"

void pd_nonlinear(Filedata *pdatain, double rphase, double orig_logic_hi, double orig_logic_lo, double time)
{

/*Examine relative phase in radians and determine value of logic low and
logic high phase detector values*/


   double rphase_mod_ui = 0.0;
   double ref_period = 0.0;

   rphase_mod_ui = fmod(rphase,2.0*pi)/(2.0*pi);
   ref_period = 1.0/(pdatain->fin);

   if(rphase_mod_ui < -0.0095)
        {
        pdatain->logic_hi_pdsignal = 0.67532467*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.67532467*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0095) && (rphase_mod_ui < -0.0090) )
        {
        pdatain->logic_hi_pdsignal = 0.68831168*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.68831168*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0090) && (rphase_mod_ui < -0.0085) )
        {
        pdatain->logic_hi_pdsignal = 0.623376623*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.623376623*orig_logic_lo;
       /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0085) && (rphase_mod_ui < -0.0080) )
        {
        pdatain->logic_hi_pdsignal = 0.584415584*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.584415584*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0080) && (rphase_mod_ui < -0.0075) )
        {
        pdatain->logic_hi_pdsignal = 0.623376623*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.623376623*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0075) && (rphase_mod_ui < -0.0070) )
        {
        pdatain->logic_hi_pdsignal = 0.636363636*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.636363636*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0070) && (rphase_mod_ui < -0.0065) )
        {
        pdatain->logic_hi_pdsignal = 0.597402597*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.597402597*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0065) && (rphase_mod_ui < -0.0060) )
        {
        pdatain->logic_hi_pdsignal = 0.61038961*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.61038961*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0060) && (rphase_mod_ui < -0.0055) )
        {
        pdatain->logic_hi_pdsignal = 0.402597403*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.402597403*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0055) && (rphase_mod_ui < -0.0050) )
        {
        pdatain->logic_hi_pdsignal = 0.350649351*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.350649351*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0050) && (rphase_mod_ui < -0.0045) )
        {
        pdatain->logic_hi_pdsignal = 0.454545455*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.454545455*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0045) && (rphase_mod_ui < -0.0040) )
        {
        pdatain->logic_hi_pdsignal = 0.402597403*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.402597403*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0040) && (rphase_mod_ui < -0.0035) )
        {
        pdatain->logic_hi_pdsignal = 0.246753247*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.246753247*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0035) && (rphase_mod_ui < -0.0030) )
        {
        pdatain->logic_hi_pdsignal = 0.272727273*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.272727273*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0030) && (rphase_mod_ui < -0.0025) )
        {
        pdatain->logic_hi_pdsignal = 0.350649351*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.350649351*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0025) && (rphase_mod_ui < -0.0020) )
        {
        pdatain->logic_hi_pdsignal = 0.337662338*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.337662338*orig_logic_lo; 
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0020) && (rphase_mod_ui < -0.0015) )
        {
        pdatain->logic_hi_pdsignal = 0.636363636*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.636363636*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0015) && (rphase_mod_ui < -0.0010) )
        {
        pdatain->logic_hi_pdsignal = 0.857142857*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.857142857*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0010) && (rphase_mod_ui < -0.0005) )
        {
        pdatain->logic_hi_pdsignal = 0.87012987*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.87012987*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= -0.0005) && (rphase_mod_ui < 0.0000) )
        {
        pdatain->logic_hi_pdsignal = 0.948051948*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.948051948*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= 0.0000) && (rphase_mod_ui < 0.0005) )
        {
        pdatain->logic_hi_pdsignal = 0.896103896*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.896103896*orig_logic_lo;
        }
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
   if( (rphase_mod_ui >= 0.0005) && (rphase_mod_ui < 0.0010) )
        {
        pdatain->logic_hi_pdsignal = 0.987012987*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.987012987*orig_logic_lo;
        }
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
   if( (rphase_mod_ui >= 0.0010) && (rphase_mod_ui < 0.0015) )
        {
        pdatain->logic_hi_pdsignal = 0.74025974*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.74025974*orig_logic_lo;
        }
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
   if( (rphase_mod_ui >= 0.0015) && (rphase_mod_ui < 0.0020) )
        {
        pdatain->logic_hi_pdsignal = 0.857142857*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.857142857*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= 0.0020) && (rphase_mod_ui < 0.0025) )
        {
        pdatain->logic_hi_pdsignal = 0.922077922*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.922077922*orig_logic_lo;
        }
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
   if( (rphase_mod_ui >= 0.0025) && (rphase_mod_ui < 0.0030) )
        {
        pdatain->logic_hi_pdsignal = 1.0*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 1.0*orig_logic_lo;
        }
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
   if( (rphase_mod_ui >= 0.0030) && (rphase_mod_ui < 0.0035) )
        {
        pdatain->logic_hi_pdsignal = 0.584415584*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.584415584*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= 0.0035) && (rphase_mod_ui < 0.0040) )
        {
        pdatain->logic_hi_pdsignal = 0.688311688*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.688311688*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= 0.0040) && (rphase_mod_ui < 0.0045) )
        {
        pdatain->logic_hi_pdsignal = 0.688311688*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.688311688*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
   if( (rphase_mod_ui >= 0.0045) && (rphase_mod_ui < 0.0050) )
        {
        pdatain->logic_hi_pdsignal = 0.792207792*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.792207792*orig_logic_lo;
        }
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
   if( (rphase_mod_ui >= 0.0050) && (rphase_mod_ui < 0.0055) )
        {
        pdatain->logic_hi_pdsignal = 0.74025974*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.74025974*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= 0.0055) && (rphase_mod_ui < 0.0060) )
        {
        pdatain->logic_hi_pdsignal = 0.714285714*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.714285714*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= 0.0060) && (rphase_mod_ui < 0.0065) )
        {
        pdatain->logic_hi_pdsignal = 0.701298701*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.701298701*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= 0.0065) && (rphase_mod_ui < 0.0070) )
        {
        pdatain->logic_hi_pdsignal = 0.779220779*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.779220779*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= 0.0070) && (rphase_mod_ui < 0.0075) )
        {
        pdatain->logic_hi_pdsignal = 0.714285714*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.714285714*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= 0.0075) && (rphase_mod_ui < 0.0080) )
        {
        pdatain->logic_hi_pdsignal = 0.714285714*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.714285714*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= 0.0080) && (rphase_mod_ui < 0.0085) )
        {
        pdatain->logic_hi_pdsignal = 0.519480519*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.519480519*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= 0.0085) && (rphase_mod_ui < 0.0090) )
        {
        pdatain->logic_hi_pdsignal = 0.597402597*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.597402597*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( (rphase_mod_ui >= 0.0090) && (rphase_mod_ui < 0.0095) )
        {
        pdatain->logic_hi_pdsignal = 0.597402597*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.597402597*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }
   if( rphase_mod_ui >= 0.0095 )
        {
        pdatain->logic_hi_pdsignal = 0.597402597*orig_logic_hi;
        pdatain->logic_lo_pdsignal = 0.597402597*orig_logic_lo;
        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/
        }

        /*printf("Changed logic_hi_pdsignal to %.2f V at time = %1.6e.\n",pdatain->logic_hi_pdsignal,time);*/

return;

}

/*

Phase_in_ui	Relative_magnitude_to_peak
-0.001000	0
-0.000950	0.675324675
-0.000900	0.688311688
-0.000850	0.623376623
-0.000800	0.584415584
-0.000750	0.623376623
-0.000700	0.636363636
-0.000650	0.597402597
-0.000600	0.493506494
-0.000550	0.597402597
-0.000500	0.61038961
-0.000450	0.402597403
-0.000400	0.350649351
-0.000350	0.454545455
-0.000300	0.402597403
-0.000250	0.246753247
-0.000200	0.272727273
-0.000150	0.350649351
-0.000100	0.337662338
-0.000050	0.636363636
0.000000	0.857142857
0.000050	0.87012987
0.000100	0.948051948
0.000150	0.896103896
0.000200	0.987012987
0.000250	0.74025974
0.000300	0.857142857
0.000350	0.922077922
0.000400	1
0.000450	0.584415584
0.000500	0.688311688
0.000550	0.688311688
0.000600	0.792207792
0.000650	0.74025974
0.000700	0.714285714
0.000750	0.701298701
0.000800	0.779220779
0.000850	0.714285714
0.000900	0.714285714
0.000950	0.519480519
0.001000	0.597402597

*/
