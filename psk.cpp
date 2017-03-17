#include"psk.h"
#include "filters.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Costas Loop
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dem_costas_BPSK(double* signal_dem, double *signal_dem_threshold, double *signal_in, double kc,double f0, double baud_rate, double fm, int N)
{

    *signal_dem_threshold=0;
    double *signal_Q = new double[N];
    //   signal_in ===> arreglo de valores de señal modulada
                   //   ndata ===> número de datos de la señal modulada
                   //   baud_rate ===> frecuencia en Hz de la señal modulada portadora
                   //   fm ===> frecuencia de muestreo
                   //  levels_I == señal I en fase		(salida)
                   //  levels_Q== señal Q en cuadratura (salida)



                //variables asociadas a los filtros de rama FIR
                // internamente define las siguientes variables internas importantes
                //  valor del polo del filtro
                // calcula los valores de los coeficientes de los filtros FIR de acuerdo a la frecuencia de corte
                // la frecuencia de corte se calcula sobre la frecuencia de baud_rate.



                    double *b11;
                    b11=(double *)malloc(sizeof(double)*(200));//número máximo de coeficientes
                    //b11=malloc(200, sizeof (double));

                    int orden=50; //orden delfiltro
                    double w[200]={0};
                    double w1[200]={0};


                // variables asociadas al filtro del lazo IIR cero, polo
                  double  polo=  0.8;
                  double aa[2]= {1 , -polo};   //numerador
                  double bb[2]= {(1-polo)/2 ,  (1-polo)/2};// denominador
                  double VV[2]={0}; //registros de estado

                 //variables asociadas al NCO  sumador
                     double xsin=0, xcos=0;

                     //int NN=16;		 // nümero de bits de tabla ROM
                     double WWW[2]={0};		// registros del integrador


                   // variables asociadas al retardador I
                       double WI[2]={0};


                 // variables asociadas al retardador Q

                        double WQ[2]={0};






        // funciones prototipos definidas

    //  		  void    calcu_fir2(double *, int , double , double ) ;
    //
    //          double firI(  double *b, int M1, double *w,  double x )  ;
    //          double firQ(  double *b, int M1,  double *w,  double x )   ;
    //          double firlazo (  double *b, double *a, double *w, double x   ) ;
    //
    //          void    vco(double *, double * , double , double *);
    //
    //          double 	  delay_Q( double , double *);
    //          double 	  delay_I( double , double *);

        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                // variables locales  para el ajuste

                 double *err=(double *)malloc(sizeof(double)*(N+1));
                 err[0]=0;
                 double Mixer1=0;
                 double Mixer2=0;
                  double Mix3=0;
                 double Mix4=0;

                 double Mix1=0, Mix2=0;

                 double fase=0, yy=0;




         //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

                    //llamada al cálculo de los coeficientes filtro FIR



            calcu_fir2( b11, orden, baud_rate, fm );

            int MMM=65536;

                 //+++++++++++++++++++++++++++++++++++++++++++++++++++++

                 //+++++++++++++++++++++++++++++++++++++++++++++++++++++
                   err[0]=0;
                  int j=0;

                   //+++++++++++++++++++++++++++++++++++++++++++++++++++
                            for(j=0; j<N; j++) {

                                vco(&xcos,&xsin, err[j] , WWW, f0, fm);   // salida del vco

                                Mixer1=  xcos*signal_in[j];  // mezclador de I
                                Mixer2=  xsin*signal_in[j]; 	   //mezclador de Q

                                Mix1=delay_I(Mixer1, WI) ;				   // retardo
                                Mix2=delay_Q(Mixer2, WQ) ;	//retardo

                                signal_dem[j]=firI(  b11,orden, w, Mix1 )  ;  			//Filtro de rama  I
                                signal_Q[j]=firQ(  b11,orden, w1, Mix2 )  ;

                               ; 		   // Filro de rama Q

                                Mix3=signal_dem[j]* signo(signal_Q[j]);
                                Mix4= signal_Q[j]*signo(signal_dem[j]);

                                fase=Mix3-Mix4;  	// fase
                                yy=  firlazo (  bb, aa, VV, fase   )  ;   //filtrado de fase
                                err[j+1]= yy * kc/fm*MMM;   // error para modular el vco


                       }

      delete[] signal_Q;
      free(b11);
      free(err);
}

void dem_costas_QPSK (double* &signal_I, double* &signal_Q,int* signal_I_length, int* &symbols_dem, double* signal_in, int signal_length, double signal_baud_rate, double signal_fm, double f0)
{
    //   signal_in     ===> arreglo de valores de se?al modulada
       //   signal_length  ===> n?mero de datos de la se?al modulada
       //   fo   ===> frecuencia en Hz de la se?al modulada portadora
       //   signal_baud_rate    ===> frecuencia de bit
       //  signal_fm ===> frecuencia de muestreo

       //  signal_I en fase		(salida)
       //  signal_Q en cuadratura (salida)


       signal_I = new double[signal_length];
       signal_Q = new double[signal_length];
       symbols_dem = new int[signal_length];

       // IMPORTANTE GANANCIA DEL NCO

       double kc=100;

       double corte; //frecuencia de corte asociado a los filtros de rama

       double NN=65536; // N?MERO DE PUNTOS DE LA RAM

       corte=2* signal_baud_rate/signal_fm;
       // corte=0.1; //aumentando

       // muestras por simbolo

       int samples_symbol= (int)(signal_fm/ signal_baud_rate) ;

         double sam_symbol= signal_fm/ signal_baud_rate ;	  // para ser usada por el generador de sincronismo NEW

       //variables asociadas a los filtros de rama FIR
       int N=75;
       double b1[75]={0};
       double w[75]={0};
       double w1[75]={0};

       // variables asociadas al filtro del lazo IIR cero, polo
       double  polo=  0.9;
       double aa[2]= {1 , -0.9};   //numerador
       double bb[2]= {0.05 ,  0.05};// denominador
       double VV[2]={0}; //registros de estado

       //variables asociadas al NCO  sumador
       double xsin=0, xcos=0;


       double WWW[2]={0};		// registros del integrador del oscilador num?rico

       // variables asociadas al retardador I
       double WI[2]={0};


       // variables asociadas al retardador Q

       double WQ[2]={0};


       // variables asociadas al retardador de bit

       double Wbit[25]={0};


       //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       // variables locales  para el ajuste

       double err=0;
       double Mixer1=0;
       double Mixer2=0;

       double Isigno;
       double Qsigno;

       double Mix1=0, Mix2=0;


       double Mixer3=0;



       // calculo de los coeficientes del filtro


       fir_coef(b1, N-1 , corte) ;


       //+++++++++++++++++++++++++++++++++++++++++++++++++++++
       err=0;

       double k3= kc/signal_fm*NN;

       double I, Idel, Q;
       //+++++++++++++++++++++++++++++++++++++++++++++++++++
       for(int j=0; j<signal_length; j++)
       {
           vco(&xcos,&xsin, f0, err , WWW, signal_fm, NN);   // salida del vco


           Mixer1=  xcos*signal_in[j];         // mezclador de I
           Mixer2=  xsin*signal_in[j]; 	   //mezclador de Q

           Mix1=delay_I(Mixer1, WI) ;				   // retardo
           Mix2=delay_Q(Mixer2, WQ) ;				   //retardo

           I=firI(  N, b1, w, Mix1 )  ;  			//Filtro de rama  I
           Q=firQ(  N, b1, w1, Mix2 )  ; 		   // Filro de rama Q

                                Isigno=signo(I);
                                Qsigno=signo(Q);

                               Mixer3=  I*Qsigno- Q*Isigno;  	// mezclador del error

                      err= firlazo (  bb, aa, VV, Mixer3   )* k3;   // filtrado del error para modular el vco


           signal_I[j]=I;
           signal_Q[j]=Q  ;


       }



       //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

       // GENERACION DEL SINCRONISMO A PARTIR DE SIGNAL_I y SIGNAL_Q

       //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

       // variables definidas

       char  *bits=0 ;
       int  j;
       char half_clock=0;	  //reloj de medio simbolo
       char clock=0; // full reloj

       double reg_clock1, reg_clock11, reg_clock2, reg_clock22;
       char clock_new,  clock_new2;


       double *dte;
       double *dte_fil ;
       char  *syn_clk; //puntero de reloj sincronizado

       // registros asociados al dte

       double W1[3]={0};	  //para sigI
       double W1A[3]={0};	  // para sigQ

       // registros del filtro de error
       double  W2[2]={0};

       // registros asociado al oscilador

       double W3[2]={0};

       //registros asociados al detector de m?ximo de reloj
       double W4[2]={0};
       double W5[2]={0};

       // asignacion din?mica de memoria  esto es para control del programa nada mas

       dte= new double[signal_length];
       dte_fil = new double[signal_length];
       syn_clk = new char[signal_length];


       for(j=0; j<signal_length; j++)
       {
           //primer paso.
           // Se calcula el error dte de acuerdo a las se?ales I,Q
           // en los instantes del reloj de medio simbolo

           dte[j]=cal_dte2(half_clock,  signal_I[j], signal_Q[j], W1, W1A );

           // segundo paso
           // el error se filtra por un filtro proporcional e integrador
           // el error filtrado se activa con reloj completo

           dte_fil[j]=fil_loop( clock,  dte[j],  W2);

           //tercer paso generar los relojes  con un oscilador n?merico

           //reg_clock1= synchro_nco( dte_fil[j],W3);	 //salida del primer contador
               reg_clock1= synchro_nco( dte_fil[j],W3, sam_symbol);	 //salida del primer contador RECTIFICADO NEW
           // la salida clock1 es el acumulado de la se?al de entrada una rampa que se incrementa conel tiempo
           // hay que calcular el m?dulo 1 de la misma.

           reg_clock11= (int) reg_clock1;

           reg_clock1= reg_clock1- reg_clock11;	  // ahora este valor est? entre  0---1

           // detectar cuando hay un m?ximo en clock 2   almacenando dos lecturas consecutivas


           W4[0]=reg_clock1;

           //reg_clock3= W4[1]; //salida de reloj retardada NO HACE NADA

           // si el valor actual es menor que el precedente se pone 1

           if ( W4[0] < W4[1])

               clock_new='1';

           else

               clock_new='0';

           W4[1]=W4[0]; //actualizo registros para el futuro

           //   actualizando el reloj full para el filtro

           clock=clock_new; //actualizando el reloj completo  que se usa en funci?n <fil_loop>

           syn_clk[j]=clock_new; // registro de la se?al de reloj  que se usar? en muestrear I, Q

           // generar el reloj de medio simbolo a partir del  reloj clock synchro  normalizado en linea 626

           if( reg_clock1 > 0.5)

               reg_clock2= reg_clock1 - 0.5;
           else
               reg_clock2= reg_clock1;		  //esto genera dos pulsos de clock2 por uno de clock1

           //================================================
           // ahora hay que detectar los m?ximos de este reloj de medio simbolo  reg_clock2 normalizado

           W5[0]=reg_clock2;

           //reg_clock3= W4[1]; //salida de reloj retardada NO HACE NADA

           // si el valor actual es menor que el precedente se pone 1

           if ( W5[0] < W5[1])

               clock_new2='1';

           else

               clock_new2='0';

           W5[1]=W5[0]; //actualizo registros para el futuro

           //   actualizando el reloj full para el filtro

           half_clock=clock_new2; //actualizando el reloj completo  que se usa en funci?n <fil_loop>

       }


       int k=0;

       double *Sig_Im, *Sig_Qm;  // variables temporales para se?ales en cuadratura muestreadas

       // Primeramente se muestrean las se?ales I, Q demoduladas con  el reloj sincr?nico syn_clk

       // se le asigna memoria din?mica

       int length = (int)(ceil((double)signal_length/(double)samples_symbol));

       Sig_Im = new double[length];
       Sig_Qm = new double[length];




       for(j=0; j<signal_length; j++)
       {
   //         printf("%d -> %c\n",j,syn_clk[j]);
           if ( syn_clk[j]=='1' && k<length)
           {
   //             printf("%f",Sig_Im[k]);
               Sig_Im[k]= signo(signal_I[j]);
               Sig_Qm[k]= signo(signal_Q[j]) ;
               k++;
           }

       }



       // se extienden las se?ales muestreadas para tener un n?mero de puntos igual a de la se?al de entrada

       // para ello se crean  signal_II y signal_QQ

       double * signal_II;
       double * signal_QQ;

       signal_II = new double[signal_length];
       signal_QQ = new double[signal_length];



       for(j=0; j< length; j++)
       {


           for (k= ( samples_symbol)*j; k< (samples_symbol)* j+ (samples_symbol)      ; k++ )  {

               signal_II[k]= Sig_Im[j];
               signal_QQ[k]= Sig_Qm[j];


           }


       }


       // ahora se asignan los simbolos.



       for (j=0; j< signal_length; j++)
       {


           if (signal_II[j]==1)
           {

               if (signal_QQ[j]==1)

                   symbols_dem[j]=0 ;
               else
                   symbols_dem[j]=1;
           }





           else
           {
               if (signal_QQ[j]==1)

                   symbols_dem[j]=2 ;

               else
                   symbols_dem[j]=3;


           }


       }





       //liberando memoria interna de la funcion

       delete []dte;
       delete []dte_fil;
       delete []syn_clk;

       delete []signal_II;
       delete []signal_QQ;

       delete []Sig_Im;
       delete []Sig_Qm;

       *signal_I_length= signal_length ;
}

