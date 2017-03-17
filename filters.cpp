#include <QtWidgets>//para abrir ventanas de diálogo, //define malloc
#include "filters.h"

void filter_IIR_FIR(double* &out, const double *in, const int length, const double* b, const double* a, const int b_length, const int a_length)
{
    out=new double[length];

    double *in_reg, *out_reg;
    in_reg = new double[b_length];

    if(a_length!=0)
        out_reg = new double[a_length-1];

    //se reinician los registros
    for(int i=0;i<b_length;i++)
        in_reg[i]=0;

    if(a_length!=0)
        for(int i=0;i<a_length-1;i++)
            out_reg[i]=0;

    //filtrado
    in_reg[0]=in[0];

    for(int i=0;i<length;i++)
    {
        //inicialización
        out[i]=0;
        in_reg[0]=in[i];
        //filtrado por los ceros
        for(int j=0;j<b_length;j++)
            out[i]+=in_reg[j]*b[j];

        //filtrado por los polos
        if(a_length!=0)
            for(int j=0;j<a_length-1;j++)
                out[i]-=out_reg[j]*a[j+1]/a[0];

        //retardos
        for(int j=b_length-1;j>0;j--)
            in_reg[j]=in_reg[j-1];

        //retardos
        if(a_length!=0)
        {
            for(int j=a_length-1;j>0;j--)
                out_reg[j]=out_reg[j-1];
            out_reg[0]=out[i];
        }


    }

    delete[] in_reg;
    if(a_length!=0)
        delete[] out_reg;
}

// Función para diseñar el filtro FIR por la forma directa I
// Comentarear si ya esta
double* filter_design_direct_I(int M, double Beta, double wc1, double wc2)
{
    int i;

    // h ideal
    double *h = (double *)malloc(sizeof(double)*(M+1));
    for (i=0; i<=M; i++){
        h[i]=(wc1/M_PI)*sinc(wc1/M_PI*(i-(M/2)))-((wc2/M_PI)*sinc(wc2/M_PI*(i-(M/2))));
    }

    // wn de la ventana de Kaiser
    double izero_beta = izero(Beta);
    double *izero_var = (double *)malloc(sizeof(double)*(M+1));
    double *wn = (double *)malloc(sizeof(double)*(M+1));
    double *y = (double *)malloc(sizeof(double)*(M+1));

    double arg_part;
    double arg;
    for(i=0; i<=M/2; i++){
        arg_part = (double)(i-(M/2))/(M/2);
        arg = sqrtf(1-powf(arg_part,2));
        y[i] = Beta*arg;
        y[M-i] = y[i];
    }

    for(i=0; i<=M/2; i++){
        izero_var[i]=izero(y[i]);
        izero_var[M-i]=izero_var[i];
        wn[i]=izero_var[i]/izero_beta;
        wn[M-i]=izero_var[M-i]/izero_beta;
    }

    // hn real
    double *hn = (double *)malloc(sizeof(double)*(M+1));
    for(i=0; i<=M; i++){
        hn[i]=h[i]*wn[i];
        //qDebug()<<hn[i];
    }

    // Liberar reserva de memoria
    free(h);
    free(izero_var);
    free(wn);
    free(y);

    return hn;
}

// Función sinc
double sinc(double x)
{
    if(x==0)
    {
        return 1;
    }
    else
    {
        return sin(M_PI*x)/(M_PI*x);
    }
}

// Compute Bessel function Izero(y) using a series approximation
// Comentarear si ya esta
double izero(double y){
double s=1, ds=1, d=0;
do
{
    d = d + 2;
    ds = ds*(y*y)/(d*d);
    s = s + ds;
}
while( ds > 1E-7 * s);
return(s);
}

