#! /usr/local/bin/octave -qf

### Functions required

function plot_transient_data(filename_with_path,plot_title,timestamp,lock_time)

max_lines = power(2,24);

# Separate path from filename

[file_path base_filename base_extension] = fileparts(filename_with_path);
filename = strcat(base_filename,base_extension);

fig_num = 0;
# date = strrep(datestr(now(),2),"/","");
pause_delay = 1.50; # Delay time after plot is created to allow viewing

m = dlmread(filename_with_path,",",[0,0,max_lines,18],"emptyvalue",NA);
time = m(:,1);
vc = m(:,6);
phasein_ui = m(:,7);
phaseo_ui = m(:,8);
phaseo_delayed_ui = m(:,9);
freq_ppm = m(:,end);

# Remove all lines that are non-numeric (i.e. header information)
# Set marker to null value in case all lines are numeric for use in setdiff() command

j = 1;
marker = [];
for i = 1:length(time)
	if (isna(time(i)) || isna(vc(i)))
		# printf("Removing time(%d) = %s and vc(%d) = %s in line %d from file %s.\n",i,time(i),i,vc(i),i,filename);
		marker(j) = i;
		j++;
	endif
endfor

time_index = 1:length(time);
saved_values = setdiff(time_index,marker);
time = time(saved_values);
vc = vc(saved_values);
phasein_ui = phasein_ui(saved_values);
phaseo_ui = phaseo_ui(saved_values);
phaseo_delayed_ui = phaseo_delayed_ui(saved_values);
freq_ppm = freq_ppm(saved_values);

fig_num++;
font_size = 12;
xval = 16.0;
yval = 10.0;
papersize_vals = [xval yval];
paperposition_vals = [0.10*xval 0.10*yval 0.80*xval 0.80*yval];
x_title_margin = 0.5; # Adds right margin for title in plots

h(fig_num) = figure(fig_num,"papersize",papersize_vals,"paperposition",paperposition_vals);

# Convert to 72 dpi (prinitng default)
dpi = 72;
pos = get (gcf, "position"); # units by default are in ppi
new_papersize = pos(3:4)./dpi;
new_papersize(1) = new_papersize(1) + x_title_margin;
set (gcf, "papersize", new_papersize);
set (gcf, "paperposition", [0, 0, new_papersize]);
hvc = axes();

plot(time/1e-06,vc,sprintf("r;vc;"),"linewidth",1.5);
title(sprintf("Control Voltage versus Time\n%s",plot_title));
legend("off");

[xlim_min xlim_max xtickvalues] = find_axis_limits_tickvalues(time/1e-6,16,0,0,0,0);
xlim([xlim_min,xlim_max]);
set(gca,'xtick',xtickvalues);

[ylim_min ylim_max ytickvalues] = find_axis_limits_tickvalues(vc,16,1,0,1,1.0);
ylim([ylim_min,ylim_max]);
set(gca,'ytick',ytickvalues);

xlabel("Time (us)");
ylabel("Amplitude (V)");
grid on;

if (!isnan(lock_time) && (lock_time > 0.0))
	[hline, h_textarrow] = add_arrow(hvc,lock_time/1e-06,lock_time,"s",font_size,xlim_min,xlim_max,0.80,ylim_min,ylim_max);
endif

print(sprintf("vcontrol_voltage_vs_time_%s.png",timestamp));

if (!isnan(lock_time) && (lock_time > 0.0))
	set( hline, "visible" ,"off")
	set(h_textarrow,"visible","off")
endif

title(sprintf("Control Voltage versus Time (Expanded x-axis)\n%s",plot_title));

vc_clipped = vc(floor(0.90*length(time)):end);
time_clipped = time(floor(0.90*length(time)):end);

[xlim_min xlim_max xtickvalues] = find_axis_limits_tickvalues(time_clipped/1e-6,10,0,0.0,0,0);

xlim([xlim_min,xlim_max]);
set(gca,'xtick',xtickvalues);
xticklabel = strsplit (sprintf ("%.3f\n", xtickvalues), "\n", true); 
set (gca, "xticklabel", xticklabel);