void dem_costas_OQPSK(double* &signal_I, double* &signal_Q, int* signal_I_length, int* &symbols_dem, double* signal_in, int signal_length, double signal_baud_rate, double signal_fm, double f0)
{

    //   signal_in     ===> arreglo de valores de se?al modulada
       //   signal_length  ===> n?mero de datos de la se?al modulada
       //   fo   ===> frecuencia en Hz de la se?al modulada portadora
       //   signal_baud_rate    ===> frecuencia de bit
       //  signal_fm ===> frecuencia de muestreo

       //  signal_I en fase		(salida)
       //  signal_Q en cuadratura (salida)


       signal_I = new double[signal_length];
       signal_Q = new double[signal_length];
       symbols_dem = new int[signal_length];

       // IMPORTANTE GANANCIA DEL NCO

       double kc=100;

       double corte; //frecuencia de corte asociado a los filtros de rama

       double NN=65536; // N?MERO DE PUNTOS DE LA RAM

       corte=2* signal_baud_rate/signal_fm;
       // corte=0.1; //aumentando

       // muestras por simbolo

       int samples_symbol= (int)(signal_fm/ signal_baud_rate) ;

         double sam_symbol= signal_fm/ signal_baud_rate ;	  // para ser usada por el generador de sincronismo NEW

       //variables asociadas a los filtros de rama FIR
       int N=75;
       double b1[75]={0};
       double w[75]={0};
       double w1[75]={0};

       // variables asociadas al filtro del lazo IIR cero, polo
       double  polo=  0.9;
       double aa[2]= {1 , -0.9};   //numerador
       double bb[2]= {0.05 ,  0.05};// denominador
       double VV[2]={0}; //registros de estado

       //variables asociadas al NCO  sumador
       double xsin=0, xcos=0;


       double WWW[2]={0};		// registros del integrador del oscilador num?rico

       // variables asociadas al retardador I
       double WI[2]={0};


       // variables asociadas al retardador Q

       double WQ[2]={0};


       // variables asociadas al retardador de bit

       double Wbit[25]={0};


       //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       // variables locales  para el ajuste

       double err=0;
       double Mixer1=0;
       double Mixer2=0;

       double Isigno;
       double Qsigno;

       double Mix1=0, Mix2=0;


       double Mixer3=0;



       // calculo de los coeficientes del filtro


       fir_coef(b1, N-1 , corte) ;


       //+++++++++++++++++++++++++++++++++++++++++++++++++++++
       err=0;

       double k3= kc/signal_fm*NN;

       double I, Idel, Q;
       //+++++++++++++++++++++++++++++++++++++++++++++++++++
       for(int j=0; j<signal_length; j++)
       {
           vco(&xcos,&xsin, f0, err , WWW, signal_fm, NN);   // salida del vco


           Mixer1=  xcos*signal_in[j];         // mezclador de I
           Mixer2=  xsin*signal_in[j]; 	   //mezclador de Q

           Mix1=delay_I(Mixer1, WI) ;				   // retardo
           Mix2=delay_Q(Mixer2, WQ) ;				   //retardo

           I=firI(  N, b1, w, Mix1 )  ;  			//Filtro de rama  I
           Q=firQ(  N, b1, w1, Mix2 )  ; 		   // Filro de rama Q


            // retarndando un bit en la rama I ibteniendo Idel

                                  Idel=delay_tbit( I , Wbit);


                               Mixer3=  (Idel*Idel-Q*Q)*( Idel*Q );  	// mezclador del error





                      err= firlazo (  bb, aa, VV, Mixer3   )* k3;   // filtrado del error para modular el vco


                              signal_I[j]=Idel;
                               signal_Q[j]=Q  ;

       }



       //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

       // GENERACION DEL SINCRONISMO A PARTIR DE SIGNAL_I y SIGNAL_Q

       //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

       // variables definidas

       char  *bits=0 ;
       int  j;
       char half_clock=0;	  //reloj de medio simbolo
       char clock=0; // full reloj

       double reg_clock1, reg_clock11, reg_clock2, reg_clock22;
       char clock_new,  clock_new2;


       double *dte;
       double *dte_fil ;
       char  *syn_clk; //puntero de reloj sincronizado

       // registros asociados al dte

       double W1[3]={0};	  //para sigI
       double W1A[3]={0};	  // para sigQ

       // registros del filtro de error
       double  W2[2]={0};

       // registros asociado al oscilador

       double W3[2]={0};

       //registros asociados al detector de m?ximo de reloj
       double W4[2]={0};
       double W5[2]={0};

       // asignacion din?mica de memoria  esto es para control del programa nada mas

       dte= new double[signal_length];
       dte_fil = new double[signal_length];
       syn_clk = new char[signal_length];


       for(j=0; j<signal_length; j++)
       {
           //primer paso.
           // Se calcula el error dte de acuerdo a las se?ales I,Q
           // en los instantes del reloj de medio simbolo

           dte[j]=cal_dte2(half_clock,  signal_I[j], signal_Q[j], W1, W1A );

           // segundo paso
           // el error se filtra por un filtro proporcional e integrador
           // el error filtrado se activa con reloj completo

           dte_fil[j]=fil_loop( clock,  dte[j],  W2);

           //tercer paso generar los relojes  con un oscilador n?merico

           //reg_clock1= synchro_nco( dte_fil[j],W3);	 //salida del primer contador
               reg_clock1= synchro_nco( dte_fil[j],W3, sam_symbol);	 //salida del primer contador RECTIFICADO NEW
           // la salida clock1 es el acumulado de la se?al de entrada una rampa que se incrementa conel tiempo
           // hay que calcular el m?dulo 1 de la misma.

           reg_clock11= (int) reg_clock1;

           reg_clock1= reg_clock1- reg_clock11;	  // ahora este valor est? entre  0---1

           // detectar cuando hay un m?ximo en clock 2   almacenando dos lecturas consecutivas


           W4[0]=reg_clock1;

           //reg_clock3= W4[1]; //salida de reloj retardada NO HACE NADA

           // si el valor actual es menor que el precedente se pone 1

           if ( W4[0] < W4[1])

               clock_new='1';

           else

               clock_new='0';

           W4[1]=W4[0]; //actualizo registros para el futuro

           //   actualizando el reloj full para el filtro

           clock=clock_new; //actualizando el reloj completo  que se usa en funci?n <fil_loop>

           syn_clk[j]=clock_new; // registro de la se?al de reloj  que se usar? en muestrear I, Q

           // generar el reloj de medio simbolo a partir del  reloj clock synchro  normalizado en linea 626

           if( reg_clock1 > 0.5)

               reg_clock2= reg_clock1 - 0.5;
           else
               reg_clock2= reg_clock1;		  //esto genera dos pulsos de clock2 por uno de clock1

           //================================================
           // ahora hay que detectar los m?ximos de este reloj de medio simbolo  reg_clock2 normalizado

           W5[0]=reg_clock2;

           //reg_clock3= W4[1]; //salida de reloj retardada NO HACE NADA

           // si el valor actual es menor que el precedente se pone 1

           if ( W5[0] < W5[1])

               clock_new2='1';

           else

               clock_new2='0';

           W5[1]=W5[0]; //actualizo registros para el futuro

           //   actualizando el reloj full para el filtro

           half_clock=clock_new2; //actualizando el reloj completo  que se usa en funci?n <fil_loop>

       }


       int k=0;

       double *Sig_Im, *Sig_Qm;  // variables temporales para se?ales en cuadratura muestreadas

       // Primeramente se muestrean las se?ales I, Q demoduladas con  el reloj sincr?nico syn_clk

       // se le asigna memoria din?mica

       int length = (int)(ceil((double)signal_length/(double)samples_symbol));

       Sig_Im = new double[length];
       Sig_Qm = new double[length];




       for(j=0; j<signal_length; j++)
       {
   //         printf("%d -> %c\n",j,syn_clk[j]);
           if ( syn_clk[j]=='1' && k<length)
           {
   //             printf("%f",Sig_Im[k]);
               Sig_Im[k]= signo(signal_I[j]);
               Sig_Qm[k]= signo(signal_Q[j]) ;
               k++;
           }

       }



       // se extienden las se?ales muestreadas para tener un n?mero de puntos igual a de la se?al de entrada

       // para ello se crean  signal_II y signal_QQ

       double * signal_II;
       double * signal_QQ;

       signal_II = new double[signal_length];
       signal_QQ = new double[signal_length];



       for(j=0; j< length; j++)
       {


           for (k= ( samples_symbol)*j; k< (samples_symbol)* j+ (samples_symbol)      ; k++ )  {

               signal_II[k]= Sig_Im[j];
               signal_QQ[k]= Sig_Qm[j];


           }


       }


       // ahora se asignan los simbolos.



       for (j=0; j< signal_length; j++)
       {


           if (signal_II[j]==1)
           {

               if (signal_QQ[j]==1)

                   symbols_dem[j]=0 ;
               else
                   symbols_dem[j]=1;
           }





           else
           {
               if (signal_QQ[j]==1)

                   symbols_dem[j]=2 ;

               else
                   symbols_dem[j]=3;


           }


       }

       //liberando memoria interna de la funcion

       delete []dte;
       delete []dte_fil;
       delete []syn_clk;

       delete []signal_II;
       delete []signal_QQ;

       delete []Sig_Im;
       delete []Sig_Qm;

       *signal_I_length= signal_length ;

}

