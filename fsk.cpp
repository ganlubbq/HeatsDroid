#include "fsk.h"
#include "utils.h"
#include "filters.h"

void polygonal (double y [], double *threshold, double x [], double f0, double f1, double baud_rate, double fm,int N)
{
    double w0=2*M_PI*f0/fm;
    double w1=2*M_PI*f1/fm;

    if(f0<f1)
    {
        w0=2*M_PI*f1/fm;
        w1=2*M_PI*f0/fm;
    }    

    //variables de recorrido
    int i=0,j=0;

    //registros del filtro de correlaci�n
    double mult_cos_w0=0, mult_sen_w0=0, mult_cos_w1=0, mult_sen_w1=0;

    //salida de los integradores
    double* integ_I1w0=(double *)malloc(sizeof(double)*(N));
    double* integ_Q1w0=(double *)malloc(sizeof(double)*(N));
    double cumul_I1w0=0;
    double cumul_Q1w0=0;

    //variables del m�todo poligonal
    double d1=0.5*(fabs(1/sin((w0-w1)/2))+fabs(1/sin((w0+w1)/2)));
    double d2=0.25/fabs(sin(w0));
    double d=d2;
    if(d1>d2)
        d=d1;

    int *points_I1w0=(int *)malloc(sizeof(int)*(N));//registros del filtro paso-bajo
    int *points_Q1w0=(int *)malloc(sizeof(int)*(N));//registros del filtro paso-bajo
    points_I1w0[0]=0;
    points_Q1w0[0]=0;
    int M_I1w0=1;
    int M_Q1w0=1;

    double tan_min_I1w0=0;
    double tan_max_I1w0=0;
    double tan_min_Q1w0=0;
    double tan_max_Q1w0=0;
    int x0_I1w0=0;
    int x0_Q1w0=0;

    double tan_min_i_I1w0=0;
    double tan_max_i_I1w0=0;
    double tan_min_i_Q1w0=0;
    double tan_max_i_Q1w0=0;

    //system output
    double* levels=y;

    //correlador
    for(i=0;i<N;i++)
    {
        //correlacion
        if(x[i]>0)
        {
            mult_cos_w0=cos(i*w0);
            mult_sen_w0=sin(i*w0);
        }
        else
        {
            mult_cos_w0=-cos(i*w0);
            mult_sen_w0=-sin(i*w0);
        }

        //integrator
        integ_I1w0[i]=mult_cos_w0+cumul_I1w0;
        cumul_I1w0=integ_I1w0[i];
        integ_Q1w0[i]=mult_sen_w0+cumul_Q1w0;
        cumul_Q1w0=integ_Q1w0[i];        
   }


    //método
    tan_min_I1w0=(integ_I1w0[1]-integ_I1w0[0]-d);
    tan_max_I1w0=(integ_I1w0[1]-integ_I1w0[0]+d);
    x0_I1w0=0;

    tan_min_Q1w0=(integ_Q1w0[1]-integ_Q1w0[0]-d);
    tan_max_Q1w0=(integ_Q1w0[1]-integ_Q1w0[0]+d);
    x0_Q1w0=0;    

    for(i=2;i<N;i++)
    {

        //in phase
        tan_min_i_I1w0=(double)(integ_I1w0[i]-integ_I1w0[x0_I1w0]-d)/(double)(i-x0_I1w0);
        tan_max_i_I1w0=(double)(integ_I1w0[i]-integ_I1w0[x0_I1w0]+d)/(double)(i-x0_I1w0);

        if (tan_max_i_I1w0<tan_min_I1w0 || tan_min_i_I1w0>tan_max_I1w0)//pto de ruptura
        {
           points_I1w0[M_I1w0]=i;
           x0_I1w0=i;
           M_I1w0=M_I1w0+1;
           tan_min_I1w0=(double)(integ_I1w0[i+1]-integ_I1w0[x0_I1w0]-d)/(double)(i+1-x0_I1w0);
           tan_max_I1w0=(double)(integ_I1w0[i+1]-integ_I1w0[x0_I1w0]+d)/(double)(i+1-x0_I1w0);
        }
        else
        {

            if(tan_min_I1w0<tan_min_i_I1w0)
            {
                tan_min_I1w0=tan_min_i_I1w0;
            }

            if(tan_max_I1w0>tan_max_i_I1w0)
            {
                tan_max_I1w0=tan_max_i_I1w0;
            }

        }

        //quadrature
        tan_min_i_Q1w0=(double)(integ_Q1w0[i]-integ_Q1w0[x0_Q1w0]-d)/(double)(i-x0_Q1w0+1);
        tan_max_i_Q1w0=(double)(integ_Q1w0[i]-integ_Q1w0[x0_Q1w0]+d)/(double)(i-x0_Q1w0+1);


        if (tan_max_i_Q1w0<tan_min_Q1w0 || tan_min_i_Q1w0>tan_max_Q1w0)//pto de ruptura
        {
           points_Q1w0[M_Q1w0]=i;           
           x0_Q1w0=i;
           M_Q1w0=M_Q1w0+1;
           tan_min_Q1w0=(double)(integ_Q1w0[i+1]-integ_Q1w0[x0_Q1w0]-d)/(double)(i+1-x0_Q1w0);
           tan_max_Q1w0=(double)(integ_Q1w0[i+1]-integ_Q1w0[x0_Q1w0]+d)/(double)(i+1-x0_Q1w0);           
        }
        else
        {
            if(tan_min_Q1w0<tan_min_i_Q1w0)
            {
                tan_min_Q1w0=tan_min_i_Q1w0;
            }

            if(tan_max_Q1w0>tan_max_i_Q1w0)
            {
                tan_max_Q1w0=tan_max_i_Q1w0;
            }
        }

    }

    points_I1w0[M_I1w0]=N;
    M_I1w0=M_I1w0+1;

    points_Q1w0[M_Q1w0]=N;
    M_Q1w0=M_Q1w0+1;

    //promediaci�n de pendientes seg�n los puntos dominantes
    double* slope_I1w0=(double *)malloc(sizeof(double)*(N));    
    double* slope_Q1w0=(double *)malloc(sizeof(double)*(N));    
    double slope=0;
    int p1=0,p2=0,L=0;

    for (i=0;i<M_I1w0-1;i++)
    {
        p1=points_I1w0[i];
        p2=points_I1w0[i+1];
//         printf("\np1->%d, p2->%d",p1,p2);
        L=p2-p1+1;

        slope=0;

        for(j=p1;j<=p2;j++)
        {
            slope+=(integ_I1w0[j]);
        }

        slope=(slope-L*integ_I1w0[p1])/(L*(L+1)/2);
        for (j=p1;j<=p2;j++)
        {
            slope_I1w0[j]=slope;
            levels[j]=slope*slope;
        }
    }

    for (i=0;i<M_Q1w0-1;i++)
    {
        p1=points_Q1w0[i];
        p2=points_Q1w0[i+1];
        //printf("\np1->%d, p2->%d",p1,p2);
        L=p2-p1+1;

        slope=0;

        for(j=p1;j<=p2;j++)
        {
            slope+=(integ_Q1w0[j]);
        }

        slope=(slope-L*integ_Q1w0[p1])/(L*(L+1)/2);
        for (j=p1;j<=p2;j++)
        {
            slope_Q1w0[j]=slope;
            levels[j]+=slope*slope;
        }
    }

    //levels[M_Q1w0]=(double)N;

    //levels=points_I1w0;
    *threshold=(double)0.5*1/4;

    delete[] integ_I1w0;
    delete[] integ_Q1w0;
    delete[] points_I1w0;
    delete[] points_Q1w0;
    delete[] slope_I1w0;
    delete[] slope_Q1w0;

    return;
}

