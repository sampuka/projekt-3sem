#ifndef RC_FFT_H
#define RC_FFT_H

#include <complex>
#include <iostream>
#include <valarray>

const double PI = 3.141592653589793238460;
 
typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

void fft(CArray& x);
void applyHammingWindow(CArray& x);

#endif