[ylim_min ylim_max ytickvalues] = find_axis_limits_tickvalues(vc_clipped/1e-03,10,0,0,0,0);
ylim([ylim_min,ylim_max]);
set(gca,'ytick',ytickvalues);
yticklabel = strsplit (sprintf ("%.1f\n", ytickvalues), "\n", true); 
set (gca, "yticklabel", yticklabel); 	

plot(time/1e-06,vc/1e-03,sprintf("r;;"),"linewidth",1.5);
xlabel("Time (us)");
ylabel("Amplitude (mV)");

print(sprintf("vcontrol_voltage_vs_time_x_%s.png",timestamp));

fig_num++;
font_size = 12;
h(fig_num) = figure(fig_num,"papersize",papersize_vals,"paperposition",paperposition_vals);dpi = 72;

pos = get (gcf, "position"); # units by default are in ppi
new_papersize = pos(3:4)./dpi;
new_papersize(1) = new_papersize(1) + x_title_margin;
set (gcf, "papersize", new_papersize);
set (gcf, "paperposition", [0, 0, new_papersize]);
hphase = axes();

plot(time/1e-06,phasein_ui,sprintf("g;Input phase;"),"linewidth",1.5)
hold on;
plot(time/1e-06,phaseo_ui,sprintf("b;Output phase;"),"linewidth",1.5)
plot(time/1e-06,phaseo_delayed_ui,sprintf("r;Output phase delayed;"),"linewidth",1.5)
hold off;

title(sprintf("Input and Output Phase versus Time\n%s",plot_title));

[xlim_min xlim_max xtickvalues] = find_axis_limits_tickvalues(time/1e-6,16,0,0,0,0);
xlim([xlim_min,xlim_max]);
set(gca,'xtick',xtickvalues);
xticklabel = strsplit (sprintf ("%.2f\n", xtickvalues), "\n", true); 
set (gca, "xticklabel", xticklabel);


[ylim_min ylim_max ytickvalues] = find_axis_limits_tickvalues([min([min(phasein_ui),min(phaseo_ui),min(phaseo_delayed_ui)]),max([max(phasein_ui),max(phaseo_ui),max(phaseo_delayed_ui)])],16,0,0,0,0);
ylim([ylim_min,ylim_max]);
set(gca,'ytick',ytickvalues);
yticklabel = strsplit (sprintf ("%.4f\n", ytickvalues), "\n", true); 
set (gca, "yticklabel", yticklabel);

x_pos = xlim_min + 0.1*(xlim_max - xlim_min);
y_pos = ylim_min + 0.2*(ylim_max - ylim_min);

[h_legend, box_string, box_position] = add_legend(hphase,x_pos,xlim_min,xlim_max,0,y_pos,ylim_min,ylim_max,0,font_size,3);

if (!isnan(lock_time) && (lock_time > 0.0))

	[hline, h_textarrow] = add_arrow(hphase,lock_time/1e-06,lock_time,"s",font_size,xlim_min,xlim_max,(ylim_min + ylim_max)/2.0,ylim_min,ylim_max);
endif

xlabel("Time (us)");
ylabel("Phase (UI)");
grid on;
pause(pause_delay);
print(sprintf("phasein_phaseo_vs_time_%s.png",timestamp));
if (!isnan(lock_time) && (lock_time > 0.0))
	set(hline,"visible","off");
	set(h_textarrow,"visible","off");
endif

fig_num++;
font_size = 12;
h(fig_num) = figure(fig_num,"papersize",papersize_vals,"paperposition",paperposition_vals);

pos = get (gcf, "position"); # units by default are in ppi
new_papersize = pos(3:4)./dpi;
new_papersize(1) = new_papersize(1) + x_title_margin;
set (gcf, "papersize", new_papersize);
set (gcf, "paperposition", [0, 0, new_papersize]);
hrphase = axes();

plot(time/1e-06,phaseo_ui - phasein_ui,sprintf(";Relative phase;"),"linewidth",1.5,"color",[153/256 51/256 255/256])

