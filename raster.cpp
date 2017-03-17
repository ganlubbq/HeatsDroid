#include "raster.h"

#include<math.h>
bool raster(bool* &bits, int *bits_length, double *signal_dem, int signal_dem_length, double signal_dem_threshold, double baud_rate, double signal_fm)
{
    int *datos = new int[signal_dem_length];

    for(int i=0; i<signal_dem_length; i++)
    {
        if(signal_dem[i]>signal_dem_threshold)
            datos[i]=1;
        else
            datos[i]=-1;
    }

    //se borra una posible primera transición
    datos[1]=datos[2];


    //baud_rate en Hz
    //se calcula con interpolación lineal la distancia entre cruces por cero en
    //el arreglo duracion y los puntos de transición en el arreglo transition

    *bits_length=ceil(float(signal_dem_length)*float(baud_rate)/float(signal_fm));
    double *duracion_time = new double[*bits_length];//almacena la duración entre cruces por cero en seg
    double *transition_time = new double[*bits_length];// el arreglo transition almacena los puntos de cambio posterior a cada duración del arreglo duracion en seg

    int j=0;
    double m=0;
    //int u=1;
    for(int i=0; i<signal_dem_length-2; i++)
    {
        if((signal_dem[i]>signal_dem_threshold && signal_dem[i+1]<signal_dem_threshold) || (signal_dem[i]<signal_dem_threshold && signal_dem[i+1]>signal_dem_threshold))
        {
            if((j+2)>*bits_length)
            {
                delete[] datos;
                *bits_length=0;
                delete[] duracion_time;
                delete[] transition_time;
                return 0;
            }

            m=(signal_dem[i+1]-signal_dem[i])*signal_fm;
            transition_time[j+1]=double(i+1)/signal_fm-signal_dem[i]/m;
            duracion_time[j]=transition_time[j+1]-transition_time[j];
            j=j+1;


        }
    }



    double envio_elem=1/baud_rate;
    int M=j-1; //longitud de los arreglos duracion_time y transition_time 
    if(M<5)
    {
        delete[] datos;
        *bits_length=0;
        delete[] duracion_time;
        delete[] transition_time;
        return 0;
    }

    int *bits_total = new int[M]; //almacena el total de bits por transición    
    double value=0;

    for(int i=0; i<M; i++)
    {        
        value=duracion_time[i]/envio_elem;
        if((value-floor(value))>=0.5)
            bits_total[i]=ceil(value);
        else
            bits_total[i]=floor(value);
    }



    //Calcula la duración en muestras de los niveles en alto y en bajo
    //la duración se guarda en muestras en el arreglo duracion
    int conteo_alto=0;
    int conteo_bajo=0;

    int *duracion= new int[*bits_length+10];
    int *transition= new int[*bits_length+10];// el arreglo transition almacena los puntos de cambio posterior a cada duración del arreglo duracion
    transition[0]=1;
    int k=0;
    for(int i=0; i<signal_dem_length-1; i++)
    {
        if(k>*bits_length+10)
        {
            delete[] datos;
            *bits_length=0;
            delete[] duracion_time;
            delete[] transition_time;
            delete[] duracion;
            delete[] transition;
            return 0;
        }

        if(datos[i]==1 && datos[i+1]==1)
            conteo_alto=conteo_alto+1;

        if(datos[i]==-1 && datos[i+1]==-1)
            conteo_bajo=conteo_bajo+1;

        //para signal bipolar
        if(datos[i]==1 && datos[i+1]==-1)
        {
            duracion[k]=conteo_alto; //almacena la duración de los símbolos
            transition[k+1]=i; //almacena los puntos de transiciones
            conteo_alto=0;
            k=k+1;
        }


        if(datos[i]==-1 && datos[i+1]==1)
        {
            duracion[k]=conteo_bajo;
            transition[k+1]=i;
            conteo_bajo=0;
            k=k+1;
        }

    }

    //corrección de los niveles
    int p=0;
    for(int i=0; i<M;i++)
    {
        if(bits_total[i]==0)
        {
            if((signal_dem[transition[i]+1]>signal_dem_threshold && signal_dem[transition[i]]<signal_dem_threshold) || (signal_dem[transition[i]+1]<signal_dem_threshold && signal_dem[transition[i]]>signal_dem_threshold))
            {
                double value=(signal_dem[transition[i]]+signal_dem[transition[i+1]+1])/2;

                for(int j=transition[i]+1;j<=transition[i+1];j++)
                    signal_dem[j]=value;

                p=1;
            }
        }

    }


    if(p==1)//si hubo corrección
    {
        //se recalculan datos, duracion y transition si hubo
        //rectificación con p=1, de lo contrario p=0
        for(int i=0; i<signal_dem_length; i++)
        {
            if(signal_dem[i]>signal_dem_threshold)
                datos[i]=1;
            else
                datos[i]=-1;
        }

        //se borra una posible primera transición
        datos[1]=datos[2];

        k=0;
        for(int i=0; i<signal_dem_length-1; i++)
        {
            if(k>*bits_length+10)
            {
                delete[] datos;
                *bits_length=0;
                delete[] duracion_time;
                delete[] transition_time;
                delete[] duracion;
                delete[] transition;
                return 0;
            }

            if(datos[i]==1 && datos[i+1]==1)
                conteo_alto=conteo_alto+1;

            if(datos[i]==-1 && datos[i+1]==-1)
                conteo_bajo=conteo_bajo+1;

            //para signal bipolar
            if(datos[i]==1 && datos[i+1]==-1)
            {
                duracion[k]=conteo_alto; //almacena la duración de los símbolos
                transition[k+1]=i; //almacena los puntos de transiciones
                conteo_alto=0;
                k=k+1;
            }


            if(datos[i]==-1 && datos[i+1]==1)
            {
                duracion[k]=conteo_bajo;
                transition[k+1]=i;
                conteo_bajo=0;
                k=k+1;
            }

        }

        M=k-2;

        for(int i=0; i<M; i++)
        {
            value=double(duracion[i])/envio_elem/signal_fm;
            if((value-floor(value))>=0.5)
                bits_total[i]=ceil(value);
            else
                bits_total[i]=floor(value);
        }
    }


    //Se recupera la secuencia de bits
    int length_bits_total=0;
    for(int i=0;i<M;i++)
        length_bits_total+=bits_total[i];


    bits= new bool[length_bits_total];
    *bits_length=length_bits_total;

    k=0;
    for(int i=0;i<M;i++)
    {
        if(k>*bits_length+10)
        {
            delete[] datos;
            *bits_length=0;
            delete[] duracion_time;
            delete[] transition_time;
            delete[] duracion;
            delete[] transition;
            delete[] bits;
            return 0;
        }

       if(i%2!= 0)
       {
            for(int j=0; j<bits_total[i]; j++)
            {
                bits[k]=0;
                k=k+1;
            }

       }

       if(i%2== 0)
       {
           for(int j=0; j<bits_total[i];j++)
           {
             bits[k]=1;
             k=k+1;

           }
       }
    }


    delete[] datos;
    delete[] duracion_time;
    delete[] transition_time;
    delete[] bits_total;
    delete[] duracion;
    delete[] transition;

    return 1;
}

