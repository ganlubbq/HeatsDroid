#include "clasificador.h"
#include "fsk.h"
#include "filters.h"
#include "detection.h"

//Para la función de clasificación de conteo de cruces por cero
#define Offset 10
#define Bins 1000
#define Umbral 0.000001
#define FD 7000


double EstimaRs(double *signal_in, int size_signal, double fo,int fm) {

    double Rs_estimate=0;

    //obtener los niveles por el PLL

    double *levels=(double *)malloc(sizeof(double)*size_signal);

    PLL_operation(signal_in, levels, size_signal,fo,fm);


    //restarle la media a niveles del PLL

    double mean_levels = mean(levels,size_signal);   //media
    double *levels_media=(double *)malloc(sizeof(double)*size_signal);

    for (int i=0; i<size_signal; i++){
        levels_media[i]=levels[i]-mean_levels;
    }



    //convertir a un tren de pulsos los niveles
    double *sign_levels=(double *)malloc(sizeof(double)*size_signal);

    for(int i=0; i<size_signal; i++){
        if(levels_media[i]<0)
            sign_levels[i]=-1;
        else
            sign_levels[i]=1;
    }


    //hallar la diferencia entre muestras consecutivas, para obtener tren de deltas
    //trabajar con el modulo
    double *delta_train=(double *)malloc(sizeof(double)*(size_signal-1));

    for (int i=1; i<size_signal; i++){
        delta_train[i-1]=sign_levels[i]-sign_levels[i-1];
    }


    for (int i=0; i<size_signal-1; i++){
        if (delta_train[i]<0)
                delta_train[i]=(-1)*delta_train[i];
    }

    //declarar variables y arreglos necesarios para FFT
    double pow2=log2(size_signal-1);
    int potencia=floor(pow2);
    int size_FFT=pow(2,potencia);

    COMPLEX *cmpl_signal = new COMPLEX[size_FFT];//(COMPLEX *)malloc(size_FFT*sizeof(COMPLEX)); //arreglo para almacenar delta_train en forma compleja
    COMPLEX *F_delta_train;// = new COMPLEX[size_FFT];//(COMPLEX *)malloc(size_FFT*sizeof(COMPLEX));  //arreglo para almacenar resultado de FFT
    double *F_delta_abs=(double *)malloc(size_FFT*sizeof(double));  //arreglo para almacenar valor absoluto de resultado de FFT

    //inicializar parte imaginaria a cero
    for(int i=0; i<size_FFT; i++){
        cmpl_signal[i].real=delta_train[i];
        cmpl_signal[i].imag=0;
    }


    //hallar FFT
    F_delta_train=FFT(cmpl_signal,size_FFT,1);
    cmpl_signal=bitRev(F_delta_train,size_FFT);

    //Valor absoluto de FFT compleja
    double real_sample, imag_sample;

    for (int i=0; i<size_FFT; i++){
        real_sample=(double)F_delta_train[i].real;
        imag_sample=(double)F_delta_train[i].imag;
        F_delta_abs[i]=(double)sqrt(powf(real_sample,2)+powf(real_sample,2));
    }



    //estimar las frecuencias que se obtienen como resultado de FFT
    double *frequencies=(double *)malloc(sizeof(double)*size_FFT); //vector para almacenar las frecuencias de la FFT
    double step=(double)fm/size_FFT;
    double current_frequency=0;

    for (int i=0; i<size_FFT; i++){
        frequencies[i]=current_frequency;
        current_frequency+=step;
    }

    QVector<double> frequencies_upper;   //vector donde se almacenaran las frecuencias de los maximos
    int start_position=5, end_position=size_FFT/2;  //posicion inicial y final para inspeccionar la FFT en busca de maximos


    //Para establecer el umbral se halla la potencia del espectro
    double spec_pow,sum=0;

    for (int i=0; i<size_FFT; i++){
        sum+=F_delta_abs[i]*F_delta_abs[i];
    }
    spec_pow=sqrt(sum/size_FFT);

    double umbral=spec_pow*4;

    //Buscar componentes de frecuencias por encima del umbral
    for(int i=start_position; i<end_position; i++){
        if(F_delta_abs[i]>umbral){
            frequencies_upper.append(frequencies[i]);
        }
    }

    //Se busca eliminar frequencias muy cercanas entre si
    QVector<double> frequencies_upper_corrected; //vector para almacenar las frecuencias de los maximos corregidos
    int size_frequencies=frequencies_upper.size();  //tamano del vector a inspeccionar

    if (frequencies_upper.size()>1){
        frequencies_upper_corrected.append(frequencies_upper[0]);
        for(int i=1; i<size_frequencies-1; i++){
            if ((frequencies_upper[i]-frequencies_upper[i-1])>20)
                frequencies_upper_corrected.append(frequencies_upper[i]);
        }

        frequencies_upper.clear();

        if (frequencies_upper_corrected.size()>2){

            QVector<double> diff_frequencies;    //vector que almacena la distancia entre picos = Rs
            int size_frequencies_corrected=frequencies_upper_corrected.size();
            double diff_temp;

            for (int i=0; i<size_frequencies_corrected-1; i++){
                diff_temp=frequencies_upper_corrected[i+1]-frequencies_upper_corrected[i];
                diff_frequencies.append(diff_temp);
            }

            //Hacer un histograma de la distancia entre picos
            int size_hist=100;
            int size_diff=diff_frequencies.size();
            double wide_bins;
            QVector<double> n(size_hist);
            QVector<double> xout(size_hist);


            wide_bins = Histograma(diff_frequencies,size_diff,n,xout,size_hist);

            //Buscar la diferencia de frecuencia con mayor conteo en el histograma
            double max=n[0], index=0;

            for(int i=1; i<size_hist;i++){
                if (n[i]>=max){
                    if (n[i]==max){
                        if(n[i+1]>0 || n[i-1]>0){
                            max=n[i];
                            index=i;
                        }
                    }
                    else{
                        index=i;
                        max=n[i];
                    }

                }
            }

            n.clear();

            //Hallar la media de las frecuencias
            double mean=0.0;
            int count=0;
            for (int i=0; i<size_diff; i++){
                if ((diff_frequencies[i]>xout[index]-wide_bins) && (diff_frequencies[i]<xout[index]+wide_bins)){
                    mean+=diff_frequencies[i];
                    count++;
                }
            }
            xout.clear();
            Rs_estimate=mean/count;

            //se libera espacio
            diff_frequencies.clear();
            n.clear();
            xout.clear();
        }
        else{
            Rs_estimate=frequencies_upper_corrected[0];
        }
    }
    else
        Rs_estimate=frequencies_upper[0];

    //se libera espacio
    free(levels);
    free(levels_media);
    free(sign_levels);
    free(delta_train);
    free(cmpl_signal);
    //free(F_delta_train);
    free(F_delta_abs);
    free(frequencies);
    frequencies_upper_corrected.clear();



    return Rs_estimate;

}

