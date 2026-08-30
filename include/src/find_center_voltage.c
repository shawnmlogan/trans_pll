# include "globals.h"


int find_center_voltage(struct filedata *pdatain,struct sig_stats *psig_stats,double max_error_percent)
{

int i = 1, max_iterations = 1000;
double vrange = 0.0, vstep = 0.0, vcenter = 0.0, delta_f = 0.0;
char value_string[NUMBER_OF_VALUE_STRINGS][LINELENGTH_OF_VALUE_STRING + 1];

vrange = (pdatain->vmax - pdatain->vmin);
vcenter = (pdatain->vmax + pdatain->vmin)/2.0;
delta_f = deltaf_vco(vcenter,pdatain->vc_model,pdatain->pvc_polynomial,pdatain->vco_polynomial_degree);
vstep = vrange;

while ((fabs(delta_f) > max_error_percent/100.0) && (i < max_iterations))
	{
	vstep = vstep/2.0;
	if (delta_f > 0.0)
		vcenter = vcenter - vstep;
	else
		vcenter = vcenter + vstep;
	delta_f = deltaf_vco(vcenter,pdatain->vc_model,pdatain->pvc_polynomial,pdatain->vco_polynomial_degree);
	i++;
	}
	/*printf("Found center voltage of %s after %d iterations.\n",add_units(vcenter,1,"V",value_string[0]),i);*/

if (i < max_iterations)
	{
	psig_stats->vcenter_voltage = vcenter;
	return EXIT_SUCCESS;
	}
else
	{
	psig_stats->vcenter_voltage = vcenter;
	return EXIT_FAILURE;
	}
}