bool raster_M(bool* &bits, int *bits_length,int *symbols, int symbols_length, int M, int code, bool diferencial,double baud_rate,double signal_fm)
{
    //Se detectan los cambios de símbolos
    int symbols_change_length=0;
    int samples_bit=qRound(signal_fm/baud_rate);
    int *total_bits, *symbols_detected;

    symbols_detected = new int[symbols_length];
    total_bits = new int[symbols_length];

    //se determina el total de bits por cambio
    *bits_length=0;
    int j=0,pos=0,total=0;
    double value=0;
    symbols_change_length=1;
    for(int i=0; i<symbols_length-1;i++)
    {

        if(symbols[i]!=symbols[i+1])
        {
            symbols_change_length+=1;
            value=(double(i-pos+1)/double(samples_bit));
            if((value-floor(value))>0.5)
                total_bits[j]=ceil(value);
            else
                total_bits[j]=floor(value);
            pos=i+1;

            symbols_detected[j]=symbols[i];//almacena el símbolo que se repite
            total+=total_bits[j];
            j+=1;



        }
    }

    symbols_change_length=j;
    //se determinan el total de bits
    if(M==4)
        total*=2;
    if(M==8)
        total*=3;
    if(M==16)
        total*=4;

   bits=new bool[total];


   //Si la modulación es diferencial se codifica de forma diferencial
   if(diferencial)
   {
       int *symbols_detected_temp;
       symbols_detected_temp = new int[symbols_change_length];
       for(int i=0;i<symbols_change_length;i++)
           symbols_detected_temp[i]=symbols_detected[i];
       for(int i=1;i<symbols_change_length;i++)
           symbols_detected[i]=symbols_detected_temp[i]-symbols_detected_temp[i-1];

       delete []symbols_detected_temp;
   }
    j=0;
    //se completan los bits
    for(int i=0;i<symbols_change_length;i++)
    {
        if(total_bits[i]!=0)
        {
            //se completa según el tipo de código y el tipo de modulación
            if(M==4)
            {
                if(code==0)
                {
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                        }
                    }
                }

                if(code==1)
                {
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                        }
                    }
                }

                if(code==2)
                {
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                        }
                    }
                }

                if(code==3)
                {
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                        }
                    }
                }
            }

            if(M==8)
            {
                if(code==0)
                {
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                }

                if(code==1)
                {
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                }

                if(code==2)
                {
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                }

                if(code==3)
                {
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                }

                if(code==4)
                {
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                }

                if(code==5)
                {
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                }

                if(code==6)
                {
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                }

                if(code==7)
                {
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                        }
                    }
                }

            }

            if(M==16)
            {
                if(code==0)
                {
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }

                if(code==1)
                {
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }

                if(code==2)
                {
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }

                if(code==3)
                {
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }

                if(code==4)
                {
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }

                if(code==5)
                {
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }

                if(code==6)
                {
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }

                if(code==7)
                {
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }

                if(code==8)
                {
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }

                if(code==9)
                {
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }

                if(code==10)
                {
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }

                if(code==11)
                {
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }

                if(code==12)
                {
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }

                if(code==13)
                {
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }

                if(code==14)
                {
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }

                if(code==15)
                {
                    if(symbols_detected[i]==1)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==2)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==3)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==4)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==5)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;;
                        }
                    }
                    if(symbols_detected[i]==6)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==7)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==8)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=0;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==9)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==10)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==11)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==12)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=1;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==13)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=0;
                        }
                    }
                    if(symbols_detected[i]==14)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=1;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==15)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=1;
                        }
                    }
                    if(symbols_detected[i]==0)
                    {
                        for(int k=0;k<total_bits[i];k++)
                        {
                            bits[j+k*2]=1;
                            bits[j+1+k*2]=0;
                            bits[j+2+k*2]=0;
                            bits[j+3+k*2]=0;
                        }
                    }
                }


            }

        }
        if(M==4)
            j+=2*total_bits[i];
        if(M==8)
            j+=3*total_bits[i];
        if(M==16)
            j+=4*total_bits[i];
    }

    *bits_length=j;

    return 1;
}