/* PLL
signal_in-->señal con la que nos queremos sincronizar
*levels-->puntero a arreglo donde se almacenara salida de PLL
size_signal,fo,fo igual a anterior
*/
void PLL_operation(double *signal_in, double *levels, int size_signal, double fo,int fm)
{
    //****************Inicializar*************************

        // Variables para la configuracion del PLL

        double Chi,B,kd,ko,ka,Kv,lock_time;

        // Variables para el filtro de lazo cero-polo
        double wn,T2,T1,a1,a2,b1,b2;
        double *b=(double *)malloc(sizeof(double)*2);
        double *a=(double *)malloc(sizeof(double)*2);

        // Variables para el funcionamiento del PLL
//        double vco_fase0,vco_out,vco_in1,vco_in0,phase_mult_out1,phase_mult_out0,cumul;
//        double *vco_out_p=(double *)malloc(sizeof(double)*(size_signal-1));

        // Variables para el filtro paso-bajo
        double A,Beta,wc,*hn;
        int M,L;

        // Variable para almacenar el producto de la salida del PLL con la señal de entrada
        double *signal_X_PLLout=(double *)malloc(sizeof(double)*size_signal);


        //***************Configurar PLL***********************

//        fo=frequency;             // frecuencia libre del VCO en Hz
        Chi=0.707;                // damping factor óptimo
        B=1;                      // amplitud del tono de salida del VCO
        kd=(2*B)/M_PI;              // Ganancia del Phase Detector step12 /109Best
        ko=2*M_PI*fo;               // Ganancia del VCO
        ka=10;                    // Ganancia del Filtro de lazo
        Kv=kd*ko*ka;              // Ganancia del Lazo
        lock_time=4*1/(fo);		  // configuración del tiempo de respuesta en segundos

        //**************Disenno del filtro de lazo cero-polo****

        wn=2*M_PI/lock_time;
        T2=(2*Chi/wn)-(1/Kv);
        T1=Kv/(wn*wn);

        //Numerador en plano S
        a1=ka*T2;
        a2=ka;

        // Denominador en plano S
        b1=T1;
        b2=1;

        // Transformacion bilineal
        b[0]=(a1*2*fm+a2)/(b1*2*fm+b2);
        b[1]=(a2-a1*2*fm)/(b1*2*fm+b2);
        a[0]=1;
        a[1]=(b2-b1*2*fm)/(b1*2*fm+b2);



        //**************Inicializacion del PLL***************
        double *phase_mult_out=(double *)malloc(sizeof(double)*size_signal);
        phase_mult_out[0]=0;
        double *vco_in=(double *)malloc(sizeof(double)*size_signal);
        vco_in[0]=0;
        double *vco_fase=(double *)malloc(sizeof(double)*size_signal);
        vco_fase[0]=0;
        double *vco_out=(double *)malloc(sizeof(double)*size_signal);
        vco_out[0]=0;
        double cumul=0;
        double *vco_out_p=(double *)malloc(sizeof(double)*size_signal);
        vco_out_p[0]=0;

        //*************Implementacion del PLL**************

        for (int i = 1; i < size_signal; ++i)
        {
            vco_out[i]=B*sin(2*M_PI*i*(fo)/fm+vco_fase[i-1]);
            vco_out_p[i]=-B*cos(2*M_PI*i*(fo)/fm+vco_fase[i-1]);	//revisar si el indice de esta forma no importa
            phase_mult_out[i]=signal_in[i]*vco_out[i];
            vco_in[i]=b[0]*phase_mult_out[i]+b[1]*phase_mult_out[i-1]-a[1]*vco_in[i-1];
            vco_fase[i]=(ko*vco_in[i-1]+vco_in[i])/fm+cumul;
            cumul=vco_fase[i];
        }

        // *************Multiplicar salida de PLL por señal de entrada*****
        for (int i = 0; i < size_signal; ++i)
        {
            signal_X_PLLout[i]=signal_in[i]*vco_out_p[i];
        }

        //*************Disenno filtro Paso-bajo************
        A=-20*log(0.001);
        if (A<21)
            Beta=0;
        else{
            if (A>=21 && A<=50)
             {
                Beta=0.5842*((double)pow((A-21),0.4))+0.7886*(A-21);
             }
             else{
                Beta=0.1102*(A-8.7);
             }
        }

//        wc=2*frequency/fm;
        wc=2*fo/fm;
        double M_double=((A-8)/(2.285*2*wc*M_PI));
        M=round(M_double);
        L=M+1;
        hn=filter_design_direct_I(M,Beta,wc,0);

        //***************Filtrado Paso-Bajo******************

        int start_signal_index=0;
        int start_filter_index=round(M/2);
        double product,acumulation=0;

        // Primera etapa del filtrado, donde se elimina las muestras iniciales
        // para compensar retardo
        for (int i = 0; i < size_signal-round(M/2); ++i)
        {
            for (int k = 0; k < M-start_filter_index ; ++k)
            {
                product=signal_X_PLLout[start_signal_index+k]*hn[start_filter_index+k];
                acumulation=acumulation+product;
            }
            levels[i]=acumulation;
            acumulation=0;

            if (start_filter_index!=0)
            {
                start_filter_index--;
            }

            if (i>M/2)
            {
                start_signal_index++;
            }
        }

        // Agregar estado final del filtro para completar filtrado

        int end_filter_index=M-1;

        for (int i = size_signal-round(M/2); i < size_signal; ++i)
        {
            for (int k = 0; k < end_filter_index; ++k)
            {
                product=signal_X_PLLout[size_signal-end_filter_index+k]*hn[k];
                acumulation=acumulation+product;
            }
            levels[i]=acumulation;
            acumulation=0;

            end_filter_index--;
        }

        delete[] b;
        delete[] a;
        delete[] signal_X_PLLout;
        delete[] phase_mult_out;
        delete[] vco_in;
        delete[] vco_fase;
        delete[] vco_out;
        delete[] vco_out_p;

        return;
}





double mean(double *array_ptr, int size_array){
    double mean, acumulado=0;

    for (int i=0; i<size_array; i++){
        acumulado+=array_ptr[i];
    }

    mean=acumulado/size_array;
    return mean;

}

