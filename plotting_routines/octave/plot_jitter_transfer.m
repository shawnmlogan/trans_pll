#! /usr/local/bin/octave -qf

### Functions required

function plot_jitter_transfer(filename_with_path,plot_title,timestamp)

max_lines = power(2,22);

# Separate path from filename

[file_path base_filename base_extension] = fileparts(filename_with_path);
filename = strcat(base_filename,base_extension);

m = dlmread(filename_with_path,",",[0,0,max_lines,3],"emptyvalue",NA);
x = m(:,1);
y = m(:,2);
z = m(:,3);
l = m(:,4);

# Remove all lines that are non-numeric (i.e. header information)
# Set marker to null value in case all lines are numeric for use in setdiff() command

j = 1;
marker = [];
for i = 1:length(x)
	if (isna(x(i)) || isna(y(i)) || isna(z(i)))
		# printf("Removing x(%d) = %s, y(%d) = %s, and z(%d) = %s in line %d from file %s.\n",i,x(i),i,y(i),i,z(i),i,filename);
		marker(j) = i;
		j++;
	endif
endfor

x_index = 1:length(x);
saved_values = setdiff(x_index,marker);
x = x(saved_values);
y = y(saved_values);
z = z(saved_values); 
l = l(saved_values);

# Find if any cases did not lock

if (sum(isna(l)) > 0)
	phase_lock_flag = 0;
else
	phase_lock_flag = 1;
endif   

fig_num = 0;
pause_delay = 1.50; # Delay time after plot is created to allow viewing

fig_num++;
xval = 15.0;
yval = 10.0;
papersize_vals = [xval yval];
paperposition_vals = [0.10*xval 0.10*yval 0.80*xval 0.80*yval];
x_title_margin = 3.0; # Adds right margin for title in plots
h(fig_num) = figure(fig_num,"papersize",papersize_vals,"paperposition",paperposition_vals);

# Convert to 72 dpi (prinitng default)
dpi = 72;
pos = get (gcf, "position"); # units by default are in ppi
new_papersize = pos(3:4)./dpi;
new_papersize(1) = new_papersize(1) + x_title_margin;
set (gcf, "papersize", new_papersize);
set (gcf, "paperposition", [0, 0, new_papersize]);
font_size = 10;

subplot(6,4,[5,6,9,10,13,14,17,18,21,22]);
	semilogx(x,y,sprintf("b;Gain;"),"linewidth",1.5);
   legend("off");
   hx1 = xlabel("Frequency (Hz)","fontsize",font_size);
   set(hx1,"units","normalized");
   set(hx1,"position",[0.50,-0.1,0]);
   
   hy1 = ylabel("Gain (dB)","fontsize",font_size);
   set(hy1,"units","normalized");
	set(hy1,"position",[-0.12,0.50,0]);

 [xlim_min xlim_max xtickvalues] = find_axis_limits_tickvalues(log10(x),16,0,0,0,0,1);
	xlim_min = power(10,xlim_min);
	xlim_max = power(10,xlim_max);
		
   xlim([xlim_min,xlim_max]);

   y_limit_min = -36;
   y_limit_max = 12;
   
   ylim([y_limit_min,y_limit_max]);
   
   xtick = get (gca, "xtick"); 
   xticklabel = strsplit (sprintf ("%.0e\n", xtick), "\n", true); 
   set (gca, "xticklabel", xtick);
   ytick = get (gca, "ytick"); 
   ytick = -36:3:12;
   set (gca, "ytick",ytick); 
   yticklabel = strsplit (sprintf ("%.0f\n", ytick), "\n", true); 
   set (gca, "yticklabel", yticklabel); 
   grid on;
   grid minor on;
   set (gca, "xgrid", "on");
   if (phase_lock_flag == 0)
   	h_text = text(10*xlim_min,-30,"Warning: one or more analyses\ndid not phase-lock!",'FontSize', font_size);
   endif

   title(sprintf("Jitter Transfer Gain versus Frequency"),"fontsize",font_size - 1);

