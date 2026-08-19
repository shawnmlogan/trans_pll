#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <locale.h>

/*Add #defines for version number*/

#define VERSION_NUMBER	1.77
#define VERSION_DATE "8/11/2026"

/*Add defines for debugging*/

/* #define DEBUG_DELAY */
/* #define DEBUG_MALLOC */
/* #define DEBUG_LOCK_DETECT */
/* #define DEBUG_LOCK_DETECT_VERBOSE */
/* #define DEBUG_SAVE_VIN_VOUT_AND_COMPARE */
/* #define DEBUG_JITTER_TRANSFER_ARRAY_SIZE */

/* #define USE_LOCK_DETECT_FLAG_TO_FILTER_RESULTS */
/* #define SAVE_FULL_OUTPUT_FILE */
/* #define SAVE_POST_PROCESSED_FILE */
/* #define SAVE_JITTER_TRANSFER_TRANS_PLL_OUTPUT_FILES */
/* #define SAVE_NOISE_SAMPLES_TO_FILE */
/* #define SAVE_DEBUG_CDR_PLL_SIGNALS */
#define DEBUG_CLKIN_DUTY_CYCLE
/* #define DEBUG_CLKIN_DUTY_CYCLE_VERBOSE */
/* #define DEBUG_PARSESTRING_TO_DOUBLES_ARRAY */
/* #define DEBUG_DIVIDED_CLOCKSTATE */

/* Add option for BER detection and display if DETECT_AND_PRINT_BER is TRUE */

#define DETECT_AND_PRINT_BER TRUE
#define NEVER_DETECTED_ERROR_FREE 100.0

/* Define lock detect control voltage tolerance */

#define LOCK_DETECT_VC_DIFF_PERCENT_LIMIT 0.10
#define RELATIVE_PHASE_SLOPE_AVERAGE_PPM_LIMIT 5e12 
#define RPHASE_UI_HISTORY_LIMIT 1e6
#define RPHASE_UI_HISTORY_LOSS_LOCK_LIMIT 1e6
#define RPHASE_UI_HISTORY_PP_LIMIT 0.15
#define RPHASE_UI_HISTORY_PP_LOSS_LOCK_LIMIT 0.30
#define MAX_NUMBER_OF_INPUT_SAMPLES_STORED 1e6

/* Define maximum time step in UI relative to VCO period */

#define MIN_DELTAT_RELATIVE_VCO_PERIOD_IN_UI 0.01

/* Declare exit characters, num_exitchars, pi, and phaseo_sample array */

#ifdef MAIN
	double pi;
#else
	extern double pi;
#endif

#ifdef EXIT_CHARS
	char exitchars[] = {'q','Q'};
	int num_exitchars = 	sizeof(exitchars)/sizeof(exitchars[0]);
#endif

typedef enum { FALSE, TRUE } Boolean;

/* Print gnuplot or octave commands to files if PRINT_GNUPLOT_OR_OCTAVE_COMMAND is TRUE */

#define PRINT_GNUPLOT_OR_OCTAVE_COMMAND FALSE

#define OCTAVE 0
#define GNUPLOT 1
#define PLOT_NORMALIZED_FREQ 0

/*Add #defines for phase detectors*/

#define PFD						0
#define DFF						1
#define NONE_VPD_HIGH		2
#define NONE_VPD_LOW			3
#define HOGGE_CP				4
#define PFD_CP					5
#define PD_EXOR				6
#define HOGGE					7
#define PD_WINDOWED			8
#define PD_ALEXANDER			9

/*Add #defines for noise type*/

#define GAUSSIAN_NOISE 0
#define UNIFORM_NOISE 1

#define LINELENGTH 128
#define LOGFILE_LINELENGTH 1024
#define CSV_FILE_LINELENGTH 512
#define FILENAME_LINELENGTH 255 /* Maximum number of characters allowed in OS X filename */
#define PREFIX_LINELENGTH 0
#define SUFFIX1_LINELENGTH 24 /* Used for appending "_<jitter_frequency>" to base filename */
#define SUFFIX2_LINELENGTH 0 /* Used for appending psd computation parameters to csv and png filename */
#define MAXIMUM_INPUT_FILENAME_LINELENGTH FILENAME_LINELENGTH - PREFIX_LINELENGTH - (SUFFIX1_LINELENGTH + SUFFIX2_LINELENGTH)
#define TITLE_LINELENGTH 512
#define COMMAND_LINELENGTH 1024
#define OUTPUT_FILE_LINELENGTH 255 /* Maximum OSX filename limit is 255 */

