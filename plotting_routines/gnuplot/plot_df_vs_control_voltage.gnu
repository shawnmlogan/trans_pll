
set datafile separator ","
pause_delay = 1.5;

# center_voltage = 3.396608e-01;
# nom_freq_Hz = 1.200000e+10;
# nom_kvco_Hz_per_V = 2.191231e+10;
# trans_pll_plot_title = sprintf("Multiplot title");
# input_filename = sprintf("gnuplot_delta_f_vs_vc.csv");
# timestamp = sprintf("062123_21_59_25");
# plot_normalized_freq = 0;
# PLOTTING_ROUTINES_DIR = sprintf("/Users/sml");

set terminal qt enhanced size 900,600 pos 0,0 font "Verdana,14";
set style textbox 2 opaque fc "white" noborder;

set term push;						# save current terminal settings

# write the graph in PNG format, pngcairo preferred to png as it supports better fonts and dashed lines

set terminal pngcairo size 1400,900 enhanced font "Verdana,14";
set output sprintf("vco_control_voltage_characteristic_%s.png",timestamp);

load(sprintf("%s/plotting_routines/gnuplot/plot_df_vs_control_voltage_multiplot.gnu",PLOTTING_ROUTINES_DIR));

# Re-run script using qt terminal;

set term pop;

load(sprintf("%s/plotting_routines/gnuplot/plot_df_vs_control_voltage_multiplot.gnu",PLOTTING_ROUTINES_DIR));

pause(pause_delay);



