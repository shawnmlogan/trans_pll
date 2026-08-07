#!/bin/bash

if [ $# -ne 0 ]; then
    echo "Usage: run_example_cases.sh "
    exit 127
fi

cd ./example_with_gnuplot_plotting_tool;
trans_pll sample_gnuplot_pfd_pll_input_file.txt;
sleep 1.0;
cd ../example_with_octave_plotting_tool;
trans_pll sample_octave_pfd_pll_input_file.txt;
sleep 1.0;
cd ..
printf "Simulations completed!\n";
