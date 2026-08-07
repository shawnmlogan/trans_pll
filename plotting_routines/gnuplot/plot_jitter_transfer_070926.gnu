
set datafile separator ","

# jitter_transfer_plot_title = sprintf("Multiplot title");
# input_filename = sprintf("./2meg_bw_0n_delay/testout_jitter_transfer_0n_delay_jitter_transfer_052323_21_22_16.csv");
# timestamp = sprintf("052323_21_22_16");
# PLOTTING_ROUTINES_DIR = sprintf("/Users/sml/");

first_order_flag = 0;
third_order_flag = 0;
pdf_flag = 0;
curve_fit_flag = 0;
detected_did_not_lock_flag = 0;
pdf_page_divisor = 1.699044244847;
file_suffix_string = sprintf("");
two_pi = 2.0*pi;
tau_sec = RZERO*C1;

# Filter gain and phase data by lock status #
		
sys_command_0 = sprintf("awk 'BEGIN {FS=\",\";} {if (NR > 1) { if (($4 !~ \"No lock\") || ($4 ~ \"Lock detect feature was not active\")) print $0;} else print $0;}' %s > tempfile9_%s;",input_filename,timestamp);

# print sys_command_0;
system(sys_command_0);

sys_command_0 = sprintf("wc -l < tempfile9_%s;",timestamp);
number_of_locked_cases = system(sys_command_0) - 1;