title(sprintf("Phase Difference versus Time\n%s",plot_title));
legend("off");

[xlim_min xlim_max xtickvalues] = find_axis_limits_tickvalues(time/1e-6,16,0,0,0,0);
xlim([xlim_min,xlim_max]);
set(gca,'xtick',xtickvalues);
xticklabel = strsplit (sprintf ("%.2f\n", xtickvalues), "\n", true); 
set (gca, "xticklabel", xticklabel);

[ylim_min ylim_max ytickvalues] = find_axis_limits_tickvalues(phaseo_ui - phasein_ui,16,0,0,0,0);
ylim([ylim_min,ylim_max]);
set(gca,'ytick',ytickvalues);
yticklabel = strsplit (sprintf ("%.4f\n", ytickvalues), "\n", true); 
set (gca, "yticklabel", yticklabel);

x_pos = xlim_min + 0.1*(xlim_max - xlim_min);
y_pos = ylim_min + 0.4*(ylim_max - ylim_min);

# [h_legend, box_string, box_position] = add_legend(hphase,x_pos,xlim_min,xlim_max,0,y_pos,ylim_min,ylim_max,0,font_size,1);
	
xlabel("Time (us)");
ylabel("Phase (UI)");
grid on;

if (!isnan(lock_time) && (lock_time > 0.0))
	[hline, h_textarrow] = add_arrow(hrphase,lock_time/1e-06,lock_time,"s",font_size,xlim_min,xlim_max,(ylim_min + ylim_max)/2.0,ylim_min,ylim_max);
endif

pause(pause_delay);
print(sprintf("rphaseo_vs_time_%s.png",timestamp));
if (!isnan(lock_time) && (lock_time > 0.0))
	set(hline,"visible","off");
	set(h_textarrow,"visible","off");
endif

# Compute PSD of input and output phase using last points of data for non-zero phase_in and phase out responses

fft_power = floor(log10(length(time))/log10(2));
fft_length = floor(power(2,fft_power));

phasein_ui_clipped = phasein_ui(end - fft_length + 1:end);
compute_phasein_ui_clipped_psd_flag = 1;
if ((any(phasein_ui_clipped) != 1) && (all(phasein_ui_clipped) != 1))
	compute_phasein_ui_clipped_psd_flag = 0;
	# printf("Identified phasein_ui_clipped as all zeroes, no PSD analysis is performed for phasein_ui_clipped.\n");
endif

phaseo_delayed_ui_clipped = phaseo_delayed_ui(end - fft_length + 1:end);
compute_phaseo_delayed_ui_clipped_psd_flag = 1;
if ((any(phaseo_delayed_ui_clipped) != 1) && (all(phaseo_delayed_ui_clipped) != 1))
	compute_phaseo_delayed_ui_clipped_psd_flag = 0;
	# printf("Identified phaseo_delayed_ui_clipped as all zeroes, no PSD analysis is performed for phaseo_delayed_ui_clipped.\n");
endif

window_divider = 4.0;
nom_freq = 1.0/mean(diff(time));
window = length(phasein_ui_clipped)/window_divider;
overlap = 0.0;
nfft = window;
fs = nom_freq;
conf = 0.95;
range ={"onesided"};
plot_type = {"semilogx" "loglog"};
detrend = {"none"};

# [spectra,freq] = pwelch(x,y,window,overlap,Nfft,Fs,"onesided","loglog","none",sloppy,results)