void HL_derivator (double y [], double *threshold, double x [], double amp, double f0, double f1, double baud_rate, double fm,int N)
{
    double w0=2*M_PI*f0/fm;
    double w1=2*M_PI*f1/fm;

    if(f0>f1)
    {
        w0=2*M_PI*f1/fm;
        w1=2*M_PI*f0/fm;
    }

    //printf("w_0->%f, w1->%f",w0,w1);

    //variables de recorrido
    int i=0,j=0;

    //registros del filtro de correlaci�n
    double mult_cos_w0=0, mult_sen_w0=0, mult_cos_w1=0, mult_sen_w1=0;

    //salida de los integradores
    double integ_I1w0=0;
    double integ_Q1w0=0;
    double cumul_I1w0=0;
    double cumul_Q1w0=0;
    double integ_I1w1=0;
    double integ_Q1w1=0;
    double cumul_I1w1=0;
    double cumul_Q1w1=0;

    //diferenciador
    int total_coeff_diff=3;
    //coeficientes del filtro paso-bajo
    double b_diff[3] = {1, 0,-1};
    //registros del filtro diferenciador
    double *r_diff_integ_I1w0=(double *)malloc(sizeof(double)*(total_coeff_diff));//registros del filtro paso-bajo
    double *r_diff_integ_Q1w0=(double *)malloc(sizeof(double)*(total_coeff_diff));//registros del filtro paso-bajo
    double *r_diff_integ_I1w1=(double *)malloc(sizeof(double)*(total_coeff_diff));//registros del filtro paso-bajo
    double *r_diff_integ_Q1w1=(double *)malloc(sizeof(double)*(total_coeff_diff));//registros del filtro paso-bajo
    //salida del filtro diferenciador
    double levels_I1w0=0;
    double levels_Q1w0=0;
    double levels_I1w1=0;
    double levels_Q1w1=0;

    //system output
    double levels=0;
    double* levels_lpf=y;

    //condici�n de umbral    
    double L=total_coeff_diff;
    double R_00=amp*amp*L/2*abs(sin(w0*L)/(sin(w0)))+amp*amp/4*powf(sin(w0*L)/(sin(w0)),2);
    double a=amp/2*sin((w0-w1)*L/2)/(sin((w0-w1)/2));
    double b=sin((w0+w1)*L/2)/(sin((w0+w1)/2));
    double R_01=powf(abs(a)+abs(b),2);
    double condition=amp*amp/4*L*L-R_00-R_01;


    //registros de filtro paso-bajo
    // Obtenci�n de Beta y M para los filtros pasabajo de las dos ramas
    double delta_w_lpf, wc1_lpf_i, Beta_lpf, Aten_lpf;
    int M_lpf;
    //dise�o del filtro paso-bajo
    //frecuencia de corte del filtro paso-bajo
    wc1_lpf_i= 6*2*M_PI*baud_rate/fm;
    // banda de transici�n del filtro paso-bajo
    delta_w_lpf = 0.15*wc1_lpf_i;
    //Atenuaci�n del filtro en dB
    Aten_lpf = 30;//-20*log10(aten_lpf);
    if(Aten_lpf > 50){
        Beta_lpf = 0.1102*(Aten_lpf-8.7);
    }
    else if(Aten_lpf >= 21 && Aten_lpf <= 50) {
        Beta_lpf = 0.5842*powf((Aten_lpf-21),0.4)+0.07886*(Aten_lpf-21);
    }
    else if(Aten_lpf < 21) {
    Beta_lpf = 0;
    }
    //orden del filtro
    //M=fix((A-8)/(2.285*2*AW));
    M_lpf = ceilf((Aten_lpf-8)/(2.285*delta_w_lpf));
    if(fmodf(M_lpf,2)!=0){
        M_lpf=M_lpf+1;
    }
    int total_coeff_lpf=M_lpf+1;
    //printf("wc->%f, delta->%f, M->%d, A->%f",wc1_lpf_i/M_PI,delta_w_lpf,M_lpf,Aten_lpf);

    //coeficientes del filtro paso-bajo
    double *b_lpf = filter_design_direct_I(M_lpf, Beta_lpf, wc1_lpf_i, 0);
    //registros del filtro paso-bajo
    double *r_lpf_cos=(double *)malloc(sizeof(double)*(total_coeff_lpf));//registros del filtro paso-bajo
    double *r_lpf_sen=(double *)malloc(sizeof(double)*(total_coeff_lpf));//registros del filtro paso-bajo


    //filtro de salida
    double *r_lpf_out=(double *)malloc(sizeof(double)*(total_coeff_lpf));//registros del filtro paso-bajo

    //inicializaci�n de los registros del filtro paso-bajo
    for(i=0;i<total_coeff_lpf;i++)
    {
        r_lpf_cos[i]=0;
        r_lpf_sen[i]=0;
        r_lpf_out[i]=0;
    }

    //inicializaci�n de los registros del filtro diferenciador
    for(i=0;i<total_coeff_diff;i++)
    {
        r_diff_integ_I1w0[i]=0;
        r_diff_integ_Q1w0[i]=0;
    }



    //demodulator
    double cumul=0;//para la determinación de la componente de directa a la salida
    for(i=0;i<N;i++)
    {
        //correlacion
        if(x[i]>0)
        {
            mult_cos_w0=cos(i*w0);
            mult_sen_w0=sin(i*w0);
            mult_cos_w1=cos(i*w1);
            mult_sen_w1=sin(i*w1);
        }
        else
        {
            mult_cos_w0=-cos(i*w0);
            mult_sen_w0=-sin(i*w0);
            mult_cos_w1=-cos(i*w1);
            mult_sen_w1=-sin(i*w1);
        }

        //integrator
        integ_I1w0=mult_cos_w0+cumul_I1w0;
        cumul_I1w0=integ_I1w0;
        integ_Q1w0=mult_sen_w0+cumul_Q1w0;
        cumul_Q1w0=integ_Q1w0;
        integ_I1w1=mult_cos_w1+cumul_I1w1;
        cumul_I1w1=integ_I1w1;
        integ_Q1w1=mult_sen_w1+cumul_Q1w1;
        cumul_Q1w1=integ_Q1w1;

        //inicializaci�n de la salida de los filtros


        //filtrado diferenciador
        levels_I1w0=0;
        levels_Q1w0=0;
        levels_I1w1=0;
        levels_Q1w1=0;
        r_diff_integ_I1w0[0]=integ_I1w0;
        r_diff_integ_Q1w0[0]=integ_Q1w0;
        r_diff_integ_I1w1[0]=integ_I1w1;
        r_diff_integ_Q1w1[0]=integ_Q1w1;
        //convoluci�n
        for(j=0;j<total_coeff_diff;j++)
        {
            levels_I1w0=levels_I1w0+r_diff_integ_I1w0[j]*b_diff[j];
            levels_Q1w0=levels_Q1w0+r_diff_integ_Q1w0[j]*b_diff[j];
            levels_I1w1=levels_I1w1+r_diff_integ_I1w1[j]*b_diff[j];
            levels_Q1w1=levels_Q1w1+r_diff_integ_Q1w1[j]*b_diff[j];
        }

        //retardo
        for(j=total_coeff_diff-1;j>0;j--)
        {
            r_diff_integ_I1w0[j]=r_diff_integ_I1w0[j-1];
            r_diff_integ_Q1w0[j]=r_diff_integ_Q1w0[j-1];
            r_diff_integ_I1w1[j]=r_diff_integ_I1w1[j-1];
            r_diff_integ_Q1w1[j]=r_diff_integ_Q1w1[j-1];
        }

        levels=levels_I1w0*levels_I1w0+levels_Q1w0*levels_Q1w0-levels_I1w1*levels_I1w1-levels_Q1w1*levels_Q1w1;


        if(condition<=0)
        {
            //filtro de salida paso-bajo
            r_lpf_out[0]=levels;
            //convoluci�n
            for(j=0;j<total_coeff_lpf;j++)
            {
                levels_lpf[i]=levels_lpf[i]+r_lpf_out[j]*b_lpf[j];
            }
            //retardo
            for(j=total_coeff_lpf-1;j>0;j--)
            {
                r_lpf_out[j]=r_lpf_out[j-1];
            }

            //cálculo del umbral
            //*threshold+=levels_lpf[i];
        }
        else
        {
            levels_lpf[i]=levels;
        }

        cumul+=levels_lpf[i];
   }

    cumul=cumul/N;
    //se elimina la componente de directa
    for(i=0;i<N;i++)
    {
        levels_lpf[i]-=cumul;
    }

    *threshold=0;

//    if(condition<=0)
//    {
//        *threshold=*threshold/N;

//    }
//    else
//    {
//        *threshold=0;//(1/4*L*L-R_00+R_01)/2;
//    }

    delete[] r_diff_integ_I1w0;
    delete[] r_diff_integ_Q1w0;
    delete[] r_diff_integ_I1w1;
    delete[] r_diff_integ_Q1w1;
    delete[] b_lpf;
    delete[] r_lpf_cos;
    delete[] r_lpf_sen;
    delete[] r_lpf_out;

    return;
}