/* Hallar el modulo de la FFT compleja
comp-->puntero a FFT compleja
fft_abs.-->puntero a arreglo donde se almacenara modulo de fft
length-->size de FFT
*/
void Halla_modulo_complex(COMPLEX *comp,double *fft_abs,int length){
    double real,imag;
    for (int i=0;i<length;i++){
        real=(double)comp[i].real;
        imag=(double)comp[i].imag;
        real=pow(real,2);
        imag=pow(imag,2);
        fft_abs[i]=(double)sqrt(real+imag);
    }
}

/*Funcion para obtener histograma
frec2-->arreglo al que se quiere aplicar histograma
size-->tamaño de frec2
&n--> se pasa referencia a vector donde se almacenaran los conteos de cada casilla
&xout--> referencia a valores de las casillas del histograma
number_Bins--> numero de casillas del histograma
*/
double Histograma(QVector<double>frec2, int size, QVector<double> &n, QVector<double> &xout,int number_Bins)
{
    /*Buscar maximo y minimo de arreglo*/
    double max=frec2[0];
    double min=max;
    double dif_min=0; /*es la diferencia entre el valor i y el minimo*/
    int bin=0;/*estima la casilla en la que va cada valor*/

    double wide_bins;


    for(int i=1;i<size;i++){
        if (frec2[i]>max)
            max=frec2[i];
        if (frec2[i]<min)
            min=frec2[i];
    }

    wide_bins=(max-min)/number_Bins;

    /*Segun la ubicacion del valor se aumenta una frecuencia del histograma*/

    for(int i=0;i<size;i++){
        dif_min=frec2[i]-min;
        bin=floor(dif_min/wide_bins);
        if(bin < n.size())
            n[bin]+=1;
    }


    /*Conformar vector xout que contiene las posiciones de las casillas*/

    xout[0]=min;

    for(int i=1;i<number_Bins;i++){
        xout[i]=xout[i-1]+wide_bins;
    }

    return wide_bins;
}


void Conforma_tiempo( QVector<double> &tiempo, int size, double Ts)
{
    for(int i=0;i<=size-1;i++)
        tiempo[i]=i*Ts;
}

void interpolar(QVector<double> &cruces, QVector<int> indices,double *signal, QVector<double> tiempos, int size, double Ts, int cantidad)
{
    int cont = 0;
    int ind, k;
    double amp1,amp2,amp_interp,t1,t2,t,tdif;
    int indice2;
    //double *tiempo_interp = new double[Offset];
    //double *amp = new double[Offset];

    QVector<double>tiempo_interp;
    QVector<double>amp;
//39622
    for(int i=0;i< size- cantidad; i++)
    {

        ind=indices[i];
        tiempo_interp.clear();
        amp.clear();

        if(ind>Offset){

            indice2=ind-5;

            for(k = 0;k <Offset;k++)
            {
                tiempo_interp.push_back(tiempos[indice2+k]);
                amp.push_back(signal[indice2+k]);

            }

            t=(tiempo_interp[5]+tiempo_interp[6])/2;
            amp_interp=0;

            for(k = 0;k < Offset;k++)
                amp_interp += amp[k]*(sin(M_PI*(t-tiempo_interp[k])/Ts))/(M_PI*(t-tiempo_interp[k])/Ts);
            amp1=amp[5];
            t1=tiempo_interp[5];
            amp2=amp[6];
            t2=tiempo_interp[6];
            tdif=t2-t1;
        }
        else{

            indice2=ind;

            for(k = 0;k < Offset;k++)
            {
                tiempo_interp.push_back(tiempos[indice2+k]);
                amp.push_back(signal[indice2+k]);
            }
            t=(tiempo_interp[0]+tiempo_interp[1])/2;
            amp_interp=0;

            for(k=0;k < Offset;k++){
                amp_interp=amp_interp+amp[k]*(sin(M_PI*(t-tiempo_interp[k])/Ts))/(M_PI*(t-tiempo_interp[k])/Ts);
            }

            amp1=amp[0];
            t1=tiempo_interp[0];
            amp2=amp[1];
            t2=tiempo_interp[1];
            tdif=t2-t1;
        }

        cont = (int)ceil(log(tdif/Umbral)/log(2));
        while(amp_interp!=0)
        {
            if(amp1*amp_interp<0)
            {
                cont--;
                amp2=amp_interp;
                t2=t;
                t=(t1+t2)/2;
                amp_interp=0;

                for(k=0;k<=Offset-1;k++)
                    amp_interp=amp_interp+amp[k]*(sin(M_PI*(t-tiempo_interp[k])/Ts))/(M_PI*(t-tiempo_interp[k])/Ts);

                if (fabs(amp1 - amp2) < Umbral || cont == 0)
                {
                    break;
                }
            }

            else
            {
                cont--;
                amp1=amp_interp;
                t1=t;
                t=(t1+t2)/2;
                amp_interp=0;

                for(k=0;k<=Offset-1;k++)
                    amp_interp=amp_interp+amp[k]*(sin(M_PI*(t-tiempo_interp[k])/Ts))/(M_PI*(t-tiempo_interp[k])/Ts);

                if (fabs(amp1 - amp2) < Umbral || cont == 0)
                {
                    break;
                }
            }

        }
        cruces.push_back(t);
    }

   // delete[] amp;
  //  delete[] tiempo_interp;

}

void Conforma_yz(QVector<double> cruces, QVector<double> &y,QVector<double> &z,int size_cruces){
int i =0;
    for (int i=0;i<=size_cruces-2;i++){
            y[i]=cruces[i+1]-cruces[i];
        }

        for (i=0;i<=size_cruces-3;i++){
            z[i]=y[i+1]-y[i];
        }
}

void Media(QVector<double>array,int size,double &media){

    double suma=0;

    for(int i=0;i<=size-1;i++){
        suma=suma+array[i];
    }
    media = suma/size;
}

void Calcula_var_Ya(QVector<double>Ya,int size_Ya,double &var_Ya){

    double media_Ya;
        //double *cuadrado_Ya;
        double valor_centrado;
        double var;
        QVector<double>cuadrado_Ya(size_Ya);
        //cuadrado_Ya=(double *)calloc(size_Ya,sizeof(double));

        Media(Ya,size_Ya, media_Ya);

        for(int i=0;i<=size_Ya-1;i++){
            valor_centrado=Ya[i]-media_Ya;
            cuadrado_Ya[i]=valor_centrado*valor_centrado;
        }

        Media(cuadrado_Ya, size_Ya, var);

    var_Ya = var;
}