if (compute_phasein_ui_clipped_psd_flag == 1)
	fig_num++;
	font_size = 12;
	h(fig_num) = figure(fig_num,"papersize",papersize_vals,"paperposition",paperposition_vals);
	hpsd = axes();
	pos = get (gcf, "position"); # units by default are in ppi
	new_papersize = pos(3:4)./dpi;
	new_papersize(1) = new_papersize(1) + x_title_margin;
	set (gcf, "papersize", new_papersize);
	set (gcf, "paperposition", [0, 0, new_papersize]);
	
	
	[spectra,freq] = pwelch(phasein_ui_clipped,window,overlap,nfft,nom_freq,"onesided","loglog","none","sloppy","power");
	loglog(freq(2:end),spectra(2:end),sprintf(";phasein\\_ui;"),"linewidth",1.0);

	legend ("off"),

	if (compute_phaseo_delayed_ui_clipped_psd_flag == 1)
		hold on;
		[spectra_o,freq_o] = pwelch(phaseo_delayed_ui_clipped,window,overlap,nfft,nom_freq,"onesided","loglog","none","sloppy","power");
		loglog(freq_o(2:end),spectra_o(2:end),sprintf(";phaseo\\_delayed\\_ui;"),"linewidth",1.0);
		hold off;
		
		xlabel("Frequency (Hz)");
		ylabel("PSD (UI^2/Hz)");
		[xlim_min xlim_max xtickvalues] = find_axis_limits_tickvalues(log10(freq(2:end)),16,0,0,0,0,1);
		xlim_min = power(10,xlim_min);
		xlim_max = power(10,xlim_max);
		xlim([xlim_min,xlim_max]);
		set(gca,'xtick',power(10,xtickvalues))
		xtick = get (gca, "xtick"); 
		xticklabel = strsplit (sprintf ("%.1e\n", xtick), "\n", true); 
		set (gca, "xticklabel", xtick);
		
		spectra_limit_min = 10.0^-29;
		spectra_limit_max = 10.0^-9;
		if (min([min(spectra_o),min(spectra)]) != 0.0)
			if (max([max(spectra_o),max(spectra)]) != 0.0)
				combined_spectre_limits = [min([min(spectra_o),min(spectra)]),max([max(spectra_o),max(spectra)])];
			else
				combined_spectre_limits = [min([min(spectra_o),min(spectra)]),spectra_limit_max];
			endif
		else
			if (max([max(spectra_o),max(spectra)]) != 0.0)
				combined_spectre_limits = [spectra_limit_min,max([max(spectra_o),max(spectra)])];
			else
				combined_spectre_limits = [spectra_limit_min,spectra_limit_max];	
			endif
		endif
		[ylim_min ylim_max ytickvalues] = find_axis_limits_tickvalues(log10(combined_spectre_limits),25,0,0,0,0,1);
		ylim_min = power(10,ylim_min);
		ylim_max = power(10,ylim_max);
		ylim([ylim_min,ylim_max]);
		set(gca,'ytick',power(10,ytickvalues))
		ytick = get (gca, "ytick"); 
		yticklabel = strsplit (sprintf ("%.1e\n", ytick), "\n", true); 
		set (gca, "yticklabel", yticklabel); 
		grid on;
		grid minor on;

		title(sprintf("PSD of %s Sampled phasein and phaseo\nData length = %d, Number of segments = %.0f, overlap = %.1f\n%s",add_units(nom_freq,3,"Hz"),length(phasein_ui_clipped),window_divider,overlap,plot_title));

		x_pos = xlim_max/100.0;
		y_pos = ylim_max/40;
		if (compute_phasein_ui_clipped_psd_flag != 0)
			[h_legend, box_string, box_position] = add_legend(hpsd,x_pos,xlim_min,xlim_max,1,y_pos,ylim_min,ylim_max,1,font_size,2);
		else
			[h_legend, box_string, box_position] = add_legend(hpsd,x_pos,xlim_min,xlim_max,1,y_pos,ylim_min,ylim_max,1,font_size,1);
		endif
		
		plotname = sprintf("psd_osc_tie_vs_offset_window_divider_%.0f_overlap_%.0f_%s.png",window_divider,100*overlap,timestamp);		
		print(plotname);
		pause(pause_delay);
	endif	