#define NUMBER_OF_PROGRESS_POINTS 10.0
#define TIME_ERROR_MAX 1E-15
#define MAXSTOREPD 50
#define MAXSTOREVCXO 50
#define SHOWPRPOINT 10
#define MAX_ITERATIONS 4
#define NUMBER_OF_SAMPLES_STORED 5
#define MAX_VCO_POLYNOMIAL 10
#define NUMBER_OF_JITTER_FREQ 36
#define NUMBER_OF_TRANSFER_FUNCTION_JITTER_CYCLES 32.0
#define MAX_NUMBER_OF_TRANSFER_FUNCTION_SAMPLES NUMBER_OF_TRANSFER_FUNCTION_JITTER_CYCLES*4096
#define MAX_JITTER_AMP_UI 1.0E1
#define EPSILON 1.0E-12
#define USE_POLYNOMIAL_MODEL 0
#define USE_TANH_MODEL 1
#define INIT_VAL 999
#define NOMINAL_HISTORY_LENGTH 1000
#define MAX_HISTORY_LENGTH 1e9

#define NUMBER_OF_VALUE_STRINGS 15 /*Used for add_units arguments*/
#define LINELENGTH_OF_VALUE_STRING 20

struct signal {
	double vpd;
	double vs1; /*Add signal injection source at node v1 11/7/99*/
	double vs2; /*Add signal injection source at node vc1 11/7/99*/
	double vc1;
	double vc2;
	double vc;
	double vco; /*Add vco control voltage 8/27/99*/
	double vq1; /*Used for data applications in Alexander and Hogge pd*/
	double vq2; /*Used for data applications in Alexander and Hogge pd*/
	double vq3; /*Used for data applications in Alexander pd*/
	double vq4; /*Used for data applications in Alexander pd*/
	double phasein;
	double phaseo;
	double phaseo_delayed; /*Allows for delay due feedback divider, applied to feedback input of phase detector*/
	double phaseoq; /*Used to generate quadrature of output signal*/
	double phaseoq_delayed; /*Allows for delay of feedback divider, applied to feedback input of phase detector*/
	int clk; /*Add for logic variables for output clock*/
	int clkq; /*Quadrature value of clock*/
	int vco_clk;
	};

struct coeff {
	double c1;
	double c2;
	double c3;
	double c4;
	double c5;
	double c6;
	double c7;
	double c8;
	double c9;
	double c10;
	double c11;
	};