bool balanced_quadricorrelator (double y [], double *threshold, double x [], double f0, double f1, double fm, int N)
{
    *threshold=0;

    double w0=2*M_PI*f0/fm;
    double w1=2*M_PI*f1/fm;
    double filter_sen=0;
    double filter_cos=0;//(double *)malloc(sizeof(double)*(N));

    //deriv filter output
    double deriv_cos=0;//(double *)malloc(sizeof(double)*(N));
    double deriv_sen=0;//(double *)malloc(sizeof(double)*(N));

    //system output
    double* levels=y;

    double wr=(w0+w1)/2; //frecuencia de referencia
    double temp=0;
    int i=0,j=0, z=0; //variables de recorrido

    //registros del filtro de correlaci�n
    double mult_cos=0, mult_sen=0, y_1c=0, y_0c=0;

    //registros de filtro paso-bajo
    // Obtenci�n de Beta y M para los filtros pasabajo de las dos ramas
    double delta_w_lpf, wc1_lpf_i, Beta_lpf, Aten_lpf, aten_lpf=0.2;
    int M_lpf;
    //dise�o del filtro paso-bajo
    //frecuencia de corte del filtro paso-bajo
    wc1_lpf_i= 0.7*qAbs<double>(w0-w1);
    // banda de transici�n del filtro paso-bajo
    delta_w_lpf = 0.3*wc1_lpf_i;
    //Atenuaci�n del filtro en dB
    Aten_lpf = 50;//-20*log10(aten_lpf);
    if(Aten_lpf > 50){
        Beta_lpf = 0.1102*(Aten_lpf-8.7);
    }
    else if(Aten_lpf >= 21 && Aten_lpf <= 50) {
        Beta_lpf = 0.5842*powf((Aten_lpf-21),0.4)+0.07886*(Aten_lpf-21);
    }
    else if(Aten_lpf < 21) {
    Beta_lpf = 0;
    }
    //orden del filtro
    //M=fix((A-8)/(2.285*2*AW));
    M_lpf = ceilf(double(Aten_lpf-8)/double(2.285*delta_w_lpf));
    if(fmodf(M_lpf,2)!=0){
        M_lpf=M_lpf+1;
    }
    int total_coeff_lpf=M_lpf+1;

    //validación
    if(total_coeff_lpf>N/10)
    {
        return 0;
    }

    //coeficientes del filtro paso-bajo
    double *b_lpf = filter_design_direct_I(M_lpf, Beta_lpf, wc1_lpf_i, 0);

    //registros del filtro paso-bajo
    double *r_lpf_cos=(double *)malloc(sizeof(double)*(total_coeff_lpf));//registros del filtro paso-bajo
    double *r_lpf_sen=(double *)malloc(sizeof(double)*(total_coeff_lpf));//registros del filtro paso-bajo


    //coeficientes del derivador
    int  total_coeff_deriv=6;
    double b_deriv[6]={0.0161787425072926,-0.0519088642721128,0.408335563724063,-0.408335563724063,0.0519088642721128,-0.0161787425072926};

    //registros del filtro derivador
    double *r_deriv_cos=(double *)malloc(sizeof(double)*(total_coeff_deriv));//registros del filtro paso-bajo
    double *r_deriv_sen=(double *)malloc(sizeof(double)*(total_coeff_deriv));//registros del filtro paso-bajo

    //inicializaci�n de los registros del filtro paso-bajo
    for(i=0;i<total_coeff_lpf;i++)
    {
        r_lpf_cos[i]=0;
        r_lpf_sen[i]=0;
    }

    //inicializaci�n de los registros del filtro derivador
    for(i=0;i<total_coeff_deriv;i++)
    {
        r_deriv_cos[i]=0;
        r_deriv_sen[i]=0;
    }

    //correlator
    for(i=0;i<N;i++)
    {
        //correlacion
        mult_cos=x[i]*cos(i*wr);
        mult_sen=x[i]*sin(i*wr);
        //y_0c=x_0c+x_1c+y_1c;
        //x_1c=x_0c;
        //y_1c=y_0c;
        filter_cos=0;
        filter_sen=0;
        deriv_cos=0;
        deriv_sen=0;


        //filtrado paso-bajo
        r_lpf_cos[0]=mult_cos;
        r_lpf_sen[0]=mult_sen;
        //convoluci�n
        for(j=0;j<total_coeff_lpf;j++)
        {
            filter_cos=filter_cos+r_lpf_cos[j]*b_lpf[j];
            filter_sen=filter_sen+r_lpf_sen[j]*b_lpf[j];
        }

        //retardo
        for(j=total_coeff_lpf-1;j>0;j--)
        {
            r_lpf_cos[j]=r_lpf_cos[j-1];
            r_lpf_sen[j]=r_lpf_sen[j-1];
        }

        //derivators
        r_deriv_cos[0]=filter_cos;
        r_deriv_sen[0]=filter_sen;
        //convoluci�n
        for(j=0;j<total_coeff_deriv;j++)
        {
            deriv_cos=deriv_cos+r_deriv_cos[j]*b_deriv[j];
            deriv_sen=deriv_sen+r_deriv_sen[j]*b_deriv[j];
        }

        //retardo
        for(j=total_coeff_deriv-1;j>0;j--)
        {
            r_deriv_cos[j]=r_deriv_cos[j-1];
            r_deriv_sen[j]=r_deriv_sen[j-1];
        }

        levels[i]=qPow(10,2)*(deriv_cos*filter_sen-deriv_sen*filter_cos);

   }

    delete[] r_lpf_cos;
    delete[] r_lpf_sen;
    delete[] r_deriv_cos;
    delete[] r_deriv_sen;
    delete[] b_lpf;

    return 1;
}

