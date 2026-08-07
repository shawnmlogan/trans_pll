clear;
unset xrange;
unset yrange;
unset title;
unset label;
unset arrow;

input_filename = ARGV[1];
trans_pll_plot_title = ARGV[2];
timestamp = ARGV[3];
lock_time_sec = ARGV[4];
PLOTTING_ROUTINES_DIR = ARGV[5];
plot_enable = ARGV[6];
max_lines_to_plot = 10000;

set datafile separator ",";
y_limits_mUI = 1000.0;
min_scale_factor = 0.001;
yrange_number_of_sigma = 12.0;

if (plot_enable == 1) {
set terminal pngcairo enhanced size 1100,800 font "Verdana,14";
# output_filename = sprintf("%s.png",substr(input_filename,1,strstrt(input_filename,".csv") - 1));
output_filename = sprintf("gnuplot_phasein_phaseo_vs_time_%s.png",timestamp);
set output output_filename;
} else {
set terminal qt enhanced size 900,600 pos 0,0 font "Verdana,14";
}

# Octave color map

color0 = sprintf("0x0072BD");
color1 = sprintf("0xD95319");
color2 = sprintf("0xEDB120");
color3 = sprintf("0x7E2F8E");
color4 = sprintf("0x77AC30");
color5 = sprintf("0x4DBEEE");
color6 = sprintf("0xA2142F");
color7 = sprintf("0xDE76F3");
color8 = sprintf("0x1DEC99");
color9 = sprintf("0xEF8BBD");

array color_array[9] = [color0,color1,color2,color1,color5,color7,color8,color9,color4];

set datafile columnheaders;
set key autotitle columnhead;

stats input_filename u 1:2 nooutput;
x_min = STATS_min_x;
x_max = STATS_max_x;

# Determine appropriate units for x-axis

if (log10(x_max) < -9) {
	x_scale_factor = 1e-12;
	x_units_text = sprintf("ps");
	} else {
		if (log10(x_max) < -6) {
		x_scale_factor = 1e-9;
		x_units_text = sprintf("ns");
		} else {
				if (log10(x_max) < -3) {
				x_scale_factor = 1e-6;
				x_units_text = sprintf("us");
				} else {
				if (log10(x_max) < 0.0) {
					x_scale_factor = 1e-3;
					x_units_text = sprintf("ms");
					} else {
						x_scale_factor = 1.0;
						x_units_text = sprintf("sec");
						}
					}
				}
			}

set format x "%.2f";
set grid x;
set grid y;
set grid lw 1.5;
set xlabel sprintf("Time (%s)",x_units_text);
set ylabel "TIE (mUI)";
set xrange [x_min:x_max];
stats input_filename u 1:7 nooutput;
column_num_7_average = STATS_mean_y;
column_num_7_rms = STATS_stddev_y;
stats input_filename u 1:8 nooutput;
column_num_8_average = STATS_mean_y;
column_num_8_rms = STATS_stddev_y;
stats input_filename u 1:9 nooutput;
column_num_9_average = STATS_mean_y;
column_num_9_rms = STATS_stddev_y;
stats input_filename u 1:10 nooutput;
column_num_10_average = STATS_mean_y;
column_num_10_rms = STATS_stddev_y;

number_of_records = STATS_records;

set style textbox 2 opaque fc "white" border lc "black" margins 1;

array averages1[4] = [column_num_7_average,column_num_8_average,column_num_9_average,column_num_10_average];

# Convert numbers to text strings

num_digits = 2;
suffix = sprintf("s");
awk_comamnd_0 = sprintf("echo \"%1.12e\" > ./.gnu_number",lock_time_sec); 
awk_comamnd_1 = sprintf("awk -f %s/plotting_routines/gnuplot/add_units.nawk num_digits=%d suffix=%s \.\/\.gnu_number > \.\/\.gnu_number_formatted",PLOTTING_ROUTINES_DIR,num_digits,suffix);

