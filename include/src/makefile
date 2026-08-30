IDIR=../include
CC=gcc
LIBS=-lm
CFLAGS=-I$(IDIR)

ODIR=obj

_DEPS = globals.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o assign.o check_data.o check_for_quit_characters.o check_for_cr_only.o check_executable.o find_date.o find_center_voltage.o compute_control_voltage_characteristic.o find_timestamp.o divided_clockstate.o clocknoise.o clocknoise1.o random_uniform_clocknoise.o random_gaussian_clocknoise.o pnoise_type.o compute.o deltaf_vco.o detect_lock.o dexor.o exor.o is_datarecovery.o is_chargepump.o and4.o inv.o initdata.o init_sig_stats.o f1.o f2.o f3.o filecheck.o first_sample.o next_sample.o parse_to_array.o parsestring_to_doubles_array.o parsecsv_to_array.o pfd_with_delay.o pddff.o pd_windowed.o pd_hogge_with_delay.o pd_alexander.o  pd_alexander_with_delay.o pop_data.o pphased.o printstruct.o sort_array_by_column_number.o unwrap_phase.o remove_whitespace.o remove_whitespace_only.o remove_carriage_return.o rkstep1.o rkstep5.o sethigh.o setlow.o vsample.o pd_nonlinear.o add_units.o add_units_underscore.o replace_string.o print_string_to_log.o create_sample_file.o plowercase.o parsestring_to_char_array.o find_moving_averages.o find_stats_column_one_of_file.o find_CLKIN.o find_CLKIN_noise_filtered.o save_vin_vout_and_compare_ber.o save_vin_vout_and_compare.o mean.o max.o min.o rms.o ffind_fourier_transfer_function.o vpd_amp_alexander.o normalize_vpd.o print_limiting_deltat_set_flag.o check_double_positive_non_zero.o pd_exor_with_delay.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

SH_RESULT:=$(shell set_install_dir.sh)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

trans_pll: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# Comment out changing symbolic link until testing is complete
SH_RESULT:=$(shell install_symbolic_link.sh)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o


