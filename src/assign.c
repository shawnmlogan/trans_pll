
#include "globals.h"

int assign(char *pidentifier,char *pentry, Filedata *pdatastruct)
{
extern double pi;
int foundentry = 0;

if (strcmp(pidentifier,"phase_detector") == 0)
   {
   strncpy(pdatastruct->ppd,pentry,LINELENGTH);
   foundentry = 1;
   }
if (strcmp(pidentifier,"alexander_phase_detector_phase_threshold(ui)") == 0)
   {
   strncpy(pdatastruct->palexander_pd_threshold_ui,pentry,LINELENGTH);
   foundentry = 1;
   }
if (strcmp(pidentifier,"fo(vco_freq_in_hz)") == 0)
   {
   pdatastruct->fo = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"vco_control_voltage_model") == 0)
   {
   strncpy(pdatastruct->pvc_model_string,pentry,LINELENGTH);
   foundentry = 1;
   }
if (strcmp(pidentifier,"fin(input_freq_in_hz)") == 0)
   {
   pdatastruct->fin = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"vco_clock_duty_cycle(fo)") == 0)
   {
   pdatastruct->vco_clk_dutycycle = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"vco_clockq_duty_cycle(fo)") == 0)
   {
   pdatastruct->vco_clkq_dutycycle = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"vco_clockq_phase(deg)") == 0)
   {
   pdatastruct->quadphase = pi * atof(pentry)/180.0;
   foundentry = 1;
   }
if (strcmp(pidentifier,"feedback_divider") == 0)
   {
   pdatastruct->D = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"feedback_divider_delay(sec)") == 0)
   {
   pdatastruct->D_delay = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"feedback_divider_clock_duty_cycle") == 0)
   {
   pdatastruct->clk_dutycycle = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"filename_with_input_data_pattern") == 0)
   {
   strncpy(pdatastruct->pdatafilein,pentry,FILENAME_LINELENGTH);
   foundentry = 1;
   }
if (strcmp(pidentifier,"jitter_amp(uipp)") == 0)
   {
   pdatastruct->jitter_amp = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"jitter_freqs(hz)") == 0)
   {
   foundentry = 1;
	if (parsestring_to_doubles_array(pentry,pdatastruct->pjitter_freq,&pdatastruct->number_of_jitter_freq,NUMBER_OF_JITTER_FREQ) != EXIT_SUCCESS)
      {
      printf("Entered too many jitter frequencies to analyze.\n");
      printf("Enter %d or less...exiting\n",NUMBER_OF_JITTER_FREQ);
      foundentry = 0;
      return EXIT_FAILURE;
      }
   }
if (strcmp(pidentifier,"phase0(degrees)") == 0)
   {
   pdatastruct->phase0 = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"tstart(sec)") == 0)
   {
   pdatastruct->TSTART = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"tstart_jt(sec)") == 0)
   {
   pdatastruct->TSTART_jt = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"tstop(sec)") == 0)
   {
   pdatastruct->TSTOP = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"deltat(sec)") == 0)
   {
   strncpy(pdatastruct->pdeltat,pentry,LINELENGTH);
   foundentry = 1;
   }
if (strcmp(pidentifier,"lock_detect_tau(sec)") == 0)
   {
   pdatastruct->lock_detect_tau = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"lock_detect_delta_vc_limit(%)") == 0)
   {
   strncpy(pdatastruct->plock_detect_delta_vc_limit_percent_string,pentry,LINELENGTH);
   foundentry = 1;
   }
if (strcmp(pidentifier,"lock_detect_rphase_pp_limit(uipp)") == 0)
   {
   strncpy(pdatastruct->plock_detect_rphase_pp_limit_ui_string,pentry,LINELENGTH);
   foundentry = 1;
   }
if (strcmp(pidentifier,"lock_detect_rphase_pp_loss_lock_limit(uipp)") == 0)
   {
   strncpy(pdatastruct->plock_detect_rphase_pp_loss_lock_limit_ui_string,pentry,LINELENGTH);
   foundentry = 1;
   }
