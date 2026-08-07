num_digits = 4;
suffix = sprintf("Hz");
awk_comamnd_0 = sprintf("echo \"%1.12e\" > ./.gnu_number",nom_freq_Hz); 
awk_comamnd_1 = sprintf("awk -f %s/plotting_routines/gnuplot/add_units.nawk num_digits=%d suffix=%s \.\/\.gnu_number > \.\/\.gnu_number_formatted",PLOTTING_ROUTINES_DIR,num_digits,suffix);

system(awk_comamnd_0);
system(awk_comamnd_1);
nom_freq_Hz_string = system("cat ./.gnu_number_formatted");
system("rm ./.gnu_number ./.gnu_number_formatted");

if (plot_normalized_freq == 0) {
	column_1 = 2;
	column_2 = 3;
	} else {
	column_1 = 4;
	column_2 = 5;
	}
max_num_tics = 12;
base = 10.0;
force_ymin = 0;
force_ymin_value = 0.0;
force_ymax = 0;
force_ymax_value = 1.0;
force_xmin = 0;
force_xmin_value = 0.0;
force_xmax = 0;
force_xmax_value = 0.0;
set grid lw 1.5;

set multiplot title sprintf("%s",trans_pll_plot_title) font "Verdana,16";
set size 0.45,0.85;
set origin 0.05,0.05;
set format x "%.1f";
set format y "%.4e";
if (plot_normalized_freq == 0) {
	set title "{/:Bold Frequency versus Control Voltage}";
	} else {
	set title sprintf("{/:Bold Frequency Difference from}\n{/:Bold %s versus Control Voltage}",nom_freq_Hz_string)
	}

set pointsize 1.5 					# larger point
set xlabel 'Control Voltage (V)' offset -1,0

if (plot_normalized_freq == 0) {
	set ylabel sprintf("Frequency (Hz)") offset -1,0;
	} else {
	set ylabel sprintf("Frequency ({/Symbol D}f %% from %s)",nom_freq_Hz_string) offset -1,0
	}

unset xrange;
unset yrange;

set print $center_voltage_vline;

    do for [n=column_1:column_1] {
        stats input_filename u 1:column_1 nooutput;
        print sprintf("%1.12e,%1.12e\n", center_voltage,1.2*STATS_min_y);
        print sprintf("%1.12e,%1.12e", center_voltage,1.2*STATS_max_y);
    }
set print;
#########

if (force_ymin == 1) {
	STATS_min_y = force_ymin_value
	}
if (force_ymax == 1) {
	STATS_max_y = force_ymax_value
	}

	yrange_increment = (STATS_max_y - STATS_min_y)/max_num_tics;
	if (base == 1.0) {
	yrange_increment = 1.0;
	} else {
		if ((STATS_max_y - STATS_min_y) == 0.0) {
			yrange_increment = base**floor(log10(0.10*abs(STATS_min_y))/log10(base));
		} else {
			yrange_increment = base**floor(log10(yrange_increment)/log10(base));
			}		
		}

if (force_ymin == 1) {
	yrange_min = force_ymin_value;
	} else {
	if ((STATS_max_y - STATS_min_y) == 0.0) {
		yrange_min = 0.90*STATS_min_y;
	} else {
		yrange_min = yrange_increment*(floor(STATS_min_y/yrange_increment) - 0.0);
	}
	loop_counter = 1;
	if (STATS_min_y < 0.0) {
		while(((yrange_min - STATS_min_y)/yrange_increment > -0.50) && (loop_counter < 100)) {
			yrange_min = yrange_min - yrange_increment;
			loop_counter = loop_counter + 1;
			}
	} else {
		if(STATS_min_y == 0) {
			yrange_min = 0.0;
		} else {
			while(((yrange_min - STATS_min_y)/yrange_increment > 0.50) && (loop_counter < 100)) {
				yrange_min = yrange_min - yrange_increment;
				loop_counter = loop_counter + 1;
				}
			}
		}
	}
	yrange_max = yrange_min;
	j = 1;
	array y_tickvalues[100];
	found_yrange_flag = 0;
	while( (found_yrange_flag != 1) && (j < 100)) {
		i = 1;
		while( (found_yrange_flag == 0) && (i < max_num_tics)) {
			y_tickvalues[i] = yrange_min + j*yrange_increment*(i-1);
			yrange_max = y_tickvalues[i];
			if((yrange_max - STATS_max_y)/(j*yrange_increment) >= 0.0) {
				found_yrange_flag = 1;
				yrange_increment = j*yrange_increment;
			} else {
				found_yrange_flag = 0;
				}
			i = i + 1;
			}
		j = j + 1;
	}