void dem_costas_8PSK (double* &signal_I, double* &signal_Q,int* signal_I_length, int* &symbols_dem, double* signal_in, int signal_length, double signal_baud_rate, double signal_fm, double f0)
{
    //   signal_in     ===> arreglo de valores de se?al modulada
        //   signal_length  ===> n?mero de datos de la se?al modulada
        //   fo   ===> frecuencia en Hz de la se?al modulada portadora
        //   signal_baud_rate    ===> frecuencia de bit
        //  signal_fm ===> frecuencia de muestreo

        //  signal_I en fase		(salida)
        //  signal_Q en cuadratura (salida)

        signal_I = new double[signal_length];
        signal_Q = new double[signal_length];
        symbols_dem = new int[signal_length];

        // IMPORTANTE GANANCIA DEL NCO

        double kc=1000;

        double corte; //frecuencia de corte asociado a los filtros de rama

        double NN=65536; // N?MERO DE PUNTOS DE LA RAM

        corte=6* signal_baud_rate/signal_fm;
        // corte=0.1; //aumentando


        // muestras por simbolo

        int samples_symbol= (int)(signal_fm/ signal_baud_rate) ;
         double sam_symbol= signal_fm/ signal_baud_rate ;	  // para ser usada por el generador de sincronismo.
        //variables asociadas a los filtros de rama FIR
        int N=75;
        double b1[75]={0};
        double w[75]={0};
        double w1[75]={0};

        // variables asociadas al filtro del lazo IIR cero, polo
        double  polo=  0.9;
        double aa[2]= {1 , -0.9};   //numerador
        double bb[2]= {0.05 ,  0.05};// denominador
        double VV[2]={0}; //registros de estado

        //variables asociadas al NCO  sumador
        double xsin=0, xcos=0;


        double WWW[2]={0};		// registros del integrador del oscilador num?rico


        // variables asociadas al retardador I
        double WI[2]={0};


        // variables asociadas al retardador Q

        double WQ[2]={0};

        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // variables locales  para el ajuste

        double err=0;
        double Mixer1=0;
        double Mixer2=0;

        double Isigno;
        double Qsigno;

        double Mix1=0, Mix2=0;

        double Mixer3=0;


        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // calculo de los coeficientes del filtro



        fir_coef(b1, N-1 , corte) ;



        //+++++++++++++++++++++++++++++++++++++++++++++++++++++
        err=0;

        double k3= kc/signal_fm*NN;

        double I, Q;

        double IQ, del_IQ2;
        //+++++++++++++++++++++++++++++++++++++++++++++++++++
        for(int j=0; j<signal_length; j++)
        {

            vco(&xcos,&xsin, f0, err , WWW, signal_fm, NN);   // salida del vco


            Mixer1=  xcos*signal_in[j];         // mezclador de I
            Mixer2=  xsin*signal_in[j]; 	   //mezclador de Q

            Mix1=delay_I(Mixer1, WI) ;				   // retardo
            Mix2=delay_Q(Mixer2, WQ) ;				   //retardo

            I=firI(  N, b1, w, Mix1 )  ;  			//Filtro de rama  I
            Q=firQ(  N, b1, w1, Mix2 )  ; 		   // Filro de rama Q


            // conformando las variables para el c?lculo del error

            IQ=2*I*Q;
            del_IQ2= (I*I)-(Q*Q);


            Mixer3=  ( (del_IQ2*del_IQ2)- (IQ*IQ) )* (IQ* del_IQ2  );  	// mezclador del error


            err= firlazo (  bb, aa, VV, Mixer3   )* k3;   // filtrado del error para modular el vco


            signal_I[j]=I;
            signal_Q[j]=Q;
        }



        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // GENERACION DEL SINCRONISMO A PARTIR DE SIGNAL_I y SIGNAL_Q

        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


        // variables definidas


        int  j;
        int half_clock=0;	  //reloj de medio simbolo
        int clock=0;         // full reloj

        double reg_clock1, reg_clock11, reg_clock2;
        int clock_new,  clock_new2;


        double *dte;
        double *dte_fil ;
        int  *syn_clk; //puntero de reloj sincronizado

        // registros asociados al dte

        double W1[3]={0};	  //para sigI
        double W1A[3]={0};	  // para sigQ

        // registros del filtro de error
        double  W2[2]={0};

        // registros asociado al oscilador

        double W3[2]={0};

        //registros asociados al detector de m?ximo de reloj
        double W4[2]={0};
        double W5[2]={0};

        // asignacion din?mica de memoria  esto es para control del programa nada mas

        dte = new double[signal_length];
        dte_fil = new double[signal_length];

        syn_clk = new int[signal_length];

        // CALCULO DEL RELOJ DE SINCRONISMO DE BIT

        for(int j=0; j<signal_length; j++) {


            //primer paso.
            // Se calcula el error dte de acuerdo a las se?ales I,Q
            // en los instantes del reloj de medio simbolo

            dte[j]=cal_dte2(half_clock,  signal_I[j], signal_Q[j], W1, W1A );

            // segundo paso
            // el error se filtra por un filtro proporcional e integrador
            // el error filtrado se activa con reloj completo

            dte_fil[j]=fil_loop( clock,  dte[j],  W2);

            //tercer paso generar los relojes  con un oscilador n?merico

            reg_clock1= synchro_nco( dte_fil[j],W3, sam_symbol);	 //salida del primer contador
            // la salida clock1 es el acumulado de la se?al de entrada una rampa que se incrementa conel tiempo
            // hay que calcular el m?dulo 1 de la misma.

            reg_clock11= (int) reg_clock1;

            reg_clock1= reg_clock1- reg_clock11;	  // ahora este valor est? entre  0---1

            // detectar cuando hay un m?ximo en clock 2   almacenando dos lecturas consecutivas


            W4[0]=reg_clock1;

            //reg_clock3= W4[1]; //salida de reloj retardada NO HACE NADA

            // si el valor actual es menor que el precedente se pone 1

            if ( W4[0] < W4[1])

                clock_new=1;

            else

                clock_new=0;

            W4[1]=W4[0]; //actualizo registros para el futuro

            //   actualizando el reloj full para el filtro

            clock=clock_new; //actualizando el reloj completo  que se usa en funci?n <fil_loop>

            syn_clk[j]=clock_new; // registro de la se?al de reloj  que se usar? en muestrear I, Q

            // generar el reloj de medio simbolo a partir del  reloj clock synchro  normalizado en linea 626

            if( reg_clock1 > 0.5)

                reg_clock2= reg_clock1 - 0.5;
            else
                reg_clock2= reg_clock1;		  //esto genera dos pulsos de clock2 por uno de clock1

            //================================================
            // ahora hay que detectar los m?ximos de este reloj de medio simbolo  reg_clock2 normalizado

            W5[0]=reg_clock2;

            //reg_clock3= W4[1]; //salida de reloj retardada NO HACE NADA

            // si el valor actual es menor que el precedente se pone 1

            if ( W5[0] < W5[1])

                clock_new2=1;

            else

                clock_new2=0;

            W5[1]=W5[0]; //actualizo registros para el futuro

            //   actualizando el reloj full para el filtro

            half_clock=clock_new2; //actualizando el reloj completo  que se usa en funci?n <fil_loop>




        }

        // calculando los umbrales para decodificar los simbolos en cada se?al

        double umbral_I,  umbral_Q;

        umbral_I= calcula_umbral(signal_I, signal_length);

        umbral_Q= calcula_umbral(signal_Q, signal_length);


        // MUESTREO DE LAS SE?ALES EN CUADRATURA CON EL RELOJ SINCRONICO

        double *Sig_Im, *Sig_Qm;  // variables temporales para se?ales en cuadratura muestreadas

        int k=0;

        // Primeramente se muestrean las se?ales I, Q demoduladas con  el reloj sincr?nico syn_clk

        // se le asigna memoria din?mica

        int length =signal_length/ samples_symbol;
        Sig_Im = new double[length];
        Sig_Qm = new double[length];


        for(int j=0; j<signal_length; j++) {


            // se muestrea al reloj alto
            if ( syn_clk[j]==1 && k<length)
            {			 //inicio if  #1


                if ( signal_I[j]>=0)  { // si la se?al es positiva

                    if ( signal_I[j]> umbral_I)

                        Sig_Im[k]=1;
                    else

                        Sig_Im[k]=0.5;



                }

                else //si es negativa {
                {


                    if (signal_I[j] > -umbral_I)

                        Sig_Im[k]=-0.5;
                    else

                        Sig_Im[k]=-1;


                }		 // end del if	  #1


                // si la se?al es positiva
                if ( signal_Q[j]>=0)  {

                    if ( signal_Q[j]> umbral_Q)

                        Sig_Qm[k]=1;
                    else
                        Sig_Qm[k]=0.5;

                }

                else //si es negativa
                {


                    if (signal_Q[j] > -umbral_Q)

                        Sig_Qm[k]=-0.5;
                    else

                        Sig_Qm[k]=-1;


                }
                k++;
            }


        }


        // se extienden las se?ales muestreadas para tener un n?mero de puntos igual a de la se?al de entrada

        // para ello se crean  signal_II y signal_QQ

        double * signal_II;
        double * signal_QQ;

        signal_II = new double[signal_length];
        signal_QQ = new double[signal_length];



        for(int j=0; j< (signal_length / samples_symbol); j++) {


            for (k= ( samples_symbol)*j  ; k< (samples_symbol)* j+ (samples_symbol)      ; k++ )  {

                signal_II[k]= Sig_Im[j];
                signal_QQ[k]= Sig_Qm[j];


            }


        }

        // ahora se asignal los simbolos.
        // codigo binario normal


        for (int j=0; j< signal_length; j++)  {


            if (signal_II[j]==1)   {

                if (signal_QQ[j]==0.5)

                    symbols_dem[j]=0 ;

                if (signal_QQ[j]==-0.5)

                    symbols_dem[j]=7;

            }
            //=======================================

            if (signal_II[j]==0.5)   {

                if (signal_QQ[j]== 1)

                    symbols_dem[j]=1 ;

                if (signal_QQ[j]==- 1)

                symbols_dem[j]=6;
            }

            //++++++++++++++++++++++++++++++++++++++++++++++

            if (signal_II[j]==-0.5)   {

                if (signal_QQ[j]== 1)

                    symbols_dem[j]=2 ;

                if (signal_QQ[j]==- 1)

                    symbols_dem[j]=5;
            }

            //+++++++++++++++++++++++++++++++++++++++++++++

            if (signal_II[j]==-1)   {

                if (signal_QQ[j]== 0.5)

                    symbols_dem[j]=3 ;

                if (signal_QQ[j]==- 0.5)

                    symbols_dem[j]=4;
            }

        }


        //liberando memoria interna de la funcion

        delete []dte;
        delete []dte_fil;
        delete []syn_clk;

        delete []signal_II;
        delete []signal_QQ;

        delete []Sig_Im;
        delete []Sig_Qm ;

        *signal_I_length= signal_length;
}

