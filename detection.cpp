#include "detection.h"
#include <math.h>

//using namespace std;

int * detect_ED(double *signal_fft, int cant_muest, int *freq_length)
{
    /*
    //se filtra paso-bajo la señal FFT
    // Obtención de Beta y M para los filtros pasabajo de las dos ramas
    double delta_w_lpf, Beta_lpf, Aten_lpf, aten_lpf;

    aten_lpf=0.001;
    // frecuencia de corte
    double wc1_lpf_s = M_PI/16;
    delta_w_lpf = 0.1*wc1_lpf_s;

    Aten_lpf = -20*log10(aten_lpf);
    if(Aten_lpf > 50){
        Beta_lpf = 0.1102*(Aten_lpf-8.7);
    }
    else if(Aten_lpf >= 21 && Aten_lpf <= 50) {
        Beta_lpf = 0.5842*pow((Aten_lpf-21),0.4)+0.07886*(Aten_lpf-21);
    }
    else if(Aten_lpf < 21) {
        Beta_lpf = 0;
    }

    int M_lpf = ceilf((Aten_lpf-8)/(2.285*delta_w_lpf));
    if(fmodf(M_lpf,2)!=0)
        M_lpf=M_lpf+1;


    // hn del pasabajo superior
    double *hn_lpf_s = filter_design_direct_I(M_lpf, Beta_lpf, wc1_lpf_s, 0);

    // Filtrado pasabajo rama superior
    double *signal_FFT_2 = filter(signal_FFT, hn_lpf_s, cant_muest, M_lpf,0);

//    double *signal_FFT_2=new double[cant_muest];
//    for(int i=0;i<cant_muest;i++)
//        signal_FFT_2[i]=signal_FFT[i];
    */


    /* FFT */
    int i;
    int k;
    //double signal_FFT = new double[cant_muest/2];

    /* Windowing and arranging */
    //double wind_percent = 0.01;
    int wind_quant = 60;

    double wind_width = cant_muest/wind_quant;

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
        pdg[count] = double(1)/wind*pdg_temp*wind_quant;
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
    pn_w = 1.5*pn_w/(wind_quant-r);

    /* Detection */
    // Al terminar este procedimiento en changes habran k elementos que indican los indices donde hay senal en pdg
    k=0;
    int *changes = new int[100];
    int signal_present = 0;
    for(i=0; i < wind_quant; i++)
    {
        if (pdg[i] > 1.3*pn_w)
        {
            if (!signal_present)
            {
                changes[k++] = i*wind-(int)wind/2;//el elemento en change tiene ya el mismo �ndice de frecuencia que la FFT
                signal_present = 1;
            }
        }
        else
        {
            if (signal_present)
                changes[k++] = i*wind-(int)wind/2;
            signal_present = 0;
        }
    }

    int *new_changes = new int[k];

    for(i=0; i<k; i++)
        new_changes[i] = changes[i];

    *freq_length=k;

    //se libera espacio
    delete []pdg;
    delete []pdg_sorted;
    delete []A1;
    delete []changes;
    //delete []signal_FFT_2;
    //delete []hn_lpf_s;

    return new_changes;
}

void BubbleSort(double pdg_array[], int wind_quant)
{
int i, j;
double temp;
for (i = 0; i < (wind_quant - 1); ++i)
{
     for (j = 0; j < wind_quant - 1 - i; ++j )
     {
          if (pdg_array[j] < pdg_array[j+1])
          {
               temp = pdg_array[j+1];
               pdg_array[j+1] = pdg_array[j];
               pdg_array[j] = temp;
          }
     }
}
}

void energy_detection(double *y, int band, double *energy, int cant_muest)
{
    double energy_temp = 0;
    for(int i = 0; i<cant_muest; i++)
    {
        energy_temp += (y[i]*y[i]);
    }
    energy[band] = energy_temp/cant_muest;
}

