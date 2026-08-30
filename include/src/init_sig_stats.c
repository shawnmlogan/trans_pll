
#include "globals.h"

void init_sig_stats(Stats *psig_stats,Filedata *pdatain)
{

/*This routine assigns initial values to signal statistics prior to analysis*/

psig_stats->vc_running_average = pdatain->initvc;
psig_stats->vc_history_pp = 0.0;
psig_stats->sigma_vc = 0.0;
psig_stats->rphase_ui_running_average = pdatain->phase0/(2.0*pi);
psig_stats->rphase_ui_history_pp = 0.0;
psig_stats->sigma_rphase_ui = 0.0;
psig_stats->relative_phase_slope_average = 0.0;
psig_stats->sigma_relative_phase_slope_average = 0.0;

}