if (number_of_locked_cases > 2) {

input_filename_filtered = sprintf("%s",sprintf("tempfile9_%s",timestamp));

sys_command_0 = sprintf("printf \"%%s\" $(diff -qs %s %s | grep -c differ) > tempfile10_%s;",input_filename_filtered,input_filename,timestamp);
system(sys_command_0);
detected_did_not_lock_flag = int(0.0 + system(sprintf("cat tempfile10_%s",timestamp)));

# print sprintf("detected_did_not_lock_flag = %d.\n",detected_did_not_lock_flag);

# Curve fit gain and phase only if gain passes through -3 dB and
# phase passes through 45 or 90 degrees

# Find end value of phase 
# STATS_pos_min_y x coordinate of a point with minimum y value
# STATS_pos_max_y x coordinate of a point with maximum y value

stats input_filename_filtered u 3:1 nooutput;
end_phase = STATS_pos_max_y;
start_freq = STATS_min_y;
end_freq = STATS_max_y;

stats input_filename_filtered u 1:2 nooutput;
max_gain = STATS_max_y;
min_gain = STATS_min_y;
number_of_points = STATS_records;

stats input_filename_filtered u 1:3 nooutput;
max_phase = STATS_max_y;
min_phase = STATS_min_y;

# print sprintf("max_phase = %.2f, min_phase = %.2f.\n",max_phase,min_phase);

sys_command_0 = sprintf("awk 'BEGIN {FS=\",\"; i = 1;found_corner_freq_flag = 0;} {if (NR > 1) {freq[i] = $1; gain[i] = $2; i = i + 1;}} END { for(j = 2; j < i; j++) { if ((gain[j - 1] > -3.0) && (gain[j] <= -3.0) && (found_corner_freq_flag == 0)) {corner_freq_Hz = freq[j - 1] + (-3.0 - gain[j - 1])*(freq[j] - freq[j - 1])/(gain[j] - gain[j - 1]); found_corner_freq_flag = 1;}} if (found_corner_freq_flag == 1) printf(\"%%1.12e\",corner_freq_Hz); else printf(\"-1.0\");}' tempfile9_%s;",timestamp);

corner_freq_estimate_Hz = system(sys_command_0) + 0.0;

# print sprintf("corner_freq_estimate_Hz = %1.6e.\n",corner_freq_estimate_Hz);

sys_command_0 = sprintf("awk 'function log10(x) {return log(x)/log(10.0);} BEGIN {FS=\",\"; i = 1;first_order_gain_slope_flag = 0; min_gain_slope = 1e12;} {if (NR > 1) {freq[i] = $1; gain[i] = $2; i = i + 1;}} END { for(j = 2; j < i; j++) { slope = (gain[j] - gain[j - 1])/log10(freq[j]/freq[j - 1]); if ( slope < min_gain_slope) min_gain_slope = slope;} if (min_gain_slope > -20.0) printf(\"1.0\\n\"); else printf(\"0.0\\n\");}' tempfile9_%s;",timestamp);

first_order_gain_slope_flag = system(sys_command_0) + 0.0;

# print sprintf("first_order_gain_slope_flag = %.0f.\n",first_order_gain_slope_flag);
# print sprintf("max_gain = %.2f min_gain = %.2f.\n",max_gain,min_gain);
# print sprintf("max_phase = %.2f (%.2e Hz) min_phase = %.2f (%.2e Hz).\n",max_phase,STATS_pos_max_y,min_phase,STATS_pos_min_y);

if ((corner_freq_estimate_Hz > 0.0) && (number_of_points > 10)) {
	if ((max_phase > -45.0) && (min_phase < -45.0)) {
		curve_fit_flag = 1;
	} else {
		curve_fit_flag = 0;
		if ((max_phase > -45.0) && (min_phase > -45.0)) {
			print sprintf("Phase does not cross -45 degrees, curve fit not performed.");
			}
	}
} else {
		curve_fit_flag = 0;
		if (corner_freq_estimate_Hz <= 0.0) {
			print sprintf("Corner frequency cannot be established from data, curve fit not performed.");
		}
		if (number_of_points <= 10) {
			print sprintf("At least 10 frequencies that phase lock are required (detected %d), curve fit not performed.",number_of_points);
		}
	}

# print sprintf("curve_fit_flag = %d.\n",curve_fit_flag);

if (pdf_flag == 1) {
		pdf_width = 14.0/(pdf_page_divisor + 0.0);pdf_height = pdf_width*(600.0/1200.0);
		set terminal pdfcairo enhanced size pdf_width,pdf_height font "Verdana,14";
	} else {
		set terminal pngcairo size 1200,600 font "Verdana,14";
	}
base_input_filename = sprintf("%s",substr(input_filename,1,strstrt(input_filename,".csv") - 1));

output_filename = sprintf("tempfile11_%s",timestamp);
set output output_filename;

max_num_tics = 16;
base = 1.0;
force_ymin = 0;
force_ymin_value = 0.0;
force_ymax = 0;
force_ymax_value = 1.0;
force_xmin = 0;
force_xmin_value = 0.0;
force_xmax = 0;
force_xmax_value = 0.0;
set style textbox 2 opaque noborder;
set style textbox 3 border lc "black" margins 2,2 opaque;

set multiplot title sprintf("%s",jitter_transfer_plot_title);
set size 0.45,0.85;
set origin 0.05,0.05;

set title sprintf("{/:Bold Jitter Transfer Gain versus Frequency}");

# set pointsize 1.5 					# larger point
set xlabel 'Frequency (Hz)' offset -1,0
set ylabel sprintf("Gain (dB)") offset 1,0;

unset xrange;
unset yrange;

# Determine if any cases did not lock using lock time in column 4

system(sprintf("awk \-f %s/plotting_routines/gnuplot/find_all_phase_lock.nawk column_num=4 %s > ./.find_all_phase_lock_flag",PLOTTING_ROUTINES_DIR,input_filename_filtered));
phase_lock_flag = 0.0 + system(sprintf("cat ./.find_all_phase_lock_flag | awk \'{print $1;}\'"));
out_of_lock_cases = 0.0 + system(sprintf("cat ./.find_all_phase_lock_flag | awk \'{print $2;}\'"));
system(sprintf("rm .find_all_phase_lock_flag"));

stats input_filename_filtered u 1:3 nooutput;
max_phase = STATS_max_y
min_phase = STATS_min_y;

stats input_filename_filtered u 1:2 nooutput;

########

if (force_xmin == 1) {
	STATS_min_x = force_xmin_value
	}
if (force_xmax == 1) {
	STATS_max_x = force_xmax_value
	}
set logscale x;
xrange_min = STATS_min_x;
xrange_max = STATS_max_x;
set xrange[xrange_min:xrange_max];
set xtics 10;

STATS_min_x = log10(STATS_min_x);
STATS_max_x = log10(STATS_max_x);

	xrange_increment = (STATS_max_x - STATS_min_x)/max_num_tics;
	if (base == 1.0) {
	xrange_increment = 1.0;
	} else {
		xrange_increment = base**floor(log10(xrange_increment)/log10(base));
		}
if (force_xmin == 1) {
	yrange_min = force_xmin_value;
	} else {	
	xrange_min = xrange_increment*(floor(STATS_min_x/xrange_increment) - 0.0);
	loop_counter = 1;
	if (STATS_min_x < 0.0) {
		while(((xrange_min - STATS_min_x)/xrange_increment > -0.50) && (loop_counter < 100)) {
			xrange_min = xrange_min - xrange_increment;
			loop_counter = loop_counter + 1;
			}
	} else {
		if(STATS_min_x == 0) {
			xrange_min = 0.0;
		} else {
			while(((xrange_min - STATS_min_x)/xrange_increment > 0.50) && (loop_counter < 100)) {
				xrange_min = xrange_min - xrange_increment;
				loop_counter = loop_counter + 1; }
				}
		}
	}
	xrange_max = xrange_min;
	j = 1;
	array x_tickvalues[100];
	found_xrange_flag = 0;
	while( (found_xrange_flag != 1) && (j < 100)) {
		i = 1;
		while( (found_xrange_flag == 0) && (i < max_num_tics)) {
			x_tickvalues[i] = xrange_min + j*xrange_increment*(i-1);
			xrange_max = x_tickvalues[i];
			if((xrange_max - STATS_max_x)/(j*xrange_increment) >= 0.0) {
				found_xrange_flag = 1;
				xrange_increment = j*xrange_increment;
			} else {
				found_xrange_flag = 0;
				}
			i = i + 1;
			}
		j = j + 1;
	}

if (force_xmax == 1) {
	xrange_max = force_xmax_value;
	}

#########

yrange_min = -36.0;
yrange_max =  12.0;
yrange_increment = 3.0;

set yrange [yrange_min:yrange_max];
set for [i=0:max_num_tics] ytics border (yrange_min + i*yrange_increment);
if (force_ymax == 1) {
	set ytics add force_ymax_value;}

set xrange [10**xrange_min:10**xrange_max];
set for [i=0:max_num_tics] xtics border (10**(xrange_min + i*xrange_increment));
if (force_xmax == 1) {
	set xtics add force_ymax_value;}

set xtics 10;
set grid mxtics lt 8 lw 0.25;
set grid xtics lt 8 lw 0.25;
set grid ytics lt 8 lw 0.25;
set format x "%g"

set grid;
set logscale x;

if (out_of_lock_cases == 1) {
	out_of_lock_text = sprintf("Warning: one analysis\ndid not phase-lock!");
} else {
	if (out_of_lock_cases > 1) {
		out_of_lock_text = sprintf("Warning: %d analyses\ndid not phase-lock!",out_of_lock_cases);
		}
}

if (phase_lock_flag == 0) {
   	set label 200 out_of_lock_text at 20*(10**xrange_min),-27 center boxed bs 2;
   }

if (curve_fit_flag == 1) {

	if (first_order_gain_slope_flag == 0.0) {
		first_order_flag = 0;
		log_filename = sprintf("second_order_jitter_transfer_curve_fit_%s.log",timestamp);
		system(sprintf("if [[ -e %s ]]; then rm %s; fi",log_filename,log_filename));
		logfile_entry = sprintf("Choosing second-order or third-order curve fit.\n");
	} else {
		first_order_flag = 1;
		log_filename = sprintf("first_order_jitter_transfer_curve_fit_%s.log",timestamp);
		system(sprintf("if [[ -e %s ]]; then rm %s; fi",log_filename,log_filename));
		logfile_entry = sprintf("Choosing first-order curve fit.\n");
	}
	
	log_file_command = sprintf("printf \"\n%%s\" \"%s\" > %s",logfile_entry,log_filename);
	system(log_file_command);

	set fit quiet;
	set fit logfile log_filename;

	# first-order transfer function
	
	if (first_order_flag == 1) {
		
		unset xrange;

		log_h(x) = 10.0*log10(1.0/(1.0 + (x/wc)**2));
		phase(x) = (180.0/pi)*(0.0 - atan2(x,wc));
		
		stats input_filename_filtered u 1:(abs($2 + 3.0)) nooutput;
		wc = STATS_pos_min_y;
		
		fit log_h(x) input_filename_filtered u ($1*two_pi):2 via wc;
		
		logfile_entry = sprintf("first-order curve fit result: wc = %g\n",wc);
		log_file_command = sprintf("printf \"%%s\" \"%s\" >> %s",logfile_entry,log_filename);
		system(log_file_command);
	
	} else {

		# second-order transfer function

		set fit v4;
		set fit quiet;
		set fit logfile log_filename;
		unset xrange;
		tau_sec_square_root = tau_sec**0.50;
				
		stats input_filename_filtered u ($1*two_pi):(abs($2 + 3.0)) nooutput;
		wn = STATS_pos_min_y;
		
		stats input_filename_filtered u ($1*two_pi):2 nooutput;
		max_gain = STATS_max_y;
		num_columns = STATS_columns;
		
		if (max_gain > 1.0) {
			damping = 0.30;
		} else {
			if (max_gain > 0.0) {
				damping = 0.8;
			} else {
				damping = 1.2;
				}
			}
		
		log_h2(x) = 20.0*log10((((((wn_square_root**2)*(wn_square_root**2))**2) + (x*(tau_sec_square_root**2)*(wn_square_root**2)*(wn_square_root**2))**2)**0.50)/((((wn_square_root**2)**2 - x**2)**2 + (2.0*(damping_square_root**2)*x*(wn_square_root**2))**2)**0.50));
		
		phase2(x) = (180.0/pi)*(atan2(x*(tau_sec_square_root**2),1.0) - atan2(2.0*(damping_square_root**2)*(x/(wn_square_root**2)),(1.0 - (x/(wn_square_root**2))**2)));

		wn_square_root = wn**0.50;
		damping_square_root = damping**0.50;
		tau_sec_square_root = tau_sec**0.50;
	
		sys_command_0 = sprintf("awk \-f %s/plotting_routines/gnuplot/add_weights.nawk %s > tempfile100_%s",PLOTTING_ROUTINES_DIR,input_filename_filtered,timestamp);
		system(sys_command_0);

		fit_command = sprintf("fit log_h2(x) \"tempfile100_%s\" u ($1*two_pi):2:%d via wn_square_root,damping_square_root,tau_sec_square_root",timestamp,num_columns + 1);
		eval fit_command;
		
		wn = wn_square_root**2;
		damping = damping_square_root**2;
		tau_sec = tau_sec_square_root**2;

		# Determine -3 dB corner frequency via binary search
		
		x = 2.0*pi*start_freq;
		delta_f_rad_per_sec = 2.0*pi*(end_freq - start_freq);
		loop_counter = 0;
		while ((abs(log_h2(x) + 3.0) > 0.10) && (loop_counter < 100)) {
			if ((log_h2(x + delta_f_rad_per_sec/2.0) + 3.0) > 0.0) {
				x = x + delta_f_rad_per_sec/2.0;
				} else {
				x = x;
				}
			delta_f_rad_per_sec = delta_f_rad_per_sec/2.0;
			loop_counter = 1 + loop_counter;
			}
		wc = x;

		logfile_entry = sprintf("second-order curve fit result: wn = %g, damping = %g, wc = %g, tau_sec = %g\n",wn,damping,wc,tau_sec);
		log_file_command = sprintf("printf \"%%s\" \"%s\" >> %s",logfile_entry,log_filename);
		system(log_file_command);
	
		stats input_filename_filtered u 1:(log_h2(column(1)*two_pi) - column(2)) nooutput;
		second_order_mean = STATS_mean_y;
		second_order_sigma = STATS_stddev_y;
		second_order_sum_sq = STATS_sumsq_y;
		unset yrange;
		stats input_filename_filtered u 1:(phase2(column(1)*two_pi) - column(3)) nooutput;
		second_order_phase_mean = STATS_mean_y;

		wn_2 = wn; wn_2_square_root = wn_square_root;
		damping_2 = damping; damping_2_square_root = damping_square_root;
		tau_sec_2 = tau_sec; tau_sec_2_square_root = tau_sec_square_root;
		
		logfile_entry = sprintf("Second-order gain error mean = %g, Second-order gain error sigma = %g,\nSecond-order gain error sum_sq = %g Second-order phase error mean = %g\n",second_order_mean,second_order_sigma,second_order_sum_sq,second_order_phase_mean);
		log_file_command = sprintf("printf \"\n%%s\" \"%s\" >> %s",logfile_entry,log_filename);
		system(log_file_command);

		# Attempt third-order fit
		
		c0 = 1e-2;
		a0 = 1e-2;
		a1 = 1e-2;
		a2 = 1e-2;
		b0 = 1.0;
		unset xrange;
				
		log_h3(x) = 20.0*log10(b0*((1.0 + (c0*x)**2)*0.50)/(((1.0 - (a2*x)**2))**2 + (a1*x - (a0*x)**3)**2)**0.50);
		
		phase3(x) = (180.0/pi)*(atan2(b0*c0*x,b0) - atan2(a1*x - (a0*x)**3,1.0 - (a2*x)**2));
		
		fit_command = sprintf("fit log_h3(x) \"tempfile100_%s\" u ($1*two_pi):2:%d via c0,a0,a1,a2",timestamp,num_columns + 1);

		eval fit_command;
		
		# Determine -3 dB corner frequency via binary search
		
		x = 2.0*pi*start_freq;
		delta_f_rad_per_sec = 2.0*pi*(end_freq - start_freq);
		loop_counter = 0;
		while ((abs(log_h3(x) + 3.0) > 0.10) && (loop_counter < 100)) {
			if ((log_h3(x + delta_f_rad_per_sec/2.0) + 3.0) > 0.0) {
				x = x + delta_f_rad_per_sec/2.0;
				} else {
				x = x;
				}
			delta_f_rad_per_sec = delta_f_rad_per_sec/2.0;
			loop_counter = 1 + loop_counter;
			}
		wc = x;
		
		logfile_entry = sprintf("Third-order curve fit result: b0 = %g, c0 = %g, a0 = %g, a1 = %g a2 = %g, wc = %g\n",b0,c0,a0,a1,a2,wc);
		log_file_command = sprintf("printf \"\n%%s\" \"%s\" >> %s",logfile_entry,log_filename);
		system(log_file_command);

		stats input_filename_filtered u 1:(log_h3(column(1)*two_pi) - column(2)) nooutput;
		
		third_order_mean = STATS_mean_y;
		third_order_sigma = STATS_stddev_y;
		third_order_sum_sq = STATS_sumsq_y;
		unset yrange;
		stats input_filename_filtered u 1:(phase3(column(1)*two_pi) - column(3)) nooutput;
		third_order_phase_mean = STATS_mean_y;
		# print sprintf("Third-order phase mean = %g.\n",third_order_phase_mean);
	
		logfile_entry = sprintf("Third-order gain error mean = %g, Third-order gain error sigma = %g,\nThird-order gain error sum_sq = %g Third-order phase error mean = %g\n",third_order_mean,third_order_sigma,third_order_sum_sq,third_order_phase_mean);
		log_file_command = sprintf("printf \"%%s\" \"%s\" >> %s",logfile_entry,log_filename);
		system(log_file_command);
		
		if ((third_order_sum_sq < second_order_sum_sq) && (abs(third_order_phase_mean) < abs(second_order_phase_mean))) {
			third_order_flag = 1;
			log_h(x) = log_h3(x);
			phase(x) = phase3(x);
			logfile_entry = sprintf("Choosing third-order curve fit over second-order curve fit.\n");
			log_file_command = sprintf("printf \"%%s\" \"%s\" >> %s",logfile_entry,log_filename);
			system(log_file_command);
			} else {
			log_h(x) = log_h2(x);
			phase(x) = phase2(x);
			wn = wn_2; wn_square_root = wn_2_square_root;
			damping = damping_2; damping_square_root = damping_2_square_root;
			tau_sec = tau_sec_2; tau_sec_square_root = tau_sec_2_square_root;
			logfile_entry = sprintf("Choosing second-order curve fit over third-order curve fit.\n");
			log_file_command = sprintf("printf \"%%s\" \"%s\" >> %s",logfile_entry,log_filename);
			system(log_file_command);
			}
				
		# Determine -3 dB corner frequency via binary search
		
		x = 2.0*pi*start_freq;
		delta_f_rad_per_sec = 2.0*pi*(end_freq - start_freq);
		loop_counter = 0;
		while ((abs(log_h(x) + 3.0) > 0.10) && (loop_counter < 100)) {
			if ((log_h(x + delta_f_rad_per_sec/2.0) + 3.0) > 0.0) {
				x = x + delta_f_rad_per_sec/2.0;
				} else {
				x = x;
				}
			delta_f_rad_per_sec = delta_f_rad_per_sec/2.0;
			loop_counter = 1 + loop_counter;
			}
		wc = x;

		wn = wn_square_root**2;
		damping = damping_square_root**2;
		tau_sec = tau_sec_square_root**2;
		
		if (third_order_flag == 1) {
			sys_command_0 = sprintf("mv %s third_order_jitter_transfer_curve_fit_%s.log",log_filename,timestamp);
			system(sys_command_0);
			}

		system(sprintf("rm tempfile100_%s",timestamp));
	
		# Convert numbers to text strings
		
		num_digits = 2;
		suffix = sprintf("rad/sec");
		awk_command_0 = sprintf("echo \"%1.12e\" > ./.gnu_number",wn); 
		awk_command_1 = sprintf("awk -f %s/plotting_routines/gnuplot/add_units.nawk num_digits=%d suffix=%s \.\/\.gnu_number > \.\/\.gnu_number_formatted",PLOTTING_ROUTINES_DIR,num_digits,suffix);
		
		system(awk_command_0);
		system(awk_command_1);
		wn_string = system("cat ./.gnu_number_formatted");
		system("rm ./.gnu_number ./.gnu_number_formatted");
	
		damping_string = sprintf("%.3f",damping);
		

		if (third_order_flag != 1) {
			num_digits = 2;
			suffix = sprintf("s");
			awk_command_0 = sprintf("echo \"%1.12e\" > ./.gnu_number",tau_sec); 
			awk_command_1 = sprintf("awk -f %s/plotting_routines/gnuplot/add_units.nawk num_digits=%d suffix=%s \.\/\.gnu_number > \.\/\.gnu_number_formatted",PLOTTING_ROUTINES_DIR,num_digits,suffix);
		
			system(awk_command_0);
			system(awk_command_1);
			tau_sec_string = system("cat ./.gnu_number_formatted");
			system("rm ./.gnu_number ./.gnu_number_formatted");
			}
		}

	# Validate corner frequency
	
	if (abs((corner_freq_estimate_Hz - wc/two_pi)/corner_freq_estimate_Hz) > 0.30) {
		fail_curve_fit_flag = 1;
		print sprintf("Curve fit result invalid and not used:\ncorner_freq_estimate_Hz = %g Hz, curve fit estimate = %g Hz (%.2f%% difference)",corner_freq_estimate_Hz,wc/two_pi,100.0*(wc/two_pi - corner_freq_estimate_Hz)/corner_freq_estimate_Hz);
		} else {
		fail_curve_fit_flag = 0;
		}

if (fail_curve_fit_flag == 0) {
		# Convert numbers to text strings
		
		num_digits = 2;
		suffix = sprintf("rad/sec");
		awk_command_0 = sprintf("echo \"%1.12e\" > ./.gnu_number",wc); 
		awk_command_1 = sprintf("awk -f %s/plotting_routines/gnuplot/add_units.nawk num_digits=%d suffix=%s \.\/\.gnu_number > \.\/\.gnu_number_formatted",PLOTTING_ROUTINES_DIR,num_digits,suffix);
		
		system(awk_command_0);
		system(awk_command_1);
		wc_string = system("cat ./.gnu_number_formatted");
		system("rm ./.gnu_number ./.gnu_number_formatted");
		
		num_digits = 2;
		suffix = sprintf("Hz");
		awk_command_0 = sprintf("echo \"%1.12e\" > ./.gnu_number",wc/two_pi); 
		awk_command_1 = sprintf("awk -f %s/plotting_routines/gnuplot/add_units.nawk num_digits=%d suffix=%s \.\/\.gnu_number > \.\/\.gnu_number_formatted",PLOTTING_ROUTINES_DIR,num_digits,suffix);
		
		system(awk_command_0);
		system(awk_command_1);
		fc_Hz_string = system("cat ./.gnu_number_formatted");
		system("rm ./.gnu_number ./.gnu_number_formatted");
		
		if (first_order_flag == 1) {
			label_1 = sprintf("{/Times-Italic=14 -3 dB frequency (f_c)} ≈ %s",fc_Hz_string);
			set label 2 label_1 at graph 0.10,0.45 front boxed bs 3;
		} else {
			if (third_order_flag != 1) {
				label_1 = sprintf("{/Times-Italic=14 {/Symbol w}_n ≈ %s}\n{{/Symbol z} {/Times-Italic=14 ≈  %s}}\n{{/Symbol t} {/Times-Italic=14 ≈  %s}}\n{/Times-Italic=14 -3 dB frequency ≈ %s}",wn_string,damping_string,tau_sec_string,fc_Hz_string);
				} else {
				label_1 = sprintf("{/Times-Italic=14 -3 dB frequency ≈ %s}",fc_Hz_string);			
				}
			}
		set label 2 label_1 at graph 0.1,0.45 front boxed bs 3;
	
		if (detected_did_not_lock_flag == 1) {
			label_3 = sprintf("{/:Italic Data includes only frequencies}\n{/:Italic and amplitudes that phase lock}");
			set label 3 label_3 at graph 0.1,0.92 font ",12" front boxed bs 3;
			}
		set xrange [10**xrange_min:10**xrange_max];
		set yrange [yrange_min:yrange_max];
		plot input_filename_filtered u 1:2 notitle with linespoints pt "o" linecolor rgb "blue" lw 2, log_h(x*two_pi) notitle w lines linecolor rgb "blue" dt "-" lw 1.5;
		} else {
			if (third_order_flag == 1) {
				sys_command_0 = sprintf("rm third_order_jitter_transfer_curve_fit_%s.log",timestamp);
				} else {
					if (first_order_flag == 0) {
						sys_command_0 = sprintf("rm %s\n",log_filename);
					} else {
						sys_command_0 = sprintf("rm first_order_jitter_transfer_curve_fit_%s.log",timestamp);
					}
				}
		system(sys_command_0);
		set xrange [10**xrange_min:10**xrange_max];
		set yrange [yrange_min:yrange_max];
		plot input_filename_filtered u 1:2 notitle with linespoints pt "o" linecolor rgb "blue" lw 2;
		}
} else {

if (detected_did_not_lock_flag == 1) {
	label_3 = sprintf("{/:Italic Data includes only frequencies}\n{/:Italic and amplitudes that phase lock}");
	set label 3 label_3 at graph 0.1,0.92 font ",12" front boxed bs 3;
	}

plot input_filename_filtered u 1:2 notitle with linespoints pt "o" linecolor rgb "blue" lw 2;

}

unset label 2;

unset xlabel;
unset ylabel;
unset xrange;
unset yrange;
unset label 200;
stats input_filename_filtered u 1:3 nooutput;

#######

yrange2_min = -270.0*(pi/180.0);
yrange2_max =  180.0*(pi/180.0);
yrange2_increment = 30.0*(pi/180.0);

set link y2 via y*(pi/180.0) inverse y*(180.0/pi);

yrange_min = -270.0;
yrange_max =  180.0;
yrange_increment = 30.0;

#######
set yrange [yrange_min:yrange_max];
set for [i=0:max_num_tics] ytics border (yrange_min + i*yrange_increment);
if (force_ymax == 1) {
	set ytics add force_ymax_value;
	}

set xrange [10**xrange_min:10**xrange_max];
set for [i=0:max_num_tics] xtics border (10**(xrange_min + i*xrange_increment));
if (force_xmax == 1) {
	set xtics add force_ymax_value;
	}

set xtics 10;
set grid mxtics lt 8 lw 0.25;
set grid xtics lt 8 lw 0.25;
set grid ytics lt 8 lw 0.25;
set format x "%g"

set size 0.45,0.85;
set origin 0.5,0.05;

set title "{/:Bold Jitter Transfer Phase versus Frequency}";

set xlabel 'Frequency (Hz)' offset -1,0
set ylabel 'Phase (deg)' offset 1,0;

set logscale x;

if ((curve_fit_flag == 1) && (fail_curve_fit_flag == 0)) {

if (detected_did_not_lock_flag == 1) {
	label_3 = sprintf("{/:Italic Data includes only frequencies}\n{/:Italic and amplitudes that phase lock}");
	set label 3 label_3 at graph 0.1,0.92 font ",12" front boxed bs 3;
	}
	set table $Mydata;
	plot input_filename_filtered u 1:($3*pi/180.0) axes x1y2 notitle with lines linecolor rgb "green" lw 2.0, phase(two_pi*x)*(pi/180.0) axes x1y2 notitle with lines linecolor rgb "green" lw 1.5 dt "-";;
	unset table;
	unset datafile separator;
	plot $Mydata index 0 u 1:2 axes x1y2 notitle with linespoints pt "o" linecolor rgb "green" lw 2.0, $Mydata index 1 u 1:2 axes x1y2 notitle with lines linecolor rgb "green" lw 1.5 dt "-" smooth unwrap;
	set datafile separator ",";
	
} else {

if (detected_did_not_lock_flag == 1) {
	label_3 = sprintf("{/:Italic Data includes only frequencies}\n{/:Italic and amplitudes that phase lock}");
	set label 3 label_3 at graph 0.1,0.92 font ",12" front boxed bs 3;
	}

	plot input_filename_filtered u 1:($3*pi/180.0) axes x1y2 notitle with linespoints pt "o" linecolor rgb "green" lw 2.0;

}

unset multiplot;

if ((curve_fit_flag == 1) && (fail_curve_fit_flag == 0)) {
	if (pdf_flag == 1) {
		output_filename = sprintf("%s_with_curve_fit_%s.pdf",base_input_filename,timestamp);
	} else {
		output_filename = sprintf("%s_with_curve_fit_%s.png",base_input_filename,timestamp);
	}
} else {
	if (pdf_flag == 1) {
		output_filename = sprintf("%s_%s.pdf",base_input_filename,timestamp);
	} else {
		output_filename = sprintf("%s_%s.png",base_input_filename,timestamp);	
	}
}

sys_command_0 = sprintf("mv tempfile11_%s %s",timestamp,output_filename);
system(sys_command_0);

system(sprintf("rm tempfile9_%s tempfile10_%s",timestamp,timestamp));
} else {
	if (number_of_locked_cases == 0) {
		print sprintf("Phase-lock not detected at any frequency of jitter transfer analysis.");
		} else {
		if (number_of_locked_cases == 1) {
			print sprintf("Phase-locked detected for only one frequency in jitter transfer analysis.");
			} else {
			print sprintf("Phase-locked detected for only %d frequencies in jitter transfer analysis.",number_of_locked_cases);
			}
		}
print sprintf("Jitter transfer results not plotted.");
}