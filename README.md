Programmer: Connor Fricke\
Latest Revision:\
\t 11-Dec-2023 --- Original Version: Created for Astronomy 3350 Final Project\
\t 19-Feb-2024 --- Revisited for adaptation to GitHub project.



Compile Command:
The following compiler command is used to compile and run this project. Currently, the requirement
is that Python 3.12 is installed in C:\Python312, and the NumPy module has been installed as well.
This command gives the include path for Python 3.12, NumPy (within Python), and our Matplotlibcpp.h file.
In addition, you must link the necessary libraries, which this command does.\

  ```g++ main.cpp -o genspec.exe -I C:\Python312\include -I include -I C:\Python312\Lib\site-packages\numpy\core\include -L C:\Python312\libs -lpython312```

These compile options were adapted from [this](https://www.youtube.com/watch?v=Xp3ntYGs8No) YouTube video.
