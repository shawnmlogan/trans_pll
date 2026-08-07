#!/bin/bash

if [[ $# -ne 1 ]]; then
printf "Usage: convert_trans_pll_input_file_to_tab_delimited.sh <trans_pll input file>\n"
exit 127;
fi

if [ ! -e $1 ]; then
	printf "File \"%s\" not found, exiting...\n" $1;
	exit 127;
fi

TIME_STAMP=$(date +%m%d%y);
BASE_FILENAME=$(print_filename_no_extension.sh $1);
FILENAME_EXT=$(print_file_extension.sh $1);
sed '1,$s/: /	/' ${BASE_FILENAME}.${FILENAME_EXT} > ${BASE_FILENAME}_${TIME_STAMP}.tab;

printf "Done. Output tab-delimited filename is \"%s\"\n" ${BASE_FILENAME}_${TIME_STAMP}.tab;