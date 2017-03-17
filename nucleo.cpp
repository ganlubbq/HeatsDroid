#include "nucleo.h"
#include "clasificador.cpp"
#include "fft.cpp"
#include "psk.h"
#include "fsk.h"
#include "raster.h"
#include "patronencontradoraster.cpp"
#include "patron.cpp"
#include "detection.h"
#include<qmath.h>


Nucleo::Nucleo()
{
    //Tiempo
    signal_in=NULL;
    signal_length=0;
    signal_in_array=NULL;
    signal_in_array_length=0;
    signal_fm=0;
    signal_hora=0;
    signal_minutos=0;
    signal_seg=0;   
    signal_max=0;
    signal_min=0;
    signal_baud_rate=0;

    //Frecuencia
    signal_fft=NULL;    
    signal_fft_length=0;
    signal_fft_max=0;
    signal_fft_max_2=0;
    signal_fft_min=0;
    signal_fft_max_pos=-1;
    signal_fft_max_pos_2=-1;
    signal_fft_max_pos_const=-1;

    //Spectrogram
    signal_spect=NULL;
    signal_spect_length=0;
    spect_max=0;
    spect_window_length=0;
    spect_window_solapamiento=0;
    spect_window=0;


    //Detección
    detect_freq=NULL;
    detect_freq_length=0;


    //Estimación
    clasif_decision=0;
    signal_amp=0;
    signal_amp_array=NULL;
    signal_baud_rate=0;
    signal_baud_rate_array=NULL;
    signal_SNR=0;
    signal_SNR_array=NULL;

    //Constelación
    signal_I=NULL;
    signal_Q=NULL;
    signal_I_length=0;
    signal_I_max=0;
    signal_I_min=0;
    signal_Q_max=0;
    signal_Q_min=0;

    //Demodulación
    signal_dem=NULL;
    signal_dem_length=0;
    symbols_dem=0;
    symbols_dem_length=0;
    signal_dem_threshold=0;
    bits=NULL;
    bits_length=0;
    patron=0;
    dem_freq_array=NULL;
    code_dem=0;
    dem_amp_max=0;
    dem_amp_min=0;
    signal_I_dem=NULL;
    signal_Q_dem=NULL;
    signal_I_dem_length=0;
    signal_I_dem_max=0;
    signal_I_dem_min=0;
    signal_Q_dem_max=0;
    signal_Q_dem_min=0;
}

Nucleo::~Nucleo()
{

    if(signal_I_length!=0)
    {
        delete[] signal_I;
        delete[] signal_Q;
    }

    if(signal_dem_length!=0)
    {
        delete[] signal_dem;
    }

    if(symbols_dem_length!=0)
    {
        delete [] symbols_dem;
    }

    if(bits_length!=0)
    {
        delete[] bits;
    }

    if(detect_freq_length!=0)
    {

        delete[] detect_freq;
        delete[] signal_amp_array;
        delete[] signal_baud_rate_array;
        delete []signal_in_array;
        signal_in_array=NULL;        
        delete []signal_SNR_array;
        signal_SNR_array=NULL;

    }    

    if(signal_spect_length!=0)
    {

        delete[] signal_spect;
    }

    if(signal_length!=0)
    {
        delete[] signal_in;
        delete[] signal_fft;
    }


    if(clasif_decision==2 || clasif_decision==3 || clasif_decision==4)
    {
        delete [] dem_freq_array;
    }

    if(signal_I_dem_length!=0)
    {
        delete []signal_I_dem;
        delete []signal_Q_dem;
        signal_I_dem_length=0;
        signal_I_dem_max=0;
        signal_I_dem_min=0;
        signal_Q_dem_max=0;
        signal_Q_dem_min=0;
    }

}

