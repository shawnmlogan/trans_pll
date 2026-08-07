#! /usr/local/bin/octave -qf

### Functions required

function plot_df_vs_control_voltage(filename_with_path,plot_title,timestamp,vco_nom_freq_Hz,center_voltage,kvco_Hz_V_at_center_voltage,plot_normalized_freq)

max_lines = power(2,8);

# Separate path from filename

[file_path base_filename base_extension] = fileparts(filename_with_path);
filename = strcat(base_filename,base_extension);

fig_num = 0;
pause_delay = 1.50; # Delay vc after plot is created to allow viewing

m = dlmread(filename_with_path,",",[0,0,max_lines,4],"emptyvalue",NA);
vc = m(:,1);
freq = m(:,2);
kvco_Hz_per_V = m(:,3);
delta_f = m(:,4);
kvco_delta_f = m(:,5);

# Remove all lines that are non-numeric (i.e. header information)
# Set marker to null value in case all lines are numeric for use in setdiff() command

j = 1;
marker = [];
for i = 1:length(vc)
	if (isna(vc(i)) || isna(freq(i)))
		marker(j) = i;
		j++;
	endif
endfor

vc_index = 1:length(vc);
saved_values = setdiff(vc_index,marker);

vc = vc(saved_values);
freq = freq(saved_values);
kvco_Hz_per_V = kvco_Hz_per_V(saved_values);
delta_f = delta_f(saved_values);
kvco_delta_f = kvco_delta_f(saved_values);

fig_num++;
xval = 16.0;
yval = 10.0;
papersize_vals = [xval yval];
paperposition_vals = [0.10*xval 0.10*yval 0.95*xval 0.90*yval];
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

h_subplot_1 = subplot(6,4,[5,6,9,10,13,14,17,18,21,22]);
h_subplot_1_axes = axes(h_subplot_1);
# Plot either frequency versus control voltage or delta_frequency versus control voltage

	if (plot_normalized_freq != 1)	
		plot(vc,freq,sprintf("b;Output frequency in Hz;"),"linewidth",1.5);
		legend("hide");
		
		title(sprintf("Frequency versus Control Voltage"),"fontsize",font_size);
		
		[xlim_min xlim_max xtickvalues] = find_axis_limits_tickvalues(vc,10,0,0,0,0);
		xlim([xlim_min,xlim_max]);
		set(gca,'xtick',xtickvalues);
		
		[ylim_min ylim_max ytickvalues] = find_axis_limits_tickvalues(freq,12,0,0,0,0,10);
		ylim([ylim_min,ylim_max]);
		set(gca,'ytick',ytickvalues);
		
		hx1 = xlabel("Control Voltage (V)","fontsize",font_size);
	   set(hx1,"units","normalized");
	   set(hx1,"position",[0.50,-0.1,0]);
	   
	   hy1 = ylabel("Frequency (Hz)","fontsize",font_size);
	   set(hy1,"units","normalized");
		set(hy1,"position",[-0.25,0.50,0]);
	
	   xticklabel = strsplit(sprintf ("%.1f\n", xtickvalues), "\n", true);
	   set (gca, "xticklabel", xticklabel,"fontsize",font_size);
	   yticklabel = strsplit(sprintf ("%.4e\n", ytickvalues), "\n", true);
	   set (gca, "yticklabel", yticklabel,"fontsize",font_size);
	   
		grid minor on;
		set (gca, "xgrid", "on");
		set (gca, "ygrid", "on");
			
		if (!isnan(center_voltage))
			[hline, h_textarrow] = add_arrow(h_subplot_1_axes,center_voltage,center_voltage,"V","Center voltage",font_size,xlim_min,xlim_max,vco_nom_freq_Hz,ylim_min,ylim_max);
		endif
		
	else
		plot(vc,delta_f,sprintf("b;frequency (%% from %s);",add_units(vco_nom_freq_Hz,4,"Hz")),"linewidth",1.5);
		legend("hide");

		title(sprintf("Frequency Difference from %s versus Control Voltage",add_units(vco_nom_freq_Hz,4,"Hz")),"fontsize",font_size - 1);
		
		[xlim_min xlim_max xtickvalues] = find_axis_limits_tickvalues(vc,10,0,0,0,0);
		xlim([xlim_min,xlim_max]);
		set(gca,'xtick',xtickvalues);
		
		[ylim_min ylim_max ytickvalues] = find_axis_limits_tickvalues(delta_f,12,0,0,0,0,10);
		ylim([ylim_min,ylim_max]);
		set(gca,'ytick',ytickvalues);
		
		hx1 = xlabel("Control Voltage (V)","fontsize",font_size);
	   set(hx1,"units","normalized");
	   set(hx1,"position",[0.50,-0.1,0]);
	   
	   hy1 = ylabel('\Deltaf (%)',"fontsize",font_size);
	   set(hy1,"units","normalized");
		set(hy1,"position",[-0.14,0.50,0]);
	
	   xticklabel = strsplit(sprintf ("%.1f\n", xtickvalues), "\n", true);
	   set(gca, "xticklabel", xticklabel,"fontsize",font_size);
	   yticklabel = strsplit(sprintf ("%.1f\n", ytickvalues), "\n", true); 
	   set(gca, "yticklabel", yticklabel,"fontsize",font_size);
	   
		grid minor on;
		set (gca, "xgrid", "on");
		set (gca, "ygrid", "on");
		
		if (!isnan(center_voltage))
			[hline, h_textarrow] = add_arrow(h_subplot_1_axes,center_voltage,center_voltage,"V","Center voltage",font_size,xlim_min,xlim_max,0.0,ylim_min,ylim_max);
		endif
		
	endif

