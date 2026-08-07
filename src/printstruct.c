
#include "globals.h"

void printstruct(char *pfout, Filedata *pdatastruct)
{
/*Returns an integer indicating how many lines were written to file*/
int i = 0;
char value_string[NUMBER_OF_VALUE_STRINGS][LINELENGTH_OF_VALUE_STRING + 1];

double *ptemppointer1, *ptemppointer2;
FILE *fpw1;

/*Do not alter values of pointers in data structure - make copies*/
ptemppointer1 = pdatastruct->pjitter_freq;
ptemppointer2 = pdatastruct->pvc_polynomial;


/*Open filename to write header - wipes out previous file with that name*/

fpw1 = fopen(pfout,"w");

if (fpw1 != NULL)
   {
   fprintf(fpw1,"phase_detector: %s\n",pdatastruct->ppd);
   fprintf(fpw1,"fo(VCO_freq_in_Hz): %1.6e\n",pdatastruct->fo);
   fprintf(fpw1,"VCO_clock_dutycycle: %1.4f\n",pdatastruct->vco_clk_dutycycle);
   if ((pdatastruct->pd == PFD) || (pdatastruct->pd == PFD_CP))
      {
      fprintf(fpw1,"tauff(sec): %1.4e\n",pdatastruct->tauff);
      fprintf(fpw1,"tauh(sec): %1.4e\n",pdatastruct->tauh);
      fprintf(fpw1,"taucp_min(sec): %1.4e\n",pdatastruct->taucp_min);
      fprintf(fpw1,"pfd_deadzone(UI): %1.4f\n",pdatastruct->pfd_dff_deadzone_ui);
      }
   if ((pdatastruct->pd == DFF) || (pdatastruct->pd == PD_EXOR) || (pdatastruct->pd == HOGGE) || (pdatastruct->pd == PD_ALEXANDER))
      {
      fprintf(fpw1,"tauff(sec): %1.4e\n",pdatastruct->tauff);
      fprintf(fpw1,"tauh(sec): %1.4e\n",pdatastruct->tauh);
      }
   if (pdatastruct->pd == PD_EXOR) 
   	fprintf(fpw1,"dff_deadzone(UI): %1.4f\n",pdatastruct->pfd_dff_deadzone_ui);   
   if (pdatastruct->pd == PD_WINDOWED)
      {
      fprintf(fpw1,"tauff(sec): %1.4e\n",pdatastruct->tauff);
      fprintf(fpw1,"tw(sec): %1.4e\n",pdatastruct->tw);     
      fprintf(fpw1,"tacq_max(sec): %1.4e\n", pdatastruct->tacq_max);
      fprintf(fpw1,"dff_deadzone(UI): %1.4f\n",pdatastruct->pfd_dff_deadzone_ui);   
      }
   fprintf(fpw1,"fin(input_freq_in_Hz): %1.6e\n",pdatastruct->fin);  
   fprintf(fpw1,"feedback_divider: %.0f\n",pdatastruct->D);
   fprintf(fpw1,"feedback_divider_delay(sec): %1.4e\n",pdatastruct->D_delay);
   if(pdatastruct->D != 1.0)
   	{
   	fprintf(fpw1,"feedback_divider_clock_duty_cycle: %1.4f\n",
   	pdatastruct->clk_dutycycle);
   	}
   if (is_datarecovery(pdatastruct->pd))
   	{
   	fprintf(fpw1,"Filename_with_input_data_pattern \"%s\"\n",pdatastruct->pdatafilein);
   		if (pdatastruct->pd == PD_ALEXANDER)
   			{
   			fprintf(fpw1,"alexander_phase_detector_phase_threshold(UI): \"%s\"\n",
   			add_units(pdatastruct->alexander_pd_threshold_ui,2,"UI",value_string[0]));
   			}
   	}
   fprintf(fpw1,"jitter_amp(UIpp): %s\n",add_units(pdatastruct->jitter_amp,2,"UI",value_string[0]));
   fprintf(fpw1,"Jitter_freqs(Hz)");
   for (i = 0; i < pdatastruct->number_of_jitter_freq; i++)
      {
      if (i == 0)
      	{
      	if (pdatastruct->number_of_jitter_freq > 1)
         	fprintf(fpw1,": %1.4e,",*ptemppointer1);
         else
         	fprintf(fpw1,": %1.4e\n",*ptemppointer1);
         }
      else
      	{
      	if (i == (pdatastruct->number_of_jitter_freq - 1))
         	fprintf(fpw1,"%1.4e\n",*ptemppointer1);
         else
         	fprintf(fpw1,"%1.4e,",*ptemppointer1);
         }
      ptemppointer1++;
      }
   fprintf(fpw1,"phase0(degrees): %1.2f\n",(180.0/pi)*pdatastruct->phase0);
   fprintf(fpw1,"TSTART(sec): %1.6e\n",pdatastruct->TSTART);
   fprintf(fpw1,"TSTART_jt(sec): %1.6e\n",pdatastruct->TSTART_jt);
   fprintf(fpw1,"TSTOP(sec): %1.6e\n",pdatastruct->TSTOP);
   fprintf(fpw1,"deltat(sec): %1.6e\n",pdatastruct->deltat);
   fprintf(fpw1,"lock_detect_tau(sec): %1.6e\n",pdatastruct->lock_detect_tau);
   if (pdatastruct->lock_detect_tau > 0.0)
   	{
		fprintf(fpw1,"lock_detect_delta_vc_limit(%%): %1.6e\n",pdatastruct->lock_detect_delta_vc_limit_percent);
		fprintf(fpw1,"lock_detect_rphase_pp_limit(UIpp): %1.6e\n",pdatastruct->lock_detect_rphase_pp_limit_ui);
		fprintf(fpw1,"lock_detect_rphase_pp_loss_lock_limit(UIpp): %1.6e\n",pdatastruct->lock_detect_rphase_pp_loss_lock_limit_ui);
   	}
   fprintf(fpw1,"number_of_print_points: %ld\n",pdatastruct->npoints);
   fprintf(fpw1,"phase_freq_step_time(sec): %1.6e\n",pdatastruct->phase_freq_step_time_in_sec);
   fprintf(fpw1,"phase_step(sec): %1.4e\n",pdatastruct->phase_step_in_sec);
   fprintf(fpw1,"freq_step(ppm): %1.4e\n",pdatastruct->freq_step_ppm);
   
   fprintf(fpw1,"ssc_start_time(sec): %1.6e\n",pdatastruct->ssc_start_time_in_sec);
   fprintf(fpw1,"max_pos_ssc(ppm): %.2f\n",pdatastruct->max_pos_ssc_ppm);
   fprintf(fpw1,"max_neg_ssc(ppm): %.2f\n",pdatastruct->max_neg_ssc_ppm);
   fprintf(fpw1,"freq_ssc(Hz): %1.4e\n",pdatastruct->freq_ssc);

   fprintf(fpw1,"phase_detector_logic_hi(V): %1.3f\n",pdatastruct->logic_hi_pdsignal);
   fprintf(fpw1,"phase_detector_logic_low(V): %1.3f\n",pdatastruct->logic_lo_pdsignal);
   fprintf(fpw1,"signal_source_vs1(V): %1.3f\n",pdatastruct->vs1);
   fprintf(fpw1,"signal_source_vs2(V): %1.3f\n",pdatastruct->vs2);
   fprintf(fpw1,"vc_model: %s\n",pdatastruct->pvc_model_string);
   fprintf(fpw1,"vc_polynomial_coeff.(Hz/V)");
   for (i = 0; i<pdatastruct->vco_polynomial_degree;i++)
      {
      if (i == 0)
         fprintf(fpw1,": %1.6e,",*ptemppointer2);
      else
      	{
      	if (i == (pdatastruct->vco_polynomial_degree - 1))
         	fprintf(fpw1,"%1.6e\n",*ptemppointer2);
         else
         	fprintf(fpw1,"%1.6e,",*ptemppointer2);
         }
      ptemppointer2++;
      }
   fprintf(fpw1,"vco_bandwidth(Hz): %1.4e\n", 1.0/(2.0 * pi * pdatastruct->vco_tau));
   fprintf(fpw1,"power_supply_max(V): %1.4f\n",pdatastruct->vmax);
   fprintf(fpw1,"power_supply_min(V): %1.4f\n",pdatastruct->vmin);
   fprintf(fpw1,"random_noise_sigma(UI_on_fin): %1.4f\n",pdatastruct->noise_amp);
   if(pdatastruct->noise_amp > 0.0)
   	{
   	if (pdatastruct->noise_type == GAUSSIAN_NOISE)
   		fprintf(fpw1,"noise_type gaussian\n");
		if (pdatastruct->noise_type == UNIFORM_NOISE)
   		fprintf(fpw1,"noise_type uniform\n");
   	if ((pdatastruct->noise_type != UNIFORM_NOISE) && (pdatastruct->noise_type != GAUSSIAN_NOISE))
   		fprintf(fpw1,"noise_type unknown\n");
   	fprintf(fpw1,"noise_bandwidth(Hz): %1.6e\n",pdatastruct->noise_bandwidth_Hz);
   	}
   fprintf(fpw1,"RS1(ohms): %1.4e\n",pdatastruct->RS1);
   fprintf(fpw1,"RS2(ohms): %1.4e\n",pdatastruct->RS2);
   fprintf(fpw1,"R1(ohms): %1.4e\n",pdatastruct->R1);
   fprintf(fpw1,"RZERO(ohms): %1.4e\n",pdatastruct->R2);
   fprintf(fpw1,"R3(ohms): %1.4e\n",pdatastruct->R3);
   fprintf(fpw1,"C1(farads): %1.4e\n",pdatastruct->C1);
   fprintf(fpw1,"C2(farads): %1.4e\n",pdatastruct->C2);
   fprintf(fpw1,"initvc(V): %1.4f\n",pdatastruct->initvc);
   fprintf(fpw1,"Output_base_filename \"%s\"\n",pdatastruct->poutput_base_filename);
   fclose(fpw1);
   }
   
   return;
}
