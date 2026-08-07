
#include "globals.h"

void initdata(Filedata *pdatastruct)
{
char *pdummy_string = "no_named_file", *ppoly = "poly", *pcsv = "csv", *pmissing_entry = "no_entry";

/*This routine assigns initial values to some of the datastructure to allow
better error checking of the inputs prior to analysis*/

strcpy(pdatastruct->pdatafilein,pdummy_string);
strcpy(pdatastruct->poutput_base_filename,pdummy_string);

pdatastruct->fin = 0.0;
pdatastruct->clk_dutycycle = INIT_VAL;
pdatastruct->quadphase = 10.0 *pi;
pdatastruct->vco_clk_dutycycle = INIT_VAL;
pdatastruct->vco_clkq_dutycycle = INIT_VAL;
pdatastruct->D = 0.0;
pdatastruct->D_delay = 0.0;
pdatastruct->vco_tau = 0.0;
strncpy(pdatastruct->pvc_model_string,ppoly,LINELENGTH);
pdatastruct->vc_model = 0;
pdatastruct->plot_outputs = 0;
pdatastruct->gnuplot = 0;
pdatastruct->octave = 0;
pdatastruct->plot_preference = GNUPLOT;
strncpy(pdatastruct->pfile_format,pcsv,FILENAME_LINELENGTH);

/* Set remaining integer and doubles to INIT_VAL for error checking */

pdatastruct->pd = INIT_VAL;
pdatastruct->fo = INIT_VAL;
pdatastruct->jitter_amp = INIT_VAL;
pdatastruct->number_of_jitter_freq = INIT_VAL;
pdatastruct->phase0 = INIT_VAL;
pdatastruct->alexander_pd_threshold_ui = INIT_VAL;
strncpy(pdatastruct->palexander_pd_threshold_ui,pmissing_entry,LINELENGTH);
pdatastruct->TSTART = INIT_VAL;
pdatastruct->TSTART_jt = INIT_VAL;
pdatastruct->TSTOP = INIT_VAL;
strncpy(pdatastruct->pdeltat,pmissing_entry,LINELENGTH);
pdatastruct->deltat = INIT_VAL;
pdatastruct->lock_detect_tau = INIT_VAL;
strncpy(pdatastruct->lock_detect_delta_vc_limit_percent_string,pmissing_entry,LINELENGTH);
strncpy(pdatastruct->lock_detect_rphase_pp_limit_ui_string,pmissing_entry,LINELENGTH);
strncpy(pdatastruct->lock_detect_rphase_pp_loss_lock_limit_ui_string,pmissing_entry,LINELENGTH);

pdatastruct->npoints = INIT_VAL;
pdatastruct->logic_hi_pdsignal = INIT_VAL;
pdatastruct->logic_lo_pdsignal = INIT_VAL;
pdatastruct->vs1 = INIT_VAL; /*Added signal source to inject current through RS1*/
pdatastruct->vs2 = INIT_VAL; /*Added signal source to inject current through RS2*/

pdatastruct->vmax = INIT_VAL;
pdatastruct->vmin = INIT_VAL;
pdatastruct->noise_amp = INIT_VAL;
pdatastruct->noise_bandwidth_Hz = INIT_VAL;
pdatastruct->noise_type = INIT_VAL;
pdatastruct->R1 = INIT_VAL;
pdatastruct->R2 = INIT_VAL;
pdatastruct->R3 = INIT_VAL;
pdatastruct->C1 = INIT_VAL;
pdatastruct->C2 = INIT_VAL;
pdatastruct->RS1 = INIT_VAL; /*Resistance of voltage source injecting signal at v1 11/7/99*/
pdatastruct->RS2 = INIT_VAL; /*Resistance of voltage source injecting signal at vc1 11/7/99*/
pdatastruct->initvc = INIT_VAL;

pdatastruct->tauh = INIT_VAL; /*No longer used*/
pdatastruct->tauff = INIT_VAL; /*Delay time for flip-flop in pd4046.c 10/16/99*/
pdatastruct->taucp_min = INIT_VAL; /*Min charge pump pulse width in pd4046.c 10/18/99*/
pdatastruct->pfd_dff_deadzone_ui = INIT_VAL; /*PFD deadzone*/
pdatastruct->tw = INIT_VAL; /*Width of time window for windowed phase detector in which DFF based phase detector is used*/
pdatastruct->tacq_max = INIT_VAL; /*Expected time at which acqusiition is over - start point for tswc clock switch circuit*/
pdatastruct->phase_freq_step_time_in_sec = INIT_VAL; /* Start time for phase or freq step input 4/15/2015*/
pdatastruct->phase_step_in_sec = INIT_VAL; /* Phase step applied at phase_freq_step_time_in_sec input 4/15/2015*/
pdatastruct->freq_step_ppm = INIT_VAL; /* Frequency step applied at phase_freq_step_time_in_sec input 4/15/2015*/
pdatastruct->ssc_start_time_in_sec = INIT_VAL; /* Start time for SSC frequency modulation 5/1/2015*/
pdatastruct->max_pos_ssc_ppm = INIT_VAL; /* Peak positive SSC modulation in ppm 5/1/2015*/
pdatastruct->max_neg_ssc_ppm = INIT_VAL; /* Peak negative SSC modulation in ppm 5/1/2015*/
pdatastruct->freq_ssc = INIT_VAL; /* Frequency of SSC modulation 5/1/2015*/

return;
}
