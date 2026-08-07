# include "globals.h"

int compute_control_voltage_characteristic(struct filedata *pdatain, struct sig_stats *psig_stats, int num_points, char *pfilename, char *ptimestamp, int plot_flag, int plot_normalized_freq)
{

int i = 1, center_voltage_index = 0;
double vrange = 0.0, vstep = 0.0, *pdelta_f, delta_f[num_points], *pvc, vc[num_points];
double *pkvco_delta_f, kvco_delta_f[num_points], *pkvco_Hz_per_V, kvco_Hz_per_V[num_points];
double kvco_delta_f_at_center_voltage = 0.0;
char value_string[NUMBER_OF_VALUE_STRINGS][LINELENGTH_OF_VALUE_STRING + 1];
char *ptitle_string, title_string[TITLE_LINELENGTH + 1];
char *psystem_command, system_command[COMMAND_LINELENGTH + 1];
char *pfilename_octave_gnuplot_command_line, filename_octave_gnuplot_command_line[OUTPUT_FILE_LINELENGTH + 1];


FILE *fpw1, *fpw2;

if ((pdelta_f = (double *) malloc(num_points*sizeof(double))) == NULL)
	printf("malloc did not succeed for pdelta_f in compute_control_voltage_characteristic.c.\n");
if ((pvc = (double *) malloc(num_points*sizeof(double))) == NULL)
	printf("malloc did not succeed for pvc in compute_control_voltage_characteristic.c.\n");
if ((pkvco_delta_f = (double *) malloc(num_points*sizeof(double))) == NULL)
	printf("malloc did not succeed for pkvco_delta_f in compute_control_voltage_characteristic.c.\n");
if ((pkvco_Hz_per_V = (double *) malloc(num_points*sizeof(double))) == NULL)
	printf("malloc did not succeed for pkvco_Hz_per_V in compute_control_voltage_characteristic.c.\n");

ptitle_string = &title_string[0];
psystem_command = &system_command[0];
pfilename_octave_gnuplot_command_line = &filename_octave_gnuplot_command_line[0];

fpw1 = fopen(pfilename, "w");
fprintf(fpw1,"Control voltage (V),Output Frequency (Hz),kvco (Hz/V),Delta f (%%),kvco (%%/V)\n");

vrange = (pdatain->vmax - pdatain->vmin);
vstep = vrange/(num_points - 1);

for(i = 0; i < num_points;i++)
	{
	if (i == 0)
		{
		vc[i] = pdatain->vmin;
		delta_f[i] = deltaf_vco(vc[i],pdatain->vc_model,pdatain->pvc_polynomial,pdatain->vco_polynomial_degree);
		kvco_delta_f[i] = 0.0;
		kvco_Hz_per_V[i] = pdatain->fo * kvco_delta_f[i];
		}
	else
		{
		vc[i] = vc[i-1] + vstep;
		delta_f[i] = deltaf_vco(vc[i],pdatain->vc_model,pdatain->pvc_polynomial,pdatain->vco_polynomial_degree);
		kvco_delta_f[i] = (delta_f[i] - delta_f[i - 1])/vstep;
		kvco_Hz_per_V[i] = pdatain->fo * kvco_delta_f[i];
		}
	if (((vc[i] - psig_stats->vcenter_voltage) <= 0.0) && ((vc[i] + vstep - psig_stats->vcenter_voltage) > 0.0))
		{
			if (i != (num_points - 1))
				center_voltage_index = i;
			else
				center_voltage_index = num_points;
		}
	fprintf(fpw1,"%1.12e,%1.12e,%1.12e,%1.12e,%1.12e\n",vc[i],(1.0 + delta_f[i])*pdatain->fo,kvco_Hz_per_V[i],
	100*delta_f[i],100*kvco_delta_f[i]);
	}

fclose(fpw1);

if ((plot_flag == 1) && (pdatain->octave == 1 ) && (pdatain->plot_preference == OCTAVE))
	{
	kvco_delta_f_at_center_voltage = (psig_stats->vcenter_voltage - vc[center_voltage_index])*(kvco_delta_f[center_voltage_index + 1] - kvco_delta_f[center_voltage_index])/vstep + kvco_delta_f[center_voltage_index];

	snprintf(ptitle_string,TITLE_LINELENGTH,"%s VCO Output Frequency Control Voltage Characteristics\nCenter voltage = %s, Kvco = %s at center voltage",add_units(pdatain->fo,4,"Hz",value_string[0]),add_units(psig_stats->vcenter_voltage,1,"V",value_string[1]),add_units(pdatain->fo*kvco_delta_f_at_center_voltage,2,"Hz/V",value_string[2]));

	snprintf(psystem_command,COMMAND_LINELENGTH,"%s/plotting_routines/octave/plot_df_vs_control_voltage.m ./%s \"%s\" \"%s\" %1.6e %1.6e %1.6e %d\n",PLOTTING_ROUTINES_DIR,pfilename,ptitle_string,ptimestamp,pdatain->fo,psig_stats->vcenter_voltage,kvco_delta_f_at_center_voltage*pdatain->fo,plot_normalized_freq);

	snprintf(pfilename_octave_gnuplot_command_line,OUTPUT_FILE_LINELENGTH,
	"%s_octave_vco_df_vs_control_voltage_plot_command_line_%s.txt",pdatain->pinput_filename,ptimestamp);

	if (PRINT_GNUPLOT_OR_OCTAVE_COMMAND == TRUE)
		{
		fpw2 = fopen(pfilename_octave_gnuplot_command_line,"w");
		fprintf(fpw2,"%s\n",psystem_command);
		fclose(fpw2);
		}
	system(psystem_command);
	}
else
	{
	if ((plot_flag == 1) && (pdatain->gnuplot == 1) && (pdatain->plot_preference == GNUPLOT))
		{
		kvco_delta_f_at_center_voltage = (psig_stats->vcenter_voltage - vc[center_voltage_index])*(kvco_delta_f[center_voltage_index + 1] - kvco_delta_f[center_voltage_index])/vstep + kvco_delta_f[center_voltage_index];
	
		snprintf(ptitle_string,TITLE_LINELENGTH,
		"{/:Bold %s VCO Output Frequency Control Voltage Characteristics}\n{/:Bold Center voltage = %s, Kvco = %s at center voltage}",
		add_units(pdatain->fo,4,"Hz",value_string[0]),add_units(psig_stats->vcenter_voltage,1,"V",value_string[1]),add_units(pdatain->fo*kvco_delta_f_at_center_voltage,2,"Hz/V",value_string[2]));
	
		snprintf(psystem_command,COMMAND_LINELENGTH,"gnuplot -e \"input_filename = \'./%s\'; trans_pll_plot_title = \'%s\'; nom_freq_Hz = %1.6e; center_voltage = %1.6e; nom_kvco_Hz_per_V = %1.6e; timestamp = \'%s\'; plot_normalized_freq = %d; PLOTTING_ROUTINES_DIR = \'%s\';\" %s/plotting_routines/gnuplot/plot_df_vs_control_voltage.gnu\n",pfilename,ptitle_string,pdatain->fo,psig_stats->vcenter_voltage,kvco_delta_f_at_center_voltage*pdatain->fo,ptimestamp,plot_normalized_freq,PLOTTING_ROUTINES_DIR,PLOTTING_ROUTINES_DIR);

		snprintf(pfilename_octave_gnuplot_command_line,OUTPUT_FILE_LINELENGTH,
		"%s_gnuplot_vco_df_vs_control_voltage_octave_plot_command_line_%s.txt",pdatain->pinput_filename,ptimestamp);

		if (PRINT_GNUPLOT_OR_OCTAVE_COMMAND == TRUE)
			{
			fpw2 = fopen(pfilename_octave_gnuplot_command_line,"w");
			fprintf(fpw2,"%s\n",psystem_command);
			fclose(fpw2);
			}
		system(psystem_command);
		}
	}

free(pdelta_f);
free(pvc);
free(pkvco_delta_f);
free(pkvco_Hz_per_V);

return EXIT_SUCCESS;
}