system(awk_comamnd_0);
system(awk_comamnd_1);
lock_time_sec_string = system("cat ./.gnu_number_formatted");
system("rm ./.gnu_number ./.gnu_number_formatted");

num_digits = 1;
suffix = sprintf("UI");
awk_comamnd_0 = sprintf("echo \"%1.12e\" > ./.gnu_number",column_num_7_average); 
awk_comamnd_1 = sprintf("awk -f %s/plotting_routines/gnuplot/add_units.nawk num_digits=%d suffix=%s \.\/\.gnu_number > \.\/\.gnu_number_formatted",PLOTTING_ROUTINES_DIR,num_digits,suffix);

system(awk_comamnd_0);
system(awk_comamnd_1);
column_num_7_average_string = system("cat ./.gnu_number_formatted");
system("rm ./.gnu_number ./.gnu_number_formatted");

num_digits = 1;
suffix = sprintf("UI");
awk_comamnd_0 = sprintf("echo \"%1.12e\" > ./.gnu_number",column_num_8_average); 
awk_comamnd_1 = sprintf("awk -f %s/plotting_routines/gnuplot/add_units.nawk num_digits=%d suffix=%s \.\/\.gnu_number > \.\/\.gnu_number_formatted",PLOTTING_ROUTINES_DIR,num_digits,suffix);

system(awk_comamnd_0);
system(awk_comamnd_1);
column_num_8_average_string = system("cat ./.gnu_number_formatted");
system("rm ./.gnu_number ./.gnu_number_formatted");

num_digits = 1;
suffix = sprintf("UI");
awk_comamnd_0 = sprintf("echo \"%1.12e\" > ./.gnu_number",column_num_9_average); 
awk_comamnd_1 = sprintf("awk -f %s/plotting_routines/gnuplot/add_units.nawk num_digits=%d suffix=%s \.\/\.gnu_number > \.\/\.gnu_number_formatted",PLOTTING_ROUTINES_DIR,num_digits,suffix);

system(awk_comamnd_0);
system(awk_comamnd_1);
column_num_9_average_string = system("cat ./.gnu_number_formatted");
system("rm ./.gnu_number ./.gnu_number_formatted");

num_digits = 1;
suffix = sprintf("UI");
awk_comamnd_0 = sprintf("echo \"%1.12e\" > ./.gnu_number",column_num_10_average); 
awk_comamnd_1 = sprintf("awk -f %s/plotting_routines/gnuplot/add_units.nawk num_digits=%d suffix=%s \.\/\.gnu_number > \.\/\.gnu_number_formatted",PLOTTING_ROUTINES_DIR,num_digits,suffix);

system(awk_comamnd_0);
system(awk_comamnd_1);
column_num_10_average_string = system("cat ./.gnu_number_formatted");
system("rm ./.gnu_number ./.gnu_number_formatted");

array average_strings[4] = [column_num_7_average_string,column_num_8_average_string,column_num_9_average_string,column_num_10_average_string];

array averages1[4] = [0.0,0.0,0.0,0.0];
array column_headers[17] = ["Time(s)","vpd(V)","vs1(V)","vs2(V)","vc1(V)","vc(V)","phasein(UI)","phaseo(UI)","phaseo_delayed(UI)","rphase(UI)","vc2(V)","VIN","CLK","CLKQ","CLKIN","freq (ppm from fin)","VOUT"];

set key noenhanced;
title_line_1 = sprintf("{/:Bold Input and Output Phases versus Time}");
multiplot_title = sprintf("%s\n%s",title_line_1,trans_pll_plot_title);

# Determine optimum range for columns 7 through 9

if ((abs(column_num_7_average) + yrange_number_of_sigma*column_num_7_rms) > (abs(column_num_8_average) + yrange_number_of_sigma*column_num_8_rms)) {
	column_range = abs(column_num_7_average) + yrange_number_of_sigma*column_num_7_rms;
	column_num_max_range = 7;
} else {
	column_range = abs(column_num_8_average) + yrange_number_of_sigma*column_num_8_rms;
	column_num_max_range = 8;
	}
