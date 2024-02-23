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

# compiler command with arguments
g++ .\*.cpp -o generate.exe

# execute program with optional input redirection
.\generate.exe 

# OPTIONAL: install required packages before running python
python -m pip install pandas
python -m pip install matplotlib
python -m pip install numpy
python -m pip install PyArrow

# once the program runs and stores the data, we can call python to plot
python plot.py