//lee el fichero .wav para extraer los valores de la señal
bool Nucleo::nucleo_comenzar_funct(QIODevice &device)
{
    //se borra el anterior arreglo de señales en la etapa de detección
    nucleo_clear_funct();

    //se lee el archivo seleccionado
    QByteArray buffer = device.readAll();
    //se cierra
    device.close();
    //este while verifica si el archivo cargado cuenta con la secuencia 'WAVE'
    int c = 0;
    bool test_wav = false;
    while (!test_wav && c < buffer.size())
    {
        switch(buffer[c])
        {
        case'W':test_wav = true;
            break;
        default:c++;
        }
    }

    if (test_wav)
    {
        if (buffer[c + 1] != 'A' && buffer[c + 2] && 'V' && buffer[c + 3] != 'E' )
        {
            return false;
        }
    }

    if(test_wav==false)
    {
        return false;
    }



    test_wav = false;
    c = 0;
    //este while verifica si el archivo cargado cuenta con la secuencia 'fmt '
    while (!test_wav && c < buffer.size())
    {
        switch(buffer[c])
        {
        case'f':test_wav = true;
            break;
        default:c++;
        }
    }
    if (test_wav)
    {
        if (buffer[c + 1] != 'm' && buffer[c + 2] && 't' && buffer[c + 3] != ' ' )
        {
            return false;
        }
    }



    test_wav = false;
    c = 0;
    //este while verifica si el archivo cargado cuenta con la secuencia 'data'
    while (!test_wav && c < buffer.size())
    {
        switch(buffer[c])
        {
        case'd':test_wav = true;
            break;
        default:c++;
        }
    }



//    if (test_wav)
//    {
//        if (buffer[c + 1] != 'a' && buffer[c + 2] && 't' && buffer[c + 3] != 'a' )
//        {
//            return false;
//        }
//    }



    c = 4;

    //se leen los datos del encabezado del archivo
    int chunkSize=0; //Cantidad de bytes que hay despues de leer los primeros 2 bytes
    chunkSize = (((buffer[c + 3] & 0xff) << 24) | ((buffer[c + 2] & 0xff) << 16) | ((buffer[c + 1] & 0xff) << 8) | (buffer[c] & 0xff));

    int subchunk1Size=0; //Cantidad de bytes que hay despues de leer los primeros 5 bytes
    subchunk1Size = (((buffer[19] & 0xff) << 24) | ((buffer[18] & 0xff) << 16) | ((buffer[17] & 0xff) << 8) | (buffer[16] & 0xff));

    quint16 formatoAudio=0;//formato de audio
    formatoAudio = ((buffer[21] & 0xff) << 8) | (buffer[20] & 0xff);

    quint16 numeroCanales=0;//número de canales
    numeroCanales = ((buffer[23] & 0xff) << 8) | (buffer[22] & 0xff);


    signal_fm = (((buffer[27] & 0xff) << 24) | ((buffer[26] & 0xff) << 16) | ((buffer[25] & 0xff) << 8) | (buffer[24] & 0xff));


    quint32 muestreoDeByte; //sampleRate * numChannels * BitsPerSample / 8
    muestreoDeByte = (((buffer[31] & 0xff) << 24) | ((buffer[30] & 0xff) << 16) | ((buffer[29] & 0xff) << 8) | (buffer[28] & 0xff));

    quint16 alineacionBloques;//numChannels * BitsPerSample / 8
    alineacionBloques = ((buffer[33] & 0xff) << 8) | (buffer[32] & 0xff);

    quint16 bitsPorMuestra=0; //Bits por muestra
    bitsPorMuestra = ((buffer[35] & 0xff) << 8) | (buffer[34] & 0xff);

    c = 35;
    test_wav = false;
    //se busca la posicion de la "d" de 'data'
    while (!test_wav)
    {
        switch(buffer[c])
        {
        case'd':test_wav = true;
            break;
        default:c++;
        }
    }

    //se supone de que despues de 'data', venga la cantidad de muestras
    c+=4;
    signal_length = (((buffer[c + 3] & 0xff) << 24) | ((buffer[c + 2] & 0xff) << 16) | ((buffer[c + 1] & 0xff) << 8) | (buffer[c] & 0xff));

    if (bitsPorMuestra == 16)
        signal_length /= 2;


    //si hay menos de 1024, no se carga la senal
    if (signal_length < 1024)
        return false;

    //a partir del valor que toma "c", se empiezan a leer las muestras
    c+=4;

    //se almacena el área de memoria
    try
    {
        signal_in = new double[signal_length];
    }
    catch(std::bad_alloc&)
    {        
        return false;
    }


    //se lee la señal, se busca máximo y mínimo, se comprueba si es todo cero las señal
    int total_null=0;
    int inicio = c;
    signal_min=0;
    signal_max=0;

    for (int i = 0; i < signal_length; i++)
    {
        switch (bitsPorMuestra)
        {
            case 16:
            {
                int xl = (int)buffer[inicio] & 0xFF;
                inicio++;
                int xh = (int)buffer[inicio] << 8;
                inicio++;
                int concatenation = xh + xl;
                signal_in[i] = 1.0*concatenation/32768.0;
                //se buscan los puntos cero
                if(signal_in[i]==0)
                    total_null+=1;
                if(signal_in[i]>signal_max)//se busca el máximo
                {
                    signal_max=signal_in[i];
                }
                if(signal_in[i]<signal_min)//se busca el mínimo
                {
                    signal_min=signal_in[i];
                }

            }
            break;
            case 8:
            {
                int x = (int)buffer[inicio];
                float percent = -(float)x/128.0;
                if(percent > 0)
                {
                    signal_in[i] = 1 - percent;
                    //se buscan los puntos cero
                    if(signal_in[i]==0)
                        total_null+=1;

                    if(signal_in[i]>signal_max)//se busca el máximo
                    {
                        signal_max=signal_in[i];
                    }
                    if(signal_in[i]<signal_min)//se busca el mínimo
                    {
                        signal_min=signal_in[i];
                    }
                }
                else
                {
                    signal_in[i] = -(1 + percent);

                    //se buscan los puntos cero
                    if(signal_in[i]==0)
                        total_null+=1;

                    if(signal_in[i]>signal_max)//se busca el máximo
                    {
                        signal_max=signal_in[i];
                    }
                    if(signal_in[i]<signal_min)//se busca el mínimo
                    {
                        signal_min=signal_in[i];
                    }
                }
                inicio++;
            }
            break;
        }
    }

    //se invalida el procesamiento de la señal si es todo cero
    if(total_null==signal_length)
    {
        signal_length=0;
        delete [] signal_in;
        signal_in=NULL;
        return false;
    }

    float seg=0;
    seg=float(signal_length)/float(signal_fm);


    signal_hora=int(seg/60/60);
    signal_minutos=int(seg/60-signal_hora*60);
    signal_seg=int(seg-signal_minutos*60-signal_hora*60*60);

    buffer.clear();
    return true;

}

