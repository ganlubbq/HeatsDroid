#include "fft.h"

COMPLEX * FFT(COMPLEX X[], int cant_muest, int inverse)
{

/* -- Cálculo de las exponenciales complejas -- */

    int j, k;
    double  pi = 3.14159265358979323846,
            cita = (2*pi)/cant_muest;

    COMPLEX scale;						// para cambiar la escala por 1/N si se quiere la iFFT

    if(inverse == 1)
    {
        scale.real = 1.0;
        scale.imag = 0;
    } else
        {
            scale.real = 1.0/cant_muest;
            scale.imag = 0;
        }


    COMPLEX * Wn = (COMPLEX *)malloc(cant_muest*sizeof(COMPLEX));

    for(k = 0; k<(cant_muest); k++)
    {
        Wn[k].real = cos(cita*k);
        Wn[k].imag = -1*inverse*sin(cita*k);
    }

/* -- Núcleo del algoritmo -- */

    int NumOfProblems = 1,
        ProblemSize = cant_muest,
        HalfSize,
        JFirst,
        JLast,
        Jtwiddle;

    COMPLEX Wj,
            Temp;

    while (ProblemSize>1)
    {
        HalfSize = ProblemSize/2;

        for(k=0;k<NumOfProblems;k++){
            JFirst = k*ProblemSize;
            JLast = JFirst + HalfSize - 1;
            Jtwiddle=0;

            for(j=JFirst;j<=JLast;j++){
               Wj = Wn[Jtwiddle];
               Temp = X[j];
               X[j] = SUMAR(Temp, X[j+HalfSize]);
               X[j+HalfSize] = MULTIPLICAR(Wj, RESTAR(Temp, X[j+HalfSize]));
               Jtwiddle = Jtwiddle + NumOfProblems;
            }
        }

        NumOfProblems = 2*NumOfProblems;
        ProblemSize = HalfSize;
    }

    free(Wn);

    if(inverse == -1)
    {
        for(k = 0; k<cant_muest; k++)
        {
           X[k] = MULTIPLICAR(X[k], scale);
         }
    }

return X;

}

/* -- Declaración de las funciones -- */

COMPLEX MULTIPLICAR(COMPLEX A, COMPLEX B)
{
    COMPLEX m;

    m.real = (A.real)*(B.real) - (A.imag)*(B.imag);
    m.imag = (A.real)*(B.imag) + (A.imag)*(B.real);

    return m;
}

COMPLEX SUMAR(COMPLEX A, COMPLEX B)
{
    COMPLEX s;

    s.real = A.real + B.real;
    s.imag = A.imag + B.imag;

    return s;
}

COMPLEX RESTAR(COMPLEX A, COMPLEX B)
{
    COMPLEX r;

    r.real = A.real - B.real;
    r.imag = A.imag - B.imag;

    return r;
}

int potencia2(double v)
{
    while (v > 0)
        v = v - 1;

    if(v == 0)
        return 2;
    else
        return 0;
}

COMPLEX * bitRev(COMPLEX Data[], int cant_muest)
{
    int Position,
        Mask,
        Target = 0;

    COMPLEX Temp;

    for (Position = 0; Position < cant_muest; ++Position)
    {
        if (Target > Position)
        {
            Temp = Data[Target];
            Data[Target] = Data[Position];
            Data[Position] = Temp;
        }

        Mask = cant_muest;

        while (Target & (Mask >>= 1))

            Target &= ~Mask;
            Target |= Mask;
    }

    return Data;
}

//Espectrograma
double **spectrogram(double *signal_in, int signal_length, int window, int window_length, int solapamiento, int step, int spect_length, double* max)//solapamiento en % e.g.0,10,15, etc
{
   double **spect;
//   int step=double(solapamiento)/double(100)*double(window_length);
//   int spect_length=double(signal_length-window_length)/double(step);
   *max=0;

   //reserva de espacio
   spect= new double*[spect_length];
   int z=0;
   for(int i=0;i<spect_length;i++)
   {
       z=i*step;
       spect[i]=get_FFT(&signal_in[z],window_length,window);
       //se busca el máximo
       for(int j=0;j<window_length/2;j++)
           if(*max<spect[i][j])
               *max=spect[i][j];
   }

   return spect;
}

