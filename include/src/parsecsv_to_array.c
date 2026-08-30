
#include "globals.h"

int parsecsv_to_array(char *pdummy,double *pvco,int *parray_size,int max_array_size)
{
/*Parses a comma separated variable string for doubles and places each
double in an array that starts at pointer pvco. It returns the number of elements it
assigned to the array to the location pointed to by parray_size. If more than  max_array_size are read in, it returns
EXIT_FAILURE, assigns array_size to max_array_size  and an error message*/

char *pdelim = ",";
char *ptoken;
int i = 0;
double *ptemp_pointer;

/*Save initial location - location 0 - of array pointed to by pvco*/

ptemp_pointer = pvco;

ptoken =  strtok(pdummy,pdelim);

while (ptoken != NULL)
   {
   *ptemp_pointer = atof(ptoken);
   ptemp_pointer++;
   i++;
   ptoken = strtok(NULL,pdelim);
   if (i > max_array_size)
      {
      /*printf("Too many elements on line %s!\n",pdummy);*/
      *parray_size = max_array_size;
      return EXIT_FAILURE;
      }
   }
   
*parray_size = i;

return EXIT_SUCCESS;
}

   
   