///espectro
void Nucleo::nucleo_fft_funct(void)
{
    //se borra la fft y se reinician variables
    if(signal_fft_length!=0)
    {
        delete []signal_fft;
    }

    signal_fft_length=0;
    signal_fft_max_pos=0;
    signal_fft_min=0;
    signal_fft_max=0;

    signal_fft_max_2=0;//se reinicia el valor del 2do máximo

    int   v;  	//potencia de la señal que se recibe de entrada
    int   k, inverse = 1;	// inverse = 1 indica que se calcula la FFT,
                                                // inverse = -1 es que se calcula la iFFT.    

    v=qLn(signal_length)/M_LN2;
    signal_fft_length=qPow(2,v);

    COMPLEX * Xcmpx = (COMPLEX *)malloc(signal_fft_length*sizeof(COMPLEX));
    COMPLEX * Y;

    int step=double(signal_length)/double(signal_fft_length);

    for (k=0; k<signal_fft_length; k++)  				// inicializo X como COMPLEX de parte imaginaria cero.
    {
        Xcmpx[k].real=signal_in[k*step];
        Xcmpx[k].imag=0;
    }


    if(signal_fft_length != 1)
    {
        Y = FFT(Xcmpx, signal_fft_length, inverse);  	// se llama a la función principal.
        Xcmpx = bitRev(Y, signal_fft_length);

        //se copia el espectro unilateral
        signal_fft_length=signal_fft_length/2;
        signal_fft = new double[signal_fft_length];
        //se determina la norma y el máximo
        for(int i=0; i<signal_fft_length; i++)
        {
            signal_fft[i]=qSqrt(Xcmpx[i].real*Xcmpx[i].real+Xcmpx[i].imag*Xcmpx[i].imag);
            if(signal_fft_max<signal_fft[i])//se calcula el máximo y la posición de la FFT
            {
                signal_fft_max=signal_fft[i];
                signal_fft_max_pos=i;

            }
            if(signal_fft_min>signal_fft[i])
                signal_fft_min=signal_fft[i];
        }


        free(Xcmpx);		// se libera la memoria ocupada.


        }
        else
        {
            QWidget *widget = new QWidget;
            QMessageBox::information(widget, "Mensaje", "La FFT no es potencia de 2");

        }

}

