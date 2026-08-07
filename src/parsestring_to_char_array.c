#include "globals.h"

int parsestring_to_char_array(char *pinput_string,char **pchar_array,int *parray_size,int max_array_size)
{
/* Parses a comma separated variable string for character strings and places each
in an array that starts at pointer pchar_array. It returns the number of elements it
assigned to the array to the location pointed to by parray_size. If more than
max_array_size are read in, it returns EXIT_FAILURE and assigns array_size
to max_array_size  and an error message */

char *pdelim = ",";
char *ptoken, token[CSV_FILE_LINELENGTH + 1], *porig_input_string;
char error_flag = 0;
int i = 0;
char **pptemp_pointer;

ptoken = &token[0];

if ((porig_input_string = (char *) calloc(strlen(pinput_string) + 1,sizeof(char))) == NULL)
	{
	printf("Memory allocation error for porig_input_string...exiting\n");
	exit(0);
	}

strcpy(porig_input_string,pinput_string);

if ((pptemp_pointer = (char **) calloc(max_array_size,sizeof(char *))) == NULL)
	{
	printf("Memory allocation error for pptemp_pointer...exiting\n");
	exit(0);
	}

for(i = 0; i < max_array_size; i++)
	{
	if ((pptemp_pointer[i] = (char *) calloc(LINELENGTH + 1,sizeof(char))) == NULL)
		{
		printf("Memory allocation error for ppchar_string[%d]...exiting\n",i);
		exit(0);
		}
	}

i = 0;
ptoken = strtok(porig_input_string,pdelim);

while ((ptoken != NULL) && (error_flag == 0))
   {
   strncpy(pptemp_pointer[i],ptoken,LINELENGTH);
   i++;
   ptoken = strtok(NULL,pdelim);
   if (i > max_array_size - 1)
      {
      printf("Too many elements on line %s!\n",pinput_string);
      *parray_size = max_array_size;
      error_flag = 1;
      }
   }
   
*parray_size = i;

for(i = 0; i < *parray_size; i++)
	strncpy(pchar_array[i],pptemp_pointer[i],LINELENGTH);
for(i = 0; i < max_array_size; i++)
	free(pptemp_pointer[i]);

free(porig_input_string);

if (error_flag != 0)
	return EXIT_FAILURE;
else
	return EXIT_SUCCESS;
}