else
	if (compute_phaseo_delayed_ui_clipped_psd_flag == 1)
		fig_num++;
		h(fig_num) = figure(fig_num,"papersize",papersize_vals,"paperposition",paperposition_vals);
		hpsd = axes();
		pos = get (gcf, "position"); # units by default are in ppi
		new_papersize = pos(3:4)./dpi;
		new_papersize(1) = new_papersize(1) + x_title_margin;
		set (gcf, "papersize", new_papersize);
		set (gcf, "paperposition", [0, 0, new_papersize]);
		
		
		[spectra_o,freq_o] = pwelch(phaseo_delayed_ui_clipped,window,overlap,nfft,nom_freq,"onesided","loglog","none","sloppy","power");
		loglog(freq_o(2:end),spectra_o(2:end),sprintf(";phaseo\\_delayed\\_ui;"),"linewidth",1.0);
		
		legend("off");
		xlabel("Frequency (Hz)");
		ylabel("PSD (UI^2/Hz)");
		[xlim_min xlim_max xtickvalues] = find_axis_limits_tickvalues(log10(freq_o(2:end)),16,0,0,0,0,1);
		xlim_min = power(10,xlim_min);
		xlim_max = power(10,xlim_max);
		xlim([xlim_min,xlim_max]);
		set(gca,'xtick',power(10,xtickvalues));
		xtick = get (gca, "xtick"); 
		xticklabel = strsplit (sprintf ("%.1e\n", xtick), "\n", true); 
		set (gca, "xticklabel", xtick);

		spectra_limit_min = 10.0^-29;
		spectra_limit_max = 10.0^-9;
		if (min(spectra_o) != 0.0)
			if (max(spectra_o) != 0.0)
				combined_spectre_limits = [min(spectra_o),max(spectra_o)];
			else
				combined_spectre_limits = [min(spectra_o),spectra_limit_max];
			endif	
		else
			if ( max(spectra_o) != 0.0)
				combined_spectre_limits = [spectra_limit_min,max(spectra_o)];
			else
				combined_spectre_limits = [spectra_limit_min,spectra_limit_max];	
			endif
		endif
		[ylim_min ylim_max ytickvalues] = find_axis_limits_tickvalues(log10(combined_spectre_limits),25,0,0,0,0,1);
		ylim_min = power(10,ylim_min);
		ylim_max = power(10,ylim_max);
		ylim([ylim_min,ylim_max]);

		set(gca,'ytick',power(10,ytickvalues));
		ytick = get (gca, "ytick"); 
		yticklabel = strsplit (sprintf ("%.1e\n", ytick), "\n", true); 
		set (gca, "yticklabel", yticklabel); 		
		grid on;
		grid minor on;
		title(sprintf("PSD of %s Sampled phaseo\nData length = %d, Number of segments = %.0f, overlap = %.1f\n%s",add_units(nom_freq,3,"Hz"),length(phasein_ui_clipped),window_divider,overlap,plot_title));

		x_pos = xlim_max/100.0;
		y_pos = ylim_max/40.0;

		if (compute_phasein_ui_clipped_psd_flag != 0)
			[h_legend, box_string, box_position] = add_legend(hpsd,x_pos,xlim_min,xlim_max,1,y_pos,ylim_min,ylim_max,1,font_size,2);
		else
			[h_legend, box_string, box_position] = add_legend(hpsd,x_pos,xlim_min,xlim_max,1,y_pos,ylim_min,ylim_max,1,font_size,1);
		endif
		
		plotname = sprintf("psd_osc_tie_vs_offset_window_divider_%.0f_overlap_%.0f_%s.png",window_divider,100*overlap,timestamp);
		print(plotname);
		pause(pause_delay);
	endif
endif

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

function [hline, h_textarrow] = add_arrow(haxes,line_x_position,x_position,x_position_units,font_size,xlim_min,xlim_max,y_position,ylim_min,ylim_max)

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

if (x_position_norm > 0.50)
	x_position_norm_text_position = x_position_norm - 0.07;
else
	x_position_norm_text_position = x_position_norm + 0.07;
endif

bottom_y_axis = axes_position(2);
top_y_axis = axes_position(2) + axes_position(4);
yval_x_position_norm = (y_position - ylim_min)/(ylim_max - ylim_min);
yval_x_position_norm = bottom_y_axis + yval_x_position_norm*(top_y_axis - bottom_y_axis);