void Calcula_suma(QVector<double>array, int size,double &suma_res){

    double suma=0;

    for(int i=0;i <=size-1;i++){
        suma += array[i];
    }
    suma_res = suma;//(*suma_res)=suma;
}

/*Nuevo*******************************/
void EstimaVarZ(QVector<int>n,QVector<double>xout,QVector<double>z,int size,double &varz){

    int tempind=0;
    int k,sizeZa;
    double tmin,tmax,var;

    /*1000 es el tamano del histograma*/
    for (int i=1;i<1000;i++){
        if(n[i]>n[tempind])
            tempind=i;
    }

    k=tempind;

    while (n[k]!=0){
        k=k-1;
    }

    tmin=xout[k];

    k=tempind;

    while (n[k]!=0){
        k=k+1;
    }

    tmax=xout[k];

    QVector<double>Za;
    sizeZa=0;

    for (int i=0;i<size;i++){
        if (z[i]>tmin && z[i]<tmax){
            Za.push_back(z[i]);
            sizeZa++;
        }
    }

    Calcula_var_Ya(Za,sizeZa,var);
    varz=var;
}

/*nuevo******************************/
void Estimafd(QVector<int>n,QVector<double>xout, double fc,double &fd){

    double tc=1/(2*fc); /*tiempo correspondiente a frecuencia de portadora*/
    double TC=0; /*aqui se almacenara el tiempo en el histograma mas cercano a tc*/
    int indice=0;
    int maxleft=0; /*almacenara el conteo maximo a la izquierda de fc*/
    int maxrigth=0; /*almacenara conteo maximo a derecha de fc*/
    double t1,t2,f1,f2,f;


    for (int i=0;i<1000;i++){
        if (fabs(tc-xout[i])<fabs(tc-TC)){
            TC=xout[i];
            indice=i;
        }
    }

    for (int i=0; i<indice; i++){
        if (n[i]>maxleft){
            maxleft=n[i];
            t1=xout[i];
        }
    }

    for (int i=indice; i<1000; i++){
        if (n[i]>maxrigth){
            maxrigth=n[i];
            t2=xout[i];
        }
    }

    f1=1/(2*t1);
    f2=1/(2*t2);
    f=f1-f2;
    fd=f;
}



void Calcula_Fc(QVector<double>Ya,int size_Ya,double &Fc_Ya_res){

    double suma, denominador, Fc_Ya;

    Calcula_suma(Ya, size_Ya, suma);
    denominador=2*suma;
    Fc_Ya=size_Ya/denominador;

    Fc_Ya_res = Fc_Ya;
}

/*n son los conteos de cada casilla, xout el vector con los centros de cada casilla*/
void Histograma(QVector<double>frec2,int size, QVector<int> &n, QVector<double>&xout)
{
    /*Buscar maximo y minimo de arreglo*/
    double max=frec2[0];
    double min=max;
    double dif_min=0; /*es la diferencia entre el valor i y el minimo*/
    int bin=0;/*estima la casilla en la que va cada valor*/
    int number_bins=Bins;
    double wide_bins;


    for(int i=1;i<size;i++){
        if (frec2[i]>max)
            max=frec2[i];
        if (frec2[i]<min)
            min=frec2[i];
    }

    wide_bins=(max-min)/number_bins;

    /*Segun la ubicacion del valor se aumenta una frecuencia del histograma*/

    for(int i=0;i<size;i++){
        dif_min=frec2[i]-min;
        bin=floor(dif_min/wide_bins);
        if(bin < n.size())
            n[bin]+=1;
    }


    /*Conformar vector xout que contiene las posiciones de las casillas*/

    xout[0]=(wide_bins/2)+min;

    for(int i=1;i<number_bins;i++){
        xout[i]=xout[i-1]+wide_bins;
    }
}

void search_Max(QVector<int> n,int size_n, int &maxN){

    int maxn;

    maxn = n[0];

    for(int i=1;i<=size_n-1;i++)
        if(n[i]>maxn)
            maxn = n[i];

    maxN = maxn;
}

/*valor de xout correspondiente a la mitad de las ocurrencias maximas */
void search_Xout(QVector<double> xout, QVector<int> n, int maxN, double &xoutVar){

    double condicion;
    int i=0;

    condicion = maxN/2;

    while( n[i] < condicion)
        i++;

    xoutVar = fabs(xout[i]);
}

void estimaCNR(double fc_Ya, double var_Za, double &cnr){

    double denominador, denominadorCuad;
    double CNR_est;
    double stdZ=sqrt(var_Za);

    denominador = 2 * M_PI * fc_Ya * stdZ;
    denominadorCuad = denominador*denominador;

    CNR_est = 3 / denominadorCuad;

    cnr = CNR_est;
}

void Sigma_cuadrado (double Fc_Ya, double CNR_estimado,double &sigma2){

    double termino1, terminoCuad, denominador, sigma2_est, fc,cnr;

    fc = Fc_Ya;
    cnr = CNR_estimado;//cnr=(*CNR_estimado);
    termino1=2*M_PI*fc;
    terminoCuad = termino1*termino1;
    denominador = terminoCuad*cnr;
    sigma2_est=1.0 / denominador;
    sigma2 = sigma2_est;//(*sigma2)=sigma2_est;
}

void Mu(double Fc_Ya,int m,int M,int fd,double &mu){
    double salida,denominador,fc;

    fc = Fc_Ya;

    denominador=2*fc+(2*m-1-M)*fd;
    salida = 1.0 / denominador;

    mu = salida;
}

void Sigma_Y(double Fc_Ya,int M,int fd,double CNR_estimado,double &sigmaY){

    double sig_temp=0;
    double miu_temp;
    double Mu_value=0;
    double primerTerm;
    double segundoTerm;
    double matriz[2][2];
    double a=0;
    double b=0;
    int m,i,k;
    double suma=0;
    double tercerTerm;
    double salida,media_temp=0;

    QVector<double>sig(M);
    QVector<double>miu(M);
    //sig=(double *)calloc(M,sizeof(double));
    //miu=(double *)calloc(M,sizeof(double));

    for(m=0;m<=M-1;m++){
        Sigma_cuadrado(Fc_Ya,CNR_estimado,sig_temp);
        sig[m]=sig_temp;
    }

    Media(sig,M, media_temp);
    primerTerm=media_temp;


    for(m=0;m<M;m++){
        Mu(Fc_Ya,m+1,M,fd, Mu_value);
        miu_temp = Mu_value*Mu_value;
        miu[m]=miu_temp;
    }

    Media(miu,M, media_temp);
    segundoTerm=media_temp;


    for(i=0;i<M;i++){
        for(k=0;k<=M-1;k++){
            Mu(Fc_Ya,i+1,M,fd,a);
            Mu(Fc_Ya,k+1,M,fd,b);
            matriz[i][k]=a*b;
        }
    }

    for(i=0;i<M;i++){
        for(k=0;k<M;k++){
            suma+=matriz[i][k];
        }
    }

    tercerTerm=suma/(M*M);

    salida=primerTerm+segundoTerm-tercerTerm;
    sigmaY=salida;
}

