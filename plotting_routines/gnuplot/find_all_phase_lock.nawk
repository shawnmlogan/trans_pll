
BEGIN {FS = ","; OFS = ","; found_all_phase_lock_flag = 1; out_of_lock_cases = 0;}
{
# Returns a 0 if any entry after row 1 
# in column column_num is non-numeric and does not indicate lock detector was not active
# column_num = 4;

if (NR > 1)
   {
   if (($column_num + 0 != $column_num) && (index(tolower($column_num),"not active") == 0))
   	{
      found_all_phase_lock_flag = 0;
      out_of_lock_cases = out_of_lock_cases + 1;
      }
   }
}
END { printf("%d %d\n",found_all_phase_lock_flag,out_of_lock_cases);}