struct filedata {
	int file_entry_flag; /* Flag to indicate if entry source is a file (1) or command line (0) */
	char *pinput_filename;
	char input_filename[FILENAME_LINELENGTH + 1];
	char *plog_filename;
	char log_filename[FILENAME_LINELENGTH + 1];
	char *ptimestamp;
	char timestamp[LINELENGTH + 1];
	int enable_plot_flag;
	char *ppd;
	int pd;
	double fo;
	char vc_model_string[LINELENGTH + 1];
	char *pvc_model_string;
	int vc_model;
	double fin;
	double vco_clk_dutycycle; /*Add duty cycle variable for vco duty cycle*/
	double clk_dutycycle; /*Add duty cycle variable for potentially divided clock at fo*/
	double quadphase;
	double vco_clkq_dutycycle; /*Add duty cycle variable to generate quadrature clock signal at fo*/
	double vco;
	double D;
	double D_delay;
	char *pdatafilein;
	char datafilein[FILENAME_LINELENGTH + 1];
	long int datafilein_number_of_lines;
	double jitter_amp;
	double *pjitter_freq;
	int number_of_jitter_freq;
	double phase0;
	double TSTART;
	double TSTART_jt; /*Add start time for jitter tolerance analysis after inject jitter at TSTART*/
	double TSTOP;
	char *pdeltat;
	double deltat;
	double lock_detect_tau;
	char *plock_detect_delta_vc_limit_percent_string;
	char lock_detect_delta_vc_limit_percent_string[LINELENGTH + 1];
	double lock_detect_delta_vc_limit_percent;
	char *plock_detect_rphase_pp_limit_ui_string;
	char lock_detect_rphase_pp_limit_ui_string[LINELENGTH + 1];
	double lock_detect_rphase_pp_limit_ui;
	char *plock_detect_rphase_pp_loss_lock_limit_ui_string;
	char lock_detect_rphase_pp_loss_lock_limit_ui_string[LINELENGTH + 1];
	double lock_detect_rphase_pp_loss_lock_limit_ui;
	long int npoints;
	double logic_hi_pdsignal;
	double logic_lo_pdsignal;
	double vs1; /*Added signal source to inject current through RS1*/
	double vs2; /*Added signal source to inject current through RS2*/
	double *pvc_polynomial;
	int vco_polynomial_degree;
	double vco_tau;
	char *palexander_pd_threshold_ui;
	double alexander_pd_threshold_ui;
	double vmax;
	double vmin;
	double noise_amp;
	double noise_bandwidth_Hz; /*Added to limit bandwidth of input noise 2/13/2023*/
	char * pnoise_type; /*Added to allow for uniform or gaussian noise option 2/13/2023*/
	int noise_type; /*Added to allow for uniform or gaussian noise option 2/13/2023*/
	double R1;
	double R2;
	double R3;
	double C1;
	double C2;
	double RS1; /*Resistance of voltage source injecting signal at v1 11/7/99*/
	double RS2;
	double initvc;
	double tauh; /*Used in pfd to model gate delay*/
	double tauff; /*Delay time for flip-flop in pfd.c 10/16/99*/
	double taucp_min; /*Min PFD pulse width in pfd.c 10/18/99*/
	double pfd_dff_deadzone_ui; /*PFD deadzone*/
	double tw; /*Width of time window for windowed phase detector in which DFF based phase detector is used*/
	double tacq_max; /*Expected time at which acqusiition is over - start point for windowed phase detector clock switch circuit*/
	double phase_freq_step_time_in_sec; /* Start time for phase or freq step input 4/15/2015*/
	double phase_step_in_sec; /* Phase step applied at phase_freq_step_time_in_sec input 4/15/2015*/
	double freq_step_ppm; /* Frequency step applied at phase_freq_step_time_in_sec input 4/15/2015*/
	
	double ssc_start_time_in_sec; /* Start time for SSC frequency modulation 5/1/2015*/
	double max_pos_ssc_ppm; /* Peak positive SSC modulation in ppm 5/1/2015*/
	double max_neg_ssc_ppm; /* Peak negative SSC modulation in ppm 5/1/2015*/
	double freq_ssc; /* Frequency of SSC modulation 5/1/2015*/
	
	char *poutput_base_filename;
	char output_base_filename[FILENAME_LINELENGTH + 1];
	char file_format[FILENAME_LINELENGTH + 1];
	char *pfile_format;
	int ff;
	int plot_outputs;
	int gnuplot;
	int octave;
	char *p_plot_preference;
	int plot_preference;
	};

typedef struct filedata Filedata;

struct sig_stats {
	double vcenter_voltage;
	double vc_running_average;
	double vc_history_pp;
	double sigma_vc;
	double rphase_ui_running_average;
	double rphase_ui_history_pp;
	double sigma_rphase_ui;
	double relative_phase_slope_average;
	double sigma_relative_phase_slope_average;
	};

typedef struct sig_stats Stats;

/*Function prototypes*/

int filecheck(char *pfilename,Filedata *pdatain);
void create_sample_file(char *pfname);
void initdata(Filedata *pdatastruct);
void print_limiting_deltat_set_flag(int deltat_set_flag, Filedata *pdatain);
char *add_units(double value,int num_digits,char *suffix,char *pvalue_string);
char *add_units_underscore(double value,int num_digits,char *suffix,char *pvalue_string);
void find_date(char *pdate_string,int max_characters);
int find_timestamp(char *pdate_string,int max_characters);
Boolean check_for_cr_only(char *pline);
Boolean check_for_quit_characters(char *pline);
Boolean check_double_positive_non_zero(double input_double,char *pdouble_name,Boolean *pcorrected_status);
void remove_whitespace(char *pstring);
void remove_whitespace_only(char *pstring);
void remove_carriage_return(char *pline);
char *plowercase(char *pinput_string);
int check_executable(char *pprogram_executable,char *preturn_string);

/*Phase detector related functions*/

