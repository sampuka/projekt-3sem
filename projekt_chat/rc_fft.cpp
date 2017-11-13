#include "rc_fft.hpp"

// Windows autism
#define _USE_MATH_DEFINES
#include "math.h"
 
// Cooley–Tukey FFT (in-place, divide-and-conquer)
// Higher memory requirements and redundancy although more intuitive
void fft(CArray& x)
{
    const size_t N = x.size();
    if (N <= 1) return;
 
    // divide
    CArray even = x[std::slice(0, N/2, 2)];
    CArray  odd = x[std::slice(1, N/2, 2)];
 
    // conquer
    fft(even);
    fft(odd);
 
    // combine
    for (size_t k = 0; k < N/2; ++k)
    {
        Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
        x[k    ] = even[k] + t;
        x[k+N/2] = even[k] - t;
    }
}

//https://en.wikipedia.org/wiki/Window_function#Hamming_window
void applyHammingWindow(CArray& x)
{
    int len = x.size();
    for(int i = 0; i < len; i++)
	x[i] = (0.54-0.46*cos(2*M_PI*i/(len-1)))*x[i];
}