bool psk_costas_loop(double* &signal_i_avg, double* &signal_q_avg, int *signal_I_length, double *signal_in,  int NDATA, double fcarrier, double baud_rate, double fs,double kc)
{
           //   signal_in ===> arreglo de valores de señal modulada
               //   ndata ===> número de datos de la señal modulada
               //   baud_rate ===> frecuencia en Hz de la señal modulada portadora
               //   fm ===> frecuencia de muestreo
               //  levels_I == señal I en fase		(salida)
               //  levels_Q== señal Q en cuadratura (salida)



            //variables asociadas a los filtros de rama FIR
            // internamente define las siguientes variables internas importantes
            //  valor del polo del filtro
            // calcula los valores de los coeficientes de los filtros FIR de acuerdo a la frecuencia de corte
            // la frecuencia de corte se calcula sobre la frecuencia de baud_rate.

 //se reserva espacio
    double *signal_I = new double[NDATA];
    double *signal_Q = new double [NDATA];

    double *b11;
    b11=(double *)malloc(sizeof(double)*(200));//número máximo de coeficientes
    //b11=malloc(200, sizeof (double));

    int orden=50; //orden delfiltro
    double w[200]={0};
    double w1[200]={0};


    // variables asociadas al filtro del lazo IIR cero, polo
    double  polo=  0.8;
    double aa[2]= {1 , -polo};   //numerador
    double bb[2]= {(1-polo)/2 ,  (1-polo)/2};// denominador
    double VV[2]={0}; //registros de estado

    //variables asociadas al NCO  sumador
    double xsin=0, xcos=0;

    int NN=16;		 // nümero de bits de tabla ROM
    double WWW[2]={0};		// registros del integrador


    // variables asociadas al retardador I
    double WI[2]={0};


    // variables asociadas al retardador Q

    double WQ[2]={0};


    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // variables locales  para el ajuste

    double *err=(double *)malloc(sizeof(double)*(NDATA+1));
    err[0]=0;
    double Mixer1=0;
    double Mixer2=0;
    double Mix3=0;
    double Mix4=0;

    double Mix1=0, Mix2=0;

    double fase=0, yy=0;




     //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //llamada al cálculo de los coeficientes filtro FIR



    calcu_fir2( b11, orden, baud_rate, fs);

    int MMM=65536;

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++
    err[0]=0;
    int j=0;

    //+++++++++++++++++++++++++++++++++++++++++++++++++++
    for(j=0; j<NDATA; j++) {

        vco(&xcos,&xsin, err[j] , WWW, fcarrier, fs);   // salida del vco

        Mixer1=  xcos*signal_in[j];  // mezclador de I
        Mixer2=  xsin*signal_in[j]; 	   //mezclador de Q

        Mix1=delay_I(Mixer1, WI) ;				   // retardo
        Mix2=delay_Q(Mixer2, WQ) ;	//retardo

        signal_I[j]=firI(  b11,orden, w, Mix1 )  ;  			//Filtro de rama  I
        signal_Q[j]=firQ(  b11,orden, w1, Mix2 )  ;

        ; 		   // Filro de rama Q

        Mix3=signal_I[j]* signo(signal_Q[j]);
        Mix4= signal_Q[j]*signo(signal_I[j]);

        fase=Mix3-Mix4;  	// fase
        yy=  firlazo (  bb, aa, VV, fase   )  ;   //filtrado de fase
        err[j+1]= yy * kc/fs*MMM;   // error para modular el vco


    }



    //determinación del ángulo de la constelación
    double Ts_m=fs/baud_rate;
    double x0=0;
    double x1=0;
    double cumul_i=0;
    double cumul_q=0;

    //validación
    if(Ts_m<10)
    {
        delete[] signal_I;
        delete[] signal_Q;
        free(b11);
        free(err);
    }

    signal_i_avg=new double[int(floorf(NDATA/Ts_m)-1)];
    signal_q_avg=new double[int(floorf(NDATA/Ts_m)-1)];
    *signal_I_length=int(floorf(NDATA/Ts_m)-1);

    for(int i=0;i<floorf(NDATA/Ts_m)-1;i++)
    {
        x0=floorf(i*Ts_m+Ts_m/4);
        x1=floorf(i*Ts_m+3*Ts_m/4);
        cumul_i=0;
        cumul_q=0;
        for(j=x0;j<=x1;j++)
        {
            cumul_i=cumul_i+signal_I[j];
            cumul_q=cumul_q+signal_Q[j];
        }

        signal_i_avg[i]=cumul_i;
        signal_q_avg[i]=cumul_q;

    }


    delete[] signal_I;
    delete[] signal_Q;
    free(b11);
    free(err);

    return 1;

}

void  calcu_fir2(double  *bb ,int M1, double  baud_rate,double fs)
{
    // cálculo de los coeficientes del filtro FIR
    // puntero de salida para los coeficientes calculados
    // número de coeficientes
    // baud rate de la señal para calcular la frecuencia de corte del filtro
    // frecuencia de muestreo del sistema
    double wpass;	 // frecuencia de paso del filtro

    double h1[200]={0}; //arreglo interno para almacenar los coeficientes



    int j=0;

    wpass= 2*M_PI*baud_rate/fs;  //frecuencia de corte del filtro

    // coeficiemtes positivos de 0 a M/2  (M/2+1) elementos

    for(j=0; j<=M1/2; j++)

        if (j==0)
            h1[0]=wpass/M_PI; else
            h1[j]= sin(wpass*j)/ (M_PI*j);

    //coeficientes totales....de 0 a M  (M+1) elementos...

    for (j=0; j<=M1; j++)
        if ( j<= (M1/2 ))
            bb[j]=h1[M1/2-j ];

        else bb[j]= h1[j-M1/2];

    // teniendi em cuenta la ventana de Hamming

    /*for (j=0; j<= M1; j++)
                       {
                     w[j]=(0.54-0.46* cos( 2*M_PI* (j)/M )) ;     // VENTANA DE HAMMING
                     bb[j]=b[j]* w[j];
                          }*/
}

double firQ( double *b, int ncoef, double *w,  double x )
{
    // filtro FIR de rama I

    // calcula la salida de un filtro FIR por puntos

    double y=0;
    int j;
    int M;

    M=ncoef;
    w[0]=x;

    for (y=0, j=0; j<=M  ; j++)
        y+=  b[j]* w[j];

    for (j=M; j>=1; j--)
        w[j]=w[j-1];

    return y;

}

double firI(double *b,int ncoef, double *w,  double x )
{
    // filtro FIR de rama I

    // calcula la salida de un filtro FIR por puntos

    double y=0;
    int j;
    int M;
    M=ncoef;
    w[0]=x;


    for (y=0, j=0; j<=M  ; j++)

        y+=  b[j]* w[j];

    for (j=M; j>=1; j--)
        w[j]=w[j-1];

    return y;
}

double firlazo ( double *b, double *a, double *w, double x   )
{
     // filtro de lazo correspondiente a un filtro IIR con polo y cero reales

    double y;

    w[0]=x - a[1]*w[1];
    y= w[0]*b[0]+ w[1]*b[1];
    w[1]=w[0];  				  // actualizo registros

    return y;
}

void   vco( double *xcos,double *xsin, double error,  double *W, double fcentral, double fs)
{
    // funcion del vco correspondiente a un sumador con wave tabla de 2^N valores

    // error de la fase a sumar
    // W registros del sumador
    // variables por referencia para el coseno y el seno calculados
    //double alfa=1;	   // ganancia del VCO
    double Ko, DCita, y;
    //double b[2]= {1, 0};
    //double a[2]= {1,  -1};
    //double fs=44100;
    int NN=65536;

    Ko= fcentral/(fs/NN);
    DCita= 2*M_PI* (Ko + error);

    W[0]=DCita  + W[1];
    y=W[0];  // valor de salida
    W[1]=W[0]; // actualizacion de registros


    *xcos=cos(y/NN);
    *xsin=sin(y/ NN);
    return;

}

double 	  delay_I( double x, double *W)
{
    //  filtro de retardador de rama   I
    double y;
    W[0]=x;
    y= W[1];
    W[1]=W[0];
    return y;
}

double 	  delay_Q( double x, double *W)
{
    //  filtro de retardador de rama   Q
    double y;
    W[0]=x;
    y= W[1];
    W[1]=W[0];
    return y;
}

double valor_abs(double x)
{
    if (x < 0.0)
        return -x;
    else
        return x;
}

double signo( double x)  {

    double y;

    if(  x==0 )  y=0;  else

        y=x/valor_abs(x);

    return y;



}

