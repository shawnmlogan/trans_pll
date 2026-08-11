Program Installation

Version 1.77 of the program is available in the compressed tar file
“trans_pll_v1p77_081126.tar.gz” and may be downloaded from
the URL provided in reference [1] or reference [2]. To install the
program and create the executable “trans_pll”, enter
Command line [1] in the directory in which you wish to locate the
program.

$ tar -xvzf trans_pll_v1p77_081126.tar.gz	Command line [1]

This will create a directory trans_pll_v1p77_081126 and
extract its directory structure:

Documentation/		examples/		plotting_routines/
README.txt		include/		src/

Navigate to the “src” subdirectory, and issue the following two UNIX
commands:

$ make							Command line [2]
$ make clean						Command line [3]

Issuing these two commands will create the executable
“trans_pll” and delete object files no longer needed. In
addition, Command line [2] will also attempt to create a symbolic
link to trans_pll in your $HOME/bin directory if this
directory exists. Assuming your $HOME/bin directory is contained in
your executable search path (UNIX PATH variable), this will allow
you to execute trans_pll from any of your subdirectories
using Command line [4] as documented in the program manual
in the Documentation directory.

$ trans_pll <input_ASCII_filename> 		Command line [4]

This README.txt file is contained in the top level directory of
trans_pll_v1p77_081126. A number of script files, a Microsoft Excel template for
plotting its output file, and a sample run line and its outputs is
contained in the examples directory, and program documention
is contained in the Documentation directory in Portable Documentation
Format.
 
References:

[1]	https://1drv.ms/u/c/12fa6404c01a3e73/IQCSM5HPlNUSTaSniCh-OcdkAbwG6d6jKG6EESzn2kn_UxU?e=aOmPQT
[2]	https://github.com/shawnmlogan/trans_pll

Initial public release: 8/5/2026
rev. 8/11/2026