if ((abs(column_num_9_average) + yrange_number_of_sigma*column_num_9_rms) > column_range) {
	column_range = abs(column_num_9_average) + yrange_number_of_sigma*column_num_9_rms;
	column_num_max_range = 9;
} else {
	column_range = column_range;
	column_num_max_range = 8;
	}
	
stats input_filename u 1:column_num_max_range nooutput;
y_mean = STATS_mean_y;
y_min = STATS_min_y;
y_max = STATS_max_y;
y_rms = STATS_stddev_y;

# Determine appropriate units for y-axis

if ((y_max - y_min) != 0.0) {
	if (log10(y_max - y_min) < -9) {
		y_scale_factor = 1e-12;
		y_units_text = sprintf("pUI");
		} else {
			if (log10(y_max - y_min) < -6) {
			y_scale_factor = 1e-9;
			y_units_text = sprintf("nUI");
			} else {
					if (log10(y_max - y_min) < -3) {
					y_scale_factor = 1e-6;
					y_units_text = sprintf("uUI");
					} else {
					if (log10(y_max - y_min) < 0.0) {
						y_scale_factor = 1e-3;
						y_units_text = sprintf("mUI");
						} else {
							y_scale_factor = 1.0;
							y_units_text = sprintf("UI");
							}
						}
					}
				}
} else {
y_scale_factor = 1e-6;
y_units_text = sprintf("uUI");
}
		
set ylabel sprintf("Amplitude (%s)",y_units_text) offset -1,0;
set format y "%.1f";

set multiplot layout 2,1 title multiplot_title;

start_column_num = 7;
end_column_num = 9;
set key on opaque;
xrange_min = x_min/x_scale_factor;
xrange_max = x_max/x_scale_factor;
set xrange [xrange_min:xrange_max];
set xtics auto;

if ((y_max - y_min) != 0.0) {
yrange_min = y_min/y_scale_factor - (1.0 - ((y_min/y_scale_factor) - int(y_min/y_scale_factor)));
yrange_max = y_max/y_scale_factor + (1.0 - ((y_max/y_scale_factor) - int(y_max/y_scale_factor)));
} else {
yrange_min = (y_min - y_mean)/y_scale_factor - 1.0;
yrange_max = (y_max - y_mean)/y_scale_factor + 1.0;
}
set yrange [yrange_min:yrange_max];
set ytics auto;
set yrange [yrange_min:yrange_max];
set ytics auto;

# print sprintf("y_scale_factor = %1.4e\n",y_scale_factor);
# print sprintf("yrange_min = %1.4e, yrange_max = %1.4e\n",yrange_min,yrange_max);

set grid x;
set grid y;
set grid lw 1.5;

if (number_of_records > 10000) {
plot for[i=start_column_num:end_column_num] input_filename u (column(1)/x_scale_factor):((column(i)-averages1[i - 6])/y_scale_factor) every 10 title column_headers[i] w lines lw 1.5;
} else {
plot for[i=start_column_num:end_column_num] input_filename u (column(1)/x_scale_factor):((column(i)-averages1[i - 6])/y_scale_factor) title column_headers[i] w lines lw 1.5;
}

# Plot relative phase

unset xrange;
unset yrange;

# Determine optimum range for column 10

stats input_filename u 1:10 nooutput;
y_mean = STATS_mean_y;
y_min = STATS_min_y;
y_max = STATS_max_y;
y_rms = STATS_stddev_y;

# Determine appropriate units for y-axis