int is_datarecovery(int phase_detector_type);
int is_chargepump(int phase_detector_type);
int pddff(struct signal *psig,double time,Filedata *pdatastruct,int *pSTATE);
int pd_windowed(struct signal *psig,double time,Filedata *pdatastruct,int *pSTATE);
int pfd_with_delay(struct signal *psig,double time,double tauh,double tauff,double taucp_min,double phase_dz_ui,int *pSTATE);
int pd_exor_with_delay(struct signal *psig,int CLKIN,double time,Filedata *pdatain,int *pSTATE);
int sethigh(struct signal *psig,double deadzone,int *pSTATE);
int setlow(struct signal *psig,double deadzone,int *pSTATE);
int pd_hogge_with_delay(struct signal *psig,double time,Filedata *pdatain,int VIN,int *pSTATE,int *perror);
int pd_alexander(struct signal *psig,Filedata *pdatain,double time,int VIN,int *pSTATE,int *perror);
int pd_alexander_with_delay(struct signal *psig,Filedata *pdatain,double time,int VIN,int *pSTATE,int *perror);
int divided_clockstate(int CLKIN,long int *pos_edge_counter,long int *neg_edge_counter,double phase0,double phase,double duty_cycle,double divider,Filedata *pdatain);
int vpd_amp_alexander(struct signal *psig,Filedata *pdatain,double phase_threshold_ui,double vpd_value_outside_phase_threshold);
double normalize_vpd(Filedata *pdatastruct,double vpd);

/*Lock detect and averaging related functions*/

void detect_lock(struct signal *psig,struct filedata *pdatain,long int lock_detect_counter,double time,Stats *psig_stats,int *plock_detect_flag, double *plock_time,int *plost_lock_detect_flag, double *plost_lock_time);
void find_moving_averages(struct signal *psig,struct filedata *pdatain,int history_length,long int loopcount,Stats *psig_stats,double *pvc_history,double *prphase_ui_history,double *pdeltaf_vco_history);

/*Functions used to compute derivatives of diff equations*/

double f1(struct coeff *pw, double vs1, double vs2, double vpd, double vc1, double vc2 );
double f2(struct coeff *pw, double vs1, double vs2, double vpd, double vc1, double vc2);
double f3(double tau, double vin, double vout);

/*Runge-Kutta related functions*/

int rkstep5(double tinit,double tfinal,struct coeff *pw,struct signal *psig, double fo,\
int vc_model, double *pvco,int degree_of_polynomial,\
double vmax, double vmin,double errmax,int iter);

int rkstep1(double tinit,double tfinal,double tau,double vin,double *pvcap,
double vmax, double vmin,double errmax,int iter);
void compute(struct coeff *pw,double R1,double R2,double R3,double C1,double C2, 
double RS1, double RS2,int ph);

/*VCO related functions*/

double deltaf_vco(double vc, int vc_model, double *pvco, int vco_polynomial_degree);

/* Noise related functions */

double clocknoise(double amplitude, double phase_val);
double random_gaussian_clocknoise(double sigma, double average_value, unsigned int seed);
double random_uniform_clocknoise(double range, double average_value, unsigned int seed);
char *pnoise_type(int noise_type);

/*Logic functions*/

int exor(int A, int B);
int dexor(double A, double B);
int inv(int A);
void dff(int D,int clock, int old_clock, int reset, int *Q, int *QB);

/*Input signal data related functions*/

int find_stats_column_one_of_file(char *pfin, double *ave_input_signal, double *min_input_signal, double *max_input_signal,long int *number_of_input_signal_lines);
void first_sample(int *pvdata,FILE *fpinputfile, char *pfin, int k0, int N);
int vsample(int *pvdata, Filedata *pdatain, double vdata_phase0, double phasein, int *perror, int N, int *pdeltaphase, double *pphase_val);
void next_sample(int *pvdata,FILE *fpinputfile, char *pfin, int N);
int find_CLKIN(struct signal *psig, Filedata *pdatain);
int find_CLKIN_noise_filtered(struct signal *psig, Filedata *pdatain,double time,double *pCLKIN_rising_edge_time,int *pCLKIN_rising_edge_flag,double *pCLKIN_falling_edge_time,int *pCLKIN_falling_edge_flag,int LAST_CLKIN,double min_pulse_width_sec,double *pon_time_sec,long int *pnumber_of_blocked_transitions);

/*Analysis data related functions*/