double* filter(double* entrada, double* hn, int N, int M, int veces)
{

    int i, j, k;
    double *delay = (double *)malloc(sizeof(double)*(M+1));
    double *salida_mult = (double *)malloc(sizeof(double)*(M+1));
    double *salida_sum = (double *)malloc(sizeof(double)*N);

    for(i=0; i<M; i++){
        delay[i]=0;
        salida_mult[i]=0;
    }

    for(i=0; i<N; i++){
        salida_sum[i]=0;
    }

    if(veces==0){
        for(i=0; i<N; i++){
            for(j=M; j>=0; j--){
                delay[j]=delay[j-1];
            }
            delay[0]=entrada[i];

            for(k=0; k<=M; k++){
                salida_mult[k]=delay[k]*hn[k];
                salida_sum[i]=salida_sum[i]+salida_mult[k];
            }
        }
    }
    else if (veces==1){
        salida_sum[0]=entrada[0]*hn[0];
        delay[0]=salida_sum[0];
        for(i=1; i<N; i++){

            for(k=1; k<M; k++){
                salida_mult[k]=delay[k-1]*hn[k]*(-1);
                salida_sum[i]=salida_sum[i]+salida_mult[k];
                if(k==M-1){
                    salida_sum[i]=entrada[i]+salida_sum[i];
                }
                ;
            }
            for(j=M-1; j>=0; j--){
                delay[j]=delay[j-1];
            }
            delay[0]=salida_sum[i];
        }
    }
    free(delay);
    free(salida_mult);
    return salida_sum;
}


double* filter(double* entrada, double* hn, int N, int M)
{

    int i, j, k;
    double *delay = (double *)malloc(sizeof(double)*(M+1));
    double *salida_sum = (double *)malloc(sizeof(double)*N);



    for(i=0; i<M; i++){
        delay[i]=0;
    }

    for(i=0; i<N; i++){
        salida_sum[i]=0;
    }



    double *salida_mult = (double *)malloc(sizeof(double)*(M+1));



    for(i=0; i<N; i++){
        for(j=M; j>=0; j--){
            delay[j]=delay[j-1];
        }
        delay[0]=entrada[i];

        for(k=0; k<=M; k++){
            salida_mult[k]=delay[k]*hn[k];
            salida_sum[i]=salida_sum[i]+salida_mult[k];
        }
    }
    free(delay);
    free(salida_mult);
    return salida_sum;
}

void iir_df1(int q, int p, double b[], double a[], int N, double x[], double *y)
  {
      double x0[q+1],
             y0[p+1];

      for(int n=0; n<N; n++)
      {
          double xn = 0,
                 yn = 0;

          x0[0] = x[n];
          for(int i=0; i<q+1; i++)
          {
              xn += b[i] * x0[i];
          }
          for(int i=1; i<p+1; i++)
          {
              yn += a[i] * y0[i];
          }

          y[n] = xn - yn;

          for(int i=q; i>=1; i--)
          {
              x0[i] = x0[i-1];
          }
          for(int i=p; i>=2; i--)
          {
              y0[i] = y0[i-1];
          }
          y0[1] = y[n];

      }
  }


static double f(double i)
{
    if (i==0)
        return 1;
    else return i*f(i-1);
}