if ((y_max - y_min) != 0.0) {
	if (log10(y_max - y_min) < -9) {
		y_scale_factor = 1e-12;
		y_units_text = sprintf("pUI");
		} else {
			if (log10(y_max - y_min) < -6) {
			y_scale_factor = 1e-9;
			y_units_text = sprintf("nUI");
			} else {
					if (log10(y_max - y_min) < -3) {
					y_scale_factor = 1e-6;
					y_units_text = sprintf("uUI");
					} else {
					if (log10(y_max - y_min) < 0.0) {
						y_scale_factor = 1e-3;
						y_units_text = sprintf("mUI");
						} else {
							y_scale_factor = 1.0;
							y_units_text = sprintf("UI");
							}
						}
					}
				}
} else {
y_scale_factor = 1e-6;
y_units_text = sprintf("uUI");
}

set ylabel sprintf("Amplitude (%s)",y_units_text) offset -1,0;
set format y "%.1f";

set xrange [xrange_min:xrange_max];
set xtics auto;

if ((y_max - y_min) != 0.0) {
yrange_min = y_min/y_scale_factor - (1.0 - ((y_min/y_scale_factor) - int(y_min/y_scale_factor)));
yrange_max = y_max/y_scale_factor + (1.0 - ((y_max/y_scale_factor) - int(y_max/y_scale_factor)));
} else {
yrange_min = (y_min - y_mean)/y_scale_factor - 1.0;
yrange_max = (y_max - y_mean)/y_scale_factor + 1.0;
}
set yrange [yrange_min:yrange_max];
set ytics auto;

# print sprintf("rphase y_scale_factor = %1.4e\n",y_scale_factor);
# print sprintf("rphase yrange_min = %1.4e, rphase yrange_max = %1.4e\n",yrange_min,yrange_max);

set grid x;
set grid y;
set grid lw 1.5;

if (lock_time_sec > 0.0) {
	set print $lock_time_vline;
	print sprintf("%1.12e,%1.12e\n",lock_time_sec/x_scale_factor,yrange_min);
	print sprintf("%1.12e,%1.12e\n",lock_time_sec/x_scale_factor,yrange_max);
	print sprintf("%1.12e,%1.12e\n",lock_time_sec/x_scale_factor,abs(yrange_min) > abs(yrange_max) ? abs(yrange_min): abs(yrange_max));
	print sprintf("%1.12e,%1.12e\n",lock_time_sec/x_scale_factor,abs(yrange_min) > abs(yrange_max) ? -abs(yrange_min): -abs(yrange_max));

	set print;
	
	if ((lock_time_sec/x_scale_factor - xrange_min)/(xrange_max - xrange_min) > 0.50) {
	set arrow 1 from (lock_time_sec/x_scale_factor - 0.10*(xrange_max - xrange_min)),yrange_min + 0.15*(yrange_max - yrange_min) to lock_time_sec/x_scale_factor,yrange_min + 0.20*(yrange_max - yrange_min);
	set label 100 sprintf("Lock time = %s",lock_time_sec_string) at (lock_time_sec/x_scale_factor - 0.05*(xrange_max - xrange_min)),yrange_min + 0.10*(yrange_max - yrange_min) right front boxed bs 2;
	} else {
	set arrow 1 from lock_time_sec/x_scale_factor + 0.10*(xrange_max - xrange_min),yrange_min + 0.15*(yrange_max - yrange_min) to lock_time_sec/x_scale_factor,yrange_min + 0.20*(yrange_max - yrange_min);
	set label 100 sprintf("Lock time = %s",lock_time_sec_string) at (lock_time_sec/x_scale_factor + 0.05*(xrange_max - xrange_min)),yrange_min + 0.10*(yrange_max - yrange_min) left front boxed bs 2;			
	}
}
	