if (strcmp(pidentifier,"number_of_print_points") == 0)
   {
   pdatastruct->npoints = atol(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"phase_detector_logic_hi(v)") == 0)
   {
   pdatastruct->logic_hi_pdsignal = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"phase_detector_logic_low(v)") == 0)
   {
   pdatastruct->logic_lo_pdsignal = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"signal_source_vs1(v)") == 0)
   {
   pdatastruct->vs1 = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"signal_source_vs2(v)") == 0)
   {
   pdatastruct->vs2 = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"vc_polynomial_coeff(hz/v)") == 0)
   {
   foundentry = 1;
   if (parsecsv_to_array(pentry,pdatastruct->pvc_polynomial,&(pdatastruct->vco_polynomial_degree),
   MAX_VCO_POLYNOMIAL) == EXIT_FAILURE)
      {
      printf("Entered too many constants to describe vco.\n");
      printf("Enter %d or less...exiting\n",MAX_VCO_POLYNOMIAL);
      foundentry = 0;
      return EXIT_FAILURE;
      }
   }
if (strcmp(pidentifier,"vco_bandwidth(hz)") == 0)
   {
   pdatastruct->vco_tau = 1.0/(2.0 * pi * atof(pentry));
   foundentry = 1;
   }
if (strcmp(pidentifier,"power_supply_max(v)") == 0)
   {
   pdatastruct->vmax = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"power_supply_min(v)") == 0)
   {
   pdatastruct->vmin = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"random_noise_sigma(ui_on_fin)") == 0)
   {
   pdatastruct->noise_amp = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"noise_type") == 0)
   {
   strncpy(pdatastruct->pnoise_type,pentry,LINELENGTH);
   foundentry = 1;
   }
if (strcmp(pidentifier,"noise_bandwidth(hz)") == 0)
   {
   pdatastruct->noise_bandwidth_Hz = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"rs1(ohms)") == 0)
   {
   pdatastruct->RS1 = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"rs2(ohms)") == 0)
   {
   pdatastruct->RS2 = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"r1(ohms)") == 0)
   {
   pdatastruct->R1 = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"rzero(ohms)") == 0)
   {
   pdatastruct->R2 = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"r3(ohms)") == 0)
   {
   pdatastruct->R3 = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"c1(farads)") == 0)
   {
   pdatastruct->C1 = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"c2(farads)") == 0)
   {
   pdatastruct->C2 = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"initvc(v)") == 0)
   {
   pdatastruct->initvc = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"tauh(sec)") == 0)
   {
   pdatastruct->tauh = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"tauff(sec)") == 0)
   {
   pdatastruct->tauff = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"tw(sec)") == 0)
   {
   pdatastruct->tw = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"taucp_min(sec)") == 0)
   {
   pdatastruct->taucp_min = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"pfd_deadzone(ui)") == 0)
   {
   pdatastruct->pfd_dff_deadzone_ui = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"tacq_max(sec)") == 0)
   {
   pdatastruct->tacq_max = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"phase_step(sec)") == 0)
   {
   pdatastruct->phase_step_in_sec = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"freq_step(ppm)") == 0)
   {
   pdatastruct->freq_step_ppm = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"phase_freq_step_time(sec)") == 0)
   {
   pdatastruct->phase_freq_step_time_in_sec = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"ssc_start_time(sec)") == 0)
   {
   pdatastruct->ssc_start_time_in_sec = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"max_pos_ssc(ppm)") == 0)
   {
   pdatastruct->max_pos_ssc_ppm = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"max_neg_ssc(ppm)") == 0)
   {
   pdatastruct->max_neg_ssc_ppm = atof(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"freq_ssc(hz)") == 0)
   {
   pdatastruct->freq_ssc = atof(pentry);
   foundentry = 1;
   }
if ((strcmp(pidentifier,"output_base_filename") == 0) || (strcmp(pidentifier,"output_filename") == 0))
   {
   strncpy(pdatastruct->poutput_base_filename,pentry,FILENAME_LINELENGTH);
   foundentry = 1;
   }
if (strcmp(pidentifier,"plot_outputs") == 0)
   {
   pdatastruct->plot_outputs = atoi(pentry);
   foundentry = 1;
   }
if (strcmp(pidentifier,"plot_preference") == 0)
   {
   strncpy(pdatastruct->p_plot_preference,pentry,LINELENGTH);
   foundentry = 1;
   }

if (foundentry == 0)
   return EXIT_FAILURE;
else
   return EXIT_SUCCESS;
}
