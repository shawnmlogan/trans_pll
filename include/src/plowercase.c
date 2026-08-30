#include <globals.h>

char *plowercase(char *pinput_string)
{
int i = 0;

for (i = 0; i < strlen(pinput_string); i++)
	pinput_string[i] = tolower(pinput_string[i]);
	
return pinput_string;
}