bool quadricorrelator (double y [], double *threshold, double x [], double f0, double f1, double fm, int N)
{
    *threshold=0;
    double w0=2*M_PI*f0/fm;
    double w1=2*M_PI*f1/fm;
    double filter_sen=0;
    double filter_cos=0;//(double *)malloc(sizeof(double)*(N));

    //deriv filter output
    double deriv_cos=0;//(double *)malloc(sizeof(double)*(N));
    //double deriv_sen=0;//(double *)malloc(sizeof(double)*(N));

    //system output
    double* levels=y;

    double wr=(w0+w1)/2; //frecuencia de referencia
    //double temp=0;
    int i=0,j=0, z=0; //variables de recorrido

    //registros del filtro de correlaci�n
    double mult_cos=0, mult_sen=0;

    //registros de filtro paso-bajo
    // Obtenci�n de Beta y M para los filtros pasabajo de las dos ramas
    double delta_w_lpf, wc1_lpf_i, Beta_lpf, Aten_lpf, aten_lpf=0.2;
    int M_lpf;
    //dise�o del filtro paso-bajo
    //frecuencia de corte del filtro paso-bajo
    wc1_lpf_i= 0.7*qAbs<double>(w0-w1);
    // banda de transici�n del filtro paso-bajo
    delta_w_lpf = 0.3*wc1_lpf_i;
    //Atenuaci�n del filtro en dB
    Aten_lpf = 50;//-20*log10(aten_lpf);
    if(Aten_lpf > 50){
        Beta_lpf = 0.1102*(Aten_lpf-8.7);
    }
    else if(Aten_lpf >= 21 && Aten_lpf <= 50) {
        Beta_lpf = 0.5842*powf((Aten_lpf-21),0.4)+0.07886*(Aten_lpf-21);
    }
    else if(Aten_lpf < 21) {
    Beta_lpf = 0;
    }
    //orden del filtro
    //M=fix((A-8)/(2.285*2*AW));
    M_lpf = ceilf((Aten_lpf-8)/(2.285*delta_w_lpf));
    if(fmodf(M_lpf,2)!=0){
        M_lpf=M_lpf+1;
    }
    int total_coeff_lpf=M_lpf+1;

    //validación
    if(total_coeff_lpf>N/10)
    {
        return 0;
    }

    //coeficientes del filtro paso-bajo
    double *b_lpf = filter_design_direct_I(M_lpf, Beta_lpf, wc1_lpf_i, 0);
    //registros del filtro paso-bajo
    double *r_lpf_cos=(double *)malloc(sizeof(double)*(total_coeff_lpf));//registros del filtro paso-bajo
    double *r_lpf_sen=(double *)malloc(sizeof(double)*(total_coeff_lpf));//registros del filtro paso-bajo


    //coeficientes del derivador
    int  total_coeff_deriv=6;
    double b_deriv[6]={0.0161787425072926,-0.0519088642721128,0.408335563724063,-0.408335563724063,0.0519088642721128,-0.0161787425072926};
    //registros del filtro derivador
    double *r_deriv_cos=(double *)malloc(sizeof(double)*(total_coeff_deriv));//registros del filtro paso-bajo

    //linea de retardo
    int L_deriv=(total_coeff_deriv/2);
    double *delay_line=(double *)malloc(sizeof(double)*(L_deriv));//registros del filtro paso-bajo

    //filtro de salida
    double *r_lpf_out=(double *)malloc(sizeof(double)*(total_coeff_lpf));//registros del filtro paso-bajo

    //inicializaci�n de los registros del filtro paso-bajo
    for(i=0;i<total_coeff_lpf;i++)
    {
        r_lpf_cos[i]=0;
        r_lpf_sen[i]=0;
        r_lpf_out[i]=0;
    }

    //inicializaci�n de los registros del filtro derivador
    for(i=0;i<total_coeff_deriv;i++)
    {
        r_deriv_cos[i]=0;
    }

    //inicializaci�n de los registros de la l�nea de retardo
    for(i=0;i<L_deriv;i++)
    {
        delay_line[i]=0;
    }


    //correlator
    for(i=0;i<N;i++)
    {
        //correlacion
        mult_cos=x[i]*cos(i*wr);
        mult_sen=x[i]*sin(i*wr);
        //y_0c=x_0c+x_1c+y_1c;
        //x_1c=x_0c;
        //y_1c=y_0c;
        filter_cos=0;
        filter_sen=0;
        deriv_cos=0;
        //deriv_sen=0;


        //filtrado paso-bajo
        r_lpf_cos[0]=mult_cos;
        r_lpf_sen[0]=mult_sen;
        //convoluci�n
        for(j=0;j<total_coeff_lpf;j++)
        {
            filter_cos=filter_cos+r_lpf_cos[j]*b_lpf[j];
            filter_sen=filter_sen+r_lpf_sen[j]*b_lpf[j];
        }

        //retardo
        for(j=total_coeff_lpf-1;j>0;j--)
        {
            r_lpf_cos[j]=r_lpf_cos[j-1];
            r_lpf_sen[j]=r_lpf_sen[j-1];
        }

        //derivators
        r_deriv_cos[0]=filter_cos;
        //convoluci�n
        for(j=0;j<total_coeff_deriv;j++)
        {
            deriv_cos=deriv_cos+r_deriv_cos[j]*b_deriv[j];
        }
        //retardo
        for(j=total_coeff_deriv-1;j>0;j--)
        {
            r_deriv_cos[j]=r_deriv_cos[j-1];
        }

        //linea de retardo del filtro paso-bajo
        for(j=L_deriv-1;j>0;j--)
        {
            delay_line[j]=delay_line[j-1];
        }
        delay_line[0]=filter_sen;

        //filtro de salida paso-bajo
        r_lpf_out[0]=deriv_cos*delay_line[L_deriv-1];
        //convoluci�n
        for(j=0;j<total_coeff_lpf;j++)
        {
            levels[i]=levels[i]+qPow(10,2)*(r_lpf_out[j]*b_lpf[j]);
        }
        //retardo
        for(j=total_coeff_lpf-1;j>0;j--)
        {
            r_lpf_out[j]=r_lpf_out[j-1];
        }
        //levels[i]=//-deriv_sen*filter_cos;
   }

    delete[] r_lpf_cos;
    delete[] r_lpf_sen;
    delete[] r_deriv_cos;
    delete[] delay_line;
    delete[] r_lpf_out;
    delete[] b_lpf;

    return 1;
}