double* get_FFT(double *signal_in, int signal_fft_length, int window)
{
    double *signal_fft;

    COMPLEX * Xcmpx = (COMPLEX *)malloc(signal_fft_length*sizeof(COMPLEX));
    COMPLEX * Y;

    //se conforma la ventana
    double *window_wave=new double[signal_fft_length];
    if(window==0)
        boxcar(window_wave,signal_fft_length);
    if(window==1)
        triang(window_wave,signal_fft_length);
    if(window==2)
        hanning(window_wave,signal_fft_length);
    if(window==3)
        hamming(window_wave,signal_fft_length);
    if(window==4)
        blackman(window_wave,signal_fft_length);
    if(window==5)
        flattop(window_wave,signal_fft_length);
    if(window==6)
        kaiser(window_wave,signal_fft_length,0.5);


    for (int k=0; k<signal_fft_length; k++)  				// inicializo X como COMPLEX de parte imaginaria cero.
    {
        Xcmpx[k].real=signal_in[k]*window_wave[k];
        Xcmpx[k].imag=0;
    }


    if(signal_fft_length != 1)
    {
        Y = FFT(Xcmpx, signal_fft_length, 1);  	// se llama a la función principal.
        Xcmpx = bitRev(Y, signal_fft_length);

        //se copia el espectro unilateral
        signal_fft_length=signal_fft_length/2;
        signal_fft = new double[signal_fft_length];
        //se determina la norma y el máximo
        for(int i=0; i<signal_fft_length; i++)
        {
            signal_fft[i]=qSqrt(Xcmpx[i].real*Xcmpx[i].real+Xcmpx[i].imag*Xcmpx[i].imag);

        }


        free(Xcmpx);		// se libera la memoria ocupada.


        }
        else
        {
            QWidget *widget = new QWidget;
            QMessageBox::information(widget, "Error", "La FFT no es potencia de 2");

        }

    delete []window_wave;

    return signal_fft;
}


double spect_window[MAX];

void boxcar(double *spect_window, int n)
{
  for (int i = 0 ; i < n ; i ++)
    spect_window[i] = 1.0;
}
void triang(double *spect_window, int n)
{
  double k1  = (double)(n & 1);
  double k2  = 1/((double)n + k1);
  int      end = (n + 1) >> 1;
  // Calculate window coefficients
  for( int i = 0 ; i < end ; i ++)
    spect_window[i] = spect_window[n-i-1] = (2.0*((double)(i+1))-(1.0-k1))*k2;
}
void hanning(double *spect_window, int n)
{
  _ftype_t k = 2*M_PI/((_ftype_t)(n+1)); // 2*pi/(N+1)

  // Calculate window coefficients
  for ( int i = 0 ; i < n ; i ++ )
    spect_window[i] = 0.5*(1.0 - cos(k*(_ftype_t)(i+1)));
}
void hamming(double *spect_window, int n )
{
  _ftype_t k = 2*M_PI/((_ftype_t)(n-1)); // 2*pi/(N-1)

  // Calculate window coefficients
  for ( int i = 0 ; i < n ; i ++ )
    spect_window[i] = 0.54 - 0.46*cos(k*(_ftype_t)i);
}
void blackman(double *spect_window, int n )
{
  _ftype_t k1 = 2*M_PI/((_ftype_t)(n-1)); // 2*pi/(N-1)
  _ftype_t k2 = 2*k1; // 4*pi/(N-1)

  // Calculate window coefficients
  for (int i = 0 ; i < n ; i ++ )
    spect_window[i] = 0.42 - 0.50*cos(k1*(_ftype_t)i) + 0.08*cos(k2*(_ftype_t)i);
}
void flattop(double *spect_window, int n )
{
  _ftype_t k1 = 2*M_PI/((_ftype_t)(n-1)); // 2*pi/(N-1)
  _ftype_t k2 = 2*k1;                   // 4*pi/(N-1)

  // Calculate window coefficients
  for ( int i = 0 ; i < n ; i ++ )
    spect_window[i] = 0.2810638602 - 0.5208971735*cos(k1*(_ftype_t)i) + 0.1980389663*cos(k2*(_ftype_t)i);
}
_ftype_t besselizero(_ftype_t x)
{
  _ftype_t temp;
  _ftype_t sum   = 1.0;
  _ftype_t u     = 1.0;
  _ftype_t halfx = x/2.0;
  int      n     = 1;

  do {
    temp = halfx/(_ftype_t)n;
    u *=temp * temp;
    sum += u;
    n++;
  } while (u >= BIZ_EPSILON * sum);
  return(sum);
}
void kaiser(double *spect_window, int n , _ftype_t b )
{
  _ftype_t tmp;
  _ftype_t k1  = 1.0/besselizero(b);
  int	   k2  = 1 - (n & 1);
  int      end = (n + 1) >> 1;

  // Calculate window coefficients
  for ( int i = 0 ; i < end ; i ++ )
  {
    tmp = (_ftype_t)(2*i + k2) / ((_ftype_t)n - 1.0);
    spect_window[end-(1&(!k2))+i] = spect_window[end-1-i] = k1 * besselizero(b*sqrt(1.0 - tmp*tmp));
  }

}