h_textarrow = annotation("textarrow",[x_position_norm_text_position x_position_norm ],[(yval_x_position_norm + 0.05)  (yval_x_position_norm)], "string",sprintf("Lock time = %s",add_units(x_position,3,"s")),"fontsize",font_size,"textcolor","black","textbackgroundcolor","white","textedgecolor","white","horizontalalignment","center","headlength",8,"headwidth",4,"headstyle","plain","linewidth",1.0);

endfunction

function [h_legend, box_string, box_position] = add_legend(h_axes,legend_x_position,xlim_min,xlim_max,x_axis_log,legend_y_position,ylim_min,ylim_max,y_axis_log,font_size,num_curves_to_include)

ha = {"left", "center", "right"};
va = {"bottom", "middle", "top"};

error_flag = 0;
legend (h_axes,"off");

h_curves = findall(gcf,'type','line');
axes_position = get(h_axes,"position");

if (num_curves_to_include > length(h_curves))
	printf("Specified too many curves to include in legend in add_legend()!\n");
	printf("Specify less than or equal to %d curves.\n",length(h_curves));
	error_flag = 1;
else
	for i = length(h_curves):-1:(length(h_curves) - num_curves_to_include + 1)
		line_color = get(h_curves(i),"color");
		line_color_string = sprintf("%.4f %.4f %.4f",line_color(1),line_color(2),line_color(3));
		if (i == length(h_curves))
			if (num_curves_to_include == 1)
				box_string = sprintf("\n\\color[rgb]{%s}{\\bf –– }\\color[rgb]{0 0 0}%s \n",line_color_string,get(h_curves(i),"DisplayName"));
			else
				box_string = sprintf("\n\\color[rgb]{%s}{\\bf –– }\\color[rgb]{0 0 0}%s ",line_color_string,get(h_curves(i),"DisplayName"));
			endif
		else
			if (i == (length(h_curves) - num_curves_to_include + 1))
				box_string = sprintf("%s\n\\color[rgb]{%s}{\\bf –– }\\color[rgb]{0 0 0}%s \n",box_string,line_color_string,get(h_curves(i),"DisplayName"));
			else
				box_string = sprintf("%s\n\\color[rgb]{%s}{\\bf –– }\\color[rgb]{0 0 0}%s ",box_string,line_color_string,get(h_curves(i),"DisplayName"));
			endif
		endif
	endfor

	if(x_axis_log == 1)
		legend_x_position_norm = axes_position(3)*log10(legend_x_position/xlim_min)/log10(xlim_max/xlim_min);
	else
		legend_x_position_norm = axes_position(3)*(legend_x_position - xlim_min)/(xlim_max - xlim_min);
	endif
	if(y_axis_log == 1)
		legend_y_position_norm = axes_position(4)*log10(legend_y_position/ylim_min)/log10(ylim_max/ylim_min);
	else
		legend_y_position_norm = axes_position(4)*(legend_y_position - ylim_min)/(ylim_max - ylim_min);
	endif
	
	box_position = [axes_position(1) + legend_x_position_norm,axes_position(2) + legend_y_position_norm,0.2,0.2];
	h_legend = annotation("textbox",box_position,"string",box_string,"horizontalalignment",ha{1},"verticalalignment",va{2},"fitboxtotext","on","fontsize",font_size,"interpreter","tex","backgroundcolor",[1 1 1],"tag","custom_legend");

endif

if (error_flag == 1)
	h_legend = [];
	box_string = [];
	box_position = [];
endif

endfunction


### End of functions

arg_list =  argv();

pkg load signal;

number_of_arguments_required = [1 2 3 4];

if (length(arg_list) != number_of_arguments_required)
   printf("Usage: plot_transient_data <csv_filename_from_trans_pll> <title_string> <time_stamp> <lock_time_string>");
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
		lock_time = str2double(arg_list{4});
   endif

endif

plot_transient_data(filename_with_path,plot_title,timestamp,lock_time);