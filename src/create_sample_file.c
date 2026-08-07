#include "globals.h"


void create_sample_file(char *pfname)
{

FILE *fpw1;

if ((fpw1 = fopen(pfname,"w")) == NULL)
	{
	printf("Unable to open sample file! Check directory permissions and file space.\n");
	}
else
	{
	fprintf(fpw1,"* Comment lines begin with a \"*\"\n");
	fprintf(fpw1,"* Phase detector options, choose dff, pfd, pd_windowed, exor, none_vpd=1,\n");
	fprintf(fpw1,"* none_vpd=0, hogge, alexander\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"phase_detector: <Enter your choice for phase detector>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Phase detector related inputs (not all are used for each phase detector)\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"tauff(sec): <Enter flip-flop delay in sec>\n");
	fprintf(fpw1,"tauh(sec): <Enter logic gate delay in sec>\n");
	fprintf(fpw1,"taucp_min(sec): <Enter minimum charge pump width in sec>\n");
	fprintf(fpw1,"pfd_deadzone(ui): <Enter pfd deadzone width in UI>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* pd_windowed phase detector related inputs\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"tw(sec): <Enter width of window>\n");
	fprintf(fpw1,"tacq_max(sec): <Enter maximum allowed acquisition time in sec>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* CDR phase detector options\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Phase threshold beyond which amplitude of Alexander phase detector saturates\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"alexander_phase_detector_phase_threshold(UI): <Enter positive threshold in UI or the word \"default\">\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* VCO related inputs\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"fo(VCO_freq_in_Hz): <Enter VCO frequency in Hz>\n");
	fprintf(fpw1,"vco_clock_duty_cycle(fo): <Enter duty cycle of VCO as a fraction of 1.0 (0.50 => 50%%)>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Choose model for control voltage characteristic\n");
	fprintf(fpw1,"* \"polynomial\" or \"poly\" allows up to ninth order polynomial deltaf = c0 + c1*vc +c2*vc +...c8*vc\n");
	fprintf(fpw1,"* \"tanh\" function requires four coefficients deltaf = c0 + c1*tanh(c2*(vc-c3))\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"vco_control_voltage_model: <Enter choice for VCO control voltage model>\n");
	fprintf(fpw1,"vc_polynomial_coeff(hz/V): <Enter VCO control voltage polynomial coefficients as comma-separated list> \n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Example kvco = c1*fo = 2.0*500e6 = 1.0 GHz/V\n");
	fprintf(fpw1,"* Vco = -(c0/c1) = -(-1/2) = 0.5 V\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"vco_bandwidth(Hz): <Enter modulation bandwidth of VCO in Hz>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Clock or data input parameters\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"fin(input_freq_in_Hz): <Enter frequency of input clock or data in Hz>\n");
	fprintf(fpw1,"feedback_divider: <Enter feedback divider>\n");
	fprintf(fpw1,"feedback_divider_delay(sec): <Enter latency of feedback divider (delay) in sec>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Feedback clock duty cycle (not used if divider is 1.0)\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"feedback_divider_clock_duty_cycle: <Enter duty cycle of feedback divided clock as a fraction of 1.0 (0.50 => 50%%)>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Input data pattern (single bit per line, text file)\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"filename_with_input_data_pattern: <Enter filename containing input data pattern>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Enter type of noise on input (gaussian or uniform)\n");
	fprintf(fpw1,"* random_noise_sigma is sigma for gaussian\n");
	fprintf(fpw1,"* noise, and for uniform noise range of [-sigma,+sigma]\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"noise_type: <Enter choice for noise type>\n");
	fprintf(fpw1,"random_noise_sigma(UI_on_fin): <Enter sigma for input noise in unit intervals (UI)>\n");
	fprintf(fpw1,"noise_bandwidth(Hz): <Enter bandwidth of input noise in Hz>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Add sinusoidal jitter or perform jitter transfer\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"jitter_amp(UIpp): <Enter any sinusoidal jitter amplitude or value to use in jitter transfer in UI>\n");
	fprintf(fpw1,"jitter_freqs(Hz): <Enter comma-separated list of jitter frequencies to use in jitter transfer in Hz>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Initial phase of input \n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"phase0(degrees): <Enter any initial phase offset in degrees>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Start and stop times of printed output, sinusoidal input jitter starts at TSTART_jt\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"TSTART(sec): <Enter start time to begin printing output data in sec>\n");
	fprintf(fpw1,"TSTART_jt(sec): <Enter start time to apply sinusoidal jitter in jitter transfer analysis in sec>\n");
	fprintf(fpw1,"TSTOP(sec): <Enter stop time of simulation in sec (overwritten in jitter transfer analysis)>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Enter desired timestep or the word \"default\"\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"deltat(sec): <Enter desired timestep in sec>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Enter desired lock detector time constant - enter 0 for no lock detector function\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"lock_detect_tau(sec): <Enter lock detector time constant in sec>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Enter phase-lock detector thresholds (used if lock detector time constant > 0)\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"lock_detect_delta_vc_limit(%%): <Enter maximum control voltage variation from center voltage (%%) or \"default\">\n");
	fprintf(fpw1,"lock_detect_rphase_pp_limit(UIpp): <Enter maximum relative phase (UIpp) or \"default\">\n");
	fprintf(fpw1,"lock_detect_rphase_pp_loss_lock_limit(UIpp): <Enter relative phase threshold to declare out of lock (UIpp) or \"default\">\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Enter desired number of points to save\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"number_of_print_points: <Enter the number of points to save to output file>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Inputs for phase or frequency step\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"phase_freq_step_time(sec): <Enter time to apply frequency or phase step in sec>\n");
	fprintf(fpw1,"phase_step(sec): <Enter magnitude of phase step in sec>\n");
	fprintf(fpw1,"freq_step(ppm): <Enter magnitude of frequency step in ppm from input frequency>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Inputs for application of SSC\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"ssc_start_time(sec): <Enter time to apply spread-spectrum in sec>\n");
	fprintf(fpw1,"max_pos_ssc(ppm): <Enter maximum positive SSC in ppm>\n");
	fprintf(fpw1,"max_neg_ssc(ppm): <Enter minimum negative SSC in ppm>\n");
	fprintf(fpw1,"freq_ssc(Hz): <Enter nominal spread spectrum frequency in Hz>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Phase detector and loop filter inputs\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"phase_detector_logic_hi(V): <Enter phase detector logic high in V>\n");
	fprintf(fpw1,"phase_detector_logic_low(V): <Enter phase detector logic low in V>\n");
	fprintf(fpw1,"signal_source_vs1(v): <Enter optional first leakage current source voltage in V>\n");
	fprintf(fpw1,"signal_source_vs2(v): <Enter optional second leakage current source voltage in V>\n");
	fprintf(fpw1,"power_supply_max(V): <Enter maximum possible signal value (limiting) in V>\n");
	fprintf(fpw1,"power_supply_min(V): <Enter minimum possible signal value (limiting) in V>\n");
	fprintf(fpw1,"rs1(ohms): <Enter source resistance for optional first leakage current source voltage in ohms>\n");
	fprintf(fpw1,"rs2(ohms): <Enter source resistance for optional second leakage current source voltage in ohms>\n");
	fprintf(fpw1,"R1(ohms): <Enter source resistance for phase detector logic high and low values in ohms>\n");
	fprintf(fpw1,"RZERO(ohms): <Enter loop filter zero resistor in ohms>\n");
	fprintf(fpw1,"R3(ohms): <Enter any resistance in series with loop filter capacitor in ohms>\n");
	fprintf(fpw1,"C1(farads): <Enter loop filter capacitor in farads>\n");
	fprintf(fpw1,"C2(farads): <Enter loop filter ripple capacitor in farads>\n");
	fprintf(fpw1,"initvc(V): <Enter initial voltage for loop filter capacitor in V>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Output filename and format (csv only)\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"output_base_filename: <Enter base name for output file (no file extension)>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Optionally plot output characteristics and vco control voltage characteristics.\n");
	fprintf(fpw1,"* Enter value of zero to omit all plots or any non-zero value to create plots.\n");
	fprintf(fpw1,"* Note - plotting requires Octave or gnuplot be installed. The value of plot_outputs\n");
	fprintf(fpw1,"* is forced to zero if one or the other programs is not installed\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"plot_outputs: <Enter 1 or 0 to enable or disable output plots>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* Enter preferred plotting tool of \"octave\" or \"gnuplot\"\n");
	fprintf(fpw1,"* Plots will be created using preferred plotting tool if\n");
	fprintf(fpw1,"* the executable is found.\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"plot_preference: <Enter choice for plotting tool>\n");
	fprintf(fpw1,"*\n");
	fprintf(fpw1,"* End of trans_pll inputs\n");
	fprintf(fpw1,"*\n");
	fclose(fpw1);
	}

return;
}


