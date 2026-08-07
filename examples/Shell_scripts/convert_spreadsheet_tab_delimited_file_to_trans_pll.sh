#!/bin/bash

if [[ $# -ne 1 ]]; then
printf "Usage: convert_spreadsheet_tab_delimited_file_to_trans_pll.sh <spreadsheet tab-delimited file>\n"
exit 127;
fi

if [ ! -e $1 ]; then
	printf "File \"%s\" not found, exiting...\n" $1;
	exit 127;
fi

TIME_STAMP=$(date +%m%d%y);
BASE_FILENAME=$(echo $1 | awk 'BEGIN{FS="."} {print $1;}');
FILENAME_EXT=$(echo $1 | awk 'BEGIN{FS="."} {print $2;}');
tr -d $'\r' < ${BASE_FILENAME}.${FILENAME_EXT} | cut -f1 > col1_${TIME_STAMP};
tr -d $'\r' < ${BASE_FILENAME}.${FILENAME_EXT} | cut -f2  | sed '1,$s/: $//' > col2_${TIME_STAMP};
paste -d":" col1_${TIME_STAMP} col2_${TIME_STAMP} | sed '1,$s/:$//' | sed '1,$s/:/: /g' | sed '1,$s/"//g' > ${BASE_FILENAME}_trans_pll_${TIME_STAMP}.txt;
rm col1_${TIME_STAMP} col2_${TIME_STAMP};

printf "Done. Output filename for trans_pll is \"%s\"\n" ${BASE_FILENAME}_trans_pll_${TIME_STAMP}.txt;