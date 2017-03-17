#ifndef FFT_H
#define FFT_H



/* FFT base-2 de una señal real utilizando el diezmado en frecuencia. */

#include <stdio.h>//define malloc
#include <math.h>

/*---------------------------------------------------------------*/
typedef struct cmpx			// Definición de número complejo.
{
    double real;
    double imag;
    cmpx(){}
    cmpx( double nreal , double nimag )
    :real(nreal) , imag(nimag){}

    double abs()
    {
        return sqrt(real*real + imag*imag);
    }
} COMPLEX;

/* --- Prototipo de las funciones --- */

int potencia2(double);                  	// verificar que la cantidad de muestras es base 2.
COMPLEX * FFT(COMPLEX [], int, int);		// función principal (algoritmo de la FFT).
COMPLEX * bitRev(COMPLEX [], int);     		// reorganizar la secuencia en BRO
COMPLEX MULTIPLICAR(COMPLEX , COMPLEX);     // multiplicar dos números complejos.
COMPLEX SUMAR(COMPLEX , COMPLEX);           // sumar dos números complejos.
COMPLEX RESTAR(COMPLEX , COMPLEX);          // restar dos números complejos.
/*---------------------------------------------------------------*/

#endif // FFT_H

/*Spectrograma*/
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define MAX 1000001
#define BIZ_EPSILON 1E-21
typedef double _ftype_t;


//Ventanas
void boxcar(double *spect_window, int n);
void triang(double *spect_window, int n);
void hanning(double *spect_window, int n);
void hamming(double *spect_window, int n );
void blackman(double *spect_window, int n );
void flattop(double *spect_window, int n );
_ftype_t besselizero(_ftype_t x);
void kaiser(double *spect_window, int n , _ftype_t b );

vector <COMPLEX> GET_FFT( vector <double> signal_in );
vector <double> ABS( vector <COMPLEX> signal_in );

//función espectrograma
double* get_FFT(double *signal_in, int signal_fft_length, int window);
double **spectrogram(double *signal_in, int signal_length, int window, int window_length, int solapamiento, int step, int signal_spect_length, double* max);//solapamiento en % e.g.0,10,15, etc

void get_spectrograma( vector <double> signal_in , int lenght , int window , int wind_size, double beta , int step , double spect[5001][5001]);


