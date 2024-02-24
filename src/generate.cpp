/*
File Name: generate.cpp
Created by: Connor Fricke
Created on: 12/5/2023
Latest Revision: 12/3/2023
Synopsis: main implementation for stellar absorption spectra program. Executable takes in user-entered values corresponding to the interval of the spectrum in wavelength (angstroms), the resolution of the resulting spectral plot, and the number of spectral absorption lines, which have uniformly distributed locations and normally distributed depths. This algorithm was initially implemented in a Jupyter Notebook using the Python language, then translated to C++ for optimization benchmarking.
Contributors: Prof. Donald Terndrup, Nathan Holty for completed Python implementation

Command Line Entry:
Compiled in Windows Powershell:
    g++ generate.cpp -o generate.exe -I C:\Python312\include -I include -I C:\Python312\Lib\site-packages\numpy\core\include -L C:\Python312\libs -lpython312
    
Once the program is running (generate.exe), it will prompt you for several entries. These entries are explained with a CMD terminal example provided below.
*/

#include <iostream>
#include <ctime>
#include <fstream>
#include <iomanip>
#include "RNG.h" // includes <cmath>, <random>, <vector>

using namespace std;

// GLOBAL CONSTANTS
const double LSIGMA(4.0), HALFSIG(0.1);

/*
getFileIOName(string&, string&):
params:
    string& fileinput - name of file we are importing our data from (likely a .txt, .tsv, or .csv file)
    string& fileoutput - name of the spectrum we are analyzing. will result in a .png, .jpg, and .pdf file named after the particular spectrum.
return:
    none
synopsis: getFileName() simply prompts the user for the name of a file to be opened and read, then prompts the user for the spectrum name in order to assign
names to the output files, which incude a .jpg, .png, and .pdf file.
*/
void getFileIOName(string& fileinput, string& fileoutput);

/*
C_linspace(double, double, int, vector<double>&):
params:
    double start - lower limit of range
    double final - upper limit of range
    int n - number of points, similar to a resolution where increasing n results in smaller stepsize between points
    vector<double>& output - passed-by-reference vector containing n values evenly spaced from start to end of interval
return:
    none
synopsis: C_linspace() is a C++ analog to the Python NumPy module function linspace(), which simply generates a ndarray with the same characteristic as the vector returned by C_linspace()
*/
void C_linspace(double start, double final, int n, vector<double>&);

/*
getPlotDetails(double&, double&, int&, int&):
params:
    double& startw
    double& finalw
    int& nspect
    int& nlines
return:
    none
synopsis:
    this function was implemented solely for abstraction and removing some clutter from the main function. it is used to prompt the user for the necessary variables which describe the resulting plot produced by the program.
*/
void getPlotDetails(double& startw, double& finalw, int& nspect, int& nlines);

// END OF FUNCTION HEADERS