int assign(char *pidentifier,char *pentry, Filedata *pdatastruct);
int pop_data(char *pfin, Filedata *pdatastruct);
int parsestring_to_char_array(char *pinput_string,char **pchar_array,int *parray_size,int max_array_size);
int parsecsv_to_array(char *pdummy,double *pvco,int *parray_size,int max_array_size);
int parsestring_to_doubles_array(char *pinput_string,double *pdoubles_array,int *parray_size,int max_array_size);
void printstruct(char *pfout, Filedata *pdatastruct);
int replace_string(char *pinput_string,char *poutput_string,char *porig_string,char *pnew_string,int max_output_string_length);
void print_string_to_log(char *pstring, Filedata *pdatain);
int check_data(Filedata *pdatastruct);
char *pphased(int ph);
void remove_dc_phaseo(char *pfilein,char *pfileout,double *gain_at_fin_in_dB, double *phase_at_fin_rad,double fj,double TSTART,double TSTOP,Filedata *pdatain);
/* void remove_dc_phaseo(char *pfilein,char *pfileout,double *gain_at_fin_in_dB, double *phase_at_fin_rad,double fj,double TSTART,double TSTOP,Filedata pdatain); */
void sort_array_by_column_number(double **parray,int rows,int cols, int sort_column_number);
int unwrap_phase(double **parray,int rows,int cols, int unwrap_column_number, char *pphase_units);
int find_center_voltage(struct filedata *pdatain,struct sig_stats *psig_stats,double max_error_percent);
void init_sig_stats(Stats *psig_stats,Filedata *pdatain);
int compute_control_voltage_characteristic(struct filedata *pdatain, struct sig_stats *psig_stats, int num_points, char *pfilename, char *ptimestamp, int plot_flag, int plot_normalized_freq);

void save_vin_vout_and_compare(int* pvin_sampled,int *pvout_sampled,int lock_detect_flag,int CLKIN,int VIN,long int number_of_input_samples,int CLKOUT,int VOUT,long int number_of_output_samples, double time,long int *pdelay, int *perror_free_flag,Filedata *pdatain);

void save_vin_vout_and_compare_ber(int* pvin_sampled,int *pvout_sampled,int lock_detect_flag,int CLKIN,int VIN,long int number_of_input_samples,int CLKOUT,int VOUT,long int number_of_output_samples, double time,long int *pdelay, int *perror_free_flag, double *pber,long int *pber_sample_size,Filedata *pdatain);

void ffind_fourier_transfer_function(double *ptime_samples,double *pphasein_samples,double *pphaseo_samples,double *pphaseo_delayed_samples,long int number_of_samples,double fj,Filedata *pdatain,double *gain_at_fin_in_dB,double *phase_at_fin_rad, char *pfileout);

double mean(double *x, long int N);
double max(double *px, long int N);
double min(double *px, long int N);
double rms(double *px, long int N,double *paverage,int zero_mean_rms_flag);

/*Phase detector non-linearity functions*/

void pd_nonlinear(Filedata *pdatain,double rphase, double orig_logic_hi, double orig_logic_low, double time);

/*Define Cash-Karp parameters for embedded Runge-Kutta method - only for rkstep5()*/

#ifdef CASH_KARP
double a2 = 1.0/5.0;
double a3 = 3.0/10.0;
double a4 = 3.0/5.0;
double a5 = 1.0;
double a6 = 7.0/8.0;
double b21 = 1.0/5.0;
double b31 = 3.0/40.0;
double b32 = 9.0/40.0;
double b41 = 3.0/10.0;
double b42= 0.0 - (9.0/10.0);
double b43 = 6.0/5.0;
double b51 = 0.0 - (11.0/54.0);
double b52 = 5.0/2.0;
double b53 = 0.0 - (70.0/27.0);
double b54 = 35.0/27.0;
double b61 = 1631.0/55296.0;
double b62 = 175.0/512.0;
double b63 = 575.0/13824.0;
double b64 = 44275.0/110592.0;
double b65 = 253.0/4096.0;
double c1 = 37.0/378.0;
double c2 = 0.0;
double c3 = 250.0/621.0;
double c4 = 125.0/594.0;
double c5 = 0.0;
double c6 = 512.0/1771.0;
double s1 = 2825.0/27648.0;
double s2 = 0.0;
double s3 = 18575.0/48384.0;
double s4 = 13525.0/55296.0;
double s5 = 277.0/14336.0;
double s6 = 1.0/4.0;
#endif

#define PLOTTING_ROUTINES_DIR "/Users/sml/cproj/trans_pll/trans_pll_v1p77_081126"
