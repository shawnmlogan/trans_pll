BEGIN {FS=",";}
{
if ($1 + 0.0 != $1)
	printf("%s,Errors\n",$0);
else
	{
	if ($2 < -12.0)
		printf("%s,1.0\n",$0);
	else
		printf("%s,0.01\n",$0);
	}
}