int main(int argc, char* argv[])
{
    // GET SPECTRUM LIMITS, RESOLUTION, & LINES
    double start(0), end(0);
    int nspect(0), nlines(0);
    
    // if we are given one command line argument, we need to open the file and read it into our parameters
    if (argc == 2)
    {
        ifstream fin;
        fin.open(argv[1]); // open first command line parameter, should be a valid file with parameters
        fin >> start >> end >> nspect >> nlines;
    }
    else
        getPlotDetails(start, end, nspect, nlines);

    // START CLOCK (we start here because we now have all the information we will need to proceed with the entire plotting algorithm)
    clock_t t;
    t = clock();

    // GET LINE DEPTHS
    vector<double> linedepths;
    normal(0.0, HALFSIG, nlines, linedepths);

    for (int i = 0; i < nlines; i++)
    {
        // GET THE ABSOLUTE VALUE OF ALL DEPTHS
        linedepths[i] = abs(linedepths[i]);

        // ENSURE NO DEPTHS GREATER THAN 1.0
        if (linedepths[i] > 1.0)
        {
            linedepths[i] = 1.0;
        }
    }

    // GET LINE LOCATIONS
    vector<double> linewavelengths;
    uniform(start, end, nlines, linewavelengths);

    // USE C_linspace() TO CREATE WAVELENGTHS VECTOR
    vector<double> wavelengths;
    C_linspace(start, end, nspect, wavelengths);

    // FOLLOWING ALGORITHM ORIGINALLY IMPLEMENTED IN PYTHON BY NATHAN HOLTY TO GENERATE FULL SPECTRUM
    vector<double> total_spectrum(nspect, 1.0);

    int num_pts(0);

    // LOOP OVER EACH LINE, GENERATE A SPECTRUM WITH A SINGLE LINE, AND MULTIPLY THEM TOGETHER
    for (int i = 0; i < nlines; i++)
    {
        double linecenter = linewavelengths[i];
        double linedepth = linedepths[i];

        // GENERATE A SPECTRUM WITH A SINGLE LINE
        vector<double> line_spectrum(nspect, 1.0);
        if (line_spectrum.size() < wavelengths.size())
        {
            cout << "Tweaking container sizes..." << endl;
            wavelengths.pop_back();
        }

        num_pts = wavelengths.size();
        for (int j = 0; j < num_pts; j++)
        {
            double w = wavelengths[j];
            double d = -gaussian(w, linecenter, linedepth, LSIGMA);
            line_spectrum[j] += d;
        }

        // MULTIPLY THE SPECTRA
        for (int k = 0; k < num_pts; k++)
        {
            total_spectrum[k] = total_spectrum[k] * line_spectrum[k];
        }

    }

    // STOP CLOCK (we stop here because the following code is actually from a C++ header file which acts as a wrapper for a python library, thus it's performance is not indicative of the C++ language for a comparison with Python)
    // t is the number of ticks that have passed since the clock was started
    t = clock() - t;

    // DISPLAY RUNNING TIME RESULTS
    cout << "CPU Time: " << t << " ticks, or " << (float)t / CLOCKS_PER_SEC << " seconds." << endl;

    // OUTPUT DATA TO A .TSV FOR PYTHON TO TAKE
    ofstream arraysOut("misc\\arrays.dat");
    // ADD COMMENT TO TOP OF FILE, THEN COLUMN TITLES UNCOMMENTED
    arraysOut << "# data file from generate.cpp" << endl << "wavelengths,spectrum" << endl;
    for (int i = 0; i < num_pts; i++)
    {
        arraysOut << fixed << setprecision(10) <<  wavelengths[i] << "," << total_spectrum[i] << endl;
    }
    arraysOut.close();
    

}

void getFileIOName(string& fileinput, string& fileoutput)
{
    cout << "Please enter your data file name: ";
    cin >> fileinput;
    cout << "Please enter your spectrum name: ";
    cin >> fileoutput;
}

void C_linspace(const double start, const double final, const int n, vector<double>& output)
{

    // CALCULATE THE STEP SIZE
    double step_size = (final - start) / n;

    // PUSH_BACK() VALUES TO VECTOR
    for (double i = start; i < final; i += step_size) { output.push_back(i); }
}

void getPlotDetails(double& startw, double& finalw, int& nspect, int& nlines)
{
    // PROMPT AND GET USER INPUT FOR ALL PASSED-BY-REFERENCE PARAMETERS
    cout << "Enter wavelength interval start and press ENTER: ";
    cin >> startw;
    while (startw < 0)
    {
        cout << "Retry: Lower limit must non-negative: ";
        cin >> startw;
    }
    cout << "Enter wavelength interval end and press ENTER: ";
    cin >> finalw;
    while (startw >= finalw)
    {
        cout << "Retry: Upper limit must be greater than lower limit: ";
        cin >> finalw;
    }
    cout << "Enter resolution and press ENTER: ";
    cin >> nspect;
    while (nspect <= 0)
    {
        cout << "Error. Entry value be greater than zero: ";
        cin >> nspect;
    }

    //// FLUSH BUFFER IN CASE USER ENTERS A FLOATING POINT VAL
    fflush(stdin);

    cout << "Enter the number of lines and press ENTER (must be an integer): ";
    cin >> nlines;
    while (nlines < 0)
    {
        cout << "Error: Number of absorption lines must be non-negative. (Zero for blank spectrum): ";
        cin >> nlines;
    }
    // FLUSH BUFFER IN CASE USER ENTERS A FLOATING POINT VAL
    fflush(stdin);

    return;
}