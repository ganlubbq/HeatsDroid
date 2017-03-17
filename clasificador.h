#ifndef CLASIFICADOR_H
#define CLASIFICADOR_H

#endif // CLASIFICADOR_H

//#include "clasificador.cpp"

#include "fft.h"
#include "qmath.h"
//#include "fsk.h"

//Estimación
double EstimaRs(double *signal_in, int size_signal, double fo,int fm);
void PLL_operation(double *, double *, int,double, int);
double mean(double *, int);
void Halla_modulo_complex(COMPLEX *,double *,int);
double Histograma(QVector<double>, int, QVector<double> &, QVector<double> &,int);
double estima_SNRdB(const double *signal_fft, const int freq_length);


//Clasificador árbol de decisiones
int clasif_arbol_decisiones(double *signal_in, int size_signal, int fm, double Rs);             //Llamar a esta funcion
void nco3 (double *,double *,double *,double ,double ,double ,double);
double delay (double , double *);
void myFir (double *, int , double*, double *, double, double);
void inicializaArrayZero (double *, int );
int sign (double );
void conformaSymbolChange(int *, int , double );
double sum (double *, int , int );
double iirlp(double *, double *, double *, double *, double );
int numberStates (QVector<double> ,int );
void Histograma(double *, int , QVector<double> &, QVector<double> &,int );
double estimaFc0 (double *, int, int);
double estimaFcFFT4 (double *,  int , int, int);
double normalizaAmp(double *, int );
void absValue (double *,COMPLEX  *, int );
void limitaSignal_in (double *, int , double *, int );
void costasLoop (double *, int , double , int ,double *,double *);
void promSymbol (int , int , double , double *, double *,double *);
int ampStates (double *, double *, int);
double searchFFT_Umbral(double *,int );
int freqStates (double *, int , int, float );
int clasificaPSKQAM(double *, int , double , int );


//Clasificación de cruces por cero
int clasif_crucesporcero_funct(double *signal_in, int signal_length, int signal_fm);