vector <COMPLEX> GET_FFT( vector <double> signal_in )
{
    vector <COMPLEX> fft;
    double   v;  	// secuencia que se recibe como entrada.
    int      k, pot, cant_muest, inverse = 1;	// inverse = 1 indica que se calcula la FFT,
    // inverse = -1 es que se calcula la iFFT.

    cant_muest = signal_in.size();

    v = logl(cant_muest)/logl(2);pot = potencia2(v);
    COMPLEX * Xcmpx,* Y;

    if(pot == 2)
    {
        Xcmpx = (COMPLEX *)malloc(cant_muest*sizeof(COMPLEX));
        Y = (COMPLEX *)malloc(cant_muest*sizeof(COMPLEX));

        for (k=0; k<cant_muest; k++)  				// inicializo X como COMPLEX de parte imaginaria cero.
        {
            Xcmpx[k].real=signal_in[k];
            Xcmpx[k].imag=0;
        }
        Y = FFT(Xcmpx, cant_muest, inverse);  	// se llama a la funci�n principal.
        Y = bitRev(Y, cant_muest);

        for( int i = 0  ;i < cant_muest ; i ++ )
            fft.push_back(COMPLEX(Y[i].real , Y[i].imag));
    }
    else
    {
        //no es pot de 2
        int intV = (int) (v + 1),
                N1 = pow(2.0,intV);

        Xcmpx = (COMPLEX *)malloc(N1*sizeof(COMPLEX));
        Y = (COMPLEX *)malloc(N1*sizeof(COMPLEX));

        for (k=0; k<cant_muest; k++)
        {
            Xcmpx[k].real=signal_in[k];
            Xcmpx[k].imag=0;
        }

        for (k=cant_muest; k<N1; k++)
        {
            Xcmpx[k].real=0;
            Xcmpx[k].imag=0;
        }

        Y = FFT(Xcmpx, N1, inverse);
        Y = bitRev(Y, N1);

        for( int i = 0  ;i < N1 ; i ++ )
            fft.push_back(COMPLEX(Y[i].real , Y[i].imag));
    }

    free(Xcmpx);
    free(Y);

    return fft;

}
vector <double> ABS( vector <COMPLEX> signal_in )
{
    vector <double> tmp;

    for( vector <COMPLEX> :: iterator i = signal_in.begin() ; i < signal_in.end() ; i ++ )
        tmp.push_back(i->abs());

    return tmp;
}

