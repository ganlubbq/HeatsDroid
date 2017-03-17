#include<fsk.h>

/* ED using noise floor estimation in signal presence */
void BubbleSort(double pdg_array[], int wind_quant);
int * detect_ED(double * signal_fft, int cant_muest, int* freq_length);

int * detect_FB(double * signal_in, int signal_fm, int signal_length, int signal_fft_length, int *detect_freq_length);
double* filter_design_direct_I(int M, double Beta, double wc1, double wc2);
double izero(double y);
double sinc(double x);
void find_min_max(double energy[], double min_max[], int n);
void energy_detection(double y[], int band, double energy[], int cant_muest);
void convolve(double p_coeffs[], int p_coeffs_n, double p_in[], double p_out[], int n);
