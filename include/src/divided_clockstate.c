
#include "globals.h"

int divided_clockstate(int CLKIN,long int *pos_edge_counter,long int *neg_edge_counter,double phase0,double phase,double duty_cycle,double divider,Filedata *pdatain)
{

int pos_edge = 0, neg_edge = 0, return_val = -1;
char *plog_string,log_string[LOGFILE_LINELENGTH + 1];

if (phase == phase0) /*Initialize all variables to initial state at start of simulation*/
   {
   *pos_edge_counter = 0;
   *neg_edge_counter = 0;
   }

if (fabs(divider*phase)/(2.0 * pi) >= (double) *pos_edge_counter)
   {
   pos_edge = 1;
   (*pos_edge_counter)++;
   }
else 
   {
   if (fabs(divider*phase)/(2.0 * pi) >= ((double) *neg_edge_counter + duty_cycle))
      {
      neg_edge = 1;
      (*neg_edge_counter)++;
      }
   }

if ((pos_edge == 0) && (neg_edge == 0))
   return_val = CLKIN;
else
   {
   if ((pos_edge == 1) && (neg_edge == 0))
     return_val = 1;
   else
      {
      if ((pos_edge == 0) && (neg_edge == 1))
         return_val = 0;
      else
         {        
         if ((pos_edge == 1) && (neg_edge == 1))
            {
            snprintf(plog_string,LOGFILE_LINELENGTH,
            "Error in divided_clockstate() function, ");
            print_string_to_log(plog_string,pdatain);
            snprintf(plog_string,LOGFILE_LINELENGTH,
            "positive and negative edges occur at same time! Exiting...\n");
				print_string_to_log(plog_string,pdatain);
				exit(0);
            }
          }
       }
    }
return return_val;
}

