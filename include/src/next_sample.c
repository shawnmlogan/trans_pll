
#include "globals.h"

void next_sample(int *pvdata,FILE *fpinputfile, char *pfin, int N)
{
/* N = NUMBER_OF_SAMPLES_STORED in data vector*/
int i = 0, reached_end_of_file = 0;


for (i=0;i < (N - 1); i++)
   pvdata[i] = pvdata[i + 1];

while (fscanf(fpinputfile,"%d",&pvdata[N - 1]) != 1)
   {
   /*Check to make sure 0 or 1!*/
   if ((pvdata[N - 1] != 0) && (pvdata[N - 1] != 1))
      {
      printf("Did not read a 0 or a 1 from input data file \"%s\"...Exiting..\n", pfin);
      exit(0);
      }
   reached_end_of_file = 0;
   if (feof(fpinputfile))
      {
      rewind(fpinputfile);
      reached_end_of_file++;
      }
   else
      {
      if (reached_end_of_file == 4)
         {
         printf("Incorrect data in input file \"%s\". Exiting...\n", pfin);
         exit(0);
         }
      }
   }

return;
}