///////////////
/// \brief Clasificador::clasificaMod
/// \param startPtr La senal
/// \param tamano   tamano de la senal
/// \param desde    Desde que posicion voy a leer la senal
/// \param fs       frecuencia de muestreo
/// \note           OJO: El algoritmo se bloqueo si se clasifica la senal en su totatidad,
///                 por lo que hay un if que pregunta si el tamano de la senal es mayor que
///                 20000 elementos. A partir de esto, la funcion "interpolar" se modifico
///                 un poco. Sobre todo lo que se realiza con el ultimo parametro
///
///
int clasif_crucesporcero_funct(double *signal_in, int signal_length, int signal_fm)
{
    int pto_inicial=0;
    int k=0; /*k sera cantidad de cruces por cero*/
    //double *t;/*tiempos correspondientes a cada muestra*/
    double Ts = 1.0 / signal_fm;
    //double *cruces;
    int size_y,size_z;
    double var_y,fc;
    double varZ;/*estimacion de sigmaZ*/
    double cnr,fd;
    double sigma2,sigmaY;/*estimacion para PSK*/

    QVector<int>indicesA_Ptr;
    QVector<double>x;
    QVector<double>y;
    QVector<double>z;

    if (signal_length > 20000)
    {
        int tamanoA = 20000;

        int mitad = signal_length / 2;

        pto_inicial = mitad - 10000;

        if (pto_inicial < 0)
            pto_inicial = 0;

        double* nueva = new double[20000];
        int cont = 0;
        int temp = pto_inicial + 20000;//tamano - ceil(Offset/2) - 1;
        for (int i = pto_inicial; i < temp; i++)
        {
            nueva[cont] = signal_in[i];
            cont++;
        }

        for (int i = 0; i < 20000; i++)
                if (nueva[i]*nueva[i+1] < 0)
                    indicesA_Ptr.push_back(i);

        k = indicesA_Ptr.size();

        QVector<double>t(tamanoA);
        Conforma_tiempo(t, tamanoA, Ts);

        interpolar(x, indicesA_Ptr, /*startPtr*/nueva, t, k, Ts, 10);

        delete[] nueva;
        indicesA_Ptr.clear();
        t.clear();

        size_y = x.size() - 1;
        size_z = x.size() - 2;

        for (int i=0;i<=x.size() - 2;i++)
            y.push_back(x[i+1] - x[i]);

        for (int i=0; i<= x.size() - 3;i++)
            z.push_back(y[i+1] - y[i]);
    }
    else
    {
        int temp = signal_length - ceil(Offset/2) - 1;
        for (int i = pto_inicial; i < temp; i++)
            if (signal_in[i]*signal_in[i+1] < 0)
                indicesA_Ptr.push_back(i);

        k = indicesA_Ptr.size();

        QVector<double>t(signal_length);
        Conforma_tiempo(t, signal_length, Ts);

        interpolar(x, indicesA_Ptr, signal_in, t, k, Ts, 0);

        indicesA_Ptr.clear();
        t.clear();

        size_y = k - 1;
        size_z = k - 2;

        for (int i=0; i <= k - 2;i++)
            y.push_back(x[i+1] - x[i]);

        for (int i=0; i <= k - 3;i++)
            z.push_back(y[i+1] - y[i]);
    }

    x.clear();

    Calcula_var_Ya(z,size_z,varZ);
    z.clear();

    Calcula_var_Ya(y, size_y, var_y);
    Calcula_Fc(y, size_y, fc);

    estimaCNR(fc, varZ, cnr);

    Sigma_cuadrado (fc, cnr, sigma2);

    QVector<int>ny(1000);
    QVector<double>xouty(1000);

    Histograma(y,size_y,ny,xouty);

    y.clear();

    Estimafd(ny,xouty,fc,fd);

    ny.clear();
    xouty.clear();

    Sigma_Y(fc, 2, fd ,cnr, sigmaY);

    if (sigmaY/sigma2<var_y/sigmaY)
        return 5;//BPSK
    else
        return 1;//BFSK
}



int clasif_arbol_decisiones(double *signal_in, int size_signal, int fm, double Rs){

    int freqStatesN = freqStates(signal_in,size_signal,fm,Rs);// devuelve x>2 estados de frequencia detectados 2->BFSK 3,4,5->4FSK  6,7,8...->8FSK


    if (freqStatesN>1){
        return freqStatesN;
    }
    else{
        int p_orQ = clasificaPSKQAM(signal_in,size_signal,Rs,fm); // devuelve 1 si QAM o 20 + phase_states si PSK dividir % 20 y resto (resultado) 1,2->PSK; 3,4,5->QPSK; 6,7,8...->8PSK
        return 	p_orQ;
    }

}


int freqStates (double *signal_in, int size_signal, int fm,float Rs){

    // Estimar tamanno FFT
    int pow2 = floor(log2(size_signal));
    int size_FFT = pow(2,pow2);

    // Transformar
    COMPLEX *cmpl_signal=(COMPLEX *)malloc(size_FFT*sizeof(COMPLEX)); //arreglo para almacenar delta_train en forma compleja
    COMPLEX *F_SIGNAL=(COMPLEX *)malloc(size_FFT*sizeof(COMPLEX));

    for (int i = 0; i<size_FFT; i++){
        cmpl_signal[i].real = signal_in[i];
        cmpl_signal[i].imag = 0;
    }

    F_SIGNAL = FFT(cmpl_signal,size_FFT,1);
    F_SIGNAL = bitRev(F_SIGNAL,size_FFT);

    // Hallar valor absoluto de resultado de FFT
    double *abs_fft = (double *) malloc (sizeof(double) * size_FFT);
    absValue(abs_fft, F_SIGNAL, size_FFT);

    double umbral = searchFFT_Umbral(abs_fft,size_FFT);

    QVector<double> indexesUpper;

    // Conformar frecuencias. Solo interesa hasta fm/2
    int halfSizeFFT = (int) floor(size_FFT/2);
    double *frequencies = (double *) malloc(sizeof(double)*(halfSizeFFT));
    double increment = (double) fm/size_FFT;

    frequencies[0] = 0;
    for (int i = 1; i < halfSizeFFT ; i++){
        frequencies[i] = frequencies [i - 1] + increment;
    }

    for (int i = 5; i < halfSizeFFT ; i++){
        if (abs_fft[i]>umbral)
                indexesUpper.append(frequencies[i]);
    }

    // Conformar histograma de frecuencias por encima del umbral
    double stepSize = 0.75*Rs;
    int size_freqHist = (int) floor((fm/2)/stepSize);
    QVector<double> n_freq(size_freqHist,0);
    int index;

    for (int i = 0; i<indexesUpper.size(); i++){
        index = (int)floor(indexesUpper[i]/stepSize);
        n_freq[index]++;
    }

    // Buscar ocurrencias no consecutivas
    int freqStatesN = 0;
    int last_index = 0;
    int new_index;

    for (int i = 1; i<size_freqHist; i++){
        if (n_freq[i]>0){
            new_index = i;
            if (new_index-last_index > 1)
                freqStatesN++;
            last_index = new_index;
        }
    }


    return freqStatesN;
}

