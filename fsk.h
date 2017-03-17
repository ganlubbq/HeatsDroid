#ifndef FSK_H
#define FSK_H

#include <math.h>
#include <QtWidgets>//para abrir ventanas de diálogo, //define malloc

void polygonal (double y [], double *threshold, double x [], double f0, double f1, double baud_rate, double fm, int N);
void HL_derivator (double y [], double *threshold, double x [], double amp, double f0, double f1, double baud_rate, double fm, int N);
bool balanced_quadricorrelator (double y [], double *threshold, double x [], double f0, double f1, double fm, int N);
bool quadricorrelator (double y [], double* threshold, double x [], double f0, double f1, double fm, int N);
bool detector_envolvente (double* &y, double *threshold, double x [], double signal_amp, double f0, double f1, double fm, int N, int selection);
bool detector_envolvente_MFSK(double* &y, int* &symbols, double x [], double baud_rate, int *freq_array, int fft_length, double fm, int N, int selection,int M);

#endif // FSK_H
