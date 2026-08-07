clear;

# trans_pll_plot_title = '{/:Bold 12.00 GHz VCO, Kvco = 4.97 GHz/V, divider = 120, divider delay = 1.0 ns, RZERO = 10.0 kohms, R2 = 1.0 ohms}\n{/:Bold C1 = 100.00 pf, C2 = 0.36 pf, Icp = 120.0 uA, Icn = -120.0 uA (Ro = 1.0 Mohms), leakage current = 0.68 pA}\n{/:Bold Gaussian noise sigma = 100.00 mUI, bandwidth = 10.0 MHz}'; timestamp = '062423_14_49_59'

# input_filename = sprintf(input_filename = "./noise_10meg_bw_1ns_delay_gnuplot/testout_1ns_delay_062423_14_49_59_0.csv");
# timestamp = sprintf("062423_14_49_59");
# lock_time_sec = 6.140000000239e-06;
# lost_lock_time_sec = 0.0;
# PLOTTING_ROUTINES_DIR = sprintf("/Users/sml");


set datafile separator ",";

awk_command_0 = sprintf("awk 'BEGIN{FS=\",\"; num_header_lines = 0;} {if ($1 + 0.0 != $1) num_header_lines = num_header_lines + 1;} END {printf(\"%%d\",num_header_lines);}' %s",input_filename);
number_of_header_lines = system(awk_command_0) + 0;

system(sprintf("tail \-n +%d %s > ./.tempfile.csv",number_of_header_lines,input_filename));

gnu_program_location = sprintf("%s/plotting_routines/gnuplot/plot_transient_data_call.gnu",PLOTTING_ROUTINES_DIR);
call gnu_program_location "./.tempfile.csv" trans_pll_plot_title timestamp lock_time_sec PLOTTING_ROUTINES_DIR;

gnu_program_location = sprintf("%s/plotting_routines/gnuplot/plot_transient_data_x_call.gnu",PLOTTING_ROUTINES_DIR);
call gnu_program_location "./.tempfile.csv" trans_pll_plot_title timestamp lock_time_sec PLOTTING_ROUTINES_DIR;

gnu_program_location = sprintf("%s/plotting_routines/gnuplot/plot_transient_data_phase_call.gnu",PLOTTING_ROUTINES_DIR);
call gnu_program_location "./.tempfile.csv" trans_pll_plot_title timestamp lock_time_sec PLOTTING_ROUTINES_DIR 0;

gnu_program_location = sprintf("%s/plotting_routines/gnuplot/plot_transient_data_phase_call.gnu",PLOTTING_ROUTINES_DIR);
call gnu_program_location "./.tempfile.csv" trans_pll_plot_title timestamp lock_time_sec PLOTTING_ROUTINES_DIR 1;

system(sprintf("rm ./.tempfile.csv"));