int clasificaPSKQAM(double *signal_in, int size_signal, double Rs, int fm){

//    double fc = estimaFc0 (signal_in, size_signal,fm);
    double pow2 = floor(log2(size_signal));
    int size_FFT = pow(2,pow2);
    double fc = estimaFcFFT4(signal_in, fm,size_FFT,1);
//    double fc = 2893.5;
    double amplitud = normalizaAmp(signal_in,size_signal);

    double *I = (double *) malloc(sizeof(double)*size_signal);
    double *Q = (double *) malloc(sizeof(double)*size_signal);
    costasLoop (signal_in, size_signal, fc, fm, I, Q);

    //Detectar estados de amplitud
    int amp_States = ampStates(I,Q,size_signal);

    //se desnormaliza
    for(int i=0;i<size_signal;i++)
        signal_in[i]=signal_in[i]*amplitud;

    if (amp_States > 1){
        return amp_States;
    }

    else{
        // Caso PSK
        double Ts = fm/Rs;
        double symbol_change_length = floor(size_signal/Ts) - 1;
        double *angle = (double *) malloc (sizeof(double)*symbol_change_length);
        promSymbol(symbol_change_length,fm,Rs,I,Q,angle);



        QVector<double> n_angle(100);
        QVector<double> xout_angle(100);
        Histograma(angle,symbol_change_length,n_angle,xout_angle,100);
        xout_angle.clear();
        int phase_states = numberStates(n_angle,100)+20;
        n_angle.clear();

        return phase_states;
    }

}

void promSymbol (int length_final, int fm, double Rs, double *I, double *Q,double *angle) {
    int *symbol_change = (int *)malloc(sizeof(int)*length_final);
    double Ts = fm/Rs;
    // int *symbol_change = (int *) malloc (sizeof (int) * symbol_change_length);
    conformaSymbolChange (symbol_change, length_final,Ts);

//    QVector<double> signal_i_avg(length_final);
//    QVector<double> signal_q_avg(length_final);

    double *signal_i_avg = (double *)malloc(sizeof(double)*length_final);
    double *signal_q_avg = (double *)malloc(sizeof(double)*length_final);

    double x0,x1;

    for (int i = 0; i < length_final; ++i)
    {
        x0 = symbol_change[i]+round(Ts/4);
        x1 = symbol_change[i]+round(3*Ts/4);

        signal_i_avg[i] = sum(I,x0,x1)/(x1-x0+1);
        signal_q_avg[i] = sum(Q,x0,x1)/(x1-x0+1);
    }

//	I.clear();
//	Q.clear();

//    QVector<double> angle (length_final);
    // double *angle = (double *) malloc (sizeof(double)*symbol_change_length);
    double temp, real, imag;
    for (int i = 0; i < length_final; ++i)
    {
        if (signal_i_avg[i] > 0){
            if (signal_q_avg[i]>0){
                real = signal_i_avg[i];
                imag = signal_q_avg[i];
                temp = atan(imag/real);
                angle[i] =  temp;
            }
            else{
                real = signal_i_avg[i];
                imag = signal_q_avg[i];
                temp = atan(imag/(-1*real));
                angle[i] =  -1*temp;            }
        }
        else {
            if (signal_q_avg[i]>0){
                real = signal_i_avg[i];
                imag = signal_q_avg[i];
                temp = atan(imag/(-1*real));
                angle[i] = M_PI-temp;
            }
            else{
                real = signal_i_avg[i];
                imag = signal_q_avg[i];
                temp = atan(imag/real);
                angle[i] =  -1*M_PI + temp;
            }
        }
    }
}