void Nucleo::nucleo_spect_funct()
{    

    int step=double(spect_window_solapamiento)/double(100)*double(spect_window_length);
    //int step=signal_length/400;
    //spect_window_solapamiento=double(step)*double(100)/double(spect_window_length);
    signal_spect_length=double(signal_length-spect_window_length)/double(step);

    signal_spect=spectrogram(signal_in, signal_length, spect_window, spect_window_length, spect_window_solapamiento, step, signal_spect_length, &spect_max);

}

///Estimación
void Nucleo::nucleo_clasif_funct(int metodo)
{    
    if(metodo==0)//árbol de decisiones
    {
        int p1=0;//para acotar el análisis

        if(signal_length>1500000)
            p1=signal_length/4;
        else
            p1=signal_length;


        int decision=clasif_arbol_decisiones(signal_in,p1,signal_fm,signal_baud_rate);

        //devuelve x>2 estados de frequencia detectados 2->BFSK 3,4,5->4FSK  6,7,8...->8FSK
        // devuelve 1 si QAM o 20 + phase_states si PSK dividir % 20 y resto (resultado) 1,2->PSK; 3,4,5->QPSK; 6,7,8...->8PSK
        if(decision==2)//BFSK
            clasif_decision=1;
        if(decision>=3 && decision<=5)//QFSK
            clasif_decision=2;
        if(decision>5 && decision<20)//8FSK
            clasif_decision=3;
        if(decision==21 || decision==22)//BPSK
            clasif_decision=5;
        if(decision==23 || decision==24 || decision==25)//QPSK
            clasif_decision=6;
        if(decision>25)//8PSK
            clasif_decision=7;
        if(decision==1)//QAM
            clasif_decision=8;
    }
    if(metodo==1)//cruces por cero, clasifica entre BFSK y BPSK
    {
        int p1=signal_length/4;
        clasif_decision=clasif_crucesporcero_funct(&signal_in[p1],signal_length/4,signal_fm);
    }
}