h_subplot_2 = subplot(6,4,[7,8,11,12,15,16,19,20,23,24]);
	h_subplot_2_axes = axes(h_subplot_2);
	axes_position = get(h_subplot_2_axes,"position");
	# Move plot on right further to right to make room for y-axis units and label
	axes_position(1) = axes_position(1) + 0.05;
	set(h_subplot_2_axes,"position",axes_position);

# Plot either Kvco versus control voltage or delta_kvco versus control voltage

if (plot_normalized_freq != 1)	
	plot(vc(2:end),kvco_Hz_per_V(2:end),sprintf("r;Kvco in Hz/V;"),"linewidth",1.5);
	legend("hide");
	title(sprintf("Kvco versus Control Voltage"),"fontsize",font_size);
	
	[xlim_min xlim_max xtickvalues] = find_axis_limits_tickvalues(vc,10,0,0,0,0);
	xlim([xlim_min,xlim_max]);
	set(gca,'xtick',xtickvalues);
	
	[ylim_min ylim_max ytickvalues] = find_axis_limits_tickvalues(kvco_Hz_per_V(2:end),10,1,0,0,0,10);

	ylim([ylim_min,ylim_max]);
	set(gca,'ytick',ytickvalues);
	
	hx1 = xlabel("Control Voltage (V)","fontsize",font_size);
   set(hx1,"units","normalized");
   set(hx1,"position",[0.50,-0.1,0]);
   
   hy1 = ylabel("Kvco (Hz/V)","fontsize",font_size);
   set(hy1,"units","normalized");
	set(hy1,"position",[-0.25,0.50,0]);

   xticklabel = strsplit(sprintf ("%.1f\n", xtickvalues), "\n", true);
   set (gca, "xticklabel", xticklabel,"fontsize",font_size);
   yticklabel = strsplit(sprintf ("%.2e\n", ytickvalues), "\n", true);
   set (gca, "yticklabel", yticklabel,"fontsize",font_size);
   
	grid minor on;
	set (gca, "xgrid", "on");
	set (gca, "ygrid", "on");
	
	if (!isnan(center_voltage))
		[hline, h_textarrow] = add_arrow(h_subplot_2_axes,center_voltage,center_voltage,"V","Center voltage",font_size,xlim_min,xlim_max,kvco_Hz_V_at_center_voltage,ylim_min,ylim_max);
	endif
	
else
	plot(vc(2:end),kvco_delta_f(2:end),sprintf("r;Kvco (%%/V);"),"linewidth",1.5);
	legend("hide");

	title(sprintf("Kvco versus Control Voltage (Normalized to %s)",add_units(vco_nom_freq_Hz,4,"Hz")),"fontsize",font_size - 1);
	
	[xlim_min xlim_max xtickvalues] = find_axis_limits_tickvalues(vc,10,0,0,0,0);
	xlim([xlim_min,xlim_max]);
	set(gca,'xtick',xtickvalues);
	
	[ylim_min ylim_max ytickvalues] = find_axis_limits_tickvalues(kvco_delta_f(2:end),10,0,0,0,0,10);
	ylim([ylim_min,ylim_max]);
	set(gca,'ytick',ytickvalues);
	
	hx1 = xlabel("Control Voltage (V)","fontsize",font_size);
   set(hx1,"units","normalized");
   set(hx1,"position",[0.50,-0.1,0]);
   
   hy1 = ylabel('Kvco (%/V)',"fontsize",font_size);
   set(hy1,"units","normalized");
	set(hy1,"position",[-0.15,0.50,0]);

   xticklabel = strsplit(sprintf ("%.1f\n", xtickvalues), "\n", true);
   set(gca, "xticklabel", xticklabel,"fontsize",font_size);
   yticklabel = strsplit(sprintf ("%.1f\n", ytickvalues), "\n", true); 
   set(gca, "yticklabel", yticklabel,"fontsize",font_size);
   
	grid minor on;
	set (gca, "xgrid", "on");
	set (gca, "ygrid", "on");
	
	if (!isnan(center_voltage))
		[hline, h_textarrow] = add_arrow(h_subplot_2_axes,center_voltage,center_voltage,"V","Center voltage",font_size,xlim_min,xlim_max,100.0*kvco_Hz_V_at_center_voltage/vco_nom_freq_Hz,ylim_min,ylim_max);
	endif
	