void    fir_coef(double *b,  int M , double corte)
{
    // corte = frecuencia de corte normalizada
    //  M orden de filtro par para lograr M+1 coeficientes impares
    //  b coeficientes calculados.
    // calcula los M+1 coeficientes a partir de la frecuencia de corte y empleando la ventana Hamming
    int j;
    double wn[100]={0};
    double h[100]={0};

    // primeramente se calcula la ventana de Hamming correspondiente con M+1 puntos

    for(j=0;j<M;j++)

        wn[j]= 0.54- 0.46* cos(2*M_PI*j/M);

    // calcula coeficientes positivos  del filtro ideal desde 1 a M/2

    for(j=1; j<=M/2; j++)
        h[j] = sin(corte*M_PI*j)/j/M_PI;

    h[0]=corte;   //redefine  el origen

    //  Calcula los coeficientes totales teniendo en cuenta la simetria par
    // ajusta simetria y multiplica por la ventana

    for(j=0;j<M; j++)	   {

        if(j<M/2)  b[j]= h[M/2-j] ; else  b[j]=h[j-M/2];	   // teniendo en cuenta la simetr�a


        b[j]=b[j]*wn[j]; // multiplicando por la ventana
    }


}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Square Loop
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool dem_quadratic_BPSK(double* signal_dem, double *signal_dem_threshold, double *signal_in, double f0, double baud_rate, double fm, int N)
{
    *signal_dem_threshold=0;
    double w0=2*M_PI*f0/fm; //frecuencia digital de la señal

        //printf("w_0->%f, w1->%f",w0,w1);

        //variables de recorrido
        int i=0,j=0;

        //diseño de filtro pasa-banda
        //definición de variables
        double wc_bpf, AW_bpf, Aten_bpf, BW_bpf, Beta_bpf, signal_in2, cumul_bpf;
        int M_bpf;
        //configuración del filtro
        wc_bpf = 2*w0;//frecuencia centro del filtro pasa-banda
        AW_bpf = 0.1*wc_bpf; //transición del filtro
        BW_bpf = 2*M_PI*baud_rate/fm; //ancho de banda del filtro pasa-banda
        //Atenuación del filtro en dB
        Aten_bpf = 40;//-20*log10(aten_lpf);
        // Obtención de Beta y M
        if(Aten_bpf > 50)
        {
            Beta_bpf = 0.1102*(Aten_bpf-8.7);
        }
        else if(Aten_bpf >= 21 && Aten_bpf <= 50)
        {
            Beta_bpf = 0.5842*powf((Aten_bpf-21),0.4)+0.07886*(Aten_bpf-21);
        }
        else if(Aten_bpf < 21)
        {
            Beta_bpf = 0;
        }
        //orden del filtro
        //M=fix((A-8)/(2.285*2*AW));
        M_bpf = ceilf((Aten_bpf-8)/(2.285*AW_bpf));

        if(fmodf(M_bpf,2)!=0)//orden par, tipo I
        {
            M_bpf=M_bpf+1;
        }
        int total_coeff_bpf=M_bpf+1;


        //validación
        if(total_coeff_bpf>100 || w0==0)
        {
            return 0;
        }

    //     printf("\nwc_bpf->%f, AW->%f, A->%f, BW->%f, M_bpf->%d\n",wc_bpf,AW_bpf,Aten_bpf,BW_bpf,M_bpf);

        //coeficientes del filtro pasa-banda
        double *b_bpf = filter_design_direct_I(M_bpf, Beta_bpf, wc_bpf-BW_bpf, wc_bpf+BW_bpf);
    //     double *b_bpf = filter_design_direct_I(M_bpf, Beta_bpf, 2*M_PI*1000/fm, 4*M_PI*2000/fm);
        //registros del filtro pasa-banda
        double *r_bpf=(double *)malloc(sizeof(double)*(total_coeff_bpf));

        //configuración del PLL
        double fo=2*f0;                // frecuencia libre del VCO en Hz
        double Chi=0.707;                // damping factor óptimo
        double B=1;                      // amplitud del tono de salida del VCO
        double kd=(2*B)/M_PI;              // Ganancia del Phase Detector step12 /109Best
        double ko=2*M_PI*fo;               // Ganancia del VCO
        double ka=10;                    // Ganancia del Filtro de lazo
        double Kv=kd*ko*ka;              // Ganancia del Lazo
        double lock_time=1/(baud_rate);  //configuración del tiempo de respuesta en segundos

        // Diseño del filtro de lazo cero-polo
        double wn=2*M_PI/lock_time; //frecuencia natural best2003
        double T2=(2*Chi/wn)-(1/Kv); //cte del filtro de lazo
        double T1=Kv/(wn*wn); //cte del filtro de lazo

        // conformación de filtro de lazo
        //numerador en el plano S
        double a1=ka*T2;
        double a2=ka;
        //denominador en el plano S
        double b1=T1;
        double b2=1;

        //transformación bilineal
        double b_1=(a1*2*fm+a2)/(b1*2*fm+b2);
        double b_2=(a2-a1*2*fm)/(b1*2*fm+b2);
        double r_2=(b2-b1*2*fm)/(b1*2*fm+b2);

        //variables del PLL
        double vco_out, vco_fase, out_pll_cos, out_pll_sin, phase_mult_out, phase_mult_out_1, vco_in, vco_in_1, cumul_pll;

        //diseño de filtro paso-bajo
        //definición de variables
        double wc_lpf, AW_lpf, Aten_lpf, BW_lpf, Beta_lpf, cumul_lpf_i, cumul_lpf_q;
        int M_lpf;
        //configuración del filtro
        wc_lpf = 2*M_PI*2*baud_rate/fm; //frecuencia centro del filtro pasa-bajo
        AW_lpf = 0.1*wc_lpf; //transición del filtro

        //Atenuación del filtro en dB
        Aten_lpf = 40;//-20*log10(aten_lpf);
        // Obtención de Beta y M
        if(Aten_lpf > 50)
        {
            Beta_lpf = 0.1102*(Aten_lpf-8.7);
        }
        else if(Aten_lpf >= 21 && Aten_lpf <= 50)
        {
            Beta_lpf = 0.5842*powf((Aten_lpf-21),0.4)+0.07886*(Aten_lpf-21);
        }
        else if(Aten_lpf < 21)
        {
            Beta_lpf = 0;
        }
        //orden del filtro
        //M=fix((A-8)/(2.285*2*AW));
        M_lpf = ceilf((Aten_lpf-8)/(2.285*AW_lpf));

        if(fmodf(M_lpf,2)!=0)//orden par, tipo I
        {
            M_lpf=M_lpf+1;
        }
        int total_coeff_lpf=M_lpf+1;


        //validación
        if(total_coeff_lpf>1000)
        {
            free(b_bpf);
            free(r_bpf);
            return 0;
        }

        //coeficientes del filtro pasa-bajo
        double *b_lpf = filter_design_direct_I(M_lpf, Beta_lpf, wc_lpf, 0);
    //     double *b_bpf = filter_design_direct_I(M_bpf, Beta_bpf, 2*M_PI*1000/fm, 4*M_PI*2000/fm);
        //registros del filtro pasa-bajo I y Q
        double *r_lpf_i=(double *)malloc(sizeof(double)*(total_coeff_lpf));


    //     printf("\nwc_lpf->%f, AW->%f, A->%f, total_coeff_lpf->%d\n",wc_lpf,AW_lpf,Aten_lpf,total_coeff_lpf);

        //inicialización de registros del filtro pasa-banda
        for(i=0;i<total_coeff_bpf;i++)
        {
            r_bpf[i]=0;
        }

        //inicialización de los registros del PLL
        vco_out=0;
        vco_fase=0;
        out_pll_cos=0;
        out_pll_sin=0;
        phase_mult_out=0;
        phase_mult_out_1=0;
        vco_in=0;
        vco_in_1=0;
        cumul_pll=0;

        //inicialización de registros del filtro pasa-bajo
        for(i=0;i<total_coeff_lpf;i++)
        {
            r_lpf_i[i]=0;
        }

        //demodulación
        for(i=0;i<N;i++)
        {
            //elevación al cuadrado y filtrado pasa-banda
            //inicialización de los registros del filtro pasa-banda
            r_bpf[0]=signal_in[i]*signal_in[i];

            //convolución
            for(j=0;j<total_coeff_bpf;j++)
            {
                cumul_bpf=cumul_bpf+r_bpf[j]*b_bpf[j];
            }

            //retardo
            for(j=total_coeff_bpf-1;j>0;j--)
            {
                r_bpf[j]=r_bpf[j-1];
            }

            //resultado del filtrado con ajuste de amplitud
            signal_in2=-2*cumul_bpf;
            cumul_bpf=0;

            //PLL
            if(i>M_bpf/2-1)//retardo del filtrado pasa-banda
            {

                vco_out=B*sin(2*M_PI*i*(fo)/fm+vco_fase);  // salida del VCO
                phase_mult_out=signal_in2*vco_out;  // Output Phase Detector (multiplicador)
                vco_in= b_1*phase_mult_out+b_2*phase_mult_out_1-r_2*vco_in_1; //Filtro de lazo y(n) = b0*x(n)+b1*x(n-1)-a1*y(n-1)
                vco_fase=(ko*(vco_in_1+vco_in)/fm)+cumul_pll; // fase del VCO (integrador)
                cumul_pll=vco_fase;
                out_pll_sin=B*sin(2*M_PI*i*(fo)/2/fm+vco_fase/2-M_PI/16); // salida del PLL en cuadratura

                //actualización de los registros
                phase_mult_out_1=phase_mult_out;
                vco_in_1=vco_in;

                //filtrado paso-bajo
                //inicialización de los registros del filtro pasa-bajo
                r_lpf_i[0]=signal_in[i-M_bpf/2]*out_pll_sin;


                //convolución
                for(j=0;j<total_coeff_lpf;j++)
                {
                    cumul_lpf_i=cumul_lpf_i+r_lpf_i[j]*b_lpf[j];
                }

                //retardo
                for(j=total_coeff_lpf-1;j>0;j--)
                {
                    r_lpf_i[j]=r_lpf_i[j-1];
                }

                //resultado del filtrado con ajuste de retardo
                if(i>M_bpf/2+M_lpf/2-1)
                {
                    signal_dem[i-M_bpf/2-M_lpf/2]=cumul_lpf_i;
                }

                cumul_lpf_i=0;

            }


        }


        //Liberación de espacio de los registros de ambos filtros
        free(b_bpf);
        free(r_bpf);
        free(r_lpf_i);
        free(b_lpf);

        return 1;
}

