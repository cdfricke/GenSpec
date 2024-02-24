Programmer: Connor Fricke (cd.fricke23@gmail.com)\
Latest Revision:\
  11-Dec-2023 --- Original Version: Created for Astronomy 3350 Final Project\
  19-Feb-2024 --- Revisited for adaptation to GitHub project.\
  22-Feb-2024 --- Re-done with python script rather than matplotlibcpp, added RNG.h and a script to run the program.

Original Version:
  The current state of the project resembles the original very little, except for the general functionality. It was created for a final project
  in an Astronomy class at OSU taught by Professor Don Terndrup, Astronomy 3350.

Compile Command:
The original project involved the use of a C++ wrapper to the Python module Matplotlib. In the current version
of the project, plotting is instead done with an actual Python script for simplicity (and because for some reason the save()
function of the C++ implementation stopped working.) The algorithm was originally created in a Jupyter notebook,
and then re-written in C++ code to speed up the spectrum generating process. Proving this could be done and demonstrating
performance increases was the entire purpose of the final project.

TO-DO:
Add header files to clean up generate.cpp, other organizational tasks.
