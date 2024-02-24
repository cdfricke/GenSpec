# Programmer: Connor Fricke
# PWSH Script for compiling and running main.cpp for GenSpec project
# GenSpec was my final project for ASTRON 3350, in which the intent is for the user to be able to 
# create spectra of stars with absorption lines on said spectrum with RNG'd widths and at RNG's locations.
# TO-DO:
#  - allow user to request that the program read a particular data file with absorption line widths and locations.
#  - modify this script file to include input redirection so that parameters necessary to generate a spectrum can
#    be read automatically.
#
# The project uses a wrapper of Python's matplotlib module to do the plotting, though in the future it may be
# more effective to do the plotting with GNUPLOT or other software. The intent is to speed up the process of
# generating the necessary points along the spectrum, which C++ is far more efficient over Python.

# get the working directory path
$scriptPath = $MyInvocation.MyCommand.path
$dir = Split-Path -Parent $scriptPath
Write-Output "Current Directory: $dir"

# compiler command with arguments
# NOTE: .o files get sent to \misc directory, .cpp files in \src, header files in \include
g++ -c -g -Wall -o $dir\misc\generate.o $dir\src\generate.cpp -I $dir\include
g++ -c -g -Wall -o $dir\misc\RNG.o $dir\src\RNG.cpp -I $dir\include
g++ -o generate.exe $dir\misc\generate.o $dir\misc\RNG.o

# execute program with parameters taken from params.in
.\generate.exe params.in

# OPTIONAL: install required packages before running python
# if packages are installed, the output of this process will be sent to install.log in \misc
Write-Output "Installing Python Packages: see install.log for details"
python -m pip install pandas matplotlib numpy pyarrow > $dir\misc\install.log

# once the program runs and stores the data, we can call python to plot
# NOTE: Plot results can be found in \out directory
python plot.py