//Función para diseñar el LPF por el método de Filanovsky
void filter_design_filanovsky(double ro, double fc, double fs, double* z_num, double* z_den)
{
    double tao=M_PI/ro;

    // Hallar n1 y n
    double nb;
    int n, n1, i, j;

    nb=2*M_PI/tao;

    n=floor(nb);
    n1=n-1;

    // Hallar A1 y A2 para encontrar la H(s)
    double A1=1, A2;
    double temp = 1;
    for(i=0; i<n1; i++){
        A1=A1*pow((2*(i+1)+1),2);
    }

    for(i=0; i<n; i++){
        temp = temp*(pow(2*(i+1),2)-1);
    }

    A2=(tao*(pow(2*M_PI/tao,2)-1)*A1)/(8*pow(M_PI/tao,2)*sin(tao/2)*temp);

    // Hallar numerador y denominador
    double *num = (double *)malloc(sizeof(double)*(n*2+2));
    double *den1 = (double *)malloc(sizeof(double)*(n*2+3));
    den1[0]= 0;
    den1[1]= 0;
    den1[2]= 1;

    for(i=0; i<(n*2+2); i++){
        num[i] = 0;
    }
    num[n*2+1] = A1;

    double den1_var2[3];
    int lenA, lenB = 3, lenC;

    for(i=0; i<n; i++){
        double *den1_var1 = (double *)malloc(sizeof(double)*(3+(2*i)));
        lenA = 3+(2*i);
        for(j=0; j<(3+(2*i)); j++){
            den1_var1[j]=den1[j];
        }

        den1_var2[0] = 1;
        den1_var2[1] = 0;
        den1_var2[2] = pow(2*(i)+1,2);

        den1 = conv(den1_var1, den1_var2, lenA, lenB, &lenC);

        free(den1_var1);
    }

    double *den2 = (double *)malloc(sizeof(double)*(n*2+3));
    den2[0]= 0;
    den2[1]= 0;
    den2[2]= 1;

    double den2_var2[3];

    for(i=0; i<n; i++){
        double *den2_var1 = (double *)malloc(sizeof(double)*(3+(2*i)));
        lenA = 3+(2*i);
        for(j=0; j<(3+(2*i)); j++){
            den2_var1[j]=den2[j];
        }

        den2_var2[0] = 1;
        den2_var2[1] = 0;
        den2_var2[2] = pow(2*(i+1),2);

        den2 = conv(den2_var1, den2_var2, lenA, lenB, &lenC);

        free(den2_var1);
    }

    double *den1_1 = (double *)malloc(sizeof(double)*(n*2+2));
    double *den2_2 = (double *)malloc(sizeof(double)*(n*2+1));

    for(i=0; i<(n*2+1); i++){
        den1_1[i]=den1[i+1];
        den2_2[i]=den2[i+2];
    }
    den1_1[(n*2)+1]=den1[(n*2)+2];

    free(den2);
    free(den1);

    double *temp_1 = (double *)malloc(sizeof(double)*(n*2+1));
    for(i=0; i<n*2+1; i++){
        temp_1[i]=0;
    }
    temp_1[n*2-1] = A2;
    double *den = (double *)malloc(sizeof(double)*(n*2+2));
    double *temp_2 = (double *)malloc(sizeof(double)*(n*4+1));
    temp_2 = conv(temp_1, den2_2, (n*2+1), (n*2+1), &lenC);
    for(i=0; i<n*2+2; i++){
        den2_2[i] = temp_2[n*2-1+i];
        den[i] = den1_1[i]+den2_2[i];
    }

    free(temp_2);
    free(temp_1);
    free(den2_2);
    free(den1_1);

    // Desnormalizando a la frecuencia de corte deseada
    int d = n*2+2;

    for(i=0; i<d-1; i++){
        den[i]=den[i]/(pow(fc,d-i-1));
    }

    // Transformación bilineal
    double *num_inv = (double *)malloc(sizeof(double)*(n*2+2));
    double *den_inv = (double *)malloc(sizeof(double)*(n*2+2));
    for(i=0; i<d; i++){
        num_inv[i] = num[d-i-1];
        den_inv[i] = den[d-i-1];
    }
    free(num);
    free(den);

    //    double *z_num = (double *)malloc(sizeof(double)*(n*2+2));
    //    double *z_den = (double *)malloc(sizeof(double)*(n*2+2));

    for(i=0; i<d; i++){
        z_num[i] = s_to_z(num_inv, d-1, i, fs);
        z_den[i] = s_to_z(den_inv, d-1, i, fs);
    }

    for(i=d-1; i>=0; i--){
        z_num[i] = z_num[i]/z_den[0];
        z_den[i] = z_den[i]/z_den[0];
    }


    // Liberar memoria
    free(num_inv);
    free(den_inv);
}

//convolution algorithm
double *conv(double *A, double *B, int lenA, int lenB, int *lenC)
{
    int nconv;
    int i, j, i1;
    double tmp;
    double *C;

    //allocated convolution array
    nconv = lenA+lenB-1;
    C = (double*) calloc(nconv, sizeof(double));

    //convolution process
    for (i=0; i<nconv; i++)
    {
        i1 = i;
        tmp = 0.0;
        for (j=0; j<lenB; j++)
        {
            if(i1>=0 && i1<lenA)
                tmp = tmp + (A[i1]*B[j]);

            i1 = i1-1;
            C[i] = tmp;
        }
    }

    //get length of convolution array
    (*lenC) = nconv;

    //return convolution array
    return(C);
}

// computes a digital filter coefficient from the analog
// coefficients using the bilinear coefficient formula
double s_to_z
(
        double a[], // s domain coefficients
        int N,      // index of last coefficient (order)
        int n,      // z domain coeffient desired
        double fs   // sample rate
        )
{
    double z_coef = 0.0;
    for (int i=0; i<= N; ++i)
    {
        double acc = 0.0;
        for (int k=max(n-N+i,0); k<=min(i,n); ++k)
        {
            acc += ((f(i)*f(N-i))/(f(k)*f(i-k)*f(n-k)*f(N-i-n+k)))*((k & 1)?-1.0:1.0);
        }
        z_coef += (a[i]*pow(2.0*fs,i)*acc);
    }
    return z_coef;
}