bool Nucleo::nucleo_constelacion_funct(int index)
{

    //Para la constelación
    //se detecta un nuevo valor de frecuencia por el momento de 4to orden de la FFT
    //se crea el eje de frecuencia
    double *freq_x = new double[signal_fft_length];
    for(int i=0; i<signal_fft_length; i++)
    {
        freq_x[i]=double(i)*double(signal_fm/2)/double(signal_fft_length);
    }
    //se calcula el momento de 4to orden
    double cumul_f_num=0;
    double cumul_f_den=0;
    for(int i=0; i<signal_fft_length; i++)
    {
        cumul_f_num+=freq_x[i]*qPow(signal_fft[i],4);
        cumul_f_den+=qPow(signal_fft[i],4);
    }

    double f0=cumul_f_num/cumul_f_den;
    //se activa el valor de frecuencia para determinar la constelación
    signal_fft_max_pos_const=f0/double(signal_fm/2)*signal_fft_length;


    double freq=double(signal_fft_max_pos_const)*double(signal_fm/2)/double(signal_fft_length);

    //se reserva espacio
//    signal_I_length=signal_length;
//    signal_I=new double[signal_length];
//    signal_Q=new double[signal_length];

    //se normaliza la amplitud
    float amp=0;
    int counter=0;

    for(int i=0;i<signal_length-2;i++)
    {
        if(signal_in[i]<signal_in[i+1] && signal_in[i+1]>=signal_in[i+2])
        {
            amp=amp+signal_in[i+1];
            counter+=1;
        }
    }

    amp=amp/counter;

    double *signal_in_norm = new double[signal_length];

    for(int i=0;i<signal_length;i++)
    {
        signal_in_norm[i]=signal_in[i]/amp;
    }

    if(index==1)//Lazo Cuadrático
    {
        if(clasif_decision==5)//BPSK
        {            
            if(!constelation_square_loop_BPSK(signal_I,signal_Q,&signal_I_length,signal_in_norm,freq,signal_baud_rate,signal_fm,signal_length))
                return 0;
        }
        if(clasif_decision>=6 && clasif_decision<=8)//QPSK
        {
            if(!constelation_square_loop_QPSK(signal_I,signal_Q, &signal_I_length,signal_in_norm,freq,signal_baud_rate,signal_fm,signal_length))
                return 0;
        }
    }

    if(index==2)//Lazo de Costas
    {
        psk_costas_loop(signal_I,signal_Q,&signal_I_length,signal_in_norm,signal_length,freq,signal_baud_rate,signal_fm,100);
    }



    delete []signal_in_norm;
    //psk_costas_loop(signal_in,signal_length,freq,signal_baud_rate,signal_fm,25,signal_I,signal_Q);

    //se determinan los máximos y mínimos de I y Q
    signal_I_max=0;
    signal_I_min=0;
    signal_Q_max=0;
    signal_Q_min=0;

    for(int i=0;i<signal_I_length;i++)
    {
        //máximo de signal_I
        if(signal_I[i]>signal_I_max)
            signal_I_max=signal_I[i];

        //mínimo de signal_I
        if(signal_I[i]<signal_I_min)
            signal_I_min=signal_I[i];

        //máximo de signal_Q
        if(signal_Q[i]>signal_Q_max)
            signal_Q_max=signal_Q[i];

        //mínimo de signal_Q
        if(signal_Q[i]<signal_Q_min)
            signal_Q_min=signal_Q[i];
    }

    return 1;
}

void Nucleo::nucleo_estimacion_param()
{
    //reducción de la señal
    double* signal_in2;
    double signal_length2=100000;
    int p0=signal_length/2;

    if(p0+signal_length2>signal_length)
    {
        signal_in2=signal_in;
        signal_length2=signal_length;
    }
    else
    {
        signal_in2=&signal_in[p0];
    }

    //Estimación de amplitud
    signal_amp=0;
    int counter=0;
    for(int i=1; i<signal_length2-2; i++)
    {
        if(signal_in2[i]<signal_in2[i+1] && signal_in2[i+1]>signal_in2[i+2])
        {
            signal_amp+=signal_in2[i+1];
            counter+=1;
        }

    }
    signal_amp=signal_amp/counter;

    //estimación de la SNR
    signal_SNR=estima_SNRdB(signal_fft,signal_fft_length);

    //Estimación del baud rate
    //se estima la frecuencia de la modulación recibida
    int period_point=0;
    int k=0;//almacena el cruce por cero anterior
    int total=0;
    double p1,p2,p3;
    for(int i=1;i<(signal_length2-1);i++)
    {
        p1=signal_in2[i-1]*signal_in2[i-1];
        p2=signal_in2[i]*signal_in2[i];
        p3=signal_in2[i+1]*signal_in2[i+1];

        if(p1<=p2 && p2>p3)
        {
            period_point=period_point+i-k;
            k=i;
            total=total+1;
        }
    }

    double freq=double(signal_fm)*double(total)/double(period_point)/double(2);
    signal_baud_rate=EstimaRs(signal_in2,signal_length2,2621,signal_fm);

    //estimación del SNR
    //signal_SNR=estima_SNR_funct(signal_in2,signal_length2);
}