bool detector_envolvente (double* &y, double *threshold, double x [], double signal_amp, double f0, double f1, double fm, int N, int selection)
{
    *threshold=0;

    double delta_w_bpf, Beta_bpf, Aten_bpf, aten_bpf=0.2;
    int M_bpf, M_lpf;

    delta_w_bpf = (double)abs(f0-((f1+f0)/2))*2*M_PI/fm;

    Aten_bpf = -20*log10(aten_bpf);
    if(Aten_bpf > 50){
        Beta_bpf = 0.1102*(Aten_bpf-8.7);
    }
    else if(Aten_bpf >= 21 && Aten_bpf <= 50) {
        Beta_bpf = 0.5842*pow((Aten_bpf-21),0.4)+0.07886*(Aten_bpf-21);
    }
    else if(Aten_bpf < 21) {
        Beta_bpf = 0;
    }

    M_bpf = ceilf((Aten_bpf-8)/(2.285*delta_w_bpf));
    if(fmodf(M_bpf,2)!=0){
        M_bpf=M_bpf+1;
    }

    //validación
    if(M_bpf>N/10)
    {
        return 0;
    }

    // Rama superior
    // Filtro Pasabanda a f0
    double wc1_bpf_s = (f0*2*M_PI/fm)+delta_w_bpf;
    double wc2_bpf_s = (f0*2*M_PI/fm)-delta_w_bpf;

    // hn del pasabanda superior
    double *hn_bpf_s = filter_design_direct_I(M_bpf, Beta_bpf, wc1_bpf_s, wc2_bpf_s);

    // Rama inferior
    // Filtro Pasabanda a f1
    double wc1_bpf_i = (f1*2*M_PI/fm)+delta_w_bpf;
    double wc2_bpf_i = (f1*2*M_PI/fm)-delta_w_bpf;

    // hn del pasabanda inferior
    double *hn_bpf_i = filter_design_direct_I(M_bpf, Beta_bpf, wc1_bpf_i, wc2_bpf_i);


    // Filtrado de la se al
    // Filtrado pasabanda rama superior
    double *salida_bpf_s = filter(x, hn_bpf_s, N, M_bpf,0);

    // Rectificaci n de media onda
    double *salida_bpf_s_rectif = new double[N];
    for(int i=0; i<N; i++){
        if(salida_bpf_s[i]<0){
            salida_bpf_s_rectif[i]=0;
        }
        else{
            salida_bpf_s_rectif[i]=salida_bpf_s[i];
        }
    }

    // Filtrado pasabanda rama inferior
    double *salida_bpf_i = filter(x, hn_bpf_i, N, M_bpf,0);


    // Rectificación de media onda
    double *salida_bpf_i_rectif = new double[N];
    for(int i=0; i<N; i++){
        if(salida_bpf_i[i]<0){
            salida_bpf_i_rectif[i]=0;
        }
        else{
            salida_bpf_i_rectif[i]=salida_bpf_i[i];
        }
    }

//    if(selection==1)//Eigen filter
//    {
//        //Diseño del filtro
//        int     p=2, q=5;                      // Orden (p: denominador, q: numerador)
//        double  wp=2*M_PI*f0/fm/5;            // Frecuencias de corte.
//        double  ws=wp + 2*M_PI*abs(f0-f1)/fm;
//        double  h, e = 1.4,                        // Este parámetro es interesante. Modificandolo
//                // se obtienen diferentes valores del tiempo de subida
//                // manteniendo la respuesta de frecuencia intacta.
//                betasT[3] = {0, 0, 25},         // pesos en la matriz Pt
//                betasF[3] = {100, 10, 10},      // pesos en la matriz Pf
//                alphas[2] = {0.005, 1-0.005},   // pesos en la matriz final P
//                powersF[3] = {2, 1, 0},         // exponente de la funcion de peso
//                b[q+1], a[p+1];                 // Los coeficientes del filtro resultantes

//        lptx_eigeniir (p, q, wp, ws, e,
//                    betasT, betasF, alphas, powersF,
//                    USTEP_DEF, TRANSBAND_DEF, WEIGHT_DEF,
//                    MAXITER_DEF, DELTA1_DEF, DELTA2_DEF,
//                    100 , b, a);



//        double *salida_lpf_s = NULL;
//        double *salida_lpf_i = NULL;

//        // Filtrado pasabajo rama inferior
//        filter_IIR_FIR(salida_lpf_s,salida_bpf_s_rectif,N,b,a,q+1,p+1);
//        // Filtrado pasabajo rama superior
//        filter_IIR_FIR(salida_lpf_i,salida_bpf_i_rectif,N,b,a,q+1,p+1);

//        for(int i=0; i<N;i++)
//        {
//            y[i]=10*(salida_lpf_s[i]-salida_lpf_i[i]);
//        }

//        free(salida_lpf_s);
//        free(salida_lpf_i);

//    }

    if(selection==1)//Filanovsky
    {
        double ro=1.6;
        double tao=M_PI/ro;

        // Hallar n1 y n
        double nb=2*M_PI/tao;
        int n=floor(nb);
        M_lpf=2*n+2;

        //validación
        if(M_lpf>N/10)
        {
            free(hn_bpf_s);
            free(hn_bpf_i);

            free(salida_bpf_s);
            free(salida_bpf_s_rectif);

            free(salida_bpf_i);
            free(salida_bpf_i_rectif);
            return 0;
        }

        double *z_num_lpf_s = (double *)malloc(sizeof(double)*(n*2+2));
        double *z_den_lpf_s = (double *)malloc(sizeof(double)*(n*2+2));

        filter_design_filanovsky(ro, f0, fm, z_num_lpf_s, z_den_lpf_s);


        double *z_num_lpf_i = (double *)malloc(sizeof(double)*(n*2+2));
        double *z_den_lpf_i = (double *)malloc(sizeof(double)*(n*2+2));

        filter_design_filanovsky(ro, f0, fm, z_num_lpf_i, z_den_lpf_i);

//        double *salida_lpf_s = NULL;
//        double *salida_lpf_i = NULL;

//        double *salida_lpf_s = NULL;
//        double *salida_lpf_i = NULL;

//        filter_iir(salida_lpf_i,salida_bpf_i,N,z_num_lpf_i,z_den_lpf_i,M_lpf,M_lpf);
//        filter_iir(salida_lpf_s,salida_bpf_s,N,z_num_lpf_s,z_den_lpf_s,M_lpf,M_lpf);

        // Filtrado pasabajo rama superior
        double *salida_num_lpf_s = filter(salida_bpf_s_rectif, z_num_lpf_s, N, M_lpf, 0);
        double *salida_lpf_s = filter(salida_num_lpf_s, z_den_lpf_s, N, M_lpf, 1);
        // Filtrado pasabajo rama inferior
        double *salida_num_lpf_i = filter(salida_bpf_i_rectif, z_num_lpf_i, N, M_lpf, 0);
        double *salida_lpf_i = filter(salida_num_lpf_i, z_den_lpf_i, N, M_lpf, 1);

        for(int i=0; i<N;i++)
        {            
            y[i]=3*(salida_lpf_s[i]-salida_lpf_i[i]);
        }

//        //se normaliza la amplitud
//        //se determina la amplitud de la señal
//        double amplitud=0;
//        int counter=0;
//        for(int i=0; i<N-2; i++)
//        {
//            if(y[i]<y[i+1] && y[i+1]>=y[i+2])
//            {
//                    amplitud+=y[i+1];
//                    counter+=1;
//            }
//        }
//        amplitud=amplitud/counter;



//        for(int i=0; i<N;i++)
//        {
//            y[i]=2*signal_amp*y[i]/amplitud;
//        }


        free(z_num_lpf_s);
        free(z_den_lpf_s);
        free(z_num_lpf_i);
        free(z_den_lpf_i);
        free(salida_num_lpf_s);
        free(salida_num_lpf_i);
        free(salida_lpf_s);
        free(salida_lpf_i);
    }

    if(selection==2)//Kaiser
    {
        // Obtención de Beta y M para los filtros pasabajo de las dos ramas
        double delta_w_lpf, Beta_lpf, Aten_lpf, aten_lpf=0.2;

        delta_w_lpf = 0.1*((f1+f0)/2)*2*M_PI/fm;

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

        M_lpf = ceilf((Aten_lpf-8)/(2.285*delta_w_lpf));
        if(fmodf(M_lpf,2)!=0){
            M_lpf=M_lpf+1;
        }

        //validación
        if(M_lpf>N/10)
        {
            free(hn_bpf_s);
            free(hn_bpf_i);

            free(salida_bpf_s);
            free(salida_bpf_s_rectif);

            free(salida_bpf_i);
            free(salida_bpf_i_rectif);
            return 0;
        }

        // Filtro pasabajo 2/10*f0
        double wc1_lpf_s = 0.2*f0*2*M_PI/fm;
        // hn del pasabajo superior
        double *hn_lpf_s = filter_design_direct_I(M_lpf, Beta_lpf, wc1_lpf_s, 0);

        // Filtro pasabajo 2/10*f1
        double wc1_lpf_i = 0.2*f1*2*M_PI/fm;
        // hn del pasabanda inferior
        double *hn_lpf_i = filter_design_direct_I(M_lpf, Beta_lpf, wc1_lpf_i, 0);

        // Filtrado pasabajo rama superior
        double *salida_lpf_s = filter(salida_bpf_s_rectif, hn_lpf_s, N, M_lpf,0);
        // Filtrado pasabajo rama inferior
        double *salida_lpf_i = filter(salida_bpf_i_rectif, hn_lpf_i, N, M_lpf,0);

        for(int i=0; i<N;i++){
            y[i]=3*(salida_lpf_s[i]-salida_lpf_i[i]);
        }

        free(hn_lpf_s);
        free(hn_lpf_i);
        free(salida_lpf_s);
        free(salida_lpf_i);
    }


//    if(selection==7)//Programación Lineal
//    {
//        //Diseño del filtro
//        double delta_linprog = 0.08;
//        double wp_s = 2*M_PI*f0/fm/10;
//        double ws_s = wp_s + 2*M_PI*abs(f0-f1)/fm;
//        double *hn_lpf;

//        //obtención del orden
//        M_lpf=linprog_order(wp_s, ws_s);

//        //validación
//        if(M_lpf>N/10)
//        {
//            free(hn_bpf_s);
//            free(hn_bpf_i);

//            free(salida_bpf_s);
//            free(salida_bpf_s_rectif);

//            free(salida_bpf_i);
//            free(salida_bpf_i_rectif);
//            return 0;
//        }


//        //determinación de los coeficientes
//        //hn_lpf = new double[M_lpf];
//        linprog_coeff(wp_s, ws_s, delta_linprog,M_lpf,hn_lpf);


//        if(hn_lpf[0]==0)
//        {

//            delete[] hn_lpf;
//            free(hn_bpf_s);
//            free(hn_bpf_i);

//            free(salida_bpf_s);
//            free(salida_bpf_s_rectif);

//            free(salida_bpf_i);
//            free(salida_bpf_i_rectif);
//            return 0;
//        }

//        // Filtrado pasabajo rama superior
//        double *salida_lpf_s = NULL;
//        double *salida_lpf_i = NULL;
//        filter_IIR_FIR(salida_lpf_s,salida_bpf_s_rectif,N,hn_lpf,0,M_lpf,0);
//        filter_IIR_FIR(salida_lpf_i,salida_bpf_i_rectif,N,hn_lpf,0,M_lpf,0);


//        for(int i=0; i<N;i++)
//        {
//            y[i]=(salida_lpf_s[i]-salida_lpf_i[i]);
//        }


//        delete [] hn_lpf;
//        free(salida_lpf_s);
//        free(salida_lpf_i);

//    }

    // Liberar reserva de memoria
    free(hn_bpf_s);
    free(hn_bpf_i);

    free(salida_bpf_s);
    free(salida_bpf_s_rectif);

    free(salida_bpf_i);
    free(salida_bpf_i_rectif);
}

