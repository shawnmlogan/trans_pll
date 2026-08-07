clear;
set datafile separator ",";

input_filename = ARGV[1];
trans_pll_plot_title = ARGV[2];
timestamp = ARGV[3];
lock_time_sec = ARGV[4];
pause_delay = 2.50;
PLOTTING_ROUTINES_DIR = ARGV[5];
max_lines_to_plot = 10000;

set terminal qt enhanced size 800,600 pos 0,0 font "Verdana,14";
set style textbox 2 opaque fc "white" border lc "black" margins 1,1;

# Convert numbers to text strings

num_digits = 2;
suffix = sprintf("s");
awk_comamnd_0 = sprintf("echo \"%1.12e\" > ./.gnu_number",lock_time_sec); 
awk_comamnd_1 = sprintf("awk -f %s/plotting_routines/gnuplot/add_units.nawk num_digits=%d suffix=%s \.\/\.gnu_number > \.\/\.gnu_number_formatted",PLOTTING_ROUTINES_DIR,num_digits,suffix);

system(awk_comamnd_0);
system(awk_comamnd_1);
lock_time_sec_string = system("cat ./.gnu_number_formatted");
system("rm ./.gnu_number ./.gnu_number_formatted");

num_digits = 2;
suffix = sprintf("s");
awk_comamnd_0 = sprintf("echo \"%1.12e\" > ./.gnu_number",lost_lock_time_sec); 
awk_comamnd_1 = sprintf("awk -f %s/plotting_routines/gnuplot/add_units.nawk num_digits=%d suffix=%s \.\/\.gnu_number > \.\/\.gnu_number_formatted",PLOTTING_ROUTINES_DIR,num_digits,suffix);

set term push;					# save current terminal settings

# write the graph in PNG format, pngcairo preferred to png as it supports better fonts and dashed lines

set terminal pngcairo size 1200,900 enhanced font "Verdana,14";
set output sprintf("gnuplot_vcontrol_voltage_vs_time_x_%s.png",timestamp);

set size 0.90,0.90;
set origin 0.05,0.05;
set format x "%.3f"
set format y "%.1f"

title_string = sprintf("{/:Bold Control Voltage versus Time}\n%s",trans_pll_plot_title);
title_offset = -1.5;
set title title_string offset title_offset;

set pointsize 1.5 					# larger point
set xlabel 'Time (us)' offset -1,0;
set ylabel 'Amplitude (mV)' offset -1,0;
set format y "%.1f";
column_num = 6;

unset xrange;
unset yrange;

stats input_filename u 0 nooutput; # Must get STATS_records from first call to stats
num_lines = STATS_records
stats input_filename u 1:(column(column_num)) nooutput; # Must get STATS_records from first call to stats
column_num_records = STATS_records;

stats input_filename u 1:(column(column_num)) every ::((num_lines - column_num_records) + floor(0.90*column_num_records) - 1) nooutput;

# print sprintf("STATS_min_y = %1.12e\n",STATS_min_y);
# print sprintf("STATS_max_y = %1.12e\n",STATS_max_y);
# print sprintf("STATS_min_x = %1.12e\n",STATS_min_x);
# print sprintf("STATS_max_x = %1.12e\n",STATS_max_x);

x_max = STATS_max_x;
x_min = STATS_min_x;
y_max = STATS_max_y;
y_min = STATS_min_y;
y_mean = STATS_mean_y;

# Convert number to text strings

num_digits = 3;
suffix = sprintf("V");
awk_comamnd_0 = sprintf("echo \"%1.12e\" > ./.gnu_number",y_mean); 
awk_comamnd_1 = sprintf("awk -f %s/plotting_routines/gnuplot/add_units.nawk num_digits=%d suffix=%s \.\/\.gnu_number > \.\/\.gnu_number_formatted",PLOTTING_ROUTINES_DIR,num_digits,suffix);

system(awk_comamnd_0);
system(awk_comamnd_1);
y_mean_string = system("cat ./.gnu_number_formatted");
system("rm ./.gnu_number ./.gnu_number_formatted");

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
set xlabel sprintf("Time (%s)",x_units_text) offset -1,0;
set format x "%.2f";

# Determine appropriate units for y-axis

if ((y_max - y_min) != 0.0) {
	if (log10(y_max - y_min) < -9) {
		y_scale_factor = 1e-12;
		y_units_text = sprintf("pV");
		} else {
			if (log10(y_max - y_min) < -6) {
			y_scale_factor = 1e-9;
			y_units_text = sprintf("nV");
			} else {
					if (log10(y_max - y_min) < -3) {
					y_scale_factor = 1e-6;
					y_units_text = sprintf("uV");
					} else {
					if (log10(y_max - y_min) < 0.0) {
						y_scale_factor = 1e-3;
						y_units_text = sprintf("mV");
						} else {
							y_scale_factor = 1.0;
							y_units_text = sprintf("V");
							}
						}
					}
				}
} else {
y_scale_factor = 1e-6;
y_units_text = sprintf("uV");
}