bool constelation_square_loop_BPSK (double* &signal_i_avg, double* &signal_q_avg, int* signal_I_length, double signal_in [], double f0, double baud_rate, double fm, int N)
{
    double *signal_i = new double[N];
    double *signal_q = new double[N];


    double w0=2*M_PI*f0/fm; //frecuencia digital de la señal

    //printf("w_0->%f, w1->%f",w0,w1);

    //variables de recorrido
    int i=0,j=0;

    //diseño de filtro pasa-banda
    //definición de variables
    double wc_bpf, AW_bpf, Aten_bpf, BW_bpf, Beta_bpf, signal_in2, cumul_bpf;
    int M_bpf;
    //configuración del filtro
    wc_bpf = 2*w0;//frecuencia centro del filtro pasa-banda
    AW_bpf = 0.1*wc_bpf; //transición del filtro
    BW_bpf = 2*M_PI*baud_rate/fm; //ancho de banda del filtro pasa-banda
    //Atenuación del filtro en dB
    Aten_bpf = 40;//-20*log10(aten_lpf);
    // Obtención de Beta y M
    if(Aten_bpf > 50)
    {
        Beta_bpf = 0.1102*(Aten_bpf-8.7);
    }
    else if(Aten_bpf >= 21 && Aten_bpf <= 50)
    {
        Beta_bpf = 0.5842*powf((Aten_bpf-21),0.4)+0.07886*(Aten_bpf-21);
    }
    else if(Aten_bpf < 21)
    {
        Beta_bpf = 0;
    }
    //orden del filtro
    //M=fix((A-8)/(2.285*2*AW));
    M_bpf = ceilf((Aten_bpf-8)/(2.285*AW_bpf));

    if(fmodf(M_bpf,2)!=0)//orden par, tipo I
    {
        M_bpf=M_bpf+1;
    }
    int total_coeff_bpf=M_bpf+1;

    //validación
    if(total_coeff_bpf>100)
        return 0;

    //coeficientes del filtro pasa-banda
    double *b_bpf = filter_design_direct_I(M_bpf, Beta_bpf, wc_bpf-BW_bpf, wc_bpf+BW_bpf);
//     double *b_bpf = filter_design_direct_I(M_bpf, Beta_bpf, 2*M_PI*1000/fm, 4*M_PI*2000/fm);
    //registros del filtro pasa-banda
    double *r_bpf=(double *)malloc(sizeof(double)*(total_coeff_bpf));

    //configuración del PLL
    double fo=2*f0;                // frecuencia libre del VCO en Hz
    double Chi=0.707;                // damping factor óptimo
    double B=1;                      // amplitud del tono de salida del VCO
    double kd=(2*B)/M_PI;              // Ganancia del Phase Detector step12 /109Best
    double ko=2*M_PI*fo;               // Ganancia del VCO
    double ka=10;                    // Ganancia del Filtro de lazo
    double Kv=kd*ko*ka;              // Ganancia del Lazo
    double lock_time=1/(baud_rate);  //configuración del tiempo de respuesta en segundos

    // Diseño del filtro de lazo cero-polo
    double wn=2*M_PI/lock_time; //frecuencia natural best2003
    double T2=(2*Chi/wn)-(1/Kv); //cte del filtro de lazo
    double T1=Kv/(wn*wn); //cte del filtro de lazo

    // conformación de filtro de lazo
    //numerador en el plano S
    double a1=ka*T2;
    double a2=ka;
    //denominador en el plano S
    double b1=T1;
    double b2=1;

    //transformación bilineal
    double b_1=(a1*2*fm+a2)/(b1*2*fm+b2);
    double b_2=(a2-a1*2*fm)/(b1*2*fm+b2);
    double r_2=(b2-b1*2*fm)/(b1*2*fm+b2);

//     printf("\n %f, %f, %f, %f, %f, %f, %f, %f\n", fo, Chi, B, kd, ko, ka, Kv, lock_time);
//     printf("%f, %f, %f, %f, %f, %f, %f", wn, T2, T1, a1, a2, b1, b2);
//        printf("%f, %f, %f", b_1, b_2, r_2);

    //variables del PLL
    double vco_out, vco_fase, out_pll_cos, out_pll_sin, phase_mult_out, phase_mult_out_1, vco_in, vco_in_1, cumul_pll;
//
    //diseño de filtro paso-bajo
    //definición de variables
    double wc_lpf, AW_lpf, Aten_lpf, BW_lpf, Beta_lpf, cumul_lpf_i, cumul_lpf_q;
    int M_lpf;
    //configuración del filtro
    wc_lpf = 2*M_PI*2*(2*baud_rate)/fm; //frecuencia centro del filtro pasa-bajo
    AW_lpf = 0.3*wc_lpf; //transición del filtro

    //Atenuación del filtro en dB
    Aten_lpf = 40;//-20*log10(aten_lpf);
    // Obtención de Beta y M
    if(Aten_lpf > 50)
    {
        Beta_lpf = 0.1102*(Aten_lpf-8.7);
    }
    else if(Aten_lpf >= 21 && Aten_lpf <= 50)
    {
        Beta_lpf = 0.5842*powf((Aten_lpf-21),0.4)+0.07886*(Aten_lpf-21);
    }
    else if(Aten_lpf < 21)
    {
        Beta_lpf = 0;
    }
    //orden del filtro
    //M=fix((A-8)/(2.285*2*AW));
    M_lpf = ceilf((Aten_lpf-8)/(2.285*AW_lpf));

    if(fmodf(M_lpf,2)!=0)//orden par, tipo I
    {
        M_lpf=M_lpf+1;
    }
    int total_coeff_lpf=M_lpf+1;


    //validación
    if(total_coeff_lpf>500)
    {
        delete []signal_i;
        delete []signal_q;
        free(b_bpf);
        free(r_bpf);
        return 0;
    }

    //coeficientes del filtro pasa-bajo
    double *b_lpf = filter_design_direct_I(M_lpf, Beta_lpf, wc_lpf, 0);
//     double *b_bpf = filter_design_direct_I(M_bpf, Beta_bpf, 2*M_PI*1000/fm, 4*M_PI*2000/fm);
    //registros del filtro pasa-bajo I y Q
    double *r_lpf_i=(double *)malloc(sizeof(double)*(total_coeff_lpf));
    double *r_lpf_q=(double *)malloc(sizeof(double)*(total_coeff_lpf));

//     printf("\nwc_lpf->%f, AW->%f, A->%f, total_coeff_lpf->%d\n",wc_lpf,AW_lpf,Aten_lpf,total_coeff_lpf);

    //inicialización de registros del filtro pasa-banda
    for(i=0;i<total_coeff_bpf;i++)
    {
        r_bpf[i]=0;

    }

    //inicialización de los registros del PLL
    vco_out=0;
    vco_fase=0;
    out_pll_cos=0;
    out_pll_sin=0;
    phase_mult_out=0;
    phase_mult_out_1=0;
    vco_in=0;
    vco_in_1=0;
    cumul_pll=0;

    //inicialización de registros del filtro pasa-bajo
    for(i=0;i<total_coeff_lpf;i++)
    {
        r_lpf_i[i]=0;
        r_lpf_q[i]=0;
    }


    //extracción de constelación
    for(i=0;i<N;i++)
    {
        //elevación al cuadrado y filtrado pasa-banda
        //inicialización de los registros del filtro pasa-banda
        r_bpf[0]=signal_in[i]*signal_in[i];

        //convolución
        for(j=0;j<total_coeff_bpf;j++)
        {
            cumul_bpf=cumul_bpf+r_bpf[j]*b_bpf[j];
        }

        //retardo
        for(j=total_coeff_bpf-1;j>0;j--)
        {
            r_bpf[j]=r_bpf[j-1];
        }

        //resultado del filtrado con ajuste de amplitud
        signal_in2=-2*cumul_bpf;
        cumul_bpf=0;

        //PLL
        if(i>M_bpf/2-1)//retardo del filtrado pasa-banda
        {
            vco_out=B*cos(2*M_PI*i*(fo)/fm+vco_fase);  // salida del VCO
            out_pll_cos=B*cos(2*M_PI*i*(fo)/2/fm+vco_fase/2-M_PI/16);  // salida del PLL en fase
            out_pll_sin=B*sin(2*M_PI*i*(fo)/2/fm+vco_fase/2-M_PI/16); // salida del PLL en cuadratura
            phase_mult_out=signal_in2*vco_out;  // Output Phase Detector (multiplicador)
            vco_in= b_1*phase_mult_out+b_2*phase_mult_out_1-r_2*vco_in_1; //Filtro de lazo y(n) = b0*x(n)+b1*x(n-1)-a1*y(n-1)
            vco_fase=(ko*(vco_in_1+vco_in)/fm)+cumul_pll; // fase del VCO (integrador)
            cumul_pll=vco_fase;
            //actualización de los registros
            phase_mult_out_1=phase_mult_out;
            vco_in_1=vco_in;

            //filtrado paso-bajo
            //inicialización de los registros del filtro pasa-bajo
            r_lpf_i[0]=signal_in[i-M_bpf/2]*out_pll_cos;
            r_lpf_q[0]=signal_in[i-M_bpf/2]*out_pll_sin;

            //convolución
            for(j=0;j<total_coeff_lpf;j++)
            {
                cumul_lpf_i=cumul_lpf_i+r_lpf_i[j]*b_lpf[j];
                cumul_lpf_q=cumul_lpf_q+r_lpf_q[j]*b_lpf[j];
            }

            //retardo
            for(j=total_coeff_lpf-1;j>0;j--)
            {
                r_lpf_i[j]=r_lpf_i[j-1];
                r_lpf_q[j]=r_lpf_q[j-1];
            }

            //resultado del filtrado con ajuste de retardo
            if(i>M_bpf/2+M_lpf/2-1)
            {
                signal_i[i-M_bpf/2-M_lpf/2]=cumul_lpf_i;
                signal_q[i-M_bpf/2-M_lpf/2]=cumul_lpf_q;
            }

            cumul_lpf_i=0;
            cumul_lpf_q=0;

        }

//         signal_q[i]=2*signal_in[i]*signal_in[i]-1;
    }

    //determinación del ángulo de la constelación
    double Ts_m=fm/baud_rate;
    double x0=0;
    double x1=0;
    double cumul_i=0;
    double cumul_q=0;

    //validación
    if(Ts_m<10)
    {
        free(b_bpf);
        free(r_bpf);
        free(r_lpf_i);
        free(r_lpf_q);
        free(b_lpf);
        delete []signal_i;
        delete []signal_q;
        return 0;
    }

    signal_i_avg=new double[int(floorf(N/Ts_m)-1)];
    signal_q_avg=new double[int(floorf(N/Ts_m)-1)];
    *signal_I_length=int(floorf(N/Ts_m)-1);


    for(i=0;i<int(floorf(N/Ts_m)-1);i++)
    {
        x0=floorf(i*Ts_m+Ts_m/4);
        x1=floorf(i*Ts_m+3*Ts_m/4);
        cumul_i=0;
        cumul_q=0;
        for(j=x0;j<=x1;j++)
        {
            cumul_i=cumul_i+signal_i[j];
            cumul_q=cumul_q+signal_q[j];
        }

        signal_i_avg[i]=cumul_i;
        signal_q_avg[i]=cumul_q;

    }


    //Liberación de espacio de los registros de ambos filtros
    free(b_bpf);
    free(r_bpf);
    free(r_lpf_i);
    free(r_lpf_q);
    free(b_lpf);
    delete []signal_i;
    delete []signal_q;

    return 1;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief constelation_square_loop_QPSK
/// \param signal_i_avg
/// \param signal_q_avg
/// \param signal_in
/// \param f0
/// \param baud_rate
/// \param fm
/// \param N
///
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool constelation_square_loop_QPSK (double* &signal_i_avg, double* &signal_q_avg,int* signal_I_length,double signal_in [], double f0, double baud_rate, double fm, int N)
{
    double *signal_i = new double[N];
    double *signal_q = new double[N];
    //double *angle = new double[N];

    double w0=2*M_PI*f0/fm; //frecuencia digital de la se�al

    //variables de recorrido
    int i=0,j=0;

    //dise�o de filtro pasa-banda
    //definici�n de variables
    double wc_bpf, AW_bpf, Aten_bpf, BW_bpf, Beta_bpf, signal_in2, cumul_bpf;
    int M_bpf;
    //configuraci�n del filtro
    wc_bpf = 4*w0;//frecuencia centro del filtro pasa-banda
    AW_bpf = 0.1*wc_bpf; //transici�n del filtro
    BW_bpf = 2*M_PI*baud_rate/fm; //ancho de banda del filtro pasa-banda
    //Atenuaci�n del filtro en dB
    Aten_bpf = 40;//-20*log10(aten_lpf);
    // Obtenci�n de Beta y M
    if(Aten_bpf > 50)
    {
        Beta_bpf = 0.1102*(Aten_bpf-8.7);
    }
    else if(Aten_bpf >= 21 && Aten_bpf <= 50)
    {
        Beta_bpf = 0.5842*powf((Aten_bpf-21),0.4)+0.07886*(Aten_bpf-21);
    }
    else if(Aten_bpf < 21)
    {
        Beta_bpf = 0;
    }
    //orden del filtro
    //M=fix((A-8)/(2.285*2*AW));
    M_bpf = ceilf((Aten_bpf-8)/(2.285*AW_bpf));

    if(fmodf(M_bpf,2)!=0)//orden par, tipo I
    {
        M_bpf=M_bpf+1;
    }
    int total_coeff_bpf=M_bpf+1;

    //validación
    if(total_coeff_bpf>500)
    {
        return 0;
    }

//     printf("\nwc_bpf->%f, AW->%f, A->%f, BW->%f, M_bpf->%d\n",wc_bpf,AW_bpf,Aten_bpf,BW_bpf,M_bpf);

    //coeficientes del filtro pasa-banda
    double *b_bpf = filter_design_direct_I(M_bpf, Beta_bpf, wc_bpf-BW_bpf, wc_bpf+BW_bpf);
//     double *b_bpf = filter_design_direct_I(M_bpf, Beta_bpf, 2*M_PI*1000/fm, 4*M_PI*2000/fm);
    //registros del filtro pasa-banda
    double *r_bpf=(double *)malloc(sizeof(double)*(total_coeff_bpf));

    //configuraci�n del PLL
    double fo=4*f0;                // frecuencia libre del VCO en Hz
    double Chi=0.707;                // damping factor �ptimo
    double B=1;                      // amplitud del tono de salida del VCO
    double kd=(2*B)/M_PI;              // Ganancia del Phase Detector step12 /109Best
    double ko=2*M_PI*fo;               // Ganancia del VCO
    double ka=10;                    // Ganancia del Filtro de lazo
    double Kv=kd*ko*ka;              // Ganancia del Lazo
    double lock_time=1/(baud_rate);  //configuraci�n del tiempo de respuesta en segundos

    // Dise�o del filtro de lazo cero-polo
    double wn=2*M_PI/lock_time; //frecuencia natural best2003
    double T2=(2*Chi/wn)-(1/Kv); //cte del filtro de lazo
    double T1=Kv/(wn*wn); //cte del filtro de lazo

    // conformaci�n de filtro de lazo
    //numerador en el plano S
    double a1=ka*T2;
    double a2=ka;
    //denominador en el plano S
    double b1=T1;
    double b2=1;

    //transformaci�n bilineal
    double b_1=(a1*2*fm+a2)/(b1*2*fm+b2);
    double b_2=(a2-a1*2*fm)/(b1*2*fm+b2);
    double r_2=(b2-b1*2*fm)/(b1*2*fm+b2);

//     printf("\n %f, %f, %f, %f, %f, %f, %f, %f\n", fo, Chi, B, kd, ko, ka, Kv, lock_time);
//     printf("%f, %f, %f, %f, %f, %f, %f", wn, T2, T1, a1, a2, b1, b2);
//        printf("%f, %f, %f", b_1, b_2, r_2);

    //variables del PLL
    double vco_out, vco_fase, out_pll_cos, out_pll_sin, phase_mult_out, phase_mult_out_1, vco_in, vco_in_1, cumul_pll;
//
    //dise�o de filtro paso-bajo
    //definici�n de variables
    double wc_lpf, AW_lpf, Aten_lpf, BW_lpf, Beta_lpf, cumul_lpf_i, cumul_lpf_q;
    int M_lpf;
    //configuraci�n del filtro
    wc_lpf = 2*M_PI*4*2*baud_rate/fm; //frecuencia centro del filtro pasa-bajo
    AW_lpf = 0.3*wc_lpf; //transici�n del filtro

    //Atenuaci�n del filtro en dB
    Aten_lpf = 40;//-20*log10(aten_lpf);
    // Obtenci�n de Beta y M
    if(Aten_lpf > 50)
    {
        Beta_lpf = 0.1102*(Aten_lpf-8.7);
    }
    else if(Aten_lpf >= 21 && Aten_lpf <= 50)
    {
        Beta_lpf = 0.5842*powf((Aten_lpf-21),0.4)+0.07886*(Aten_lpf-21);
    }
    else if(Aten_lpf < 21)
    {
        Beta_lpf = 0;
    }
    //orden del filtro
    //M=fix((A-8)/(2.285*2*AW));
    M_lpf = ceilf((Aten_lpf-8)/(2.285*AW_lpf));


    if(fmodf(M_lpf,2)!=0)//orden par, tipo I
    {
        M_lpf=M_lpf+1;
    }
    int total_coeff_lpf=M_lpf+1;

    //validación
    if(total_coeff_lpf>500)
    {
        delete []signal_i;
        delete []signal_q;
        free(b_bpf);
        free(r_bpf);
        return 0;
    }

    //coeficientes del filtro pasa-bajo
    double *b_lpf = filter_design_direct_I(M_lpf, Beta_lpf, wc_lpf, 0);
//     double *b_bpf = filter_design_direct_I(M_bpf, Beta_bpf, 2*M_PI*1000/fm, 4*M_PI*2000/fm);
    //registros del filtro pasa-bajo I y Q
    double *r_lpf_i=(double *)malloc(sizeof(double)*(total_coeff_lpf));
    double *r_lpf_q=(double *)malloc(sizeof(double)*(total_coeff_lpf));

    //printf("\nwc_lpf->%f, AW->%f, A->%f, total_coeff_lpf->%d\n",wc_lpf,AW_lpf,Aten_lpf,total_coeff_lpf);

    //inicializaci�n de registros del filtro pasa-banda
    for(i=0;i<total_coeff_bpf;i++)
    {
        r_bpf[i]=0;

    }

    //inicializaci�n de los registros del PLL
    vco_out=0;
    vco_fase=0;
    out_pll_cos=0;
    out_pll_sin=0;
    phase_mult_out=0;
    phase_mult_out_1=0;
    vco_in=0;
    vco_in_1=0;
    cumul_pll=0;

    //inicializaci�n de registros del filtro pasa-bajo
    for(i=0;i<total_coeff_lpf;i++)
    {
        r_lpf_i[i]=0;
        r_lpf_q[i]=0;
    }

    //extracci�n de constelaci�n
    for(i=0;i<N;i++)
    {
        //elevaci�n a la cuarta y filtrado pasa-banda
        //inicializaci�n de los registros del filtro pasa-banda
        r_bpf[0]=signal_in[i]*signal_in[i]*signal_in[i]*signal_in[i];

        //convoluci�n
        for(j=0;j<total_coeff_bpf;j++)
        {
            cumul_bpf=cumul_bpf+r_bpf[j]*b_bpf[j];
        }

        //retardo
        for(j=total_coeff_bpf-1;j>0;j--)
        {
            r_bpf[j]=r_bpf[j-1];
        }

        //resultado del filtrado con ajuste de amplitud
        signal_in2=-8*cumul_bpf;
        cumul_bpf=0;

        //PLL
        if(i>M_bpf/2-1)//retardo del filtrado pasa-banda
        {

            vco_out=B*cos(2*M_PI*i*(fo)/fm+vco_fase);  // salida del VCO
            out_pll_cos=B*cos(2*M_PI*i*(fo)/4/fm+vco_fase/4+M_PI/2-M_PI/16);  // salida del PLL en fase
            out_pll_sin=B*sin(2*M_PI*i*(fo)/4/fm+vco_fase/4+M_PI/2-M_PI/16); // salida del PLL en cuadratura
            phase_mult_out=signal_in2*vco_out;  // Output Phase Detector (multiplicador)
            vco_in= b_1*phase_mult_out+b_2*phase_mult_out_1-r_2*vco_in_1; //Filtro de lazo y(n) = b0*x(n)+b1*x(n-1)-a1*y(n-1)
            vco_fase=(ko*(vco_in_1+vco_in)/fm)+cumul_pll; // fase del VCO (integrador)
            cumul_pll=vco_fase;
            //actualizaci�n de los registros
            phase_mult_out_1=phase_mult_out;
            vco_in_1=vco_in;

            //filtrado paso-bajo
            //inicializaci�n de los registros del filtro pasa-bajo
            r_lpf_i[0]=signal_in[i-M_bpf/2]*out_pll_cos;
            r_lpf_q[0]=signal_in[i-M_bpf/2]*out_pll_sin;

            //convoluci�n
            for(j=0;j<total_coeff_lpf;j++)
            {
                cumul_lpf_i=cumul_lpf_i+r_lpf_i[j]*b_lpf[j];
                cumul_lpf_q=cumul_lpf_q+r_lpf_q[j]*b_lpf[j];
            }

            //retardo
            for(j=total_coeff_lpf-1;j>0;j--)
            {
                r_lpf_i[j]=r_lpf_i[j-1];
                r_lpf_q[j]=r_lpf_q[j-1];
            }

            //resultado del filtrado con ajuste de retardo
            if(i>M_bpf/2+M_lpf/2-1)
            {
                signal_i[i-M_bpf/2-M_lpf/2]=cumul_lpf_i;
                signal_q[i-M_bpf/2-M_lpf/2]=cumul_lpf_q;
            }

            cumul_lpf_i=0;
            cumul_lpf_q=0;

        }

//         signal_q[i]=2*signal_in[i]*signal_in[i]-1;
    }

    //determinaci�n del �ngulo de la constelaci�n
    double Ts_m=fm/baud_rate;
    double x0=0;
    double x1=0;
    double cumul_i=0;
    double cumul_q=0;

    //validación
    if(Ts_m<10)
    {
        free(b_bpf);
        free(r_bpf);
        free(r_lpf_i);
        free(r_lpf_q);
        free(b_lpf);
        delete []signal_i;
        delete []signal_q;
        return 0;
    }

    signal_i_avg=new double[int(floorf(N/Ts_m)-1)];
    signal_q_avg=new double[int(floorf(N/Ts_m)-1)];
    *signal_I_length=int(floorf(N/Ts_m)-1);

    for(i=0;i<floorf(double(N)/Ts_m)-1;i++)
    {
        x0=floorf(i*Ts_m+Ts_m/4);
        x1=floorf(i*Ts_m+3*Ts_m/4);
        cumul_i=0;
        cumul_q=0;
        for(j=x0;j<=x1;j++)
        {
            cumul_i=cumul_i+signal_i[j];
            cumul_q=cumul_q+signal_q[j];
        }

        signal_i_avg[i]=cumul_i/(x1-x0+1);
        signal_q_avg[i]=cumul_q/(x1-x0+1);

//        double angle_I=atan(abs(cumul_i/cumul_q));

//        if(cumul_i>0 && cumul_q>0)
//        {
//            angle[i]=angle_I;
//        }

//        if(cumul_i<0 && cumul_q>0)
//        {
//            angle[i]=M_PI-angle_I;
//        }

//        if(cumul_i<0 && cumul_q<0)
//        {
//            angle[i]=-M_PI+angle_I;
//        }

//        if(cumul_i>0 && cumul_q<0)
//        {
//            angle[i]=-angle_I;
//        }
    }

    //Liberaci�n de espacio de los registros de ambos filtros
    free(b_bpf);
    free(r_bpf);
    free(r_lpf_i);
    free(r_lpf_q);
    free(b_lpf);

    return 1;
}

void dem_quadratic_BPSK(double* y, double *x, double f0, double baud_rate, double fm, int filtro, int length)
{
    //filtro: 1-Notch-Notch, 2 Kaiser-Kaiser
    // Señales
    double *signal_2;
    double *signal_3;
    double *demodulator;
    // Datos para el PLL
    double vco_out=0;
    double fo=2*f0;
    double ko=2*M_PI*fo;
    double vco_fase=0;
    double phase_mult_out[2]={0,0};
    double vco_in[2]={0,0};
    double cumul=0;
    double freq_div_out=0;


    // Selección del método de filtrado a utilizar
    if (filtro == 1)
    {
        // definición de arreglos para los filtros
        double coef_num_notch_bpf[3]={0,0,0};
        double coef_den_notch_bpf[3]={1,1,1};
        double a[2]={1,1};
        double b[2]={1,1};
        double coef_num_notch_lpf[3]={1,1,1};
        double coef_den_notch_lpf[3]={1,1,1};

        // notch pasabanda
        double r_notch_bpf = 0.8;
        // filtro de Lazo_Cuadratico
        double chi=0.707;                    // damping factor óptimo
        double lock_time=1/(4*baud_rate);    // tiempo en el que se debe enganchar el Lazo_Cuadratico
        // notch pasabajo
        double r_notch_lpf = baud_rate/fm;

        // diseño de todos los coeficientes de los filtros

        filter_design_coef_num_notch_BPF(f0, coef_num_notch_bpf, r_notch_bpf, fm);
        filter_design_coef_den_notch_BPF(f0, coef_den_notch_bpf, r_notch_bpf, fm);
        filter_design_coef_lazo(f0, a, b, chi, lock_time, fm);

        filter_design_coef_num_notch_LPF(f0, coef_num_notch_lpf, r_notch_lpf, fm);
        filter_design_coef_den_notch_LPF(f0, coef_den_notch_lpf, r_notch_lpf, fm);

        signal_2=(double *)malloc(sizeof(double)*3);
        signal_2[2] = 0;
        signal_2[1] = 0;
        signal_2[0] = 0;
        signal_3=(double *)malloc(sizeof(double)*3);
        signal_3[2] = 0;
        signal_3[1] = 0;
        signal_3[0] = 0;
        demodulator=(double *)malloc(sizeof(double)*3);
        demodulator[2] = 0;
        demodulator[1] = 0;
        demodulator[0] = 0;

        for (int i = 0; i < length; i++)
        {
            // square
            signal_2[2] = signal_2[1];
            signal_2[1] = signal_2[0];
            signal_2[0] = x[i]*x[i];

            // eliminación de cd
            // filtro pasa banda

            signal_3[2]=signal_3[1];
            signal_3[1]=signal_3[0];
            signal_3[0]=coef_num_notch_bpf[0]*signal_2[0]+coef_num_notch_bpf[1]*signal_2[1]+coef_num_notch_bpf[2]*signal_2[2]-coef_den_notch_bpf[1]*signal_3[1]-coef_den_notch_bpf[2]*signal_3[2];

            // filtro de Lazo_Cuadratico
            phase_mult_out[1]=phase_mult_out[0];
            phase_mult_out[0]=vco_out*signal_3[0];
            vco_in[1]=vco_in[0];
            vco_in[0]= b[0]*phase_mult_out[0]+b[1]*phase_mult_out[1]-a[1]*vco_in[1];

            // vco
            vco_fase=(ko*(vco_in[1]+vco_in[0])/fm)+cumul;       // fase del vco (integrador)
            cumul=vco_fase;
            vco_out=sin(2*M_PI*i*(fo)/fm+vco_fase);

            // divisor
            freq_div_out=sin((2*M_PI*i*(fo)/fm+vco_fase)/2);      // output del divisor de frecuencia

            // demodulador
            demodulator[2]=demodulator[1];
            demodulator[1]=demodulator[0];
            demodulator[0]= x[i]*freq_div_out;          // multiplicador del vco_out*buffer

            // filtro notch pasabajo

            y[2]=y[1];
            y[1]=y[0];
            y[0]=coef_num_notch_lpf[0]*demodulator[0]+coef_num_notch_lpf[1]*demodulator[1]+coef_num_notch_lpf[2]*demodulator[2]-coef_den_notch_lpf[1]*y[1]-coef_den_notch_lpf[2]*y[2];

            // almacenando resultados en el arreglo de salida
            y[i]=y[0];
        }
        free(demodulator);
        free(signal_3);
        free(signal_2);
    }
    else if (filtro == 2)
    {
        // Diseño del filtro pasa-banda Kaiser
        double wc1=2*(2*f0*M_PI)/fm;  //frecuencia centro del pasa-banda en relación a pi.
        double A=-20*log10(0.001);  //13.98;
        double Beta;
        int M1, L1;

        if (A<21){
            Beta=0;
        }
        else if (A>=21 && A<=50){
            Beta=0.5842*powf((A-21),0.4)+0.07886*(A-21);
        }
        else if (A>=50){
            Beta=0.1102*(A-8.7);
        }

        double AW1=2*M_PI*baud_rate/fm;
        M1 = ceilf((A-8)/(2.285*0.1*wc1));
        M1=M1+1;  // esto es L1
        if(fmodf(M1,2)!=0){
            M1=M1+1;
        }

        double wc1_bpf = wc1+(AW1/(2));
        double wc2_bpf = wc1-(AW1/(2));

        double *hn_bpf = filter_design_direct_I(M1, Beta, wc1_bpf, wc2_bpf);   // Coeficientes del pasabanda


        demodulator = (double *)malloc(sizeof(double)*length);
        //for(i=0;i<length;i++){
        //	demodulator[i]=0;
        //}
        signal_3 = (double *)malloc(sizeof(double)*length);
        signal_2 = (double *)malloc(sizeof(double)*length);
        double demodulator_preview[3] = {0,0,0};

        // Señal de entrada al cuadrado
        for (int i=0; i<length; i++)
        {
            signal_2[i]=powf(x[i],2);
            //p++;
        }

        // Filtrado pasa-banda Kaiser
        signal_3 = filter(signal_2, hn_bpf, length, M1);
        free(signal_2);


        // Diseño del filtro de Lazo_Cuadratico
        double a[2]={1,1};
        double b[2]={1,1};
        double chi=0.707;                    // damping factor óptimo
        double lock_time=1/(4*baud_rate);    // tiempo en el que se debe enganchar el Lazo_Cuadratico

        filter_design_coef_lazo(f0, a, b, chi, lock_time, fm);

        //p = d;
        // Lazo_Cuadratico
        for (int i = 0; i < length; i++)
        {
            // Filtro de Lazo_Cuadratico
            phase_mult_out[1]=phase_mult_out[0];
            phase_mult_out[0]=vco_out*signal_3[i];
            vco_in[1]=vco_in[0];
            vco_in[0]= b[0]*phase_mult_out[0]+b[1]*phase_mult_out[1]-a[1]*vco_in[1];

            // VCO
            vco_fase=(ko*(vco_in[1]+vco_in[0])/fm)+cumul;       // fase del VCO (integrador)
            cumul=vco_fase;
            vco_out=sin(2*M_PI*i*(fo)/fm+vco_fase);

            // Divisor
            freq_div_out=sin((2*M_PI*i*(fo)/fm+vco_fase)/2);      // Output del divisor de frecuencia

            // Demodulador
            demodulator_preview[2]=demodulator_preview[1];
            demodulator_preview[1]=demodulator_preview[0];
            demodulator_preview[0]=x[i]*freq_div_out;          // multiplicador del vco_out*buffer
            //p++;
            demodulator[i]=demodulator_preview[0];
        }
        free(signal_3);

        // Diseño del filtro paso-bajo Kaiser
        A=-20*log10(0.001); //13.98;
        if (A<21){
            Beta=0;
        }
        else if (A>=21 && A<=50){
            Beta=0.5842*powf((A-21),0.4)+0.07886*(A-21);
        }
        else if(A>=50){
            Beta=0.1102*(A-8.7);
        }

        double wc=2*f0*M_PI/fm;
        AW1=0.1*wc;

        M1 = ceilf((A-8)/(2.285*AW1));
        M1=M1+1;  // esto es L1
        if(fmodf(M1,2)!=0)
        {
            M1=M1+1;
        }

        double *hn_lpf = filter_design_direct_I(M1, Beta, wc, 0);

        // Filtrado pasa-bajo Kaiser
        y = filter(demodulator, hn_lpf, length, M1);

        free(hn_lpf);
        free(demodulator);
    }
}

void filter_design_coef_num_notch_BPF(double frec_signal, double *coef_num_notch_bpf, double r, int fm)
{
    coef_num_notch_bpf[1]=(1-r*r)*cos(2*M_PI*(2*frec_signal/fm));
    coef_num_notch_bpf[2]=-1*(1-r*r);
}

void filter_design_coef_den_notch_BPF(double frec_signal, double *coef_den_notch_bpf, double r, int fm)
{
    coef_den_notch_bpf[1]=-1*(1+r*r)*cos(2*M_PI*(2*frec_signal/fm));
    coef_den_notch_bpf[2]=r*r;
}

// Diseño de los coeficientes del filtro de Lazo_Cuadratico
void filter_design_coef_lazo(double frec_signal, double *a, double *b, double chi, double lock_time, int fm)
{
    // Configuración del PLL

    double fo=2*frec_signal;                 // frecuencia libre del VCO
    //double chi=0.707;                      // damping factor óptimo
    double kd=2/M_PI;                          // Ganancia del Phase Detector
    double ko=2*M_PI*fo;						// Ganancia del VCO
    double ka=10;							// Ganancia del Filtro
    double Kv=kd*ko*ka;						// Ganancia del Lazo_Cuadratico
    //double lock_time=1/(4*baud_rate);      // tiempo en el que se debe enganchar el Lazo_Cuadratico

    // Diseño del filtro de Lazo_Cuadratico cero-polo
    double wn=2*M_PI/lock_time;
    double T2=(2*chi/wn)-(1/Kv);
    double T1=Kv/(wn*wn);

    // Creando filtro del Lazo_Cuadratico
    // Numerador en el plano S
    double b1= T1;
    double b2= 1;

    // Denominador en el plano S
    double a1= ka*T2;
    double a2=ka;

    // Transformación bilineal
    b[0]=(a1*2*fm+a2)/(b1*2*fm+b2);
    b[1]=(a2-a1*2*fm)/(b1*2*fm+b2);

    a[0]=1;
    a[1]=(b2-b1*2*fm)/(b1*2*fm+b2);
}

// Diseño de los coeficientes del filtro notch pasabajo
void filter_design_coef_num_notch_LPF(double frec_signal, double *coef_num_notch_lpf, double r1, int fm)
{
    coef_num_notch_lpf[0] = (1+r1*r1)/2;
    coef_num_notch_lpf[1] = (1+r1*r1)/2*(-2*cos(2*M_PI*(2*frec_signal/fm)));
    coef_num_notch_lpf[2] = (1+r1*r1)/2;
}

void filter_design_coef_den_notch_LPF(double frec_signal, double *coef_den_notch_lpf, double r1, int fm)
{
    coef_den_notch_lpf[1]=-1*(1+r1*r1)*cos(2*M_PI*(2*frec_signal/fm));
    coef_den_notch_lpf[2]=r1*r1;
}

//Nuevas funciones para la demodulación de señales QPSK
// filtro FIR de rama I
double firQ( int M, double *b, double *w,  double x )
{

    // calcula la salida de un filtro FIR por puntos

    double y=0;
    int j;
    w[0]=x;

    for (y=0, j=0; j<M  ; j++)
        y+=  b[j]* w[j];

    for (j=M-1; j>=1; j--)
        w[j]=w[j-1];

    return y;


}

// filtro FIR de rama I
double firI(int M,  double *b, double *w,  double x )
{

    // calcula la salida de un filtro FIR por puntos

    double y=0;
    int j;
    w[0]=x;


    for (y=0, j=0; j<M  ; j++)
        y+=  b[j]* w[j];

    for (j=M-1; j>=1; j--)
        w[j]=w[j-1];


    return y;


}

// funcion del vco correspondiente a un sumador con wave tabla de 2^N valores

void   vco( double *xcos,double *xsin, double fc1, double error,  double *W, double fm, int NN)
{
    // fc frecuencia central
    // error de la fase a sumar
    // W registros del sumador
    // variables por referencia para el coseno y el seno calculados

    double Ko, DCita, y;
    double b[2]= {1, 0};
    double a[2]= {1,  -1};



    Ko= fc1/(fm/NN);
    DCita= 2*M_PI* (Ko + error);

    W[0]=DCita  + W[1];
    y=W[0];  // valor de salida
    W[1]=W[0]; // actualizacion de registros


    *xcos=cos(y/NN);
    *xsin=sin(y/ NN);
    return;

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// FUNCIONES PARA EL SINCRONISMO DE SIMBOLO
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Esta funcion calcula el error de simbolo a partir las se�ales demoduladas en cuadratura seg�n
// el algoritmo de Gardner.
// Se activa con el reloj generado de medio simbolo

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

double  cal_dte2(  char  clock, double sigI , double sigQ,double *regI,  double *regQ ) {

    // sigI ,  sigQ son las se�ales en cuadratura recibidas


    double error;

    regI[0]= sigI; //cargando las se�ales
    regQ[0]= sigQ;

    // se calcula el error total debido a ambas se�ales

    error= regI[1] * ( sigI - regI[2]) + regQ[1] * ( sigQ -  regQ[2]);

    // se activa seg�n el reloj

    if (clock==1)	{
        regI[2]=regI[1];	   //actualizando registros
        regI[1]=regI[0];
        regQ[2]=regQ[1];	   //actualizando registros
        regQ[1]=regQ[0];  }


    else


        error=0;		// no se actualiza el registro ni se da erro








    return error;




}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// filtro de dte del tipo integrador y proporcional
// con constantes kp ki propias importantes
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

double  fil_loop(char clock , double  error, double *reg  )   {

    double dte_fil=0;
    double kp=0.005;
    double ki=0.001;

    reg[0]= error + reg[1];

    dte_fil= reg[0] * (kp+ki)- kp*reg[1];

    if (clock==1)

        reg[1]=reg[0];
    else

        dte_fil=0;


    return dte_fil;

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// funcion para el oscilador de sincronismo a partir del error filtrado tde fil
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
double synchro_nco( double dtefil ,double* reg,  double sam_symbol)
{
    double clock;

    // esta constante expresa la cantidad de muestras en un tiempo de simbolo
    // o la relacion en fm/ baud_rate= 44100/882=50
    // double  kgen= 1/50.0;
    double  kgen= 1/ floor(sam_symbol);		 //señal generalizada

    double xin;

    xin=dtefil + kgen;  // la entrada es la suma del error filtrado + constante del generador

    // se produce la acumuacion

    reg[0]= xin + reg[1];

    clock= reg[0];

    reg[1]=reg[0]; // se actualiza el registro del acumulador

    return clock; // se retorna el acumulador
}

double synchro_nco( double dtefil ,double* reg)
{
    double clock;

    // esta constante expresa la cantidad de muestras en un tiempo de simbolo
    // o la relacion en fm/ baud_rate= 44100/882=50
    double  kgen= 1/50.0;
    double xin;

    xin=dtefil + kgen;  // la entrada es la suma del error filtrado + constante del generador

    // se produce la acumuacion

    reg[0]= xin + reg[1];

    clock= reg[0];

    reg[1]=reg[0]; // se actualiza el registro del acumulador

    return clock; // se retorna el acumulador

}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//	  funcion para implementar el retardo de 1/2 simbolo en OQPSK

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

double 	  delay_tbit( double x, double *W)  {
    double y;
    int j;
    W[0]=x;
    y= W[24];

    for(j=24;j>0;j--)
        W[j]=W[j-1];
    return y;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// funci�n para demodular  8PSK
// dada cada una de las se�ales en cuadratura retorna el nivel de umbral
// se calcula con el modulo de la signal
// por lo que se obtienen dos niveles sim�tricos positivos y negativo.


double  calcula_umbral( double *signal,  double signal_length)
{

    double umbral;

    double max_sig_mod=0;

    double  bins=10;

    double  dx;

    int j;

    for (j=0; j<signal_length; j++)
    {

        if ( fabs( signal[j]) > max_sig_mod )

            max_sig_mod= fabs(signal[j]);
    }


    dx= max_sig_mod/bins;  // intervalo

    // definiendo  el arreglo de frecuencias

    int freq[10]= { 0};

    // calculo del histograma



    for (j=0; j<signal_length; j++)

        freq[ (int) ( (fabs(signal[j])- 0.001)/dx)]++;

    // ahora hay que hallar los dos primeros m�ximos de las freq

    int max1=0;int  max2=0;


    for (j=0; j< 10; j++) {


        if  ( freq[j] > freq[max1] ) {                                            	 ;

            max2=max1; // se almacena en el segundo

            max1= j; }

        else			 // si no est� en el primer maximo se pregunta por el segundo

        {  if ( freq[j] > freq[max2])

                max2= j;	 }

    }

    // ahora podemos calcular los niveles de umbral

    double range[10]={0};  // arreglo con los puntos medios de	cada bin


    for (j=0; j<10; j++)

        range[j]=	dx/2 + dx*(j);





    umbral= (range[max1]+ range[max2])/2;

    return umbral;

}
