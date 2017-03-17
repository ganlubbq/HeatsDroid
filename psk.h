#ifndef PSK_H
#define PSK_H

#include <math.h>
#include <QtWidgets>//para abrir ventanas de diálogo, //define malloc
#include"fsk.h"

///Lazo de Costas
//Constelación
bool psk_costas_loop(double* &signal_i_avg, double* &signal_q_avg, int *signal_I_length, double *signal_in,  int NDATA, double fcarrier, double baud_rate, double fs,double kc);
//void psk_costas_loop(double *signal_in,  int NDATA, double fcarrier, double baud_rate, double fs,double kc, double *signal_i_avg, double *signal_q_avg);//prototipo de la funci�n
void  calcu_fir2(double  *bb ,int M1, double  baud_rate,double fs);
double firI( double *b, int ncoef, double *w,  double x );
double firQ( double *b, int ncoef, double *w,  double x );
double firlazo ( double *b, double *a, double *w, double x   );
void vco( double *xcos,double *xsin, double error,  double *W, double fcentral, double fs);
double delay_I( double x, double *W);
double delay_Q( double x, double *W);
double valor_abs(double x);
double signo(double x);

//Demodulación
void dem_costas_BPSK(double* signal_dem, double *signal_dem_threshold, double *signal_in, double kc, double f0, double baud_rate, double fm, int N);
void dem_costas_QPSK (double* &signal_I, double* &signal_Q,int* signal_I_length, int* &symbols_dem, double* signal_in, int signal_length, double signal_baud_rate, double signal_fm, double f0);
void dem_costas_OQPSK(double* &signal_I, double* &signal_Q, int* signal_I_length, int* &symbols_dem, double* signal_in, int signal_length, double signal_baud_rate, double signal_fm, double f0);
void dem_costas_8PSK (double* &signal_I, double* &signal_Q,int* signal_I_length, int* &symbols_dem, double* signal_in, int signal_length, double signal_baud_rate, double signal_fm, double f0);
double firI(int M, double *b, double *w,  double x);
double firQ(int M, double *b, double *w,  double x);
//double firlazo(double *b, double *a, double *w, double x);
void vco(double *, double * , double , double , double *, double , int );
//double delay_Q( double , double *);
//double delay_I( double , double *);
void fir_coef(double *, int , double );


// funciones para el sincronismo de bit
double  cal_dte2(char , double, double, double*, double*   );	// calculo del error
double  fil_loop(char, double ,double*);   // calculo del error filtrado
double synchro_nco(double dtefil, double* reg, double sam_symbol); // generador de reloj sincr�nico
double  synchro_nco( double  ,double*);

//   funci�n espec�fica para OQPSK
double delay_tbit( double , double *);

// funci�n para demodular  8PSK
// dada cada una de las se�ales en cuadratura I, Q calcula el nivel de umbral


double  calcula_umbral( double * ,  double);

///Square Loop
//Constelación
bool constelation_square_loop_BPSK (double* &signal_i_avg, double* &signal_q_avg,int* signal_I_length,double signal_in [], double f0, double baud_rate, double fm, int N);
bool constelation_square_loop_QPSK (double* &signal_i_avg, double* &signal_q_avg,int* signal_I_length,double signal_in [], double f0, double baud_rate, double fm, int N);

//Demodulación

bool dem_quadratic_BPSK(double* signal_dem, double *signal_dem_threshold, double *signal_in, double f0, double baud_rate, double fm, int N);

void filter_design_coef_num_notch_BPF(double frec_signal, double *coef_num_notch_bpf, double r, int fm);

void filter_design_coef_den_notch_BPF(double frec_signal, double *coef_den_notch_bpf, double r, int fm);

// Diseño de los coeficientes del filtro de Lazo_Cuadratico
void filter_design_coef_lazo(double frec_signal, double *a, double *b, double chi, double lock_time, int fm);

// Diseño de los coeficientes del filtro notch pasabajo
void filter_design_coef_num_notch_LPF(double frec_signal, double *coef_num_notch_lpf, double r1, int fm);

void filter_design_coef_den_notch_LPF(double frec_signal, double *coef_den_notch_lpf, double r1, int fm);

#endif // PSK_H