void costasLoop (double *signal_in, int size_signal, double fc, int fm,double *I,double *Q){

    int kc = 50;

    // Funcion de matlab qpsk_costas_loop2
    double polo = 0.9; 	// define la posición del polo real para el filtro polo cero.
    int N = 16;			// tamaño del acumulador
    int orden = 50;		// orden del filtro de rama FIR

//	QVector<double> W(2,0);
     double *W = (double *)malloc(sizeof(double)*2);
     W[0] = 0;
     W[1] = 0;

    double b2[51] = {0.0017, 0.0019, 0.0022, 0.0028, 0.0036, 0.0046, 0.0058, 0.0073, 0.0090, 0.0109, 0.0130,
                   0.0152, 0.0176, 0.0201 , 0.0226, 0.0251, 0.0276, 0.0300, 0.0322, 0.0343, 0.0361, 0.0376,
                   0.0388, 0.0397, 0.0403, 0.0405, 0.0403, 0.0397, 0.0388, 0.0376, 0.0361, 0.0343, 0.0322,
                   0.0300, 0.0276, 0.0251, 0.0226, 0.0201, 0.0176, 0.0152, 0.0130, 0.0109, 0.0090, 0.0073,
                   0.0058, 0.0046, 0.0036, 0.0028, 0.0022, 0.0019, 0.0017};
    double *b1 = b2;
    int M = orden + 1;

//	QVector<double> V1(orden+1,0);
//	QVector<double> V1A(orden+1,0);
     double *V1 = (double *) malloc (sizeof(double)*(orden+1));
     double *V1A = (double *) malloc (sizeof(double)*(orden+1));

     inicializaArrayZero(V1,orden+1);
     inicializaArrayZero(V1A,orden+1);

//	QVector<double> WW(2,0);
//	QVector<double> VV(2,0);
     double *WW = (double *)malloc (sizeof(double)*2);
     double *VV = (double *)malloc (sizeof(double)*2);

     inicializaArrayZero(WW,2);
     inicializaArrayZero(VV,2);

//	QVector<double> a4(2);
     double* a4 = (double *)malloc(sizeof(double)*2);
     a4[0] = 1;
     a4[1] = -1*polo;

//     QVector<double> b4(2);
     double* b4 = (double *)malloc(sizeof(double)*2);
     b4[0] = (1-polo)/2;
     b4[1] = (1-polo)/2;

//     QVector<double> W3(2,0);
//     QVector<double> W3A(2,0);
      double *W3 = (double *)malloc (sizeof(double)*2);
      double *W3A = (double *)malloc (sizeof(double)*2);
      inicializaArrayZero(W3,2);
      inicializaArrayZero(W3A,2);

     double error = 0;
     double xcos = 0;
     double xsin = 0;
     double mixer1, mixer2;
     double mix1, mix2;
     double mixer3, mixer4;
     double fase;
     double yy;

//     QVector<double> I(size_signal);
//     QVector<double> Q(size_signal);
//     double *I = (double *)malloc(sizeof(double)*size_signal);
//     double *Q = (double *)malloc(sizeof(double)*size_signal);

    for (int i = 0; i < size_signal; ++i)
    {
        nco3(&xcos, &xsin, W, fc, error, N, fm);

        mixer1 = xcos*signal_in[i];
        mixer2 = xsin*signal_in[i];

        mix1 = delay(mixer1,W3);
        mix2 = delay(mixer2,W3A);

        myFir(I,i,V1,b1,M,mix1);
        myFir(Q,i,V1A,b1,M,mix2);

        mixer3 = I[i]*sign(Q[i]);
        mixer4 = Q[i]*sign(I[i]);

        fase = mixer3-mixer4;
        yy = iirlp(WW,VV,b4,a4,fase);
        error=yy*kc/fm*pow(2,N);
    }

    free(W3);
    free(W3A);
    free(WW);
    free(VV);
    free(V1);
    free(V1A);
    free(a4);
    free(b4);
    free(W);

    /***********************************************************/
//	int del = 1;
//    QVector<int> symbol_change(length_final);

}

void nco3 (double *xcosPtr, double *xsinPtr,double *W,double fc,double error,double N,double fm){

    double K0 = fc/(fm/pow(2,N));
    double dCita = 2*M_PI*K0 + 2*M_PI*error;

    // double b0 = 1;
    // double b1 = 0;

    // double a0 = 1;
    // double a1 = -1;

    W[0] = dCita + W[1];
    W[1] = W[0];
    (*xcosPtr)=cos(W[0]/pow(2,N));
    (*xsinPtr)=sin(W[0]/pow(2,N));

}

double delay (double mixer, double *W3){

    W3[0] = mixer;
    double y = W3[1];
    W3[1] = W3[0];

    return y;
}

void myFir (double *I_Q,int j, double *V1, double *b1, double M, double mix1){

    double suma1 = 0 ;
    V1[0] = mix1;

    for (int i = 0; i < M; ++i)
    {
        suma1 += V1[i]*b1[i];
    }

    I_Q[j] = suma1;

    for (int i = M-1; i >= 1; --i)
    {
        V1[i] = V1[i-1];
    }

}

int sign (double value){
    if (value<0)
        return -1;
    else if(value>0)
        return 1;
    else
        return 0;
}

double iirlp(double *WW, double *VV,double *b4, double *a4, double fase ){

    WW[0] = fase;
    VV[0] = WW[0]*b4[0]+WW[1]*b4[1]-a4[1]*VV[1];
    double y = VV[0];
    VV[1] = VV[0];
    WW[1] = WW[0];

    return y;
}

void conformaSymbolChange( int *symbol_change, int length, double Ts){
    for (int i = 0; i < length; ++i)
    {
        symbol_change[i] = (int) round(i*Ts);
    }
}

double sum (double *array, int desde, int hasta){

    double suma = 0;

    for (int i = desde; i <= hasta; ++i)
    {
        suma+=array[i];
    }

    return suma;
}

void Histograma(double *frec2, int size, QVector<double> &n, QVector<double> &xout,int number_Bins)
{
    /*Buscar maximo y minimo de arreglo*/
    double max=frec2[0];
    double min=max;
    double dif_min=0; /*es la diferencia entre el valor i y el minimo*/
    int bin=0;/*estima la casilla en la que va cada valor*/

    double wide_bins;


    for(int i=1;i<size;i++){
        if (frec2[i]>max)
            max=frec2[i];
        if (frec2[i]<min)
            min=frec2[i];
    }

    wide_bins=(max-min)/number_Bins;

    /*Segun la ubicacion del valor se aumenta una frecuencia del histograma*/

    for(int i=0;i<size-1;i++){
        dif_min=frec2[i]-min;
        bin=floor(dif_min/wide_bins);
        if(bin < n.size())
            n[bin]+=1;
        else
            n[number_Bins-1]+=1;
    }


    /*Conformar vector xout que contiene las posiciones de las casillas*/

    xout[0]=min;

    for(int i=1;i<number_Bins;i++){
        xout[i]=xout[i-1]+wide_bins;
    }
}

int numberStates (QVector<double> n, int size){

    double max = n[0];

    // Encontrar el maximo
    for ( int i = 1; i<size ; i++){
        if (n[i]>max)
            max = n[i];
    }

    double umbral = max/1.5;
    QVector<int> indexUpper;
    // Buscar puntos por encima del umbral
    for (int i = 0; i<size; i++){
        if (n[i]>umbral)
            indexUpper.append(i);
    }

    // Detectar estados no consecutivos
    int nonconsecutiveStates = 0;
    int states;

    if (indexUpper.size()>1){
        for (int i = 0; i<indexUpper.size()-1; i++){
            if ((indexUpper[i+1]-indexUpper[i])>1)
                nonconsecutiveStates++;
        }
        states = nonconsecutiveStates+1;
    }
    else
        states = 2;

    indexUpper.clear();

    return states;

}

double estimaFc0 (double *signal_in, int size_signal, int fm){

    double *signal_in2 = (double *)malloc(sizeof(double)*size_signal);
    //***************Elevar signal al cuadrado*************

    for (int i = 0; i < size_signal; ++i)
    {
        signal_in2[i]=signal_in[i]*signal_in[i];
    }

    //***************Estimar fc por maximos****************

    int period_point=0;
    int k=0;
    int total=0;

    for (int i = 1; i < size_signal-1; ++i)
    {
        if (signal_in2[i-1]<=signal_in2[i] && signal_in2[i]>signal_in2[i+1]) //deteccion de maximo
        {
            period_point=period_point+i+1-k;
            k=i+1;
            total++;
        }
    }

    free(signal_in2);

    double period=(double)period_point/total;
    double frequency=fm/period/2;
    return frequency;
}