///Demodulación
bool Nucleo::nucleo_dem_funct(int index)
{

    //métodos BFSK
    if(clasif_decision==1)
    {
        double f0=double(signal_fft_max_pos)*double(signal_fm/2)/double(signal_fft_length);
        double f1=double(signal_fft_max_pos_2)*double(signal_fm/2)/double(signal_fft_length);

        if(f0==f1)
        {
            return 0;
        }

        //Ajuste Poligonal
        if(index==0)
        {
            polygonal(signal_dem, &signal_dem_threshold, signal_in, f0, f1, signal_baud_rate, signal_fm, signal_length);
        }

        //Detector de envolvente
        if(index>=1 && index<=2)//Detector de Envolvente 1-Filanovsky, 2-Kaiser,3-Eigenvalue,7-Kaiser
        {
            return detector_envolvente(signal_dem, &signal_dem_threshold, signal_in, signal_amp,f0, f1, signal_fm, signal_length,index);
        }

        if(index==3)//Diferenciador
        {            
            //se determina la amplitud de la señal
            double amplitud=0;
            int counter=0;
            for(int i=0; i<signal_length-2; i++)
            {
                if(signal_in[i]<signal_in[i+1] && signal_in[i+1]>=signal_in[i+2])
                {
                        amplitud+=signal_in[i+1];
                        counter+=1;
                }
            }

            amplitud=amplitud/counter;
            HL_derivator(signal_dem, &signal_dem_threshold, signal_in, amplitud,f0, f1, signal_baud_rate, signal_fm, signal_length);
        }
        if(index==4)//Balanced Quadricorrelator
        {            
            return balanced_quadricorrelator(signal_dem, &signal_dem_threshold, signal_in, f0, f1, signal_fm, signal_length);
        }
        if(index==5)//Quadricorrelator
        {
            return quadricorrelator(signal_dem, &signal_dem_threshold, signal_in, f0, f1, signal_fm, signal_length);
        }
    }

    //métodos 4FSK, 8FSK, 16FSK
    if(clasif_decision>=2 && clasif_decision<=4)
    {
        return detector_envolvente_MFSK(signal_dem,symbols_dem,signal_in,signal_baud_rate,dem_freq_array,signal_fft_length,signal_fm,signal_length,0,qPow(clasif_decision,2));
    }

    //métodos BPSK
    if(clasif_decision==5)
    {
        double f0=double(signal_fft_max_pos)*double(signal_fm/2)/double(signal_fft_length);

        //Lazo Cuadrático
        if(index==1)
        {

            return dem_quadratic_BPSK(signal_dem, &signal_dem_threshold ,signal_in, f0, signal_baud_rate, signal_fm, signal_length);

        }

        //Lazo de Costas
        if(index==2)
        {
            //se normaliza la señal
            double amplitud=0;
            int counter=0;
            for(int i=0; i<signal_length-2; i++)
            {
                if(signal_in[i]<signal_in[i+1] && signal_in[i+1]>=signal_in[i+2])
                {
                        amplitud+=signal_in[i+1];
                        counter+=1;
                }
            }

            amplitud=amplitud/counter;

            double *signal_norm = new double[signal_length];
            for(int i=0; i<signal_length;i++)
                signal_norm[i]=signal_in[i]/amplitud;

            //se demodula
            dem_costas_BPSK(signal_dem, &signal_dem_threshold ,signal_norm, 100, f0, signal_baud_rate, signal_fm, signal_length);

            //se desnormaliza la amplitud
            for(int i=0; i<signal_length;i++)
                signal_dem[i]=signal_dem[i]*amplitud;

            delete[] signal_norm;
        }
    }

    //métodos QPSK, DQPSK, OQPSK, 8PSK
    if(clasif_decision>=6 && clasif_decision<=9)
    {
       double f0=double(signal_fft_max_pos)*double(signal_fm/2)/double(signal_fft_length);

       //se normaliza la señal
       double amplitud=0;
       int counter=0;
       for(int i=0; i<signal_length-2; i++)
       {
           if(signal_in[i]<signal_in[i+1] && signal_in[i+1]>=signal_in[i+2])
           {
                   amplitud+=signal_in[i+1];
                   counter+=1;
           }
       }

       amplitud=amplitud/counter;

       double *signal_norm = new double[signal_length];
       for(int i=0; i<signal_length;i++)
           signal_norm[i]=signal_in[i]/amplitud;

       //se demodula
       //QPSK, DQPSK
       if(clasif_decision==6 || clasif_decision==7)
           dem_costas_QPSK(signal_I_dem, signal_Q_dem,  &signal_I_dem_length, symbols_dem, signal_in, signal_length, signal_baud_rate, signal_fm, f0);

       //OQPSK
       if(clasif_decision==8)
           dem_costas_OQPSK(signal_I_dem, signal_Q_dem, &signal_I_dem_length, symbols_dem, signal_in, signal_length, signal_baud_rate, signal_fm, f0);


       //8PSK
       if(clasif_decision==9)
           dem_costas_8PSK(signal_I_dem, signal_Q_dem, &signal_I_dem_length, symbols_dem, signal_in, signal_length, signal_baud_rate, signal_fm, f0);

       symbols_dem_length=signal_I_dem_length;

       //se desnormaliza la amplitud
       for(int i=0; i<signal_length;i++)
           signal_dem[i]=signal_dem[i]*amplitud;

       delete[] signal_norm;
    }


    return 1;
}


