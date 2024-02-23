/*
File Name: main.cpp
Created by: Connor Fricke
Created on: 12/5/2023
Latest Revision: 12/3/2023
Synopsis: main implementation for stellar absorption spectra program. Executable takes in user-entered values corresponding to the interval of the spectrum in wavelength (angstroms), the resolution of the resulting spectral plot, and the number of spectral absorption lines, which have uniformly distributed locations and normally distributed depths. This algorithm was initially implemented in a Jupyter Notebook using the Python language, then translated to C++ for optimization benchmarking.
Contributors: Prof. Donald Terndrup, Nathan Holty for completed Python implementation

Command Line Entry:
Compiled in Windows Powershell:
    g++ main.cpp -o genspec.exe -I C:\Python312\include -I include -I C:\Python312\Lib\site-packages\numpy\core\include -L C:\Python312\libs -lpython312
    
Once the program is running (GenSpec.exe), it will prompt you for several entries. These entries are explained with a CMD terminal example provided below.

Terminal:
Please enter your data file name: myData.tsv
# The above line will eventually be used to give functionality to a data file parser. Currently, instead of getting real line locations and depths, they are
# generated uniformly and normally
Please enter your spectrum name: spectrum2
# The above line does not require a file extension type (.txt, .png, etc.) it simply allows you to specify the name of the file, which will then automatically be
# saved in .png, .jpg, and .pdf formats within the saved_spectra directory.
Enter wavelength interval start and press ENTER: 900
# The above line can be a floating point value or an integer. It specifies the leftmost value on the x axis of the resulting plots.
Enter wavelength interval end and press ENTER: 2900
# The above line specified the rightmost value of the x-axis. It can also be integer or floating point type.
Enter resolution and press ENTER: 2048
# The above line tells the C_linspace() function how many points to generate, which specifies the size of our vectors where we store our spectra. Should be an int
Enter the number of lines and press ENTER (must be an integer): 99
# Specify the number of absorption lines to add to the blank spectrum, which have depths and locations that are normally distributed and uniformly distributed,
# respectively.
CPU Time: 22 ticks, or 0.022 seconds.
Generating files...
Done!
*/

#include "matplotlibcpp.h"
#include <cmath>
#include <random>
#include <ctime>

namespace plt = matplotlibcpp;
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
gaussian(double, double, couble, double):
params:
    double x - corresponding position along the x-axis
    double center - mean of gaussian distribution
    double N - normalization constant of function
    double - width of gaussian distribution
return:
    double y - corresponds to the value y = G(x) where G(x) is the well known Gaussian distribution function.
synopsis: implementation of the well-known Gaussian function, y = N*exp(-0.5 * (x-u)^2)
*/
double gaussian(double w, double center, double depth, double sigma);

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

/*
normal(double, double, int, vector<double>&):
params:
    double mean - center of normal distribution
    double width - width of distribution
    int SIZE - size of the resulting vector
    vector<double>& output - passed by reference vector of size SIZE which stores values generated according to the normal distribution
return:
    none
synopsis:
    this function is used to generate a container of values which are generated randomly using the <random> library and a random_device seed, an mt19937 generator, and the normal_distribution<double> distribution. The resulting values are expected to be distributed according to the normal distribution with a width and mean defined by the parameters mean and width
*/
void normal(double mean, double width, int SIZE, vector<double>&);

/*
uniform(double, double, int, vector<double>&):
params:
    double start - lower limit of interval over which numbers may be generated
    double end - upper limit of interval over which numbers may be generated
    int - size of the resulting vector
    vector<double>& output - passed by reference vector of size SIZE which stores uniformly distributed values
return:
    none
synopsis:
    this function is used to generate a container of values which are generated randomly using the <random> library, including a random_device seed, an mt19937 generator, and the uniform_real_dstribution from the library. Resulting values are expected to be distributed normally, with upper and lower limits of their possible values set by the first two parameters passed to the function, respectively.
*/
void uniform(double start, double end, int SIZE, vector<double>&);
// END OF FUNCTION HEADERS

int main()
{
    // PROMPT FOR DATAFILE_NAME and SPECTRUM_NAME
    string DataFile_Name, Spectrum_Name;
    getFileIOName(DataFile_Name, Spectrum_Name);


    // PROMPT FOR SPECTRUM LIMITS, RESOLUTION, & LINES
    double start(0), end(0);
    int nspect(0), nlines(0);
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

        for (int j = 0; j < wavelengths.size(); j++)
        {
            double w = wavelengths[j];
            double d = -gaussian(w, linecenter, linedepth, LSIGMA);
            line_spectrum[j] += d;
        }

        // MULTIPLY THE SPECTRA
        for (int k = 0; k < total_spectrum.size(); k++)
        {
            total_spectrum[k] = total_spectrum[k] * line_spectrum[k];
        }

    }

    // STOP CLOCK (we stop here because the following code is actually from a C++ header file which acts as a wrapper for a python library, thus it's performance is not indicative of the C++ language for a comparison with Python)
    // t is the number of ticks that have passed since the clock was started
    t = clock() - t;

    // DISPLAY RUNNING TIME RESULTS
    cout << "CPU Time: " << t << " ticks, or " << (float)t / CLOCKS_PER_SEC << " seconds." << endl;

    // USE MATPLOTLIBCPP TO CREATE PLOT FIGURE
    plt::plot(wavelengths, total_spectrum, "purple" /*format*/);
    plt::title("Resulting Spectrum");
    plt::xlabel("Wavelength (Angstrom)");
    plt::ylabel("Spectrum");

    // SAVE RESULTING SPECTRUM AS .PNG, .JPG, AND .PDF FILES
    // NOT SURE WHY THIS DOESN'T WORK ANYMORE
    /*
    cout << "Generating files...\n";
    plt::save("saved_spectra\\" + Spectrum_Name + ".png");
    plt::save("saved_spectra\\" + Spectrum_Name + ".jpg");
    plt::save("saved_spectra\\" + Spectrum_Name + ".pdf");
    cout << "Done!\n";
    */
    plt::show();

}

void getFileIOName(string& fileinput, string& fileoutput)
{
    cout << "Please enter your data file name: (NOT CURRENTLY USED)";
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

double gaussian(double x, double center, double N, double sigma)
{
    // y = A*exp(-0.5 * (x-u)^2) decribes the gaussian or "normal" distribution function, which resembles the well known bell curve when plotted over a range x
    double y = N * exp(-0.5 * pow(x - center, 2) / pow(sigma, 2));
    return y;
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

void normal(double mean, double width, int SIZE, vector<double>& output)
{
    random_device rd;
    mt19937 generator(rd());
    normal_distribution<double> distribution(mean, width);

    for (int i = 0; i < SIZE; i++)
    {
        output.push_back(distribution(generator));
    }
}

void uniform(double start, double end, int SIZE, vector<double>& output)
{
    random_device rd;
    mt19937 generator(rd());
    uniform_real_distribution<double> distribution(start, end);

    for (int i = 0; i < SIZE; i++)
    {
        output.push_back(distribution(generator));
    }
}