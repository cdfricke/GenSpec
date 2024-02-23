#ifndef RNG_H
#define RNG_H

#include <cmath>
#include <random>
#include <vector>

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
void normal(double mean, double width, int SIZE, std::vector<double> &);

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
void uniform(double start, double end, int SIZE, std::vector<double> &);

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

#endif