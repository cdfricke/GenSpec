#include "RNG.h" // also includes <cmath>, <random>, <iostream>

using namespace std;

// **********************************************************************
void normal(double mean, double width, int SIZE, vector<double> &output)
{
    random_device rd;
    mt19937 generator(rd());
    normal_distribution<double> distribution(mean, width);

    for (int i = 0; i < SIZE; i++)
    {
        output.push_back(distribution(generator));
    }
}

// **********************************************************************
void uniform(double start, double end, int SIZE, vector<double> &output)
{
    random_device rd;
    mt19937 generator(rd());
    uniform_real_distribution<double> distribution(start, end);

    for (int i = 0; i < SIZE; i++)
    {
        output.push_back(distribution(generator));
    }
}

// ***********************************************************************
double gaussian(double x, double center, double N, double sigma)
{
    // y = A*exp(-0.5 * (x-u)^2) decribes the gaussian or "normal" distribution function, which resembles the well known bell curve when plotted over a range x
    double y = N * exp(-0.5 * pow(x - center, 2) / pow(sigma, 2));
    return y;
}