if (lost_lock_time_sec > 0.0) {
	set print $lost_lock_time_vline;
	print sprintf("%1.12e,%1.12e\n",lost_lock_time_sec/x_scale_factor,yrange_min);
	print sprintf("%1.12e,%1.12e\n",lost_lock_time_sec/x_scale_factor,yrange_max);
	print sprintf("%1.12e,%1.12e\n",lost_lock_time_sec/x_scale_factor,abs(yrange_min) > abs(yrange_max) ? abs(yrange_min): abs(yrange_max));
	print sprintf("%1.12e,%1.12e\n",lost_lock_time_sec/x_scale_factor,abs(yrange_min) > abs(yrange_max) ? -abs(yrange_min): -abs(yrange_max));

	set print;
	
	if ((lost_lock_time_sec/x_scale_factor - xrange_min)/(xrange_max - xrange_min) > 0.50) {
	set arrow 2 from (lost_lock_time_sec/x_scale_factor - 0.10*(xrange_max - xrange_min)),yrange_min + 0.75*(yrange_max - yrange_min) to lost_lock_time_sec/x_scale_factor,yrange_min + 0.80*(yrange_max - yrange_min);
	set label 200 sprintf("Lock lost at %.3f %s",lost_lock_time_sec/x_scale_factor,x_units_text) at (lost_lock_time_sec/x_scale_factor - 0.05*(xrange_max - xrange_min)),yrange_min + 0.70*(yrange_max - yrange_min) right front boxed bs 2;
	} else {
	set arrow 2 from lost_lock_time_sec/x_scale_factor + 0.10*(xrange_max - xrange_min),yrange_min + 0.75*(yrange_max - yrange_min) to lost_lock_time_sec/x_scale_factor,yrange_min + 0.80*(yrange_max - yrange_min);
	set label 200 sprintf("Lock lost at %.3f us",lost_lock_time_sec/x_scale_factor) at (lost_lock_time_sec/x_scale_factor + 0.05*(xrange_max - xrange_min)),yrange_min + 0.70*(yrange_max - yrange_min) left front boxed bs 2;			
	}
}
averages1[4] = 0.0;
if (num_lines > 10000) {
	skip_values = ceil(num_lines/max_lines_to_plot);
	if (lock_time_sec > 0.0) {
		if (lost_lock_time_sec > 0.0) {
			plot input_filename u (column(1)/x_scale_factor):((column(10) - averages1[4])/y_scale_factor) every skip_values title "Relative phase" with lines lw 1.5 lc rgb color_array[4], $lock_time_vline using 1:2 title '' w impulse dt 8 linecolor rgb "black", $lost_lock_time_vline using 1:2 title '' w impulse dt 8 linecolor rgb "black";
	} else {
			plot input_filename u (column(1)/x_scale_factor):((column(10) - averages1[4])/y_scale_factor) every skip_values title "Relative phase" with lines lw 1.5 lc rgb color_array[4], $lock_time_vline using 1:2 title '' w impulse dt 8 linecolor rgb "black";	
		}
	} else {
	plot input_filename u (column(1)/x_scale_factor):((column(10) - averages1[4])/y_scale_factor) every skip_values title "Relative phase" with lines lw 1.5 lc rgb color_array[4];
	}
} else {
	if (lock_time_sec > 0.0) {
		if (lost_lock_time_sec > 0.0) {
			plot input_filename u (column(1)/x_scale_factor):((column(10) - averages1[4])/y_scale_factor) title "Relative phase" with lines lw 1.5 lc rgb color_array[4], $lock_time_vline using 1:2 title '' w impulse dt 8 linecolor rgb "black", $lost_lock_time_vline using 1:2 title '' w impulse dt 8 linecolor rgb "black";
		} else {
			plot input_filename u (column(1)/x_scale_factor):((column(10) - averages1[4])/y_scale_factor) title "Relative phase" with lines lw 1.5 lc rgb color_array[4], $lock_time_vline using 1:2 title '' w impulse dt 8 linecolor rgb "black";
		}
	} else {
	plot input_filename u (column(1)/x_scale_factor):((column(10) - averages1[4])/y_scale_factor) title "Relative phase" with lines lw 1.5 lc rgb color_array[4];
	}
}
unset multiplot;

if (plot_enable != 1) {
pause(5);
}



