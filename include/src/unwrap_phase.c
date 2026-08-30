# include "globals.h"

int unwrap_phase(double **parray,int rows,int cols, int unwrap_column_number, char *pphase_units)
{
int i = 0, j = 0, flag_phase_adjust = 1, last_valid_row = 0, num_iterations = 0;

double phase_delta = 0.0, max_phase_delta = 1e-12;
double phase_wrap = 360.0;

char phase_units_internal[80], *pphase_units_internal;
pphase_units_internal = &phase_units_internal[0];

strncpy(pphase_units_internal,pphase_units,79);
for( ; *pphase_units_internal; ++pphase_units_internal)
	*pphase_units_internal = tolower(*pphase_units_internal);
pphase_units_internal = pphase_units_internal - strlen(pphase_units);

if (strcmp(pphase_units_internal,"degrees") == 0)
	{
	phase_wrap = 360.0;
	num_iterations = 0;
	}
else
	{
	if(strcmp(pphase_units_internal,"radians") == 0)
		{
		phase_wrap = 2.0*M_PI;
		num_iterations = 0;
		}
	else
		{
		if(strcmp(pphase_units_internal,"nowrap") == 0)
			{
			phase_wrap = 0.0;
			num_iterations = rows + 1; /* This will skip while loop */
			}
		else
			{
			printf("Did not recognize units \"%s\" for phase wrap in function unwrap_phase().\nSpecify \"degrees\", \"radians\" or \"nowrap\". Exiting...\n",pphase_units);
			exit(0);
			}
		}
	}

while ((flag_phase_adjust == 1) && (num_iterations < (rows + 1)))
	{
	flag_phase_adjust = 0;
	for(i = 0;i < rows;i++)
		{
		if (isnan(parray[i][unwrap_column_number - 1]) == 0)
			{
			if (i == 0)
				{
				phase_delta = 0;
				if (parray[i][unwrap_column_number - 1] > phase_wrap/2.0)
					{
					parray[i][unwrap_column_number - 1] = parray[i][unwrap_column_number - 1] - phase_wrap;
					flag_phase_adjust = 1;
					}
				else
					{
					if (parray[i][unwrap_column_number - 1] < (0.0 - phase_wrap/2.0))
						{
						parray[i][unwrap_column_number - 1] = parray[i][unwrap_column_number - 1] + phase_wrap;
						flag_phase_adjust = 1;
						}
					}
				last_valid_row = i;
				}
			else
				{
				phase_delta = parray[i][unwrap_column_number - 1] - parray[last_valid_row][unwrap_column_number - 1];
				last_valid_row = i;
				}
			if (phase_delta > phase_wrap/2.0)
				{
				parray[i][unwrap_column_number - 1] = parray[i][unwrap_column_number - 1] - phase_wrap;
				flag_phase_adjust = 1;
				}
			else
				{
				if (phase_delta < (0.0 - phase_wrap/2.0))
					{
					parray[i][unwrap_column_number - 1] = parray[i][unwrap_column_number - 1] + phase_wrap;
					flag_phase_adjust = 1;
					}
				}
			}
		}
	num_iterations++;
	}

/* for(i = 0;i < rows;i++)
	{
	if (i == 0)
		phase_delta = 0;
	else
		{
		phase_delta = parray[i][unwrap_column_number - 1] - parray[i - 1][unwrap_column_number - 1];
		}
	if (fabs(phase_delta) > fabs(max_phase_delta))
		max_phase_delta = phase_delta;
	}
if (num_iterations > 1)
	printf("Required %ld iterations, ending maximum phase_delta = %.3f %s.\n",
num_iterations,max_phase_delta,pphase_units_internal); */

return EXIT_SUCCESS;
}
			