int *detect_FB(double *signal_in, int signal_fm, int signal_length, int signal_fft_length, int *detect_freq_length)
{   /* FFT */
    int i;
    int k;

    //double signal_FFT = new double[cant_muest/2];

    /*Selecting the Number of Filters NoF
    practical criterion: maximum bandwidth 4 kHz*/
    int NoF;
    int BW_max = 4000;     // 2*10*4kHz = 80ksamples


    if (signal_fm >= 2*10*BW_max)
    {
        NoF =  ceil(float(signal_fm)/(2*BW_max));
    }
    else
    {
        NoF = 10;
    }

    // Filter desing parameters
    double BW_trans, BW, Beta, At;
    int M_bpf;

    //normalized filters bandwidth
    BW = M_PI/NoF;

    // transition band
    BW_trans = 0.15*BW;

    //Filter atenuation [dB]
    At = 30;//-20*log10(at);

    //Beta
    if(At > 50){
        Beta = 0.1102*(At-8.7);
    }
    else if(At >= 21 && At <= 50) {
        Beta = 0.5842*powf((At-21),0.4)+0.07886*(At-21);
    }
    else if(At < 21) {
    Beta = 0;
    }

    // vector of Fa and Fb
    double *wc = new double[NoF+1];
    wc[0] = 0;
    wc[NoF] = 1;
    for(i = 1; i<=NoF-1; i++)
    {
        wc[i] = double(M_PI*i)/NoF;
    }

    //Order of the filter
    //M=fix((A-8)/(2.285*2*AW));
    M_bpf = ceilf((At-8)/(2.285*BW_trans));
    if(fmodf(M_bpf,2) != 0){
        M_bpf = M_bpf + 1;
    }

    int total_coeff_bpf = M_bpf+1;
    double *e = new double[NoF];

    int y_temp_length = signal_length+total_coeff_bpf-1;
    double *y_temp = new double[y_temp_length];

    int count2;
    count2 = 0;
    for(count2 = 0; count2<NoF; count2++)
    {
        //Bandass filter coefficients
        double *bpf = filter_design_direct_I(M_bpf, Beta, wc[count2], wc[count2+1]);      
        convolve(bpf, total_coeff_bpf, signal_in, y_temp, signal_length);
        free(bpf);
    //double *pdg_sorted = new double[100];
    //BubbleSort(pdg_sorted, wind_quant);
    //void convolve(double p_coeffs[], int p_coeffs_n, double p_in[], double p_out[], int n)

        energy_detection(y_temp, count2, e, signal_length);
    }

    double *min_max = new double[2];
    find_min_max(e, min_max, NoF);

    double tresh = 0.2*(min_max[1]-min_max[0]);


    /* Detection */
    //int *band_detected = new int[NoF];

    k = 0;
    double *changes = new double[NoF*2];

    for(i=0;i<NoF;i++)
    {
        if(e[i]>tresh)
        {
            changes[k++] = double(i*signal_fft_length)/(NoF);           
            changes[k++] = double((i+1)*signal_fft_length)/(NoF);
        }
    }

    *detect_freq_length=k;

    int *new_changes = new int[k];
    for(i=0; i<k; i++)
        new_changes[i] = (int)changes[i];

    delete []wc;
    delete []e;
    delete []y_temp;
    delete []min_max;
    delete []changes;

    return new_changes;
}

//void find_min_max(double energy[], double &min, double &max, int n)
void find_min_max(double energy[], double min_max[], int n)
{
    //n = NoF;
    int count;
    double num, min, max;

    min = energy[0];
    max = energy[0];
    count=1;
    while(count<n)
    {
        num = energy[count];
        if(num > max)
            max = num;
        if(num < min)
            min = num;
        count++;
    }
    min_max[0] = min;
    min_max[1] = max;
}

void convolve(double p_coeffs[], int p_coeffs_n, double p_in[], double p_out[], int n)
{
  int i, k, start, end;
  double tmp;

  for (k = 0; k < p_coeffs_n+n-1; k++)  //  position in output
  {
    tmp = 0;

    start = (0>k+1-n ? 0 : k+1-n);
    end = (k<p_coeffs_n ? k : p_coeffs_n);

    for (i = start; i <= end; i++)  //  position in coefficients array
    {
      tmp += (p_coeffs[i]*p_in[k-i+1]);
    }
    p_out [k] = tmp;
  }
}
