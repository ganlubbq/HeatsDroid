#ifndef FILTERS_H
#define FILTERS_H

#include <QtWidgets>//para abrir ventanas de diálogo, //define malloc

//filtrado
void filter_IIR_FIR(double* &out, const double *in, const int length, const double* b, const double* a, const int b_length, const int a_length);
double* filter(double* entrada, double* hn, int length, int M, int veces);//veces=0 para filtrar los ceros, veces=1 para filtrar los polos
double* filter(double* entrada, double* hn, int N, int M);
void iir_df1(int q, int p, double b[], double a[], int N, double x[], double *y);

//filtro Kaiser
double* filter_design_direct_I(int M, double Beta, double wc1, double wc2);
double izero(double y);
double sinc(double x);

//Filanovsky
#define max(a,b) (((a)>(b))?(a):(b))
#define min(a,b) ((a<b)?a:b)
double s_to_z
(
        double a[], // s domain coefficients
        int N,      // index of last coefficient (order)
        int n,      // z domain coeffient desired
        double fs   // sample rate
        );
static double f(double i);// simple factorial function
double *conv(double *A, double *B, int lenA, int lenB, int *lenC);//convolution algorithm
void filter_design_filanovsky(double ro, double fc, double fs, double* z_num, double* z_den);//Función para diseñar el LPF por el método de Filanovsky

#endif // FILTERS_H