bool Nucleo::nucleo_raster_funct(void)
{
    //4FSK,8PSK,16PSK
    if(clasif_decision>=2 && clasif_decision<=4)
    {
        return raster_M(bits,&bits_length,symbols_dem,symbols_dem_length,qPow(clasif_decision,2),code_dem,0,signal_baud_rate,signal_fm);
    }

    //QPSK, DQPSK,OQPSK
    if(clasif_decision>=6 && clasif_decision<=8)
    {        
        if(clasif_decision==7)
            return raster_M(bits,&bits_length,symbols_dem,symbols_dem_length,4,code_dem,1,signal_baud_rate,signal_fm);
        else
        {
            qDebug()<<"code_dem"<<code_dem;
            return raster_M(bits,&bits_length,symbols_dem,symbols_dem_length,4,code_dem,0,signal_baud_rate,signal_fm);
        }
    }

    //8PSK
    if(clasif_decision==9)
        return raster_M(bits,&bits_length,symbols_dem,symbols_dem_length,8,code_dem,0,signal_baud_rate,signal_fm);

    //BFSK, BPSK
    if(clasif_decision==5 || clasif_decision==1)
        return raster(bits,&bits_length,signal_dem,signal_dem_length,signal_dem_threshold,signal_baud_rate,signal_fm);

}