subplot(6,4,[7,8,11,12,15,16,19,20,23,24]);

	semilogx(x,z,sprintf("g;Phase;"),"linewidth",1.5);
   legend("off");
   hx2 = xlabel("Frequency (Hz)","fontsize",font_size);
   set(hx2,"units","normalized");
   set(hx2,"position",[0.50,-0.10,0]);
   
   hy2 = ylabel("Phase (deg)","fontsize",font_size);
   set(hy2,"units","normalized");
	set(hy2,"position",[-0.12,0.50,0]);

 [xlim_min xlim_max xtickvalues] = find_axis_limits_tickvalues(log10(x),16,0,0,0,0,1);
	xlim_min = power(10,xlim_min);
	xlim_max = power(10,xlim_max);
		
   xlim([xlim_min,xlim_max]);
 
   z_limit_min = -270.0;
   z_limit_max = 180.0;
   
   ylim([z_limit_min,z_limit_max]);
   
   xtick = get (gca, "xtick"); 
   xticklabel = strsplit (sprintf ("%.0e\n", xtick), "\n", true); 
   set (gca, "xticklabel", xtick,"fontsize",font_size);
   ytick = get (gca, "ytick"); 
   ytick = z_limit_min:30:z_limit_max;
   set (gca, "ytick", ytick,"fontsize",font_size);
   yticklabel = strsplit (sprintf ("%.0f\n", ytick), "\n", true); 
   set (gca, "yticklabel", yticklabel,"fontsize",font_size);
   grid on;
   grid minor on;
   set (gca, "xgrid", "on");

   title(sprintf("Jitter Transfer Phase versus Frequency"),"fontsize",font_size - 1);

	subplot(6,4,[1,2,3,4]);

	htext_title = text(0.50,1.0,sprintf("Loop Parameters: %s",plot_title));
	set(htext_title,"horizontalalignment","center","verticalalignment","top");
	set(htext_title,"fontsize",font_size,"fontweight","bold");
	axis off;

plotname = sprintf("jitter_transfer_%s.png",timestamp);
orient("landscape");

print(plotname);
endfunction

function [xlim_min xlim_max xtickvalues] = find_axis_limits_tickvalues(x,max_num_ticks,force_min,force_min_value,force_max,force_max_value,base = 2)

if (force_min == 1)
	x_min = force_min_value;
else
	x_min = min(x);
endif

if (force_max == 1)
	x_max = force_max_value;
else
	x_max = max(x);
endif

xtick_increment = (x_max - x_min)/max_num_ticks;
if (base == 1.0)
	xtick_increment = 1.0;
else
	if ((x_max - x_min) == 0.0)
		xtick_increment = power(base,floor(log10(0.10*x_min)/log10(base)));
	else
		xtick_increment = power(base,floor(log10(xtick_increment)/log10(base)));
	endif
endif

if (force_min == 1)
	xlim_min = x_min;
else
	if ((x_max - x_min) == 0.0)
		xlim_min = 0.90*x_min;
	else
		xlim_min = xtick_increment*(floor(x_min/xtick_increment) - 0.0);
	endif
	loop_counter = 1;
	if (x_min < 0.0)
		while(((xlim_min - x_min)/xtick_increment > -0.50) && (loop_counter < 100))
			xlim_min = xlim_min - xtick_increment;
			loop_counter++;
		endwhile
	else
		if(x_min == 0)
			xlim_min = 0.0;
		else
			while(((xlim_min - x_min)/xtick_increment > 0.50) && (loop_counter < 100))
				xlim_min = xlim_min - xtick_increment;
				loop_counter++;
			endwhile
		endif
	endif
endif

xlim_max = xlim_min;
found_xlim_max_flag = 0;
j = 1;
while ((found_xlim_max_flag != 1) && (j < 100))
	i = 1;
	clear xtickvalues;
	while( (found_xlim_max_flag == 0) && (i < max_num_ticks))
		xtickvalues(i) = xlim_min + j*xtick_increment*(i-1);
		xlim_max = xtickvalues(i);
		if((xlim_max - x_max)/(j*xtick_increment) >= 0.0)
			found_xlim_max_flag = 1;
			xtick_increment = j*xtick_increment;
		else
			found_xlim_max_flag = 0;
		endif
		i++;
	endwhile
j++;
endwhile

if (force_max == 1)
	xlim_max = x_max;
	clear forced_xtickvalues;
	for i = 1:length(xtickvalues)
		if (xtickvalues(i) < x_max)
			forced_xtickvalues(i) = xtickvalues(i);
		endif
	endfor
	forced_xtickvalues = [forced_xtickvalues,x_max];
	clear xtickvalues;
	xtickvalues = forced_xtickvalues;
endif

endfunction


### End of functions

arg_list =  argv();

number_of_arguments_required = [1 2 3];

if (length(arg_list) != number_of_arguments_required)
   printf("Usage: octave pnoise.m <csv_filename_with_log10_phase_noise_data_x_y_data> <title_string> <time_stamp>");
   exit;
else
   filename_with_path = arg_list{1}; 
   if (exist(filename_with_path) != 2)
       printf("File %s does not exist! Terminating\n",filename_with_path);
       exit;
   else
   	[file_path base_filename base_extension] = fileparts(filename_with_path);
		filename = strcat(base_filename,base_extension);
		printf("Read input data filename as %s.\n",filename);
   	plot_title = arg_list{2};
#		printf("Read plot_title as %s.\n",plot_title);
		timestamp = arg_list{3};
   endif
endif

plot_jitter_transfer(filename_with_path,plot_title,timestamp);