
#include "globals.h"

int parse_to_array(char *pdummy,double *pvco)
{
/*Parses a comma separated variable string for doubles and places each
double in an array that starts at pointer pvco*. Returns a 0 if successful
and a 1 if the end of the string is not reached and MAX_VCO_POLYNOMIAL have
been read in*/

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
   if (i > MAX_VCO_POLYNOMIAL)
      {
      return(1);
      }
   }
/*ptemp_pointer = pvco;
for (i = 0; i< MAX_VCO_POLYNOMIAL;i++)
   {
   printf("vco[%d] = %2.4e ",i,*ptemp_pointer);
   ptemp_pointer++;
   }
printf("\n");*/

return(0);
}

   
   