if (force_ymax == 1) {
	yrange_max = force_ymax_value;
	}

#########

xrange_min = STATS_min_x;
xrange_max = STATS_max_x;

set xrange [xrange_min:xrange_max];
set xtics auto;
set yrange [yrange_min:yrange_max];
set ytics border yrange_min,yrange_increment,yrange_max;

set print $center_voltage_vline;
    do for [n=column_1:column_1] {
        stats input_filename u 1:column_1 nooutput;
        print sprintf("%1.12e,%1.12e\n", center_voltage,yrange_min);
        print sprintf("%1.12e,%1.12e", center_voltage,yrange_max);
    }
set print;
set grid;
set dashtype 8 (16,16);

if (plot_normalized_freq == 0) {
	if (center_voltage > 0.50) {
		set arrow 1 from (center_voltage - 0.10),yrange_min + 0.45*(yrange_max - yrange_min) to center_voltage,nom_freq_Hz;
		set label 100 sprintf("Center voltage\n%.1f mV",center_voltage/1e-03) at (center_voltage - 0.12), (yrange_min + 0.45*(yrange_max  - yrange_min)) right front boxed bs 2;
	} else {
		set arrow 1 from (center_voltage + 0.10),yrange_min + 0.45*(yrange_max - yrange_min) to center_voltage,nom_freq_Hz;
		set label 100 sprintf("Center voltage\n%.1f mV",center_voltage/1e-03) at (center_voltage + 0.12), (yrange_min + 0.45*(yrange_max  - yrange_min)) left front boxed bs 2;
	}
	} else {
	if (center_voltage > 0.50) {
		set arrow 1 from (center_voltage - 0.10),0.07*yrange_min to center_voltage,0;
		set label 100 sprintf("Center voltage\n%.1f mV",center_voltage/1e-03) at (center_voltage - 0.12),yrange_min + 0.45*(yrange_max - yrange_min) right front boxed bs 2;
	} else {
		set arrow 1 from (center_voltage + 0.10),0.07*yrange_min to center_voltage,0;
		set label 100 sprintf("Center voltage\n%.1f mV",center_voltage/1e-03) at (center_voltage + 0.12),yrange_min + 0.45*(yrange_max - yrange_min) left front boxed bs 2;
		}
	}

plot input_filename u 1:column_1 title '' with lines linecolor rgb "blue", $center_voltage_vline using 1:2 title '' w impulse dt 8 linecolor rgb "black";

unset xlabel;
unset ylabel;
unset xrange;
unset yrange;

max_num_tics = 10;
base = 10.0;
if (plot_normalized_freq == 0) {
	force_ymin = 1;
	} else {
	force_ymin = 0;
}
force_ymin_value = 0.0;
force_ymax = 0;
force_ymax_value = 1.0;
force_xmin = 0;
force_xmin_value = 0.0;
force_xmax = 0;
force_xmax_value = 0.0;

set print $center_voltage_vline;
     do for [n=column_2:column_2] {
        stats input_filename u 1:column_2 every ::2 nooutput;
        print sprintf("%1.12e,%1.12e\n", center_voltage,1.2*STATS_min_y);
        print sprintf("%1.12e,%1.12e", center_voltage,1.2*STATS_max_y);
    }
set print;
########

if (force_ymin == 1) {
	STATS_min_y = force_ymin_value
	}
if (force_ymax == 1) {
	STATS_max_y = force_ymax_value
	}

	yrange_increment = (STATS_max_y - STATS_min_y)/max_num_tics;
	if (base == 1.0) {
	yrange_increment = 1.0;
	} else {
		if ((STATS_max_y - STATS_min_y) == 0.0) {
			yrange_increment = base**floor(log10(0.10*abs(STATS_min_y))/log10(base));
		} else {
			yrange_increment = base**floor(log10(yrange_increment)/log10(base));
			}		
		}
