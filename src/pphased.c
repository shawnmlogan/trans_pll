
#include "globals.h"

char *pphased(int ph)
{
char temp[LINELENGTH + 1], *ptemp;
char *pph0 = "4046", *pph1 = "dff", *pph2 = "none_vpd=1", *pph3 = "none_vpd=0";
char *pph4 = "hogge with charge pump", *pph5 = "4046 with charge pump";
char *pph7 = "hogge";
char *pph8 = "hogge with charge pump and rfd";

ptemp = &temp[0];

switch(ph)
   {
   case 0:
      strcpy(ptemp,pph0);
      break;
   case 1:
      strcpy(ptemp,pph1);
      break;
   case 2:
      strcpy(ptemp,pph2);
      break;
   case 3:
      strcpy(ptemp,pph3);
      break;
    case 4:
       strcpy(ptemp,pph4);
       break;
    case 5:
       strcpy(ptemp,pph5);
       break;
    case 7:
       strcpy(ptemp,pph7);
       break;
    case 8:
       strcpy(ptemp,pph8);
       break;
    default:
       printf("Error! Did not recognize phase detector in pphased.c..exiting\n");
       exit(0);
    }
return(ptemp);
}
   
      