endif

subplot(6,4,[1,2,3,4]);

	htext_title = text(0.50,1.0,sprintf("%s",plot_title));
	set(htext_title,"horizontalalignment","center","verticalalignment","top");
	set(htext_title,"fontsize",font_size,"fontweight","bold");
	axis off;

plotname = sprintf("vco_control_voltage_characteristic_%s.png",timestamp);
orient("landscape");
print(plotname);

endfunction

function value_label = add_units(value,num_digits,suffix)

if (abs(value) < 1e-9)
   value_label = sprintf("%.*f p%s",num_digits,value/1e-12,suffix);
else
   if (abs(value) < 1e-6)
      value_label = sprintf("%.*f n%s",num_digits,value/1e-09,suffix);
   else
      if (abs(value) < 1e-3)
         value_label = sprintf("%.*f u%s",num_digits,value/1e-06,suffix);
      else
         if (abs(value) < 1.0)
            value_label = sprintf("%.*f m%s",num_digits,value/1e-03,suffix);
         else
            if (abs(value) < 1e3)
               value_label = sprintf("%.*f %s",num_digits,value,suffix);
            else  
               if (abs(value) < 1e6)
                  value_label = sprintf("%.*f k%s",num_digits,value/1e3,suffix);
               else
                  if (abs(value) < 1e9)
                      value_label = sprintf("%.*f M%s",num_digits,value/1e6,suffix);
                  else
                      value_label = sprintf("%.*f G%s",num_digits,value/1e9,suffix);
                  endif
               endif
            endif
         endif
      endif
   endif
endif

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
		xtick_increment = power(base,floor(log10(0.10*abs(x_min))/log10(base)));
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

function [hline, h_textarrow] = add_arrow(haxes,line_x_position,x_position,x_position_units,x_position_text,font_size,xlim_min,xlim_max,y_position,ylim_min,ylim_max)

# Note: If using add_legend.m to create a custom legend location,
# the following line is not required and will enable
# the octave legend. 

if ((length(findall(gcf,"tag","custom_legend")) == 0) && (length(findall(gcf,"tag","legend")) == 1))
	legend ("autoupdate", "off");
endif

hold on;

hline = line ("xdata",[line_x_position,line_x_position], "ydata",[ylim_min,ylim_max], "linewidth",1.5, "linestyle",":","DisplayName",add_units(x_position,2,x_position_units),"color","k");

axes_position = get(haxes,"position");

left_x_axis = axes_position(1);
right_x_axis = axes_position(1) + axes_position(3);
	
x_position_norm = (line_x_position - xlim_min)/(xlim_max - xlim_min);
x_position_norm = left_x_axis + x_position_norm*(right_x_axis - left_x_axis);

if ((x_position_norm - left_x_axis) > 0.50)
	x_position_norm_text_position = x_position_norm - 0.07;
else
	x_position_norm_text_position = x_position_norm + 0.07;
endif

bottom_y_axis = axes_position(2);
top_y_axis = axes_position(2) + axes_position(4);
yval_x_position_norm = (y_position - ylim_min)/(ylim_max - ylim_min);
yval_x_position_norm = bottom_y_axis + yval_x_position_norm*(top_y_axis - bottom_y_axis);


h_textarrow = annotation("textarrow",[x_position_norm_text_position x_position_norm ],[(yval_x_position_norm - 0.05)  (yval_x_position_norm)], "string",sprintf("%s\n %s",x_position_text,add_units(x_position,1,"V")),"fontsize",font_size,"textcolor","black","textbackgroundcolor","white","textedgecolor","white","horizontalalignment","center","headlength",8,"headwidth",4,"headstyle","plain","linewidth",1.0);

endfunction

### End of functions

arg_list =  argv();

pkg load signal;

number_of_arguments_required = [7];

if (length(arg_list) != number_of_arguments_required)
   printf("Usage: plot_df_vs_control_voltage <csv_filename_from_trans_pll> <title_string> <timestamp> ,<vco_nom_freq_Hz> <center_voltage> <kvco_Hz_V_at_center_voltage> <plot_normalized_freq>");
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
		# logfile_name = sprintf("%s_%s.log",base_filename,timestamp);
		# diary off;
		# diary(logfile_name);
		vco_nom_freq_Hz = str2double(arg_list{4});
		center_voltage = str2double(arg_list{5});
		kvco_Hz_V_at_center_voltage = str2double(arg_list{6});
		plot_normalized_freq = str2num(arg_list{7});
   endif

endif

plot_df_vs_control_voltage(filename_with_path,plot_title,timestamp,vco_nom_freq_Hz,center_voltage,kvco_Hz_V_at_center_voltage,plot_normalized_freq);