if (force_ymin == 1) {
	yrange_min = force_ymin_value;
	} else {	
	if ((STATS_max_y - STATS_min_y) == 0.0) {
		yrange_min = 0.90*STATS_min_y;
	} else {
		yrange_min = yrange_increment*(floor(STATS_min_y/yrange_increment) - 0.0);
	}

	loop_counter = 1;
	if (STATS_min_y < 0.0) {
		while(((yrange_min - STATS_min_y)/yrange_increment > -0.50) && (loop_counter < 100)) {
			yrange_min = yrange_min - yrange_increment;
			loop_counter = loop_counter + 1;
			}
	} else {
		if(STATS_min_y == 0) {
			yrange_min = 0.0;
		} else {
			while(((yrange_min - STATS_min_y)/yrange_increment > 0.50) && (loop_counter < 100)) {
				yrange_min = yrange_min - yrange_increment;
				loop_counter = loop_counter + 1;
				}
			}
		}
	}

	yrange_max = yrange_min;
	j = 1;
	array y_tickvalues[100];
	found_yrange_flag = 0;
	while( (found_yrange_flag != 1) && (j < 100)) {
		i = 1;
		while( (found_yrange_flag == 0) && (i < max_num_tics)) {
			y_tickvalues[i] = yrange_min + j*yrange_increment*(i-1);
			yrange_max = y_tickvalues[i];
			if((yrange_max - STATS_max_y)/(j*yrange_increment) >= 1.0) {
				found_yrange_flag = 1;
				yrange_increment = j*yrange_increment;
			} else {
				found_yrange_flag = 0;
				}
			i = i + 1;
			}
		j = j + 1;
	}

if (force_ymax == 1) {
	yrange_max = force_ymax_value;
	}

#########
xrange_min = STATS_min_x;
xrange_max = STATS_max_x;

set xrange [xrange_min:xrange_max];
set xtics auto;

set yrange [yrange_min:yrange_max];
set ytics border yrange_min,yrange_increment,yrange_max;

set print $center_voltage_vline;
    do for [n=column_2:column_2] {
        stats input_filename u 1:column_2 nooutput;
        print sprintf("%1.12e,%1.12e\n", center_voltage,yrange_min);
        print sprintf("%1.12e,%1.12e", center_voltage,yrange_max);
    }
set print;
set size 0.45,0.85;
set origin 0.5,0.05;
set format x "%.1f";
set format y "%.2e";
if (plot_normalized_freq == 0) {
	set title "{/:Bold Kvco versus Control Voltage}";
	} else {
	set title sprintf("{/:Bold Kvco versus Control Voltage}\n{/:Bold Normalized to %s}",nom_freq_Hz_string)
	}

set xlabel 'Control Voltage (V)' offset -1,0

if (plot_normalized_freq == 0) {
	set ylabel 'Kvco (Hz/V)' offset -1,0;
	} else {
	set ylabel 'Kvco (%/V)' offset 0,0;
	}

set grid;

if (plot_normalized_freq == 0) {
	if (center_voltage > 0.50) {
		set arrow 1 from (center_voltage - 0.10),(yrange_min + (0.93*nom_kvco_Hz_per_V  - yrange_min)) to center_voltage,nom_kvco_Hz_per_V;
		set label 100 sprintf("Center voltage\n%.1f mV",center_voltage/1e-03) at (center_voltage - 0.12), (yrange_min + (0.93*nom_kvco_Hz_per_V  - yrange_min)) right front boxed bs 2;
		} else {
		set arrow 1 from (center_voltage + 0.10),(yrange_min + (0.93*nom_kvco_Hz_per_V  - yrange_min)) to center_voltage,nom_kvco_Hz_per_V;
		set label 100 sprintf("Center voltage\n%.1f mV",center_voltage/1e-03) at (center_voltage + 0.12), (yrange_min + (0.93*nom_kvco_Hz_per_V  - yrange_min)) left front boxed bs 2;
		}
	} else {
	if (center_voltage > 0.50) {
		set arrow 1 from (center_voltage - 0.10),(yrange_min + (93.0*nom_kvco_Hz_per_V/nom_freq_Hz  - yrange_min))) to center_voltage,100.0*nom_kvco_Hz_per_V/nom_freq_Hz
		set label 100 sprintf("Center voltage\n%.1f mV",center_voltage/1e-03) at (center_voltage - 0.12), (yrange_min + (93.0*nom_kvco_Hz_per_V/nom_freq_Hz  - yrange_min)) right front boxed bs 2;
	} else {
		set arrow 1 from (center_voltage + 0.10),(yrange_min + (93.0*nom_kvco_Hz_per_V/nom_freq_Hz  - yrange_min)) to center_voltage,100.0*nom_kvco_Hz_per_V/nom_freq_Hz
		set label 100 sprintf("Center voltage\n%.1f mV",center_voltage/1e-03) at (center_voltage + 0.12), (yrange_min + (93.0*nom_kvco_Hz_per_V/nom_freq_Hz  - yrange_min)) left front boxed bs 2;
		} 
	}

set dashtype 8 (16,16);
plot input_filename u 1:column_2 every ::2 title '' with lines linecolor rgb "red", $center_voltage_vline using 1:2 title '' w impulse dt 8 linecolor rgb "black";

unset multiplot;