bool detector_envolvente_MFSK(double* &y, int* &symbols, double x [], double baud_rate, int *freq_array, int fft_length, double fm, int N, int selection,int M)
{

    //arreglo para guardar las múltiples salidas
    double **output;
    //se reserva espacio
    output = new double*[M];
    for(int i=0;i<M;i++)
    {
        output[i]=new double[N];
    }

    //Constantes de los filtros
    double delta_w_bpf, delta_w_lpf, Beta_bpf, Beta_lpf, Aten_bpf, aten_bpf=0.2;
    int M_bpf, M_lpf;

    delta_w_bpf = (double)0.1*baud_rate*2*M_PI/fm;
    delta_w_lpf=delta_w_bpf;


    // Constantes del filtrado pasa-banda
    Aten_bpf = -20*log10(aten_bpf);
    if(Aten_bpf > 50)
    {
        Beta_bpf = 0.1102*(Aten_bpf-8.7);
    }
    else if(Aten_bpf >= 21 && Aten_bpf <= 50)
    {
        Beta_bpf = 0.5842*pow((Aten_bpf-21),0.4)+0.07886*(Aten_bpf-21);
    }
    else if(Aten_bpf < 21)
    {
        Beta_bpf = 0;
    }

    Beta_lpf=Beta_bpf;
    M_bpf = ceilf((Aten_bpf-8)/(2.285*delta_w_bpf));
    M_lpf=M_bpf;

    if(fmodf(M_bpf,2)!=0)
    {
        M_bpf=M_bpf+1;
        M_lpf=M_bpf+1;
    }

    //validación
    if(M_bpf>N/10)//Si el total de coeficientes del filtro es muy grande
    {
        //se libera el arreglo output
        for(int i=0;i<M;i++)
        {
            delete output[i];
        }

        delete output;

        return 0;
    }

    //se determina el ancho de banda del filtro pasa-banda por el mínimo de la diferencia de frecuencia
    double W_bpf=freq_array[1]-freq_array[0];
    for(int i=1;i<M-1;i++)
    {
        if(W_bpf>(freq_array[i+1]-freq_array[i]))
           W_bpf=(freq_array[i+1]-freq_array[i]);
    }

//        M=1;
    //Procesamiento por las M ramas
    for(int z=0; z<M; z++)
    {

        // Diseño del Filtro Pasabanda
        double wc1_bpf_s = double(freq_array[z])/double(fft_length)*double(M_PI)-W_bpf/2/double(fft_length)*double(M_PI);
        double wc2_bpf_s = double(freq_array[z])/double(fft_length)*double(M_PI)+W_bpf/2/double(fft_length)*double(M_PI);

        // hn del pasabanda
        double *hn_bpf_s = filter_design_direct_I(M_bpf, Beta_bpf, wc1_bpf_s, wc2_bpf_s);

        // Diseño del filtro pasobajo
        double wc1_lpf_s = (4*baud_rate*2*M_PI/fm)-delta_w_lpf;
//        double wc1_lpf_s = W_bpf/2/double(fft_length)*double(M_PI);
        // hn del pasabajo
        double *hn_lpf_s = filter_design_direct_I(M_lpf, Beta_lpf, wc1_lpf_s, 0);

        // Filtrado pasa banda de la señal
        double *salida_bpf_s = filter(x, hn_bpf_s, N, M_bpf,0);

        // Rectificación de media onda
        double *salida_bpf_s_rectif = new double[N];
        for(int i=0; i<N; i++)
        {
            if(salida_bpf_s[i]<0)
            {
                salida_bpf_s_rectif[i]=0;
            }
            else
            {
                salida_bpf_s_rectif[i]=salida_bpf_s[i];
            }
        }

        // Filtrado pasabajo
        output[z] = filter(salida_bpf_s_rectif, hn_lpf_s, N, M_lpf,0);

        //se liberan espacios
        free(hn_lpf_s);
        free(hn_bpf_s);
        free(salida_bpf_s);

    }

    //se copia la salida y se establecen los símbolos
    for(int i=0;i<N;i++)
    {
        int k=0;//almacena el símbolo de mayor duración
        double output_max=0;//almacena la mayor salida
        for(int z=0;z<M;z++)
        {            
            y[i]+=(z+1)*output[z][i];

            if(output[z][i]>output_max)
            {
                k=z;
                output_max=output[z][i];
            }
        }
        symbols[i]=k;

    }



    //se libera el arreglo output
    for(int i=0;i<M;i++)
    {
        delete output[i];
    }

    delete output;


}