double estimaFcFFT4 (double *signal_in, int fm, int size_FFT, int desde){

    //hallar FFT del tren de deltas
    double fcarrier_num = 0;
    double fcarrier_den = 0;

    COMPLEX *cmpl_signal=(COMPLEX *)malloc(size_FFT*sizeof(COMPLEX)); //arreglo para almacenar delta_train en forma compleja
    COMPLEX *F_SIGNAL=(COMPLEX *)malloc(size_FFT*sizeof(COMPLEX));

    //inicializar parte imaginaria a cero
     for(int i=desde; i<size_FFT; i++){
         cmpl_signal[i].real=signal_in[i];
         cmpl_signal[i].imag=0;
     }

     F_SIGNAL = FFT(cmpl_signal,size_FFT,1);
     F_SIGNAL = bitRev(F_SIGNAL,size_FFT);

     double *abs_fft = (double *) malloc (sizeof(double) * size_FFT);
     absValue(abs_fft, F_SIGNAL, size_FFT);

//     free(cmpl_signal);
//     free(F_SIGNAL);
//     QFile filedx("F_Signal.txt");
//     if (filedx.open(QIODevice::WriteOnly))
//     {
//         QTextStream out(&filedx);
//         for (int i = 0; i < size_FFT; i++)
//             out << abs_fft[i] << "\n";
//         filedx.close();
//     }
//     filedx.close();

     double *frequencies=(double *)malloc(sizeof(double)*size_FFT); //vector para almacenar las frecuencias de la FFT
     double step=(double)fm/size_FFT;
     double current_frequency=0;

     for (int i=0; i<size_FFT; i++){
         frequencies[i]=current_frequency;
         current_frequency+=step;
     }

     for (int i = 0; i< (int)floor(size_FFT/2); i++){
         fcarrier_num+=frequencies[i]*pow(abs_fft[i],4);
         fcarrier_den+=pow(abs_fft[i],4);
     }

     free(frequencies);
     free(abs_fft);

     double fcarrier = fcarrier_num/fcarrier_den;
     return fcarrier;

}

double normalizaAmp (double *signal_in, int size_signal){
    int counter = 0;
    double amplitud = 0;

    for (int i = 0; i < size_signal-1; ++i)
    {
        if ((signal_in[i]<signal_in[i+1]) && (signal_in[i+1]>=signal_in[i+2]))
        {
            amplitud+=signal_in[i+1];
            counter++;
        }
    }

    amplitud = amplitud / counter;

    for (int i = 0; i < size_signal; ++i)
    {
        signal_in[i]=signal_in[i]/amplitud;
    }

    return amplitud;
}

void absValue (double *abs_fft,COMPLEX  *fft, int size){

    for (int i = 0 ; i < size ; i++){
        abs_fft[i] = sqrt(pow(fft[i].real,2)+pow(fft[i].imag,2));
    }
}

void limitaSignal_in (double *signal_in, int size_signal, double *new_signal_in, int size_FFT){
    for (int i = 0; i<size_FFT; i++){
        new_signal_in[i] = signal_in [i];
    }
}

void inicializaArrayZero (double *array, int size){

    for (int i = 0; i<size; i++){
        array[i]=0;
    }
}

int ampStates (double *I, double *Q, int size){

    double *inst_amp = (double *) malloc(sizeof(double)*size);

    for (int i = 0; i < size; i++){
        inst_amp[i] = sqrt(pow(I[i],2)+pow(Q[i],2));
    }

    QVector<double> n_amp(100);
    QVector<double> xout_amp(100);

    Histograma(inst_amp,size,n_amp,xout_amp,100);
    int ampStatesNum = numberStates(n_amp,100);

    free(inst_amp);
    n_amp.clear();
    xout_amp.clear();

    return ampStatesNum;
}

double searchFFT_Umbral(double *abs_fft,int size_FFT){

    double max = abs_fft[5]; //Puede ocurrir que al inicio exista una fuerte componente de frecuencia

    for (int i = 6; i<round(size_FFT/2); i++){
        if (abs_fft[i]>max)
            max = abs_fft[i];
    }

    double umbral = max/5;

    return umbral;

}

double estima_SNRdB(const double * signal_fft, const int freq_length)
{
    int i;


    //determinación de la potencia de ruido
    int wind_quant = 60;


    double wind_width = freq_length/wind_quant;

    // wind_width = wind_width + 0.5;
    int wind = (int)wind_width;
    double *pdg = new double[wind_quant];
    double *pdg_sorted = new double[wind_quant];

    // ToDo: Cambiar el 99 por las variables correspondientes
    for(int count = 0; count<wind_quant; count++)
    {
        double pdg_temp = 0;
        for(int i = 0; i<=wind-1; i++)
        {
            //pdg_temp = pdg_temp + signal_FFT_2[count*wind+i]*signal_FFT_2[count*wind+i];
            pdg_temp = pdg_temp + signal_fft[count*wind+i]*signal_fft[count*wind+i];
        }
//        pdg[count] = double(1)/wind*pdg_temp*wind_quant;
        pdg[count] = double(pdg_temp)/wind;
        // Hacer una copia de pdg q luego va a ordenarse
        pdg_sorted[count] = pdg[count];
    }

    BubbleSort(pdg_sorted, wind_quant);

    /* Integrating */

    double A;
    double *A1 = new double[wind_quant];

    int s;
    for(s=0; s<wind_quant-1; s++)
    {
        A = (pdg_sorted[s] + pdg_sorted[s+1])/2;
        A1[s+1] = A1[s] + A;
        // arreglo con la acumulaci�n del �rea bajo la curva hasta la muestra s
    }

    // 90% de la frecuencia ocupada
    int r = 0;
    while (A1[r] < 0.9 * A1[wind_quant-1])
    {
        r = r + 1;
    };

    double pn_w = 0;
    for(i=r; i<wind_quant; i++)
    {
        pn_w = pn_w + pdg_sorted[i];
    }

    //  1.5 de la incertidumbre
    pn_w = pn_w/(wind_quant-r);

    //determinación de la potencia de la señal
    double ps=0;
    for(i=0;i<freq_length;i++)
        ps+=qPow(signal_fft[i],2);

    ps=ps/freq_length;
    ps-=pn_w;

    return 10*qLn(ps/pn_w)/M_LOG10E;
}
