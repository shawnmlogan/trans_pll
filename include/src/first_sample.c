
#include "globals.h"

void first_sample(int *pvdata,FILE *fpinputfile, char *pfin, int k0, int N)
{
/*N = number of samples to read into vector pointed to by pvdata from file pointed to by
fpinputfile with name pointed to be pfin. Does NOT change pvdata*/

int i = 0, reached_end_of_file = 0;

do
   {
   if (i < k0 - 1)
   	{
   	pvdata[i] = 0;
   	i++;
   	}
   else
   	{
	   if (fscanf(fpinputfile,"%d",&pvdata[i]) == 1)
	      {
	      /*Check to make sure 0 or 1!*/
		   if ((pvdata[i] != 0) && (pvdata[i] != 1))
		      {
		      printf("Did not read a 0 or a 1 from input data file \"%s\"...Exiting..\n", pfin);
		      exit(0);
		      }
	      i++;
	      }
	   else
	      {
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
      }
   }
while (i < N);
return;
}