set ylabel sprintf("Amplitude ({/Symbol D}V from %s) (%s)",y_mean_string,y_units_text) offset -1,0;
set format y "%.1f";

#######

if ((y_max - y_min) != 0.0) {
yrange_min = (y_min - y_mean)/y_scale_factor;
yrange_max = (y_max - y_mean)/y_scale_factor;
yrange_min = (y_min - y_mean)/y_scale_factor - int((y_max - y_min)/(4.0*y_scale_factor));
yrange_max = (y_max - y_mean)/y_scale_factor + int((y_max - y_min)/(4.0*y_scale_factor));
} else {
yrange_min = (y_min - y_mean)/y_scale_factor - 1.0;
yrange_max = (y_max - y_mean)/y_scale_factor + 1.0;
}
set yrange [yrange_min:yrange_max];
set ytics auto;

#########

# print sprintf("yrange_min = %1.12e\n",yrange_min);
# print sprintf("yrange_max = %1.12e\n",yrange_max);

########

xrange_min = x_min/x_scale_factor;
xrange_max = x_max/x_scale_factor;
set xrange [xrange_min:xrange_max];
set xtics auto;

#########

# print sprintf("xrange_min = %1.12e %s\n",xrange_min,x_units_text);
# print sprintf("xrange_max = %1.12e %s\n",xrange_max,x_units_text);

#########


if ((lock_time_sec > 0.0) && (lock_time_sec/x_scale_factor > xrange_min)) {
	set print $lock_time_vline;
    do for [n=column_num:column_num] {
        # stats input_filename u ($1/x_scale_factor):(column(column_num)/y_scale_factor) every ::((num_lines - column_num_records) + floor(0.90*column_num_records))  nooutput;
        print sprintf("%1.12e,%1.12e\n", lock_time_sec/x_scale_factor,yrange_min);
        print sprintf("%1.12e,%1.12e", lock_time_sec/x_scale_factor,yrange_max);
    }
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
	set label 200 sprintf("Lock lost at %s",lost_lock_time_sec_string) at (lost_lock_time_sec/x_scale_factor - 0.05*(xrange_max - xrange_min)),yrange_min + 0.70*(yrange_max - yrange_min) right front boxed bs 2;
	} else {
	set arrow 2 from lost_lock_time_sec/x_scale_factor + 0.10*(xrange_max - xrange_min),yrange_min + 0.75*(yrange_max - yrange_min) to lost_lock_time_sec/x_scale_factor,yrange_min + 0.80*(yrange_max - yrange_min);
	set label 200 sprintf("Lock lost at %s",lost_lock_time_sec_string) at (lost_lock_time_sec/x_scale_factor + 0.05*(xrange_max - xrange_min)),yrange_min + 0.70*(yrange_max - yrange_min) left front boxed bs 2;			
	}
}

set grid lw 1.5;
set dashtype 8 (16,16);
label_1 = sprintf("{/Times-Italic=16 Expanded x-axis}");
set label 1 label_1 at graph 0.1,0.90 front boxed bs 2;

if (num_lines > 10000) {
	skip_values = ceil(num_lines/max_lines_to_plot);
	if ((lock_time_sec > 0.0) && (lock_time_sec/x_scale_factor > xrange_min)) {
	plot input_filename u ($1/x_scale_factor):((column(column_num) - y_mean)/y_scale_factor) every skip_values title '' with lines linecolor rgb "red", $lock_time_vline using 1:2 title '' w impulse dt 8 linecolor rgb "black";
	} else {
	plot input_filename u ($1/x_scale_factor):((column(column_num) - y_mean)/y_scale_factor) every skip_values title '' with lines linecolor rgb "red";
	}
} else {
	if ((lock_time_sec > 0.0) && (lock_time_sec/x_scale_factor > xrange_min)) {
	plot input_filename u ($1/x_scale_factor):((column(column_num) - y_mean)/y_scale_factor) title '' with lines linecolor rgb "red", $lock_time_vline using 1:2 title '' w impulse dt 8 linecolor rgb "black";
	} else {
	plot input_filename u ($1/x_scale_factor):((column(column_num) - y_mean)/y_scale_factor) title '' with lines linecolor rgb "red";
	}
}

set term pop;
replot;
pause(pause_delay);
unset label 1;