void Nucleo::nucleo_patron_raster_funct(void)
{
    patrones.clear();

    int ancho=0;
    if (ancho == 0 || ancho > bits_length)
    {
        QVector <PatronEncontradoRaster> resultados;

        //llevando a char la cadena
        QVector<char> bits_char;

        for (int i = 0; i < bits_length; i++)
        {
            if(bits[i])
                bits_char.push_back('1');
            else
                bits_char.push_back('0');
        }

        Patron asd;
        resultados = asd.RasterPolinomio(bits_length, bits_char, 0, 8);


//        ancho = resultados[0].sec.numeroDondeSeEncuentra;

        asd.getData(patrones);

        if(patrones.size()==0)
        {
            //QMessageBox::information(this, "Advertencia", "No se encontró un patrón para esta secuencia.");
            patron=sqrt(bits_length);
            patrones.push_back(patron);
        }
        else
            patron=patrones[0];


    }
}

int Nucleo::nucleo_filtrar_funct(int index)
{
    int index_2=2*(index-1);

    double delta_w_bpf, Beta_bpf, Aten_bpf, aten_bpf=0.01;
    int M_bpf;

    double w0=(double(detect_freq[index_2])/double(signal_fft_length))*M_PI;
    double w1=(double(detect_freq[index_2+1])/double(signal_fft_length))*M_PI;

    delta_w_bpf = qAbs<double>(w1-w0);

    Aten_bpf = -20*log10(aten_bpf);

    if(Aten_bpf > 50){
        Beta_bpf = 0.1102*(Aten_bpf-8.7);
    }
    else if(Aten_bpf >= 21 && Aten_bpf <= 50) {
        Beta_bpf = 0.5842*powf((Aten_bpf-21),0.4)+0.07886*(Aten_bpf-21);
    }
    else if(Aten_bpf < 21) {
    Beta_bpf = 0;
    }

    M_bpf = ceilf((Aten_bpf-8)/(2.285*delta_w_bpf));

    //validación con el orden del filtro
    if(M_bpf>500 || M_bpf<=0)
    {
        //no se puede filtrar
        return 0;
    }

    if(fmodf(M_bpf,2)!=0)
    {
        M_bpf=M_bpf+1;
    }


// Filtro Pasabanda a f0
    double wc1_bpf = w0;
    double wc2_bpf = w1;

// hn del pasabanda superior
    double *hn_bpf = filter_design_direct_I(M_bpf, Beta_bpf, wc1_bpf, wc2_bpf);

// filtrado
    double *salida_bpf = filter(signal_in_array[index], hn_bpf, signal_length, M_bpf);

    //se copia la salida
    for(int i=0;i<signal_length;i++)
        signal_in_array[index][i]=salida_bpf[i];

    delete []hn_bpf;
    delete []salida_bpf;

    return 1;
}


void Nucleo::nucleo_clear_funct()
{
    if(detect_freq_length!=0)
    {
//        for(int i=0; i<detect_freq_length;i++)
//            delete []signal_in_array[i];

        delete []signal_in_array;
        signal_in_array=NULL;
        delete []detect_freq;
        detect_freq_length=0;
        delete []signal_amp_array;
        signal_amp_array=NULL;
        delete []signal_baud_rate_array;
        signal_baud_rate_array=NULL;
    }

    //si se habia cargado previamente una senal, se borra
    if(signal_length!=0)
    {
        delete[] signal_in;
    }

    if(signal_fft_length!=0)
    {
        delete []signal_fft;
    }

    if(signal_spect_length!=0)
    {
        delete[] signal_spect;
        signal_spect_length=0;
    }

    if(detect_freq_length!=0)
    {
        detect_freq_length=0;
        delete[] detect_freq;
    }

    signal_length=0;
    signal_fft_length=0;

}

void Nucleo::nucleo_detect_funct(int method)
{    
    //Bank filter
    if(method==0)
        detect_freq=detect_FB(signal_in, signal_fm, signal_length, signal_fft_length,&detect_freq_length);

    //Energy Detector
    if(method==1)
        detect_freq=detect_ED(signal_fft, signal_fft_length,&detect_freq_length);

    //validación
    for(int i=0;i<detect_freq_length;i++)
        if(detect_freq[i]<0)
            detect_freq[i]=0;
}
