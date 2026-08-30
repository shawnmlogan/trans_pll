
#include "globals.h"

int vpd_amp_alexander(struct signal *psig,Filedata *pdatain,double phase_threshold_ui,double vpd_value_outside_phase_threshold)
{

int error_flag = 0;
double rphase_ui;

rphase_ui = (psig->phaseo_delayed - psig->phasein)/(2.0 * pi);

if ((fabs((fabs(rphase_ui) - 0.50)) < phase_threshold_ui) && (phase_threshold_ui > 0.0))
	psig->vpd = (fabs((fabs(rphase_ui) - 0.50))/phase_threshold_ui) * vpd_value_outside_phase_threshold;
else
	psig->vpd = vpd_value_outside_phase_threshold;

if (error_flag != 0)
	return EXIT_FAILURE;
else
	return EXIT_SUCCESS;
}