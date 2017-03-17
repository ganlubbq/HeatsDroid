#ifndef NUCLEO_H
#define NUCLEO_H
#include <QFileInfo>//para QIODevice
#include <QtWidgets>//para abrir ventanas de diálogo
#include <QDebug>
#include <qmath.h>
//#include "nucleo.cpp"
#include "clasificador.h"
#include "fft.h"
#include "patronencontradoraster.h"
#include "patron.h"

class Nucleo
{
public:
    Nucleo();
    ~Nucleo();
    static Nucleo *nucleo_instance();

public slots:

    //Sistema
    bool nucleo_comenzar_funct(QIODevice &device);//rellena los datos de la señal, calcula fft y espectrograma, devuelve false si no se completó la lectura
    void nucleo_clear_funct(void);//para borrar todo el arreglo de señales

    //Tiempo
    int signal_length;
    double* signal_in;
    double** signal_in_array;
    int signal_in_array_length;
    quint32 signal_fm; //frecuencia de muestreo
    int signal_hora;
    int signal_minutos;
    int signal_seg;  
    float signal_min;
    float signal_max;
    //float baud_rate;

    //Frecuencia
    double* signal_fft;    
    int signal_fft_length;
    float signal_fft_min;
    float signal_fft_max;
    float signal_fft_max_2;//para el caso de una señal BFSK
    int signal_fft_max_pos;
    int signal_fft_max_pos_2;//para el caso de una señal BFSK
    int signal_fft_max_pos_const;//se determina el valor de frecuencia para la constelación
    void nucleo_fft_funct(void);//calcula la FFT de signal_in

    //Espectro
    double **signal_spect;
    double signal_spect_length;
    double spect_max;
    double spect_window_length;
    int spect_window_solapamiento;
    double spect_window;
    void nucleo_spect_funct(void);

    //Detección
    int *detect_freq;
    int detect_freq_length;
    int nucleo_filtrar_funct(int index);
    void nucleo_detect_funct(int method);
    void nucleo_fft_array_funct(int index);

    //Estimación, 1-BFSK, 2-4FSK, 3-8FSK, 4-16FSK, 5-BPSK, 6-QPSK, 7-8PSK, 8-QAM
    int clasif_decision;
    double signal_amp;
    double signal_baud_rate;
    double signal_SNR;
    double *signal_amp_array;
    double *signal_baud_rate_array;
    double *signal_SNR_array;
    void nucleo_estimacion_param(void);//estima amplitud, SNR y baud_rate de la señal

    void nucleo_clasif_funct(int metodo);

    //Constelación
    double *signal_I;
    double *signal_Q;
    int signal_I_length;
    double signal_I_max;
    double signal_I_min;
    double signal_Q_max;
    double signal_Q_min;
    bool nucleo_constelacion_funct(int index);//1-BFSK, 2-4FSK, 3-8FSK, 4-16FSK, 5-BPSK, 6-QPSK, 7-8PSK, 8-QAM

    //Demodulación
    bool nucleo_dem_funct(int index);//retorna un cero si no se pudo demodular correctamente
    double *signal_dem;
    int signal_dem_length;
    int *symbols_dem;
    int symbols_dem_length;
    double signal_dem_threshold;
    bool *bits;
    int bits_length;
    bool nucleo_raster_funct(void);//determina los bits a partir de los niveles demodulados
    int patron;
    int *dem_freq_array;//almacena en enteros los valores de frecuencia de 4FSK o 8FSK en relación a la longitud del arreglo de FFT
    QVector<int> patrones;
    void nucleo_patron_raster_funct(void);//determina los patrones encontrados a partir de la secuencia de bits
    int code_dem;//código para la demodulación M-ary
    double dem_amp_max;//determina el máximo de la amplitud de la señal demodulada
    double dem_amp_min;//determina el mínimo de la amplitud de la señal demodulada
    double *signal_I_dem;
    double *signal_Q_dem;
    int signal_I_dem_length;
    double signal_I_dem_max;
    double signal_I_dem_min;
    double signal_Q_dem_max;
    double signal_Q_dem_min;



private:




};

#endif // NUCLEO_H
