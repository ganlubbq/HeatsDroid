#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>//para abrir ventanas de diálogo
#include<QDebug>
#include <qmath.h>
#include <QErrorMessage>

int u=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //General
    //Tiempo
    gen_graph_tiempo = new graph_1D(this);//se reserva espacio para la clase    
    ui->gen_graph_tiempo_GL->addWidget(gen_graph_tiempo);//se pone el dibujo en el grid layout correspondiente
    gen_tiempo_despH_habilitar=false;
    gen_tiempo_centerH=0;
    gen_tiempo_windowH=0;
    gen_tiempo_despV_habilitar=false;
    gen_tiempo_centerV=0;
    gen_tiempo_windowV=0;
    ui->gen_tiempo_despH_SB->setRange(0,0);
    ui->gen_tiempo_despV_SB->setRange(0,0);
    ui->gen_spect_despH_SB->setRange(0,0);
    ui->gen_spect_despV_SB->setRange(0,0);
    ui->gen_tiempo_zoomH_SL->setDisabled(true);
    ui->gen_tiempo_zoomV_SL->setDisabled(true);
    ui->gen_spect_zoomH_SL->setDisabled(true);
    ui->gen_spect_zoomV_SL->setDisabled(true);
    ui->gen_inf_GB->setDisabled(true);

    //Spectrogram
    gen_graph_spect = new graph_SPECT(this);//se reserva espacio para la clase
    ui->gen_graph_spect_GL->addWidget(gen_graph_spect);//se pone el dibujo en el grid layout correspondiente
    gen_spect_despH_habilitar=false;
    gen_spect_despV_habilitar=false;
    gen_spect_centerH=0;
    gen_spect_windowH=0;
    gen_spect_centerV=0;
    gen_spect_windowV=0;
    //Parámetros
    ui->spect_window_length_LE->setReadOnly(true);
    ui->gen_spect_GB->setEnabled(false);

    //Detect
    //Frecuencia
    //detect_graph_fft = new graph_1D(this);//se reserva espacio para la clase
    //ui->detect_graph_fft_GL->addWidget(detect_graph_fft);//se pone el dibujo en el grid layout correspondiente
    detect_graph_fft_scene = new QGraphicsScene;
    ui->detect_graph_fft_GV->setScene(detect_graph_fft_scene);
    detect_graph_fft_scene->setBackgroundBrush(Qt::black);
    detect_fft_despH_habilitar=false;
    detect_fft_centerH=0;
    detect_fft_windowH=0;
    detect_fft_despV_habilitar=false;
    detect_signals_CB_hab=false;
    detect_fft_centerV=0;
    detect_fft_windowV=0;
    //Panel de control   
    ui->clasif_mod_GB->setDisabled(true);
    ui->detect_SS_GB->setDisabled(true);
    ui->detect_filtro_GB->setDisabled(true);

    detect_f0_hab=false;
    detect_f1_hab=false;
    detect_f0_LE_hab=false;
    detect_f1_LE_hab=false;
    detect_PB_hab=true;
    //Spectrogram
    detect_graph_spect = new graph_SPECT(this);//se reserva espacio para la clase
    ui->detect_graph_spect_VL->addWidget(detect_graph_spect);//se pone el dibujo en el grid layout correspondiente
    detect_spect_despH_habilitar=false;
    detect_spect_despV_habilitar=false;
    detect_spect_centerH=0;
    detect_spect_windowH=0;
    detect_spect_centerV=0;
    detect_spect_windowV=0;
    //se habilitan controles
    ui->detect_fft_zoomH_SL->setDisabled(true);
    ui->detect_fft_zoomV_SL->setDisabled(true);
    ui->detect_spect_zoomH_SL->setDisabled(true);
    ui->detect_spect_zoomV_SL->setDisabled(true);

    //Estimación
    //Frecuencia
    clasif_graph_fft = new graph_1D(this);//se reserva espacio para la clase    
    ui->clasif_graph_fft_GL->addWidget(clasif_graph_fft);//se pone el dibujo en el grid layout correspondiente
    clasif_fft_despH_habilitar=false;
    clasif_fft_centerH=0;
    clasif_fft_windowH=0;
    clasif_fft_despV_habilitar=false;
    clasif_fft_centerV=0;
    clasif_fft_windowV=0;
    //Panel de Control
    ui->clasif_param_GB->setDisabled(true);
    clasif_resultados_habilitar=false;
    clasif_modu_habilitar=false;
    //Constelación
    clasif_graph_const = new graph_1D(this);
    clasif_graph_const->pintar_coord_hab=false;
    ui->clasif_graph_const_GL->addWidget(clasif_graph_const);//se pone el dibujo en el grid layout correspondiente
    clasif_const_centerH=0;
    clasif_const_windowH=0;
    clasif__const_despH_habilitar=false;
    clasif_const_centerV=0;
    clasif_const_windowV=0;
    clasif__const_despV_habilitar=false;
    clasif_const_hab=false;
    ui->clasif_mod_GB->setDisabled(true);
    clasif_const_lines_hab=false;

    //Spectrogram
    clasif_graph_spect = new graph_SPECT(this);//se reserva espacio para la clase    
    ui->clasif_graph_spect_GL->addWidget(clasif_graph_spect);//se pone el dibujo en el grid layout correspondiente
    clasif_spect_despH_habilitar=false;
    clasif_spect_despV_habilitar=false;
    clasif_spect_centerH=0;
    clasif_spect_windowH=0;
    clasif_spect_centerV=0;
    clasif_spect_windowV=0;
    //se deshabilitan controles
    ui->clasif_fft_despH_SB->setRange(0,0);
    ui->clasif_fft_despV_SB->setRange(0,0);
    ui->clasif_const_despH_SB->setRange(0,0);
    ui->clasif_const_despV_SB->setRange(0,0);
    ui->clasif_spect_despH_SB->setRange(0,0);
    ui->clasif_spect_despV_SB->setRange(0,0);
    ui->clasif_fft_zoomH_SL->setDisabled(true);
    ui->clasif_fft_zoomV_SL->setDisabled(true);
    ui->clasif_const_zoomH_SL->setDisabled(true);
    ui->clasif_const_zoomV_SL->setDisabled(true);
    ui->clasif_spect_zoomH_SL->setDisabled(true);
    ui->clasif_spect_zoomV_SL->setDisabled(true);

    //Demodular
    //Tiempo
    dem_graph_tiempo = new graph_1D(this);//se reserva espacio para la clase    
    ui->dem_graph_tiempo_GL->addWidget(dem_graph_tiempo);//se pone el dibujo en el grid layout correspondiente
    dem_tiempo_despH_habilitar=false;
    dem_tiempo_centerH=0;
    dem_tiempo_windowH=0;
    dem_tiempo_despV_habilitar=false;
    dem_tiempo_centerV=0;
    dem_tiempo_windowV=0;    
    ui->dem_L->setDisabled(true);
    dem_PB_habilitar=true;


    //Frecuencia
    dem_graph_fft = new graph_1D(this);//se reserva espacio para la clase
    ui->dem_graph_fft_GL->addWidget(dem_graph_fft);//se pone el dibujo en el grid layout correspondiente
    dem_fft_despH_habilitar=false;
    dem_fft_centerH=0;
    dem_fft_windowH=0;
    dem_fft_despV_habilitar=false;
    dem_fft_centerV=0;
    dem_fft_windowV=0;
    dem_f0_SB_hab=false;
    dem_freq_CB_hab=false;

    //Raster
    dem_graph_raster = new graph_raster(this);
    ui->dem_raster_GL->addWidget(dem_graph_raster);
    dem_raster_despH_range=0;
    dem_raster_despH_habilitar=0;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
    dem_raster_centerH=0;//almacena el centro de la ventana que se visualiza
    dem_raster_windowH=0;//almacena el ancho de la ventana que da el zoom horizontal
    dem_raster_despV_range=0;
    dem_raster_despV_habilitar=0;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
    dem_raster_centerV=0;//almacena el centro de la ventana que se visualiza verticalmente
    dem_raster_windowV=0;//almacena el ancho de la ventana que da el zoom vertical
    //se deshabilitan controles
    ui->dem_tiempo_despH_SB->setRange(0,0);
    ui->dem_tiempo_despV_SB->setRange(0,0);
    ui->dem_fft_despH_SB->setRange(0,0);
    ui->dem_fft_despV_SB->setRange(0,0);
    ui->dem_raster_despH_SB->setRange(0,0);
    ui->dem_raster_despV_SB->setRange(0,0);
    ui->dem_tiempo_zoomH_SL->setDisabled(true);
    ui->dem_tiempo_zoomV_SL->setDisabled(true);
    ui->dem_fft_zoomH_SL->setDisabled(true);
    ui->dem_fft_zoomV_SL->setDisabled(true);
    ui->dem_raster_zoomH_SL->setDisabled(true);
    ui->dem_raster_zoomV_SL->setDisabled(true);

    ///Panel de control
    //parámetros
    //ui->param_baud_LE->setValidator(new QDoubleValidator(1.0, 1000.0, 4, ui->param_baud_LE));
    //clasificación

    //Demodulación
    ui->dem_metodos_CB->setDisabled(true);
    ui->dem_PB->setDisabled(true);
    ui->dem_freq_GB->setDisabled(true);//se esconde el panel de frecuencia
    ui->dem_raster_GB->setDisabled(true);
    ui->dem_exportar_PB->setDisabled(true);
    dem_patrones_CB_hab=false;
    dem_cod_CB_hab=false;
    //se vacía el comboBox de los métodos
    if(ui->dem_raster_CB->count()!=0)
    {
        int total=ui->dem_raster_CB->count();
        for(int i=1;i<=total;i++)
        {
            ui->dem_raster_CB->removeItem(total-i);
        }
    }

    ui->dem_freq_CB->setVisible(false);
    ui->dem_freq_L->setVisible(false);
    ui->dem_freq_L->setGeometry(10,30,61,16);
    ui->dem_freq_CB->setGeometry(70,30,101,22);
    ui->dem_cod_L->setVisible(false);
    ui->dem_cod_CB->setVisible(false);

    //se colocan los tabs de visualización y parámetros en los valores iniciales
    ui->tabWidget->setCurrentIndex(0);
    ui->toolBox->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{

    delete ui;
}



void MainWindow::on_actionSalir_triggered()
{

    close();
}


void MainWindow::on_actionAbrir_triggered()
{
   information(0,0,0);


   //se abre la ventana para la búsqueda de la señal
   QString files = QFileDialog::getOpenFileName(this, tr("Abrir archivo"), ".", tr("Archivos de audio (*.wav)"));

   if(files.isNull())
   {

       information(0,0,5);
       return;
   }

    if (files.isEmpty())
    {        

        information(0,0,3);
        return;
    }
   QFile file(files);
   if (!file.open(QIODevice::ReadOnly))
   {

       information(0,0,4);
       return;
   }

   //se manda a procesar el archivo en la clase nucleo(se extraen los datos, se calcula la FFT)
   bool out=nucleo.nucleo_comenzar_funct(file);

   if(out)
   {       
       ui->param_signal_name->setText("Archivo: "+ files.section('/', -1));

       //se manda a calcular la FFT y el primer máximo
       nucleo.nucleo_fft_funct();

       //se calcula el espectrograma
       nucleo.spect_window=0;
       nucleo.spect_window_length=1024;
       int step=1;
       if(nucleo.signal_length>400)
           step=double(nucleo.signal_length)/double(400);
       nucleo.spect_window_solapamiento=double(step)*double(100)/double(nucleo.spect_window_length);
       nucleo.nucleo_spect_funct();

       //se estiman los parámetros
       nucleo.nucleo_estimacion_param();
       this->restart_graphics(0);//se completan las funcionalidades visuales
       //se pasa a la pestaña general
       ui->tabWidget->setCurrentIndex(0);
       ui->toolBox->setCurrentIndex(0);
    }


   else
   {       
       restart_graphics(-1);       
       //se pasa a la pestaña general
       ui->tabWidget->setCurrentIndex(0);
       information(0,0,6);
   }

   file.close();//se cierra el archivo

}


void MainWindow::on_gen_tiempo_zoomH_SL_valueChanged(int value)
{
    float range=ui->gen_tiempo_zoomH_SL->maximum();
    int center=gen_tiempo_centerH;
    float window=0;
    //obtención de la ventana de zoom con ley trigonométrica
    if(value>range/2)
    {
       window=nucleo.signal_length*(1-qSin(value/range*M_PI/2));
    }
    else
    {
        window=nucleo.signal_length*(range-value+1)/(range+1)/2;
    }

    gen_tiempo_windowH=window;

    //aplicación del zoom
    if(center-window<0)
    {
        gen_graph_tiempo->x_p1=0;
    }
    else
    {
        gen_graph_tiempo->x_p1=center-window;
    }

    if(center+window>nucleo.signal_length-1)
    {
        gen_graph_tiempo->x_p2=nucleo.signal_length;
    }
    else
    {
        gen_graph_tiempo->x_p2=center+window;
    }

    //se modifica el range del desplazamiento horizontal en función del total de puntos a desplazar
    if(value!=0)
    {

        int L=(gen_graph_tiempo->x_p1+nucleo.signal_length-gen_graph_tiempo->x_p2)/100;
        if(L==0)
        {
            L=1;
        }
        gen_tiempo_despH_range=L;
        ui->gen_tiempo_despH_SB->setRange(0,L);
        //se recoloca el valor del despH
        gen_tiempo_despH_habilitar=false;
        ui->gen_tiempo_despH_SB->setValue(float(center)*float(L)/float(nucleo.signal_length));
        gen_tiempo_despH_habilitar=true;
    }

    else
    {
        gen_tiempo_despH_range=0;
        ui->gen_tiempo_despH_SB->setRange(0,0);
        gen_graph_tiempo->x_p1=0;
        gen_graph_tiempo->x_p2=nucleo.signal_length-1;

        //se colocar en el centro el desplazamiento horizontal
        gen_tiempo_despH_habilitar=false;
        gen_tiempo_centerH=nucleo.signal_length/2;
        ui->gen_tiempo_despH_SB->setValue(nucleo.signal_length/2);
        gen_tiempo_despH_habilitar=true;

    }
    gen_graph_tiempo->update();//redibuja la señal
}

void MainWindow::on_gen_tiempo_despH_SB_valueChanged(int value)
{
    if(gen_tiempo_despH_habilitar && gen_tiempo_despH_range!=0)
    {

        double range=gen_tiempo_despH_range;
        double center=double(nucleo.signal_length)*double(value)/range;
        double window=gen_tiempo_windowH;

        gen_tiempo_centerH=center;



        if(center-window<0)
        {
            gen_graph_tiempo->x_p1=0;
        }
        else
        {
            gen_graph_tiempo->x_p1=center-window;
        }

        if(center+window>nucleo.signal_length-1)
        {
            gen_graph_tiempo->x_p2=nucleo.signal_length;
        }
        else
        {
            gen_graph_tiempo->x_p2=center+window;
        }



        gen_graph_tiempo->update();//redibuja la señal
    }
}



void MainWindow::on_gen_tiempo_zoomV_SL_valueChanged(int value)
{
    float range=ui->gen_tiempo_zoomV_SL->maximum();
    float center=gen_tiempo_centerV;
    float window=0;
    float max_heigthV=nucleo.signal_max-nucleo.signal_min;

    //obtención de la ventana de zoom con ley trigonométrica
    if(value>range/2)
    {
       window=max_heigthV*(1-qSin(value/range*M_PI/2));
    }
    else
    {
        window=max_heigthV*(range-value+1)/(range+1)/2;

    }

    gen_tiempo_windowV=window;

    //aplicación del zoom
    if(center-window<nucleo.signal_min)
    {
        gen_graph_tiempo->y_p1=(nucleo.signal_min)*2;
    }
    else
    {
        gen_graph_tiempo->y_p1=(center-window)*2;
    }

    if(center+window>nucleo.signal_max)
    {
        gen_graph_tiempo->y_p2=(nucleo.signal_max)*2;
    }
    else
    {
        gen_graph_tiempo->y_p2=(center+window)*2;
    }

    //se modifica el range del desplazamiento vertical en función del total de puntos a desplazar
    if(value!=0)
    {
        gen_tiempo_despV_range=100;
        ui->gen_tiempo_despV_SB->setRange(-gen_tiempo_despV_range,gen_tiempo_despV_range);
        //se recoloca el valor del despV
        gen_tiempo_despV_habilitar=false;
        ui->gen_tiempo_despV_SB->setValue(-double(center)*double(gen_tiempo_despV_range)/double(max_heigthV));
        gen_tiempo_despV_habilitar=true;
    }

    else
    {
        gen_tiempo_despV_range=0;
        ui->gen_tiempo_despV_SB->setRange(0,0);
        gen_graph_tiempo->y_p1=nucleo.signal_min*2;
        gen_graph_tiempo->y_p2=nucleo.signal_max*2;

        //se coloca en el centro el desplazamiento vertical
        gen_tiempo_despV_habilitar=false;
        gen_tiempo_centerV=(nucleo.signal_max+nucleo.signal_min);
        ui->gen_tiempo_despV_SB->setValue((nucleo.signal_max+nucleo.signal_min));
        gen_tiempo_despV_habilitar=true;
    }

     gen_graph_tiempo->update();//redibuja la señal
}


void MainWindow::on_gen_tiempo_despV_SB_valueChanged(int value)
{

    if(gen_tiempo_despV_habilitar && gen_tiempo_despV_range!=0)
    {
        value=-value;
        float range=gen_tiempo_despV_range;
        float center=(nucleo.signal_max-nucleo.signal_min)*value/range;
        float window=gen_tiempo_windowV;

        gen_tiempo_centerV=center;

        gen_graph_tiempo->y_p1=(center-window)*2;
        gen_graph_tiempo->y_p2=(center+window)*2;
        gen_graph_tiempo->update();//redibuja la señal
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    ui->toolBox->setCurrentIndex(index);
    information(0,0,0);
}

void MainWindow::on_clasif_PB_clicked()
{

    int selec=ui->clasif_metodos_CB->currentIndex();

    //se reinicia el clasificador
    ui->clasif_resultado_CB->setCurrentIndex(0);

    nucleo.nucleo_clasif_funct(selec);//se manda a clasificar

    //se completa la clasificación en la interfaz visual
    clasif_modu_habilitar=true;
    //se deshabilitan los controles
    ui->clasif_L2->setDisabled(true);
    ui->clasif_L3->setDisabled(true);
    ui->clasif_const_CB->setDisabled(true);
    ui->clasif_resultado_CB->setDisabled(true);
    clasif_const_lines_hab=false;
    ui->clasif_const_lines->setChecked(false);
    clasif_graph_const->const_lines=0;
    clasif_const_lines_hab=true;

    ui->clasif_resultado_CB->setCurrentIndex(nucleo.clasif_decision);
    ui->clasif_L2->setDisabled(false);
    ui->clasif_resultado_CB->setDisabled(false);

}

void MainWindow::on_detect_fft_zoomH_SL_valueChanged(int value)
{
    qreal scalex =float(5-1)/float(100-1)*float(value-1)+1;
    qreal scaley =float(5-1)/float(100-1)*float(ui->detect_fft_zoomV_SL->value()-1)+1;
    QMatrix matrix;
    matrix.scale(scalex, scaley);
    //matrix.rotate(rotateSlider->value());
    ui->detect_graph_fft_GV->setMatrix(matrix);
}


void MainWindow::on_detect_fft_zoomV_SL_valueChanged(int value)
{
    qreal scalex =float(5-1)/float(100-1)*float(ui->detect_fft_zoomH_SL->value()-1)+1;
    qreal scaley =float(5-1)/float(100-1)*float(value-1)+1;
    QMatrix matrix;
    matrix.scale(scalex,scaley);
    //matrix.rotate(rotateSlider->value());
    ui->detect_graph_fft_GV->setMatrix(matrix);
}

void MainWindow::on_clasif_fft_zoomH_SL_valueChanged(int value)
{
    float range=ui->clasif_fft_zoomH_SL->maximum();
    int center=clasif_fft_centerH;
    float window=0;
    //obtención de la ventana de zoom con ley trigonométrica
    if(value>range/2)
    {
       window=nucleo.signal_fft_length*(1-qSin(value/range*M_PI/2));
    }
    else
    {
        window=nucleo.signal_fft_length*(range-value+1)/(range+1)/2;
    }

    clasif_fft_windowH=window;

    //aplicación del zoom
    if(center-window<0)
    {
        clasif_graph_fft->x_p1=0;
    }
    else
    {
        clasif_graph_fft->x_p1=center-window;
    }

    if(center+window>nucleo.signal_fft_length-1)
    {
        clasif_graph_fft->x_p2=nucleo.signal_length;
    }
    else
    {
        clasif_graph_fft->x_p2=center+window;
    }

    //se modifica el range del desplazamiento horizontal en función del total de puntos a desplazar
    if(value!=0)
    {

        int L=(clasif_graph_fft->x_p1+nucleo.signal_fft_length-clasif_graph_fft->x_p2)/100;
        if(L==0)
        {
            L=1;
        }
        clasif_fft_despH_range=L;
        ui->clasif_fft_despH_SB->setRange(0,L);
        //se recoloca el valor del despH
        clasif_fft_despH_habilitar=false;
        ui->clasif_fft_despH_SB->setValue(float(center)*float(L)/float(nucleo.signal_fft_length));
        clasif_fft_despH_habilitar=true;
    }

    else
    {
        clasif_fft_despH_range=0;
        ui->clasif_fft_despH_SB->setRange(0,0);
        clasif_graph_fft->x_p1=0;
        clasif_graph_fft->x_p2=nucleo.signal_fft_length-1;

        //se colocar en el centro el desplazamiento horizontal
        clasif_fft_despH_habilitar=false;
        clasif_fft_centerH=nucleo.signal_fft_length/2;
        ui->clasif_fft_despH_SB->setValue(nucleo.signal_fft_length/2);
        clasif_fft_despH_habilitar=true;

    }
    clasif_graph_fft->update();//redibuja la señal
}

void MainWindow::on_clasif_fft_despH_SB_valueChanged(int value)
{
    if(clasif_fft_despH_habilitar && clasif_fft_despH_range!=0)
    {

        double range=clasif_fft_despH_range;
        double center=double(nucleo.signal_fft_length)*double(value)/range;
        double window=clasif_fft_windowH;

        clasif_fft_centerH=center;

        if(center-window<0)
        {
            clasif_graph_fft->x_p1=0;
        }
        else
        {
            clasif_graph_fft->x_p1=center-window;
        }

        if(center+window>nucleo.signal_fft_length-1)
        {
            clasif_graph_fft->x_p2=nucleo.signal_fft_length;
        }
        else
        {
            clasif_graph_fft->x_p2=center+window;
        }
        clasif_graph_fft->update();//redibuja la señal
    }
}

void MainWindow::on_clasif_fft_zoomV_SL_valueChanged(int value)
{
    float range=ui->clasif_fft_zoomV_SL->maximum();
    float center=clasif_fft_centerV;
    float window=0;
    float max_heigthV=nucleo.signal_fft_max-nucleo.signal_fft_min;

    //obtención de la ventana de zoom con ley trigonométrica
    if(value>range/2)
    {
       window=max_heigthV*(1-qSin(value/range*M_PI/2));
    }
    else
    {
        window=max_heigthV*(range-value+1)/(range+1)/2;
    }

    clasif_fft_windowV=window;

    //aplicación del zoom
    if(center<nucleo.signal_fft_min)
    {
        clasif_graph_fft->y_p1=nucleo.signal_fft_min;
    }
    else
    {
        clasif_graph_fft->y_p1=center;
    }

    if(center+window>nucleo.signal_fft_max)
    {
        clasif_graph_fft->y_p2=nucleo.signal_fft_max;
    }
    else
    {
        clasif_graph_fft->y_p2=center+window;
    }

    //se modifica el range del desplazamiento vertical en función del total de puntos a desplazar
    if(value!=0)
    {
        clasif_fft_despV_range=100;
        ui->clasif_fft_despV_SB->setRange(-clasif_fft_despV_range,0);
    }

    else
    {
        clasif_fft_despV_range=0;
        ui->clasif_fft_despV_SB->setRange(0,0);
        clasif_graph_fft->y_p1=nucleo.signal_fft_min;
        clasif_graph_fft->y_p2=nucleo.signal_fft_max;

        //se colocar en el centro el desplazamiento vertical
        clasif_fft_despV_habilitar=false;
        clasif_fft_centerV=nucleo.signal_fft_min;
        ui->clasif_fft_despV_SB->setValue(-clasif_fft_despV_range);
        clasif_fft_despV_habilitar=true;
    }

     clasif_fft_despV_habilitar=true;
     clasif_graph_fft->update();//redibuja la señal
}

void MainWindow::on_clasif_fft_despV_SB_valueChanged(int value)
{
    if(clasif_fft_despV_habilitar && clasif_fft_despV_range!=0)
    {


        value=-value;//-ui->clasif_fft_despV_SB->minimum();
        float range=clasif_fft_despV_range;
        float center=(nucleo.signal_fft_max-nucleo.signal_fft_min)*value/range;
        float window=clasif_fft_windowV;

        clasif_fft_centerV=center;

        clasif_graph_fft->y_p1=center;
        clasif_graph_fft->y_p2=center+window;
        clasif_graph_fft->update();//redibuja la señal
    }
}

void MainWindow::on_dem_fft_zoomH_SL_valueChanged(int value)
{
    float range=ui->dem_fft_zoomH_SL->maximum();
    int center=dem_fft_centerH;
    float window=0;
    //obtención de la ventana de zoom con ley trigonométrica
    if(value>range/2)
    {
       window=nucleo.signal_fft_length*(1-qSin(value/range*M_PI/2));
    }
    else
    {
        window=nucleo.signal_fft_length*(range-value+1)/(range+1)/2;
    }

    dem_fft_windowH=window;

    //aplicación del zoom
    if(center-window<0)
    {
        dem_graph_fft->x_p1=0;
    }
    else
    {
        dem_graph_fft->x_p1=center-window;
    }

    if(center+window>nucleo.signal_fft_length-1)
    {
        dem_graph_fft->x_p2=nucleo.signal_length;
    }
    else
    {
        dem_graph_fft->x_p2=center+window;
    }

    //se modifica el range del desplazamiento horizontal en función del total de puntos a desplazar
    if(value!=0)
    {

        int L=(dem_graph_fft->x_p1+nucleo.signal_fft_length-dem_graph_fft->x_p2)/100;
        if(L==0)
        {
            L=1;
        }
        dem_fft_despH_range=L;
        ui->dem_fft_despH_SB->setRange(0,L);
        //se recoloca el valor del despH
        dem_fft_despH_habilitar=false;
        ui->dem_fft_despH_SB->setValue(float(center)*float(L)/float(nucleo.signal_fft_length));
        dem_fft_despH_habilitar=true;
    }

    else
    {
        dem_fft_despH_range=0;
        ui->dem_fft_despH_SB->setRange(0,0);
        dem_graph_fft->x_p1=0;
        dem_graph_fft->x_p2=nucleo.signal_fft_length-1;

        //se colocar en el centro el desplazamiento horizontal
        dem_fft_despH_habilitar=false;
        dem_fft_centerH=nucleo.signal_fft_length/2;
        ui->dem_fft_despH_SB->setValue(nucleo.signal_fft_length/2);
        dem_fft_despH_habilitar=true;

    }
    dem_graph_fft->update();//redibuja la señal
}

void MainWindow::on_dem_fft_despH_SB_valueChanged(int value)
{
    if(dem_fft_despH_habilitar && dem_fft_despH_range!=0)
    {

        double range=dem_fft_despH_range;
        double center=double(nucleo.signal_fft_length)*double(value)/range;
        double window=dem_fft_windowH;

        dem_fft_centerH=center;

        if(center-window<0)
        {
            dem_graph_fft->x_p1=0;
        }
        else
        {
            dem_graph_fft->x_p1=center-window;
        }

        if(center+window>nucleo.signal_fft_length-1)
        {
            dem_graph_fft->x_p2=nucleo.signal_fft_length;
        }
        else
        {
            dem_graph_fft->x_p2=center+window;
        }
        dem_graph_fft->update();//redibuja la señal
    }
}

void MainWindow::on_dem_fft_zoomV_SL_valueChanged(int value)
{
    float range=ui->dem_fft_zoomV_SL->maximum();
    float center=dem_fft_centerV;
    float window=0;
    float max_heigthV=nucleo.signal_fft_max-nucleo.signal_fft_min;

    //obtención de la ventana de zoom con ley trigonométrica
    if(value>range/2)
    {
       window=max_heigthV*(1-qSin(value/range*M_PI/2));
    }
    else
    {
        window=max_heigthV*(range-value+1)/(range+1)/2;
    }

    dem_fft_windowV=window;

    //aplicación del zoom
    if(center<nucleo.signal_fft_min)
    {
        dem_graph_fft->y_p1=nucleo.signal_fft_min;
    }
    else
    {
        dem_graph_fft->y_p1=center;
    }

    if(center+window>nucleo.signal_fft_max)
    {
        dem_graph_fft->y_p2=nucleo.signal_fft_max;
    }
    else
    {
        dem_graph_fft->y_p2=center+window;
    }

    //se modifica el range del desplazamiento vertical en función del total de puntos a desplazar
    if(value!=0)
    {
        dem_fft_despV_range=100;
        ui->dem_fft_despV_SB->setRange(-dem_fft_despV_range,0);
    }

    else
    {
        dem_fft_despV_range=0;
        ui->dem_fft_despV_SB->setRange(0,0);
        dem_graph_fft->y_p1=nucleo.signal_fft_min;
        dem_graph_fft->y_p2=nucleo.signal_fft_max;

        //se colocar en el centro el desplazamiento vertical
        dem_fft_despV_habilitar=false;
        dem_fft_centerV=nucleo.signal_fft_min;
        ui->dem_fft_despV_SB->setValue(-dem_fft_despV_range);
        dem_fft_despV_habilitar=true;
    }

     dem_fft_despV_habilitar=true;
     dem_graph_fft->update();//redibuja la señal
}

void MainWindow::on_dem_fft_despV_SB_valueChanged(int value)
{
    if(dem_fft_despV_habilitar && dem_fft_despV_range!=0)
    {
        value=-value;//-ui->dem_fft_despV_SB->minimum();
        float range=dem_fft_despV_range;
        float center=(nucleo.signal_fft_max-nucleo.signal_fft_min)*value/range;
        float window=dem_fft_windowV;

        dem_fft_centerV=center;

        dem_graph_fft->y_p1=center;
        dem_graph_fft->y_p2=center+window;
        dem_graph_fft->update();//redibuja la señal
    }
}

void MainWindow::on_dem_tiempo_zoomH_SL_valueChanged(int value)
{
    float range=ui->dem_tiempo_zoomH_SL->maximum();
    int center=dem_tiempo_centerH;
    float window=0;
    //obtención de la ventana de zoom con ley trigonométrica
    if(value>range/2)
    {
       window=nucleo.signal_length*(1-qSin(value/range*M_PI/2));
    }
    else
    {
        window=nucleo.signal_length*(range-value+1)/(range+1)/2;
    }

    dem_tiempo_windowH=window;

    //aplicación del zoom
    if(center-window<0)
    {
        dem_graph_tiempo->x_p1=0;
    }
    else
    {
        dem_graph_tiempo->x_p1=center-window;
    }

    if(center+window>nucleo.signal_length-1)
    {
        dem_graph_tiempo->x_p2=nucleo.signal_length;
    }
    else
    {
        dem_graph_tiempo->x_p2=center+window;
    }

    //se modifica el range del desplazamiento horizontal en función del total de puntos a desplazar
    if(value!=0)
    {

        int L=(dem_graph_tiempo->x_p1+nucleo.signal_length-dem_graph_tiempo->x_p2)/100;
        if(L==0)
        {
            L=1;
        }
        dem_tiempo_despH_range=L;
        ui->dem_tiempo_despH_SB->setRange(0,L);
        //se recoloca el valor del despH
        dem_tiempo_despH_habilitar=false;
        ui->dem_tiempo_despH_SB->setValue(float(center)*float(L)/float(nucleo.signal_length));
        dem_tiempo_despH_habilitar=true;
    }

    else
    {
        dem_tiempo_despH_range=0;
        ui->dem_tiempo_despH_SB->setRange(0,0);
        dem_graph_tiempo->x_p1=0;
        dem_graph_tiempo->x_p2=nucleo.signal_length-1;

        //se colocar en el centro el desplazamiento horizontal
        dem_tiempo_despH_habilitar=false;
        dem_tiempo_centerH=nucleo.signal_length/2;
        ui->dem_tiempo_despH_SB->setValue(nucleo.signal_length/2);
        dem_tiempo_despH_habilitar=true;

    }
    dem_graph_tiempo->update();//redibuja la señal
}

void MainWindow::on_dem_tiempo_despH_SB_valueChanged(int value)
{
    if(dem_tiempo_despH_habilitar && dem_tiempo_despH_range!=0)
    {

        double range=dem_tiempo_despH_range;
        double center=double(nucleo.signal_length)*double(value)/range;
        double window=dem_tiempo_windowH;

        dem_tiempo_centerH=center;

        if(center-window<0)
        {
            dem_graph_tiempo->x_p1=0;
        }
        else
        {
            dem_graph_tiempo->x_p1=center-window;
        }

        if(center+window>nucleo.signal_length-1)
        {
            dem_graph_tiempo->x_p2=nucleo.signal_length;
        }
        else
        {
            dem_graph_tiempo->x_p2=center+window;
        }



        dem_graph_tiempo->update();//redibuja la señal
    }
}

void MainWindow::on_dem_tiempo_zoomV_SL_valueChanged(int value)
{
    float signal_min=0;
    float signal_max=0;
    if(nucleo.dem_amp_max>10*nucleo.signal_amp || qAbs(nucleo.dem_amp_min)>10*nucleo.signal_amp)
    {
        signal_min=nucleo.signal_min;
        signal_max=nucleo.signal_max;
    }
    else
    {
        signal_min=qMin(nucleo.signal_min,float(nucleo.dem_amp_min));
        signal_max=qMax(nucleo.signal_max,float(nucleo.dem_amp_max));
    }
    float range=ui->dem_tiempo_zoomV_SL->maximum();
    float center=dem_tiempo_centerV;
    float window=0;
    float max_heigthV=signal_max-signal_min;


    //obtención de la ventana de zoom con ley trigonométrica
    if(value>range/2)
    {
       window=max_heigthV*(1-qSin(value/range*M_PI/2));
    }
    else
    {
        window=max_heigthV*(range-value+1)/(range+1)/2;
    }

    dem_tiempo_windowV=window;

    //aplicación del zoom
    if(center-window<signal_min)
    {
        dem_graph_tiempo->y_p1=(signal_min)*2;
    }
    else
    {
        dem_graph_tiempo->y_p1=(center-window)*2;
    }

    if(center+window>signal_max)
    {
        dem_graph_tiempo->y_p2=(signal_max)*2;
    }
    else
    {
        dem_graph_tiempo->y_p2=(center+window)*2;
    }

    //se modifica el range del desplazamiento vertical en función del total de puntos a desplazar
    if(value!=0)
    {
        dem_tiempo_despV_range=100;
        ui->dem_tiempo_despV_SB->setRange(-dem_tiempo_despV_range,dem_tiempo_despV_range);
        //se recoloca el valor del despV
        dem_tiempo_despV_habilitar=false;
        ui->dem_tiempo_despV_SB->setValue(-double(center)*double(dem_tiempo_despV_range)/double(max_heigthV));
        dem_tiempo_despV_habilitar=true;
    }

    else
    {
        dem_tiempo_despV_range=0;
        ui->dem_tiempo_despV_SB->setRange(0,0);
        dem_graph_tiempo->y_p1=signal_min*2;
        dem_graph_tiempo->y_p2=signal_max*2;

        //se colocar en el centro el desplazamiento vertical
        dem_tiempo_despV_habilitar=false;
        dem_tiempo_centerV=(signal_max+signal_min);
        ui->dem_tiempo_despV_SB->setValue((signal_max+signal_min));
        dem_tiempo_despV_habilitar=true;
    }

     dem_graph_tiempo->update();//redibuja la señal
}

void MainWindow::on_dem_tiempo_despV_SB_valueChanged(int value)
{
    if(dem_tiempo_despV_habilitar && dem_tiempo_despV_range!=0)
    {
        float signal_min=qMin(nucleo.signal_min,float(nucleo.dem_amp_min));
        float signal_max=qMax(nucleo.signal_max,float(nucleo.dem_amp_max));

        value=-value;
        float range=dem_tiempo_despV_range;
        float center=(signal_max-signal_min)*value/range;
        float window=dem_tiempo_windowV;

        dem_tiempo_centerV=center;

        dem_graph_tiempo->y_p1=(center-window)*2;
        dem_graph_tiempo->y_p2=(center+window)*2;
        dem_graph_tiempo->update();//redibuja la señal
    }
}

void MainWindow::on_gen_spect_zoomH_SL_valueChanged(int value)
{

    int range=ui->gen_spect_zoomH_SL->maximum();

    if(value==0)
    {
        gen_spect_centerH=nucleo.spect_window_length/4;
        gen_spect_windowH=nucleo.spect_window_length/8;
        gen_spect_despH_habilitar=false;
        ui->gen_spect_despH_SB->setRange(0,0);
        ui->gen_spect_despH_SB->setValue(0);
        gen_spect_despH_habilitar=true;
    }
    else
    {
        //se recalcula la ventana
        gen_spect_windowH=nucleo.spect_window_length/4*(1-double(value)/double(range));
        //se actualiza la coordenada
        gen_graph_spect->x1=gen_spect_centerH-gen_spect_windowH;
        gen_graph_spect->x2=gen_spect_centerH+gen_spect_windowH;

        if(gen_graph_spect->x1<0)
            gen_graph_spect->x1=0;
        if(gen_graph_spect->x2>nucleo.spect_window_length/2)
            gen_graph_spect->x2=nucleo.spect_window_length/2;

        //se recalcula el despH
        gen_spect_despH_habilitar=false;
        range=nucleo.spect_window_length/2-gen_graph_spect->x2+gen_graph_spect->x1;
        ui->gen_spect_despH_SB->setRange(0,range);
        int value_H=double(gen_spect_centerH)*double(range)/double(nucleo.spect_window_length/2);
        ui->gen_spect_despH_SB->setValue(value_H);
        gen_spect_despH_habilitar=true;
    }

    gen_graph_spect->repaint();
}

void MainWindow::on_clasif_resultado_CB_currentIndexChanged(int index)
{
    if(clasif_modu_habilitar)
    {
        //se pide el baud rate
        nucleo.signal_baud_rate = ui->param_baud_LE->text().toFloat();//copia el baud_rate

        if(nucleo.signal_baud_rate==0)
        {
            ui->clasif_resultado_CB->setCurrentIndex(0);//se reinicia el comboBox de resultados
            information(2,0,5);
            return;
        }

        if(qIsNaN(nucleo.signal_baud_rate))//Se pregunta si no es un integer
        {
            ui->clasif_resultado_CB->setCurrentIndex(0);//se reinicia el comboBox de resultados
            information(2,1,2);
            return;
        }


        //se borra el arreglo de frecuencias si antes fue 4FSK o 8FSK
        if(nucleo.clasif_decision==2 || nucleo.clasif_decision==3)
            delete [] nucleo.dem_freq_array;

        nucleo.clasif_decision=index;




        if(index==2)//4FSK
        {
            //se reserva espacio dinámico en el arreglo de frecuencias
            nucleo.dem_freq_array=new int[4];
            nucleo.detect_freq_length=4;
            //se piden los valores de frecuencia
            for(int i=0;i<4;i++)
                nucleo.dem_freq_array[i]=float(i+1)*float(nucleo.signal_fft_length)/float(4)-float(nucleo.signal_fft_length)/float(8);
        }

        if(index==3)//8FSK
        {
            //se reserva espacio dinámico en el arreglo de frecuencias
            nucleo.dem_freq_array=new int[8];
            nucleo.detect_freq_length=8;
            //se piden los valores de frecuencia
            for(int i=0;i<8;i++)
                nucleo.dem_freq_array[i]=float(i+1)*float(nucleo.signal_fft_length)/float(8)-float(nucleo.signal_fft_length)/float(16);
        }

        if(index==4)//16FSK
        {
            //se reserva espacio dinámico en el arreglo de frecuencias
            nucleo.dem_freq_array=new int[16];
            nucleo.detect_freq_length=16;
            //se piden los valores de frecuencia
            for(int i=0;i<16;i++)
                nucleo.dem_freq_array[i]=float(i+1)*float(nucleo.signal_fft_length)/float(16)-float(nucleo.signal_fft_length)/float(32);
        }

        //se reinician los gráficos
        restart_graphics(2);

    }
}

void MainWindow::on_dem_PB_clicked()
{
    if(dem_PB_habilitar)
    {
        nucleo.signal_baud_rate = ui->param_baud_LE->text().toFloat();//copia el baud_rate

        if (nucleo.signal_baud_rate == 0.0)
        {
            information(3,0,6);

            ui->toolBox->setCurrentIndex(2);//se va a la pestaña detectar
        }

        //se borra la posible modulación
        if(nucleo.signal_dem_length!=0)
        {
            delete[] nucleo.signal_dem;
            nucleo.signal_dem_length=0;
        }
        dem_graph_tiempo->signal_2_length=0;
        dem_graph_tiempo->signal_3_length=0;

        //se borran los posibles símbolos
        if(nucleo.symbols_dem_length!=0)
        {
            delete []nucleo.symbols_dem;
            nucleo.symbols_dem_length=0;
        }

        //se borra el posible raster
        if(nucleo.bits_length!=0)
        {
            delete[] nucleo.bits;
            nucleo.bits_length=0;

        }

        //se reinician el zoom y desp de los gráficos de tiempo y frecuencia
        ui->dem_tiempo_zoomH_SL->setValue(0);
        ui->dem_tiempo_zoomV_SL->setValue(0);
        ui->dem_tiempo_despV_SB->setRange(0,0);
        ui->dem_tiempo_despH_SB->setRange(0,0);
        ui->dem_fft_zoomH_SL->setValue(0);
        ui->dem_fft_zoomV_SL->setValue(0);
        ui->dem_fft_despH_SB->setRange(0,0);
        ui->dem_fft_despV_SB->setRange(0,0);

        //Métodos BFSK
        if(ui->clasif_resultado_CB->currentIndex()==1)
        {
            //se reserva espacio dinámico
            nucleo.signal_dem_length=nucleo.signal_length;
            nucleo.signal_dem = new double[nucleo.signal_dem_length];

            bool correct_dem=nucleo.nucleo_dem_funct(ui->dem_metodos_CB->currentIndex());

            if(!correct_dem)
            {

                restart_graphics(3);

                this->restart_graphics(3);
                information(3,0,7);
                return;
            }


        }

        //Métodos 4FSK, 8FSK, 16FSK
        if(ui->clasif_resultado_CB->currentIndex()>=2 && ui->clasif_resultado_CB->currentIndex()<=4)
        {
            //se reserva espacio dinámico
            nucleo.signal_dem_length=nucleo.signal_length;
            nucleo.signal_dem = new double[nucleo.signal_dem_length];
            nucleo.symbols_dem_length=nucleo.signal_length;
            nucleo.symbols_dem = new int[nucleo.signal_dem_length];

            bool correct_dem=nucleo.nucleo_dem_funct(ui->dem_metodos_CB->currentIndex());

            if(!correct_dem)
            {

                //se borran los posibles símbolos
                if(nucleo.symbols_dem_length==0)
                {
                    delete[] nucleo.symbols_dem;
                    nucleo.symbols_dem_length=0;

                }

                restart_graphics(3);

                this->restart_graphics(3);
                information(3,0,7);
                return;
            }

            //se asigna el código para la demodulación
            nucleo.code_dem=ui->dem_cod_CB->currentIndex();
        }

        //Métodos BPSK
        if(ui->clasif_resultado_CB->currentIndex()==5)
        {           
            //se reserva espacio dinámico
            nucleo.signal_dem_length=nucleo.signal_length;
            nucleo.signal_dem = new double[nucleo.signal_dem_length];

            //Lazo Cuadrático
            if(ui->dem_metodos_CB->currentIndex()==0)
            {
                //se manda a demodular
                bool correct_dem=nucleo.nucleo_dem_funct(1);

                if(!correct_dem)
                {
                    restart_graphics(3);
                    this->restart_graphics(3);
                    information(3,0,7);
                    return;
                }
            }

            //Lazo de Costas
            if(ui->dem_metodos_CB->currentIndex()==1)
            {
                //se manda a demodular
                nucleo.nucleo_dem_funct(2);
            }

        }

        //Métodos QPSK, DQPSK, OQPSK, 8PSK
        if(ui->clasif_resultado_CB->currentIndex()>=6 && ui->clasif_resultado_CB->currentIndex()<=9)
        {
            //se reserva espacio dinámico
            nucleo.signal_dem_length=nucleo.signal_length;
            nucleo.signal_dem = new double[nucleo.signal_dem_length];

            //se manda a demodular
            bool correct_dem=nucleo.nucleo_dem_funct(1);            

            if(!correct_dem)
            {
                restart_graphics(3);
                this->restart_graphics(3);
                information(3,0,7);
                return;
            }
        }

        //se determinan los máximos y mínimos de la señal demodulada
        nucleo.dem_amp_max=0;
        nucleo.dem_amp_min=0;

        for(int i=0;i<nucleo.signal_dem_length;i++)
        {
            if(nucleo.dem_amp_max<nucleo.signal_dem[i])
                nucleo.dem_amp_max=nucleo.signal_dem[i];
            if(nucleo.dem_amp_min>nucleo.signal_dem[i])
                nucleo.dem_amp_min=nucleo.signal_dem[i];
        }

        float signal_min=0;
        float signal_max=0;
        if(nucleo.dem_amp_max>10*nucleo.signal_amp || qAbs(nucleo.dem_amp_min)>10*nucleo.signal_amp)
        {
            signal_min=nucleo.signal_min;
            signal_max=nucleo.signal_max;
        }
        else
        {
            signal_min=qMin(nucleo.signal_min,float(nucleo.dem_amp_min));
            signal_max=qMax(nucleo.signal_max,float(nucleo.dem_amp_max));
        }

        //se manda a dibujar la demodulación
        dem_graph_tiempo->y_p1=signal_min*2;
        dem_graph_tiempo->y_p2=signal_max*2;

        //Si es FSK o BPSK
        if(nucleo.clasif_decision>=1 && nucleo.clasif_decision<=5)
        {
            dem_graph_tiempo->signal_3=NULL;
            dem_graph_tiempo->signal_3_length=0;
            dem_graph_tiempo->signal_2=nucleo.signal_dem;
            dem_graph_tiempo->signal_2_length=nucleo.signal_dem_length;

        }

        //Si es QPSK, DQPSK, OQPSK, 8PSK
        if(nucleo.clasif_decision>=6 && nucleo.clasif_decision<=9)
        {
            dem_graph_tiempo->signal_2=nucleo.signal_I_dem;
            dem_graph_tiempo->signal_2_length=nucleo.signal_I_dem_length;
            dem_graph_tiempo->signal_3=nucleo.signal_Q_dem;
            dem_graph_tiempo->signal_3_length=nucleo.signal_I_dem_length;


        }

        dem_graph_tiempo->repaint();

        //Determinación del raster
        {
            if(nucleo.bits_length!=0)
            {
                nucleo.bits_length=0;
                delete[] nucleo.bits;

            }
            //se reinician el zoom y desplazamiento
            dem_raster_despH_habilitar=false;
            dem_raster_despV_habilitar=false;
            ui->dem_raster_despH_SB->setRange(0,0);
            ui->dem_raster_despV_SB->setRange(0,0);
            ui->dem_raster_zoomH_SL->setValue(0);
            ui->dem_raster_zoomV_SL->setValue(0);
            ui->dem_raster_zoomH_SL->setDisabled(false);
            ui->dem_raster_zoomV_SL->setDisabled(false);
            dem_raster_despH_habilitar=true;
            dem_raster_despV_habilitar=true;

            if(!nucleo.nucleo_raster_funct())
            {                
                //error en la determinación del raster

                //se borra el posible raster
                if(nucleo.bits_length!=0)
                {
                    delete[] nucleo.bits;
                    nucleo.bits_length=0;

                }

                restart_graphics(3);

                information(3,0,5);
            }




            else
            {               
                //Determinación del patrón y actualización en los controles
                nucleo.nucleo_patron_raster_funct();

                if(nucleo.patron==500 && nucleo.patrones.size()==1)
                {
                    //Problemas en la determinación del patrón


                    //se borra el posible raster
                    if(nucleo.bits_length!=0)
                    {

                        delete[] nucleo.bits;
                        nucleo.bits_length=0;

                    }

                    restart_graphics(3);

                    information(3,0,5);


                }

                else
                {


                    ui->dem_raster_LE->setText(QString("%1").arg(nucleo.patron));
                    dem_patrones_CB_hab=false;
                    ui->dem_raster_CB->clear();
                    for(int i=0;i<nucleo.patrones.size();i++)
                        ui->dem_raster_CB->addItem(QString("%1").arg(nucleo.patrones[i]),1);

                    ui->dem_raster_GB->setDisabled(false);
                    dem_patrones_CB_hab=true;

                    //se manda a dibujar el raster
                    dem_graph_raster->bits=nucleo.bits;
                    dem_graph_raster->bits_length=nucleo.bits_length;
                    dem_graph_raster->patron=nucleo.patron;
                    dem_graph_raster->x1=0;
                    dem_graph_raster->x2=nucleo.patron;
                    dem_graph_raster->y1=0;
                    dem_graph_raster->y2=ceil(double(nucleo.bits_length)/double(nucleo.patron));                    

                    //se coloca el centro del dibujo
                    dem_raster_centerH=(dem_graph_raster->x1+dem_graph_raster->x2)/2;
                    dem_raster_windowH=(dem_graph_raster->x2-dem_graph_raster->x1)/2;
                    dem_raster_centerV=(dem_graph_raster->y1+dem_graph_raster->y2)/2;
                    dem_raster_windowV=(dem_graph_raster->y2-dem_graph_raster->y1)/2;

                    dem_graph_raster->repaint();

                    //se habilita el botón de exportar
                    ui->dem_exportar_PB->setEnabled(true);
                    //se habilita el Código
                    ui->dem_cod_L->setEnabled(true);
                    dem_cod_CB_hab=false;
                    ui->dem_cod_CB->setEnabled(true);
                    dem_cod_CB_hab=true;
                }

            }

        }


    }
}

void MainWindow::on_toolBox_currentChanged(int index)
{


    ui->tabWidget->setCurrentIndex(index);//se cambia a la pestaña correspondiente



    if(index==0)//Pestaña general
    {
        //se reasocioan los punteros
        gen_graph_tiempo->signal=nucleo.signal_in;
    }

    if(index==2)//Pestaña Estimar
    {
        clasif_graph_fft->signal=nucleo.signal_fft;
        clasif_graph_const->signal_I=nucleo.signal_I;
        clasif_graph_const->signal_Q=nucleo.signal_Q;
    }

    if(index==3)//Demodular
    {
        //se reasocioan los punteros
        dem_graph_tiempo->signal=nucleo.signal_in;
        dem_graph_tiempo->signal_2=nucleo.signal_dem;
        dem_graph_fft->signal=nucleo.signal_fft;
        dem_graph_raster->bits=nucleo.bits;

        //se pide el baud rate
        nucleo.signal_baud_rate = ui->param_baud_LE->text().toFloat();//copia el baud_rate

        if (nucleo.signal_baud_rate == 0.0 && nucleo.signal_length!=0)
        {
            information(3,0,6);
            ui->toolBox->setCurrentIndex(2);//se va a la pestaña detectar
            return;
        }

        if(qIsNaN(nucleo.signal_baud_rate))//Se pregunta si no es un integer
        {
            ui->clasif_resultado_CB->setCurrentIndex(0);//se reinicia el comboBox de resultados
            information(2,1,2);
            ui->toolBox->setCurrentIndex(2);//se va a la pestaña detectar
            return;
        }


    }

    information(0,0,0);

}

void MainWindow::on_dem_f0_SB_valueChanged(int value)
{

    if(dem_f0_SB_hab)
    {
        int value=ui->dem_f0_SB->value();
        //se actualizan los valores correspondientes
        nucleo.signal_fft_max_pos=value;
        dem_graph_fft->signal_lines[0]=float(nucleo.signal_fft_max_pos);
        ui->dem_f0_LE->setText(QString("%1").arg(float(nucleo.signal_fft_max_pos)*float(nucleo.signal_fm/2)/float(nucleo.signal_fft_length)));
        information(0,0,0);
        dem_graph_fft->repaint();
    }

}


void MainWindow::on_dem_f1_SB_valueChanged(int value)
{
    if(dem_f0_SB_hab)
    {


        //se actualiza el scroll y el resto de las variables relacionadas
        dem_f0_SB_hab=false;

        int index=ui->clasif_resultado_CB->currentIndex();
        //si es modulación 4FSK o 8FSK
        if(index==2 || index==3)
        {
            int index2=ui->dem_freq_CB->currentIndex();
            nucleo.dem_freq_array[index2]=value;
            //se asocian las líneas de frecuencia en el gráfico de FFT
            dem_graph_fft->signal_lines_length=nucleo.detect_freq_length;
            dem_graph_fft->signal_lines=nucleo.dem_freq_array;
            dem_graph_fft->signal_lines_selecc=ui->dem_freq_CB->currentIndex();

        }
        else
        {
            nucleo.signal_fft_max_pos_2=value;
            dem_graph_fft->signal_lines[1]=float(nucleo.signal_fft_max_pos_2);
        }

        ui->dem_f1_LE->setText(QString("%1").arg(float(value)*float(nucleo.signal_fm/2)/float(nucleo.signal_fft_length)));
        //restart_graphics(5);
        dem_f0_SB_hab=true;
        information(0,0,0);
        dem_graph_fft->repaint();
    }
}

void MainWindow::on_clasif_const_CB_currentIndexChanged(int index)
{
    if(clasif_const_hab)
    {
        //se borra la posible constelación
        if(nucleo.signal_I_length!=0)
        {
            delete[] nucleo.signal_I;
            delete[] nucleo.signal_Q;
            nucleo.signal_I_length=0;
        }
        clasif_graph_const->signal_I_length=0;


        if(index==1)//Lazo Cuadrático
        {            

            //se calcula la constelación
            if(nucleo.nucleo_constelacion_funct(index))
            {
                //se manda a dibujar la constelación
                //se pasa el marco de dibujo
                clasif_graph_const->x_const_p1=nucleo.signal_I_min;
                clasif_graph_const->x_const_p2=nucleo.signal_I_max;
                clasif_graph_const->y_const_p1=nucleo.signal_Q_min;
                clasif_graph_const->y_const_p2=nucleo.signal_Q_max;

                //se asocian las señales
                clasif_graph_const->signal_I_length=nucleo.signal_I_length;
                clasif_graph_const->signal_I=nucleo.signal_I;
                clasif_graph_const->signal_Q=nucleo.signal_Q;
                clasif_graph_const->diezm_const=1;


                //se controla el zoom y desplazamiento
                ui->clasif_const_zoomH_SL->setRange(1,100);
                ui->clasif_const_zoomH_SL->setValue(1);
                ui->clasif_const_despH_SB->setRange(0,0);
                clasif_const_windowH=1;
                clasif_const_centerH=(nucleo.signal_I_max+nucleo.signal_I_min)/2;
                ui->clasif_const_zoomV_SL->setRange(1,100);
                ui->clasif_const_zoomV_SL->setValue(1);
                ui->clasif_const_despV_SB->setRange(0,0);
                clasif_const_windowV=1;
                clasif_const_centerV=(nucleo.signal_Q_max+nucleo.signal_Q_min)/2;
                information(0,0,0);
            }
            else//no se pudo obtener la constelación
            {               
               information(2,3,4);

            }
        }

        if(index==2)//Lazo de Costas
        {
            //se calcula la constelación
            if(nucleo.nucleo_constelacion_funct(index))
            {

                //se manda a dibujar la constelación
                //se pasa el marco de dibujo
                clasif_graph_const->x_const_p1=nucleo.signal_I_min;
                clasif_graph_const->x_const_p2=nucleo.signal_I_max;
                clasif_graph_const->y_const_p1=nucleo.signal_Q_min;
                clasif_graph_const->y_const_p2=nucleo.signal_Q_max;

                //se asocian las señales
                clasif_graph_const->signal_I_length=nucleo.signal_I_length;
                clasif_graph_const->signal_I=nucleo.signal_I;
                clasif_graph_const->signal_Q=nucleo.signal_Q;
                clasif_graph_const->diezm_const=1;

                //se controla el zoom y desplazamiento
                ui->clasif_const_zoomH_SL->setRange(1,100);
                ui->clasif_const_zoomH_SL->setValue(1);
                ui->clasif_const_despH_SB->setRange(0,0);
                clasif_const_windowH=1;
                clasif_const_centerH=(nucleo.signal_I_max+nucleo.signal_I_min)/2;
                ui->clasif_const_zoomV_SL->setRange(1,100);
                ui->clasif_const_zoomV_SL->setValue(1);
                ui->clasif_const_despV_SB->setRange(0,0);
                clasif_const_windowV=1;
                clasif_const_centerV=(nucleo.signal_Q_max+nucleo.signal_Q_min)/2;
                information(0,0,0);
            }
            else
            {
                information(2,3,4);
            }
        }

        ui->clasif_const_zoomH_SL->setEnabled(true);
        ui->clasif_const_zoomV_SL->setEnabled(true);        
        clasif_graph_const->repaint();
    }
}



void MainWindow::on_clasif_const_zoomH_SL_valueChanged(int value)
{

    //se modifica el desp horizontal
    if(value==1)//si está en el zoom mínimo
    {
        clasif__const_despH_habilitar=false;
        ui->clasif_const_despH_SB->setRange(0,0);
        clasif__const_despH_habilitar=true;
    }

    else
    {

        ui->clasif_const_despH_SB->setRange(-100,100);

    }

    clasif_const_windowH=(nucleo.signal_I_max-nucleo.signal_I_min)/value/2;

    float centro=clasif_const_centerH+ui->clasif_const_despH_SB->value()*(nucleo.signal_I_max-nucleo.signal_I_min)/100/2;
    clasif_graph_const->x_const_p1=centro-clasif_const_windowH;
    clasif_graph_const->x_const_p2=centro+clasif_const_windowH;

    //se manda a redibujar
    clasif_graph_const->repaint();


}

void MainWindow::on_clasif_const_despH_SB_valueChanged(int value)
{
    if(clasif__const_despH_habilitar)
    {
        float centro=clasif_const_centerH+value*(nucleo.signal_I_max-nucleo.signal_I_min)/100/2;
        clasif_graph_const->x_const_p1=centro-clasif_const_windowH;
        clasif_graph_const->x_const_p2=centro+clasif_const_windowH;
        //se manda a redibujar
        clasif_graph_const->repaint();

    }
}

void MainWindow::on_clasif_const_zoomV_SL_valueChanged(int value)
{
    //se modifica el desp vertical
    if(value==1)//si está en el zoom mínimo
    {
        clasif__const_despV_habilitar=false;
        ui->clasif_const_despV_SB->setRange(0,0);
        clasif__const_despV_habilitar=true;
    }

    else
    {

        ui->clasif_const_despV_SB->setRange(-100,100);

    }

    clasif_const_windowV=(nucleo.signal_Q_max-nucleo.signal_Q_min)/value/2;

    float centro=clasif_const_centerV-ui->clasif_const_despV_SB->value()*(nucleo.signal_Q_max-nucleo.signal_Q_min)/100/2;
    clasif_graph_const->y_const_p1=centro-clasif_const_windowV;
    clasif_graph_const->y_const_p2=centro+clasif_const_windowV;

    //se manda a redibujar
    clasif_graph_const->repaint();
}

void MainWindow::on_clasif_const_despV_SB_valueChanged(int value)
{
    value=-value;
    if(clasif__const_despV_habilitar)
    {
        float centro=clasif_const_centerV+value*(nucleo.signal_Q_max-nucleo.signal_Q_min)/100/2;
        clasif_graph_const->y_const_p1=centro-clasif_const_windowV;
        clasif_graph_const->y_const_p2=centro+clasif_const_windowV;
        //se manda a redibujar
        clasif_graph_const->repaint();

    }
}

void MainWindow::on_dem_raster_zoomH_SL_valueChanged(int value)
{
    int range=ui->dem_raster_zoomH_SL->maximum();

    if(value==0)
    {
        dem_raster_centerH=nucleo.patron/2;
        dem_raster_windowH=nucleo.patron/2;
        dem_raster_despH_habilitar=false;
        ui->dem_raster_despH_SB->setRange(0,0);
        ui->dem_raster_despH_SB->setValue(0);
        dem_raster_despH_habilitar=true;
    }
    else
    {
        //se recalcula la ventana
        dem_raster_windowH=nucleo.patron*(1-double(value)/double(range))/2;
        //se actualiza la coordenada
        dem_graph_raster->x1=dem_raster_centerH-dem_raster_windowH;
        dem_graph_raster->x2=dem_raster_centerH+dem_raster_windowH;

        //se recalcula el despH
        dem_raster_despH_habilitar=false;
        range=nucleo.patron-dem_graph_raster->x2+dem_graph_raster->x1;
        ui->dem_raster_despH_SB->setRange(0,range);
        int value_H=double(dem_raster_centerH)*double(range)/double(nucleo.patron);
        ui->dem_raster_despH_SB->setValue(value_H);
        dem_raster_despH_habilitar=true;
    }
    dem_graph_raster->repaint();

}

void MainWindow::on_dem_raster_despH_SB_valueChanged(int value)
{
    if(dem_raster_despH_habilitar)
    {
        int range=ui->dem_raster_despH_SB->maximum();
        dem_raster_centerH=double(value)*double(nucleo.patron)/double(range);
        dem_graph_raster->x1=dem_raster_centerH-dem_raster_windowH;
        dem_graph_raster->x2=dem_raster_centerH+dem_raster_windowH;
        dem_graph_raster->repaint();
    }
}

void MainWindow::on_dem_raster_zoomV_SL_valueChanged(int value)
{
    int range=ui->dem_raster_zoomV_SL->maximum();

    if(value==0)
    {
        dem_raster_centerV=nucleo.patron/2;
        dem_raster_windowV=nucleo.patron/2;
        dem_raster_despV_habilitar=false;
        ui->dem_raster_despV_SB->setRange(0,0);
        ui->dem_raster_despV_SB->setValue(0);
        dem_raster_despV_habilitar=true;
    }
    else
    {
        //se recalcula la ventana
        dem_raster_windowV=nucleo.patron*(1-double(value)/double(range))/2;
        //se actualiza la coordenada
        dem_graph_raster->y1=dem_raster_centerV-dem_raster_windowV;
        dem_graph_raster->y2=dem_raster_centerV+dem_raster_windowV;

        //se recalcula el despV
        dem_raster_despV_habilitar=false;
        range=nucleo.patron-dem_graph_raster->y2+dem_graph_raster->y1;
        ui->dem_raster_despV_SB->setRange(0,range);
        int value_V=double(dem_raster_centerV)*double(range)/double(nucleo.patron);
        ui->dem_raster_despV_SB->setValue(value_V);
        dem_raster_despV_habilitar=true;
    }
    dem_graph_raster->repaint();
}

void MainWindow::on_dem_raster_despV_SB_valueChanged(int value)
{
    if(dem_raster_despV_habilitar)
    {
        int range=ui->dem_raster_despV_SB->maximum();
        int y_max=floor(double(nucleo.bits_length)/double(nucleo.patron));
        dem_raster_centerV=double(value)*double(y_max)/double(range);
        dem_graph_raster->y1=dem_raster_centerV-dem_raster_windowV;
        dem_graph_raster->y2=dem_raster_centerV+dem_raster_windowV;
        dem_graph_raster->repaint();
    }
}

void MainWindow::on_dem_raster_CB_currentIndexChanged(int index)
{
    if(dem_patrones_CB_hab)
    {
        ui->dem_raster_LE->setText(QString("%1").arg(nucleo.patrones[index]));
        nucleo.patron=nucleo.patrones[index];

        //se manda a dibujar el nuevo raster
        //se reinician el zoom y desplazamiento
        dem_raster_despH_habilitar=false;
        dem_raster_despV_habilitar=false;
        ui->dem_raster_despH_SB->setRange(0,0);
        ui->dem_raster_despV_SB->setRange(0,0);
        ui->dem_raster_zoomH_SL->setValue(0);
        ui->dem_raster_zoomV_SL->setValue(0);
        dem_raster_despH_habilitar=true;
        dem_raster_despV_habilitar=true;

        //se manda a dibujar el raster
        dem_graph_raster->bits=nucleo.bits;
        dem_graph_raster->bits_length=nucleo.bits_length;
        dem_graph_raster->patron=nucleo.patron;
        dem_graph_raster->x1=0;
        dem_graph_raster->x2=nucleo.patron;
        dem_graph_raster->y1=0;
        dem_graph_raster->y2=ceil(double(nucleo.bits_length)/double(nucleo.patron));

        //se coloca el centro del dibujo
        dem_raster_centerH=(dem_graph_raster->x1+dem_graph_raster->x2)/2;
        dem_raster_windowH=(dem_graph_raster->x2-dem_graph_raster->x1)/2;
        dem_raster_centerV=(dem_graph_raster->y1+dem_graph_raster->y2)/2;
        dem_raster_windowV=(dem_graph_raster->y2-dem_graph_raster->y1)/2;

        dem_graph_raster->repaint();
        information(0,0,0);
    }
}

void MainWindow::on_clasif_const_lines_clicked(bool checked)
{
    if(clasif_const_lines_hab)
    {
        clasif_graph_const->const_lines=checked;
        clasif_graph_const->repaint();
        information(0,0,0);
    }

}

void MainWindow::on_dem_raster_LE_returnPressed()
{

//    //validación del valor del patrón
//    bool ok=0;
//    int value=ui->dem_raster_LE->text().toInt(&ok);



//    else
//    {
//        nucleo.patron=ui->dem_raster_LE->text().toFloat();

//        //se manda a dibujar el nuevo raster
//        //se reinician el zoom y desplazamiento
//        dem_raster_despH_habilitar=false;
//        dem_raster_despV_habilitar=false;
//        ui->dem_raster_despH_SB->setRange(0,0);
//        ui->dem_raster_despV_SB->setRange(0,0);
//        ui->dem_raster_zoomH_SL->setValue(0);
//        ui->dem_raster_zoomV_SL->setValue(0);
//        dem_raster_despH_habilitar=true;
//        dem_raster_despV_habilitar=true;

//        //se manda a dibujar el raster
//        dem_graph_raster->bits=nucleo.bits;
//        dem_graph_raster->bits_length=nucleo.bits_length;
//        dem_graph_raster->patron=nucleo.patron;
//        dem_graph_raster->x1=0;
//        dem_graph_raster->x2=nucleo.patron;
//        dem_graph_raster->y1=0;
//        dem_graph_raster->y2=ceil(double(nucleo.bits_length)/double(nucleo.patron));

//        //se coloca el centro del dibujo
//        dem_raster_centerH=(dem_graph_raster->x1+dem_graph_raster->x2)/2;
//        dem_raster_windowH=(dem_graph_raster->x2-dem_graph_raster->x1)/2;
//        dem_raster_centerV=(dem_graph_raster->y1+dem_graph_raster->y2)/2;
//        dem_raster_windowV=(dem_graph_raster->y2-dem_graph_raster->y1)/2;

//        dem_graph_raster->repaint();
//    }
}

void MainWindow::on_dem_exportar_PB_clicked()
{
    QString mFilePath = QFileDialog::getSaveFileName(this);

    if(mFilePath.isEmpty())
        return;
    if (!mFilePath.endsWith(".txt"))
        mFilePath.push_back(".txt");
    QFile file(mFilePath);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        out << ui->param_signal_name->text() << "\n";
        out << "Frecuencia de muestreo: " << nucleo.signal_fm << "\n";
        out << "Clasificación de la señal: " << ui->clasif_resultado_CB->itemText(nucleo.clasif_decision) << "\n";
        out << "Método de demodulación: " << ui->dem_metodos_CB->currentText() << "\n";
        out << "Período del raster: " << nucleo.patron << "\n";
        out << "Total de bits: " << nucleo.bits_length << "\n";;
        out << "Bits: " << "\n";

        for (int i = 0; i < nucleo.bits_length; i++)
        {
            out << nucleo.bits[i] << " ";
            if((i+1)%nucleo.patron==0)
                out << "\n";
        }

    }
    file.close();
}

void MainWindow::on_detect_PB_clicked()
{
    //se deshabilitan controles
    detect_f0_hab=false;
    detect_f1_hab=false;
    detect_f0_LE_hab=false;
    detect_f1_LE_hab=false;

    //se activan controles
    ui->detect_signals_CB->setDisabled(false);
    ui->detect_L2->setDisabled(false);

    //se salva la señal orginal, se borra el anterior arreglo de señales, y se realiza la estimación
    if(nucleo.detect_freq_length!=0)
    {        
        //se hace salva de la señal original
        nucleo.signal_in = new double[nucleo.signal_length];

        for(int i=0;i<nucleo.signal_length;i++)
        {
            nucleo.signal_in[i]=nucleo.signal_in_array[0][i];

        }

        //se manda a calcular la fft para el proceso de detección
        nucleo.nucleo_fft_funct();

        for(int i=0; i<nucleo.detect_freq_length;i++)
            delete []nucleo.signal_in_array[i];


        delete []nucleo.signal_in_array;        
        delete []nucleo.signal_amp_array;
        delete []nucleo.signal_baud_rate_array;
        delete []nucleo.signal_SNR_array;

    }

    //se borra la detección anterior y los rectángulos correspondientes
    if(nucleo.detect_freq_length!=0)
    {
        detect_retangles.clear();
        nucleo.detect_freq_length=0;
        delete []nucleo.detect_freq;
        detect_graph_fft_scene->clear();        

    }

    //Se detectan las señales en función del método escogido

    //Filter Bank
    if(ui->detect_met_CB->currentIndex()==0)
    {
        nucleo.nucleo_detect_funct(0);

    }

    //Energy Detector
    if(ui->detect_met_CB->currentIndex()==1)
    {
        nucleo.nucleo_detect_funct(1);
    }

    //se replican las señales
    //se reserva espacio
    if(nucleo.detect_freq_length!=0)
    {
        //Tiempo
        nucleo.signal_in_array_length=nucleo.detect_freq_length;
        nucleo.signal_in_array = new double*[nucleo.detect_freq_length];

        for(int i=0; i<nucleo.detect_freq_length;i++)
            nucleo.signal_in_array[i]=new double[nucleo.signal_length];


        nucleo.signal_amp_array = new double[nucleo.detect_freq_length];
        nucleo.signal_baud_rate_array = new double[nucleo.detect_freq_length];
        nucleo.signal_SNR_array = new double[nucleo.detect_freq_length];

    }

    //se copian las señales
    for(int i=0; i<nucleo.detect_freq_length;i++)
    {
        for(int j=0;j<nucleo.signal_length;j++)
            nucleo.signal_in_array[i][j]=nucleo.signal_in[j];


        nucleo.signal_amp_array[i]=nucleo.signal_amp;
        nucleo.signal_baud_rate_array[i]=nucleo.signal_baud_rate;
        nucleo.signal_SNR_array[i]=nucleo.signal_SNR;
    }

    //se borra el arreglo de señales
    delete []nucleo.signal_in;
    nucleo.signal_in=nucleo.signal_in_array[0];


    //se borran las señales del comboBox de signals
    detect_signals_CB_hab=false;
    ui->detect_signals_CB->clear();
    //se rellena el ComboBox de señales
    ui->detect_signals_CB->addItem(QString("señal"));
    for(int i=1; i<=nucleo.detect_freq_length/2; i++)
    {
        ui->detect_signals_CB->addItem(QString("señal_")+QString("%1").arg(i));
    }
    detect_signals_CB_hab=true;

    //estimación
    nucleo.nucleo_estimacion_param();

    //se redibujan las señales
    restart_graphics(1);

    //se inhabilita el panel de filtrado
    detect_f0_hab=false;
    detect_f1_hab=false;
    detect_f0_LE_hab=false;
    detect_f1_LE_hab=false;
    ui->detect_f0_LE->clear();
    ui->detect_f1_LE->clear();
    ui->detect_f0_SB->setValue(0);
    ui->detect_f1_SB->setValue(0);
    ui->detect_filtro_GB->setEnabled(false);
    detect_f0_hab=true;
    detect_f1_hab=true;
    detect_f0_LE_hab=true;
    detect_f1_LE_hab=true;

}

void MainWindow::on_detect_signals_CB_currentIndexChanged(int index)
{    

    if(detect_signals_CB_hab)
    {
        //se apuntan signal_in y frecuencia
        if(index!=-1)
        {

            nucleo.signal_in=nucleo.signal_in_array[index];

        }

        if(index!=0 && index!=-1)
        {

            //se modifican los scroll y lineedits
            detect_f0_hab=false;
            detect_f1_hab=false;
            detect_f0_LE_hab=false;
            detect_f1_LE_hab=false;

            //se actualiza el valor del centro de frecuencia
            int index_2=2*(index-1);
            int f0=nucleo.detect_freq[index_2];
            int f1=nucleo.detect_freq[index_2+1];
            float f0_f=float(f0)/float(nucleo.signal_fft_length)*float(nucleo.signal_fm)/float(2);
            float f1_f=float(f1)/float(nucleo.signal_fft_length)*float(nucleo.signal_fm)/float(2);
            float center_f=(f1_f+f0_f)/2;
            float ancho_f=(f1_f-f0_f)/2;
            int center=float(f1+f0)/float(2);
            int ancho=min(center-f0,f1-center);
            int center_pos=float(center-ancho-10)*float(100)/float(nucleo.signal_fft_length-2*(ancho+10));
            ui->detect_f0_SB->setValue(center_pos);
            ui->detect_f0_LE->setText(QString("%1").arg(center_f));

            //se actualiza la banda de frecuencia
            int ancho_min=3;
            int ancho_max=min(center,nucleo.signal_fft_length-center);
            int ancho_pos=float(ancho-ancho_min)*float(100)/float(ancho_max-ancho_min);
            ui->detect_f1_SB->setValue(ancho_pos);
            ui->detect_f1_LE->setText(QString("%1").arg(2*ancho_f));

            //se habilitan los controles
            ui->detect_filtro_GB->setDisabled(false);
            detect_f0_hab=true;
            detect_f1_hab=true;
            detect_f0_LE_hab=true;
            detect_f1_LE_hab=true;
            nucleo.signal_amp=nucleo.signal_amp_array[index];


        }

        if(index==0)
        {
            //se deshabilitan los controles
            detect_f0_hab=false;
            detect_f1_hab=false;
            detect_f0_LE_hab=false;
            detect_f1_LE_hab=false;
            ui->detect_f0_LE->clear();
            ui->detect_f1_LE->clear();
            ui->detect_f0_SB->setValue(0);
            ui->detect_f1_SB->setValue(0);
            ui->detect_PB->setDisabled(false);
            ui->detect_L1->setDisabled(false);
            ui->detect_met_CB->setDisabled(false);
            nucleo.signal_amp=nucleo.signal_amp_array[index];
            ui->detect_filtro_GB->setEnabled(false);
        }


        //se actualiza el espectro
        nucleo.nucleo_fft_funct();

        //se estiman los parámetros
        nucleo.nucleo_estimacion_param();

        restart_graphics(1);

        information(0,0,0);

    }

}

void MainWindow::on_detect_filter_PB_clicked()
{
    //se captura la señal a filtrar
    int index=ui->detect_signals_CB->currentIndex();

    //se manda a filtrar
    if(!nucleo.nucleo_filtrar_funct(index))
    {
        information(2,2,2);
        return;
    }

    //se recalcula la frecuencia
    nucleo.nucleo_fft_funct();

    //se estiman los parámetros
    nucleo.nucleo_estimacion_param();
    nucleo.signal_amp_array[index]=nucleo.signal_amp;

    //se determina el espectrograma
    this->on_spect_PB_clicked();

    //this->detect_pintar_fft();
    restart_graphics(1);
    information(0,0,0);


}

void MainWindow::on_detect_f1_SB_valueChanged(int value)
{

    if(nucleo.detect_freq_length!=0 && detect_f1_hab)
    {

        //se calculan el centro y el ancho de la banda en muestras
        int index=ui->detect_signals_CB->currentIndex();
        int index_2=2*(index-1);
        int f0=nucleo.detect_freq[index_2];
        int f1=nucleo.detect_freq[index_2+1];
        int center=(f0+f1)/2;
        int ancho_min=3;
        int ancho_max=min(center,nucleo.signal_fft_length-center);

        //nuevos valores de center, f0 y f1
        int ancho=float(ancho_max-ancho_min)/float(100)*float(value)+ancho_min;
        f0=center-ancho;
        f1=center+ancho;
        nucleo.detect_freq[index_2]=f0;
        nucleo.detect_freq[index_2+1]=f1;
        //se actualiza el line edit
        float ancho_f=float(ancho)/float(nucleo.signal_fft_length)*float(nucleo.signal_fm)/float(2);
        detect_f1_LE_hab=false;
        ui->detect_f1_LE->setText(QString("%1").arg(ancho_f*2));
        detect_f1_LE_hab=true;
        restart_graphics(1);
    }
}


///value=0 se llama desde el botón abrir, 1 desde el botón detectar
void MainWindow::restart_graphics(int value)
{
    if(value==-1)//Restaura todos los gráficos y botones a nulo
    {
        //se deshabilitan todos los controles
        gen_spect_despH_habilitar=false;
        gen_spect_despV_habilitar=false;
        gen_tiempo_despH_habilitar=false;
        gen_tiempo_despV_habilitar=false;
        detect_f0_hab=false;
        detect_f0_LE_hab=false;
        detect_f1_hab=false;
        detect_f1_LE_hab=false;
        detect_fft_despH_habilitar=false;
        detect_fft_despV_habilitar=false;
        detect_signals_CB_hab=false;
        detect_spect_despH_habilitar=false;
        detect_spect_despV_habilitar=false;
        clasif_const_hab=false;
        clasif_const_lines_hab=false;
        clasif_fft_despH_habilitar=false;
        clasif_fft_despV_habilitar=false;
        clasif_modu_habilitar=false;
        clasif_resultados_habilitar=false;
        clasif_spect_despH_habilitar=false;
        clasif_spect_despV_habilitar=false;
        clasif__const_despH_habilitar=false;
        clasif__const_despV_habilitar=false;
        dem_cod_CB_hab=false;
        dem_f0_SB_hab=false;
        dem_fft_despH_habilitar=false;
        dem_fft_despV_habilitar=false;
        dem_freq_CB_hab=false;
        dem_patrones_CB_hab=false;
        dem_raster_despH_habilitar=false;
        dem_raster_despV_habilitar=false;
        dem_tiempo_despH_habilitar=false;
        dem_tiempo_despV_habilitar=false;

        //se inicializan zoom y desplazamiento
        //General
        ui->gen_tiempo_zoomH_SL->setValue(0);
        ui->gen_tiempo_zoomH_SL->setEnabled(false);
        ui->gen_tiempo_zoomV_SL->setValue(0);
        ui->gen_tiempo_zoomV_SL->setEnabled(false);
        ui->gen_spect_zoomH_SL->setValue(0);
        ui->gen_spect_zoomH_SL->setEnabled(false);
        ui->gen_spect_zoomV_SL->setValue(0);
        ui->gen_spect_zoomV_SL->setEnabled(false);
        ui->gen_inf_GB->setEnabled(false);
        ui->gen_spect_GB->setEnabled(false);
        ui->param_signal_name->setEnabled(false);
        ui->param_fm_L->setEnabled(false);
        ui->param_signal_duracion->setEnabled(false);
        ui->gen_spect_long_vent_L->setEnabled(false);
        ui->gen_spect_solap_L->setEnabled(false);
        ui->gen_spect_vent_L->setEnabled(false);
        ui->spect_window_length_LE->setEnabled(false);
        ui->spect_window_length_SpinB->setEnabled(false);
        ui->spect_window_solap_LE->setEnabled(false);
        ui->spect_solap_SpinB->setEnabled(false);
        ui->spect_color->setEnabled(false);
        ui->spect_PB->setEnabled(false);
        ui->spect_window_CB->setEnabled(false);

        //Detectar
        ui->detect_fft_zoomH_SL->setValue(0);
        ui->detect_fft_zoomH_SL->setEnabled(false);
        ui->detect_fft_zoomV_SL->setValue(0);
        ui->detect_fft_zoomV_SL->setEnabled(false);
        ui->detect_spect_zoomH_SL->setValue(0);
        ui->detect_spect_zoomH_SL->setEnabled(0);
        ui->detect_spect_zoomV_SL->setValue(0);
        ui->detect_spect_zoomV_SL->setEnabled(0);

        //Estimación
        ui->clasif_spect_zoomH_SL->setValue(0);
        ui->clasif_spect_zoomH_SL->setEnabled(false);
        ui->clasif_fft_zoomH_SL->setValue(0);
        ui->clasif_fft_zoomH_SL->setEnabled(false);
        ui->clasif_const_zoomH_SL->setValue(0);
        ui->clasif_const_zoomH_SL->setEnabled(false);
        ui->clasif_spect_zoomV_SL->setValue(0);
        ui->clasif_spect_zoomV_SL->setEnabled(false);
        ui->clasif_fft_zoomV_SL->setValue(0);
        ui->clasif_fft_zoomV_SL->setEnabled(false);
        ui->clasif_const_zoomV_SL->setValue(0);
        ui->clasif_const_zoomV_SL->setEnabled(false);

        //Demodulación
        ui->dem_tiempo_zoomH_SL->setValue(0);
        ui->dem_tiempo_zoomH_SL->setEnabled(false);
        ui->dem_fft_zoomH_SL->setValue(0);
        ui->dem_fft_zoomH_SL->setEnabled(false);
        ui->dem_raster_zoomH_SL->setValue(0);
        ui->dem_raster_zoomH_SL->setEnabled(false);
        ui->dem_tiempo_zoomV_SL->setValue(0);
        ui->dem_tiempo_zoomV_SL->setEnabled(false);
        ui->dem_fft_zoomV_SL->setValue(0);
        ui->dem_fft_zoomV_SL->setEnabled(false);
        ui->dem_raster_zoomV_SL->setValue(0);
        ui->dem_raster_zoomV_SL->setEnabled(false);

        //Modificación del panel de control
        //General
        ui->param_signal_name->setText("Archivo: ");
        ui->param_fm_L->setText("Frecuencia de Muestreo [Hz]:");
        ui->param_signal_duracion->setText("Duración [hh:mm:ss]");
        ui->gen_inf_GB->setEnabled(false);
        ui->spect_window_length_LE->clear();
        ui->spect_window_solap_LE->clear();
        ui->spect_window_CB->clear();
        ui->spect_color->setChecked(false);
        ui->gen_spect_GB->setEnabled(false);

        //Detectar
        ui->detect_met_CB->clear();
        ui->detect_signals_CB->clear();
        ui->detect_f0_LE->clear();
        ui->detect_f1_LE->clear();
        ui->detect_f0_SB->setValue(0);
        ui->detect_f1_SB->setValue(0);
        ui->detect_filtro_GB->setEnabled(false);
        ui->detect_SS_GB->setEnabled(false);

        //Estimación
        ui->param_amp_LE->clear();
        ui->param_amp_LE->setEnabled(false);
        ui->param_amp_L->setEnabled(false);
        ui->param_SNR_LE->clear();
        ui->param_SNR_LE->setEnabled(false);
        ui->param_SNR_L->setEnabled(false);
        ui->param_baud_LE->clear();
        ui->param_baud_LE->setEnabled(false);
        ui->param_baud_rate_L->setEnabled(false);
        ui->clasif_param_GB->setDisabled(true);

        ui->param_SNR_L->setEnabled(false);

        ui->clasif_metodos_CB->clear();
        ui->clasif_resultado_CB->clear();
        ui->clasif_const_CB->clear();
        ui->clasif_mod_GB->setEnabled(false);

        //se borran todos los gráficos
        gen_graph_tiempo->inicializar();
        gen_graph_tiempo->update();
        gen_graph_tiempo->update();
        gen_graph_spect->inicializar();
        gen_graph_spect->update();
        detect_graph_fft_scene->clear();
        detect_graph_fft_scene->update();
        detect_graph_spect->inicializar();
        detect_graph_spect->update();
        clasif_graph_spect->inicializar();
        clasif_graph_spect->update();
        clasif_graph_fft->inicializar();
        clasif_graph_fft->update();
        clasif_graph_const->inicializar();
        clasif_graph_const->update();
        clasif_graph_const->inicializar();
        clasif_graph_const->update();
        dem_graph_tiempo->inicializar();
        dem_graph_tiempo->update();
        dem_graph_fft->inicializar();
        dem_graph_fft->update();
        dem_graph_raster->inicializar();
        dem_graph_raster->update();

    }

    if(value!=3 && value!=-1)
    {
        ///Actualización de los gráficos
        ///Reinicia la interfaz gráfica al cargarse o detectarse una nueva señal
        ///
        ///Tab General
        //se asocia la señal
        gen_graph_tiempo->signal=nucleo.signal_in;
        gen_graph_tiempo->pintar_coord_hab=true;
        //se colocan los ejes horizontales y verticales
        gen_graph_tiempo->x_p1=0;
        gen_graph_tiempo->x_p2=nucleo.signal_length;
        gen_graph_tiempo->y_p1=nucleo.signal_min*2;
        gen_graph_tiempo->y_p2=nucleo.signal_max*2;
        gen_graph_tiempo->diezm_factor=50000;//total de puntos a pintar en la pantalla
        gen_graph_tiempo->fm=nucleo.signal_fm;
        //se actualizan los valores de centro y window para visualizar toda la señal
        gen_tiempo_centerH=nucleo.signal_length/2;
        gen_tiempo_windowH=nucleo.signal_length/2;
        gen_tiempo_centerV=(nucleo.signal_max+nucleo.signal_min)/2;
        gen_tiempo_windowV=nucleo.signal_max-nucleo.signal_min;
        //se actualizan los controles de zoom y desp de la interfaz
        gen_tiempo_despH_habilitar=false;
        gen_tiempo_despV_habilitar=false;
        ui->gen_tiempo_zoomH_SL->setValue(0);
        ui->gen_tiempo_zoomV_SL->setValue(0);
        ui->gen_spect_zoomH_SL->setValue(0);
        ui->gen_spect_zoomV_SL->setValue(0);
        ui->gen_tiempo_zoomH_SL->setRange(0,nucleo.signal_length/2);//para hacer zoom hasta dos muestras
        ui->gen_tiempo_despH_SB->setRange(0,0);
        ui->gen_tiempo_despV_SB->setRange(0,0);
        gen_tiempo_despH_habilitar=true;
        gen_tiempo_despV_habilitar=true;

        //Spectrogram
        gen_spect_despH_habilitar=false;
        gen_spect_despV_habilitar=false;
        gen_spect_centerH=nucleo.spect_window_length/4;
        gen_spect_windowH=nucleo.spect_window_length/8;
        gen_spect_centerV=nucleo.signal_spect_length/2;
        gen_spect_windowV=nucleo.signal_spect_length/4;
        ui->gen_spect_despH_SB->setRange(0,0);
        ui->gen_spect_despV_SB->setRange(0,0);
        ui->gen_spect_zoomH_SL->setValue(0);
        ui->gen_spect_zoomV_SL->setValue(0);
        gen_graph_spect->x1=0;
        gen_graph_spect->x2=nucleo.spect_window_length/2;
        gen_graph_spect->y1=0;
        gen_graph_spect->y2=nucleo.signal_spect_length;
        gen_graph_spect->spect_max=nucleo.spect_max;
        gen_graph_spect->spect=nucleo.signal_spect;
        gen_graph_spect->spect_length=nucleo.signal_spect_length;
        gen_graph_spect->Tiempo=double(nucleo.signal_length)/double(nucleo.signal_fm);
        if(value==0)
        {
            gen_graph_spect->spect_color_scale=0;
            ui->spect_color->setChecked(false);
        }
        gen_graph_spect->pintar_coord_hab=true;
        gen_graph_spect->fm_tiempo=nucleo.signal_fm;
        gen_graph_spect->fm_freq=double(nucleo.spect_window_length)/nucleo.signal_fm;
        gen_spect_despH_habilitar=true;
        gen_spect_despV_habilitar=true;
        ui->gen_spect_GB->setEnabled(true);
        //se habilitan controles
        ui->gen_tiempo_zoomH_SL->setDisabled(false);
        ui->gen_tiempo_zoomV_SL->setDisabled(false);
        ui->gen_spect_zoomH_SL->setDisabled(false);
        ui->gen_spect_zoomV_SL->setDisabled(false);
        ui->gen_inf_GB->setDisabled(false);


        ///Detectar
        //Fourier
        ui->detect_fft_zoomH_SL->setValue(0);
        ui->detect_fft_zoomV_SL->setValue(0);
        ui->detect_spect_zoomH_SL->setValue(0);
        ui->detect_spect_zoomV_SL->setValue(0);
        //se modifica el zoom horizontal
        ui->detect_fft_zoomH_SL->setRange(1,100);//para hacer zoom hasta dos muestras
        ui->detect_fft_zoomV_SL->setRange(1,100);
//        ui->detect_f0_SB->setRange(0,100);
//        ui->detect_f1_SB->setRange(0,100);
        if(value==0)
        {
            //se plotea el espectro en la pestaña Detectar
            int diezm=nucleo.signal_fft_length/40000;
            if (diezm==0)
                diezm=1;
            QPolygonF polygon(nucleo.signal_fft_length-diezm);
            int x1=0;
            int y1=0;
            //se detecta el máximo de esta transformada
            float max_fft=0;
            for(int i=0; i<nucleo.signal_fft_length-diezm; i+=diezm)
            {
                if(max_fft<nucleo.signal_fft[i])
                    max_fft=nucleo.signal_fft[i];
            }
            //se construye el polígono
            int width=ui->detect_graph_fft_GV->width();
            int height=ui->detect_graph_fft_GV->height();
            for(int i=0; i<nucleo.signal_fft_length-diezm; i+=diezm)
            {
                x1=(float(i)/float(nucleo.signal_fft_length-diezm))*float(width);
                y1=-(float(nucleo.signal_fft[i])/float(max_fft))*float(height);

                polygon << QPointF(x1,y1);
            }
            //se borra la escena
            detect_graph_fft_scene->clear();
            //se confecciona la escena
            QPen painter;
            painter.setBrush(Qt::green);
            painter.setStyle(Qt::SolidLine);
            painter.setCapStyle(Qt::SquareCap);
            painter.setJoinStyle(Qt::MiterJoin);
            detect_graph_fft_scene->addPolygon(polygon,painter);
        }

        //se plotean las coordenadas
        QPen painter;
        painter.setColor(Qt::gray);
        painter.setStyle(Qt::DashLine);
        painter.setCapStyle(Qt::SquareCap);
        painter.setJoinStyle(Qt::MiterJoin);
        int width=ui->detect_graph_fft_GV->width();
        int height=ui->detect_graph_fft_GV->height()-10;

        //líneas horizontales
        double y2=-double(height)/double(2);
        double y1=y2+double(height)/double(4);
        double y3=y2-double(height)/double(4);
        detect_graph_fft_scene->addLine(0,y1,width,y1,painter);
        detect_graph_fft_scene->addLine(0,y2,width,y2,painter);
        detect_graph_fft_scene->addLine(0,y3,width,y3,painter);
        //líneas verticales
        double x2=double(width)/double(2);
        double x1=x2+double(width)/double(4);
        double x3=x2-double(width)/double(4);
        detect_graph_fft_scene->addLine(x1,0,x1,-height,painter);
        detect_graph_fft_scene->addLine(x2,0,x2,-height,painter);
        detect_graph_fft_scene->addLine(x3,0,x3,-height,painter);

        //texto
        //texto horizontal
        double y3_value=nucleo.signal_fft_max*double(3)/double(4);
        QGraphicsTextItem *text1=detect_graph_fft_scene->addText(QString("%1").arg(y3_value));
        text1->setDefaultTextColor(Qt::white);
        text1->setPos(0,y3-17);
        double y2_value=nucleo.signal_fft_max/double(2);
        QGraphicsTextItem *text2=detect_graph_fft_scene->addText(QString("%1").arg(y2_value));
        text2->setDefaultTextColor(Qt::white);
        text2->setPos(0,y2-17);
        double y1_value=nucleo.signal_fft_max/double(4);
        QGraphicsTextItem *text3=detect_graph_fft_scene->addText(QString("%1").arg(y1_value));
        text3->setDefaultTextColor(Qt::white);
        text3->setPos(0,y1-17);

        //texto vertical
        double x1_value=nucleo.signal_fm*double(3)/double(8);
        QGraphicsTextItem *text1_x=detect_graph_fft_scene->addText(QString("%1").arg(x1_value));
        text1_x->setDefaultTextColor(Qt::white);
        text1_x->setPos(x1,-17);
        double x2_value=nucleo.signal_fm/double(4);
        QGraphicsTextItem *text2_x=detect_graph_fft_scene->addText(QString("%1").arg(x2_value));
        text2_x->setDefaultTextColor(Qt::white);
        text2_x->setPos(x2,-17);
        double x3_value=nucleo.signal_fm/double(8);
        QGraphicsTextItem *text3_x=detect_graph_fft_scene->addText(QString("%1").arg(x3_value));
        text3_x->setDefaultTextColor(Qt::white);
        text3_x->setPos(x3,-17);

        //Spectrogram
        detect_spect_despH_habilitar=false;
        detect_spect_despV_habilitar=false;
        detect_spect_centerH=nucleo.spect_window_length/4;
        detect_spect_windowH=nucleo.spect_window_length/8;
        detect_spect_centerV=nucleo.signal_spect_length/2;
        detect_spect_windowV=nucleo.signal_spect_length/4;
        ui->detect_spect_despH_SB->setRange(0,0);
        ui->detect_spect_despV_SB->setRange(0,0);
        ui->detect_spect_zoomH_SL->setValue(0);
        ui->detect_spect_zoomV_SL->setValue(0);
        detect_graph_spect->x1=0;
        detect_graph_spect->x2=nucleo.spect_window_length/2;
        detect_graph_spect->y1=0;
        detect_graph_spect->y2=nucleo.signal_spect_length;
        detect_graph_spect->spect_max=nucleo.spect_max;
        detect_graph_spect->spect=nucleo.signal_spect;
        detect_graph_spect->spect_length=nucleo.signal_spect_length;
        if(value==0)
            detect_graph_spect->spect_color_scale=0;
        detect_graph_spect->pintar_coord_hab=true;
        detect_graph_spect->fm_tiempo=nucleo.signal_fm;
        detect_graph_spect->fm_freq=double(nucleo.spect_window_length)/nucleo.signal_fm;
        detect_graph_spect->Tiempo=double(nucleo.signal_length)/double(nucleo.signal_fm);
        detect_spect_despH_habilitar=true;
        detect_spect_despV_habilitar=true;
        //se habilitan controles
        ui->detect_fft_zoomH_SL->setDisabled(false);
        ui->detect_fft_zoomV_SL->setDisabled(false);
        ui->detect_spect_zoomH_SL->setDisabled(false);
        ui->detect_spect_zoomV_SL->setDisabled(false);

        ///Tab Estimación
        //Fourier
        clasif_graph_fft->signal=nucleo.signal_fft;
        clasif_graph_fft->pintar_coord_hab=true;
        ui->clasif_param_GB->setDisabled(false);
        ui->clasif_fft_zoomH_SL->setValue(0);
        ui->clasif_fft_zoomV_SL->setValue(0);
        ui->clasif_const_zoomH_SL->setValue(0);
        ui->clasif_const_zoomV_SL->setValue(0);
        ui->clasif_const_zoomH_SL->setDisabled(true);
        ui->clasif_const_zoomV_SL->setDisabled(true);
        //se colocan los ejes horizontales y verticales
        clasif_graph_fft->x_p1=0;
        clasif_graph_fft->x_p2=nucleo.signal_fft_length;
        clasif_graph_fft->y_p1=nucleo.signal_fft_min;
        clasif_graph_fft->y_p2=nucleo.signal_fft_max;
        clasif_graph_fft->diezm_factor=20000;//nucleo.signal_fft_length;//total de puntos a pintar en pantalla
        clasif_graph_fft->fm=double(2)*double(nucleo.signal_fft_length)/nucleo.signal_fm;
        //se actualizan los valores de centro y window para visualizar toda la señal
        clasif_fft_centerH=nucleo.signal_fft_length/2;
        clasif_fft_windowH=nucleo.signal_fft_length/2;
        clasif_fft_centerV=nucleo.signal_fft_min;
        clasif_fft_windowV=nucleo.signal_fft_max-nucleo.signal_fft_min;
        //se modifica el zoom horizontal y vertical
        ui->clasif_fft_zoomH_SL->setRange(0,nucleo.signal_fft_length/2);//para hacer zoom hasta dos muestras
        //se deshabilita el despH y despV
        ui->clasif_fft_despH_SB->setRange(0,0);
        ui->clasif_fft_despV_SB->setRange(0,0);


        //Constelación
        //se reinician el zoom y desp
        ui->clasif_const_despH_SB->setRange(0,0);
        ui->clasif_const_despV_SB->setRange(0,0);
        ui->clasif_const_CB->setCurrentIndex(0);
        ui->clasif_const_lines->setDisabled(true);
        ui->clasif_const_lines->setChecked(false);
        clasif_graph_const->const_lines=false;
        ui->clasif_met_L->setEnabled(true);

        //Spectrogram
        clasif_spect_despH_habilitar=false;
        clasif_spect_despV_habilitar=false;
        clasif_spect_centerH=nucleo.spect_window_length/4;
        clasif_spect_windowH=nucleo.spect_window_length/8;
        clasif_spect_centerV=nucleo.signal_spect_length/2;
        clasif_spect_windowV=nucleo.signal_spect_length/4;
        ui->clasif_spect_despH_SB->setRange(0,0);
        ui->clasif_spect_despV_SB->setRange(0,0);
        ui->clasif_spect_zoomH_SL->setValue(0);
        ui->clasif_spect_zoomV_SL->setValue(0);
        clasif_graph_spect->x1=0;
        clasif_graph_spect->x2=nucleo.spect_window_length/2;
        clasif_graph_spect->y1=0;
        clasif_graph_spect->y2=nucleo.signal_spect_length;
        clasif_graph_spect->spect_max=nucleo.spect_max;
        clasif_graph_spect->spect=nucleo.signal_spect;
        clasif_graph_spect->spect_length=nucleo.signal_spect_length;
        if(value==0)
            clasif_graph_spect->spect_color_scale=0;
        clasif_graph_spect->pintar_coord_hab=true;
        clasif_graph_spect->fm_tiempo=nucleo.signal_fm;
        clasif_graph_spect->fm_freq=double(nucleo.spect_window_length)/double(nucleo.signal_fm);
        clasif_graph_spect->Tiempo=double(nucleo.signal_length)/double(nucleo.signal_fm);
        clasif_spect_despH_habilitar=true;
        clasif_spect_despV_habilitar=true;
        //se habilitan controles
        ui->clasif_fft_zoomH_SL->setDisabled(false);
        ui->clasif_fft_zoomV_SL->setDisabled(false);
        ui->clasif_spect_zoomH_SL->setDisabled(false);
        ui->clasif_spect_zoomV_SL->setDisabled(false);
    }

    if(value!=-1)
    {
        ///Tab Demodulación
        // Modificación de los Gráficos
        ui->dem_raster_zoomH_SL->setDisabled(true);
        ui->dem_raster_zoomV_SL->setDisabled(true);
        ui->dem_tiempo_zoomH_SL->setValue(0);
        ui->dem_tiempo_zoomV_SL->setValue(0);
        ui->dem_fft_zoomH_SL->setValue(0);
        ui->dem_fft_zoomV_SL->setValue(0);
        ui->dem_raster_zoomH_SL->setValue(0);
        ui->dem_raster_zoomV_SL->setValue(0);
        //Tiempo
        //se plotea la señal en la pestaña general
        dem_graph_tiempo->pintar_coord_hab=true;
        dem_graph_tiempo->signal=nucleo.signal_in;
        //se colocan los ejes horizontales y verticales
        dem_graph_tiempo->x_p1=0;
        dem_graph_tiempo->x_p2=nucleo.signal_length;
        dem_graph_tiempo->y_p1=nucleo.signal_min*2;
        dem_graph_tiempo->y_p2=nucleo.signal_max*2;
        dem_graph_tiempo->diezm_factor=50000;//total de puntos a pintar en la pantalla
        dem_graph_tiempo->fm=nucleo.signal_fm;
        //se actualizan los valores de centro y window para visualizar toda la señal
        dem_tiempo_centerH=nucleo.signal_length/2;
        dem_tiempo_windowH=nucleo.signal_length/2;
        dem_tiempo_centerV=(nucleo.signal_max+nucleo.signal_min)/2;
        dem_tiempo_windowV=nucleo.signal_max-nucleo.signal_min;
         //se reinician el zoom horizontal y vertical
        ui->dem_tiempo_zoomH_SL->setRange(0,nucleo.signal_length/2);//para hacer zoom hasta dos muestras
        ui->dem_tiempo_despH_SB->setRange(0,0);
        ui->dem_tiempo_despV_SB->setRange(0,0);
        dem_graph_tiempo->update();


        //Fourier
        //se plotea el espectro en la pestaña Detectar
        dem_graph_fft->pintar_coord_hab=true;
        dem_graph_fft->signal=nucleo.signal_fft;
        //se colocan los ejes horizontales y verticales
        dem_graph_fft->x_p1=0;
        dem_graph_fft->x_p2=nucleo.signal_fft_length;
        dem_graph_fft->y_p1=nucleo.signal_fft_min;
        dem_graph_fft->y_p2=nucleo.signal_fft_max;
        dem_graph_fft->diezm_factor=20000;//nucleo.signal_fft_length;//total de puntos a pintar en pantalla
        dem_graph_fft->fm=double(2)*double(nucleo.signal_fft_length)/double(nucleo.signal_fm);
        //se actualizan los valores de centro y window para visualizar toda la señal
        dem_fft_centerH=nucleo.signal_fft_length/2;
        dem_fft_windowH=nucleo.signal_fft_length/2;
        dem_fft_centerV=nucleo.signal_fft_min;
        dem_fft_windowV=nucleo.signal_fft_max-nucleo.signal_fft_min;
         //se modifica el zoom horizontal y vertical
        ui->dem_fft_zoomH_SL->setRange(0,nucleo.signal_fft_length/2);//para hacer zoom hasta dos muestras
        ui->dem_fft_despH_SB->setRange(0,0);
        ui->dem_fft_despV_SB->setRange(0,0);
        //se borran las posibles líneas de frecuencia
        if(value!=3)
            dem_graph_fft->signal_lines_length=0;

        //Raster
        dem_graph_raster->bits=nucleo.bits;
        dem_graph_raster->bits_length=nucleo.bits_length;
        dem_raster_despH_range=0;
        dem_raster_despH_habilitar=0;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
        dem_raster_centerH=0;//almacena el centro de la ventana que se visualiza
        dem_raster_windowH=0;//almacena el ancho de la ventana que da el zoom horizontal
        dem_raster_despV_range=0;
        dem_raster_despV_habilitar=0;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
        dem_raster_centerV=0;//almacena el centro de la ventana que se visualiza verticalmente
        dem_raster_windowV=0;//almacena el ancho de la ventana que da el zoom vertical
        ui->dem_raster_despH_SB->setRange(0,0);
        ui->dem_raster_despV_SB->setRange(0,0);
        dem_graph_raster->update();


        //se habilitan controles
        ui->dem_tiempo_zoomH_SL->setDisabled(false);
        ui->dem_tiempo_zoomV_SL->setDisabled(false);
        ui->dem_fft_zoomH_SL->setDisabled(false);
        ui->dem_fft_zoomV_SL->setDisabled(false);
        ui->dem_raster_zoomH_SL->setDisabled(true);
        ui->dem_raster_zoomV_SL->setDisabled(true);
    }


    ///Modificación del panel de control
    //Al abrirse una nueva señal
    if(value==0)
    {
        //Tab General
        ui->gen_inf_GB->setEnabled(true);
        ui->gen_spect_GB->setEnabled(true);
        ui->param_signal_name->setEnabled(true);
        ui->param_fm_L->setEnabled(true);
        ui->param_signal_duracion->setEnabled(true);
        ui->gen_spect_long_vent_L->setEnabled(true);
        ui->gen_spect_solap_L->setEnabled(true);
        ui->gen_spect_vent_L->setEnabled(true);
        ui->spect_window_length_LE->setEnabled(true);
        ui->spect_window_length_SpinB->setEnabled(true);
        ui->spect_window_solap_LE->setEnabled(true);
        ui->spect_solap_SpinB->setEnabled(true);
        ui->spect_color->setEnabled(true);
        ui->spect_PB->setEnabled(true);
        ui->spect_window_CB->setEnabled(true);

        ui->spect_window_length_SpinB->setValue(10);
        ui->spect_solap_SpinB->setValue(nucleo.spect_window_solapamiento);
        nucleo.spect_window_length=1024;
        ui->spect_window_length_LE->setText("1024");
        ui->spect_window_solap_LE->setText(QString("%1").arg(nucleo.spect_window_solapamiento));
        ui->spect_window_CB->clear();
        ui->spect_window_CB->addItem("Rectangular");
        ui->spect_window_CB->addItem("Triangular");
        ui->spect_window_CB->addItem("Hanning");
        ui->spect_window_CB->addItem("Hamming");
        ui->spect_window_CB->addItem("Blackman");
        ui->spect_window_CB->addItem("Flattop");
        ui->spect_window_CB->addItem("Kaiser");

        //se completan visualmente los datos de la señal en el tab de parámetros
        ui->param_fm_L->setText("Frecuencia de muestreo [Hz]:"+ QString("%1").arg(nucleo.signal_fm));
        if(nucleo.signal_hora<10 && nucleo.signal_minutos<10 && nucleo.signal_seg<10)
        {
            ui->param_signal_duracion->setText("Duración [hh:mm:ss]:" + QString(" 0") + QString("%1").arg(nucleo.signal_hora)+ QString(":")+ QString("0") + QString("%1").arg(nucleo.signal_minutos) + QString(":") + QString("0") + QString("%1").arg(nucleo.signal_seg));
        }
        if(nucleo.signal_hora<10 && nucleo.signal_minutos<10 && nucleo.signal_seg>=10)
        {
            ui->param_signal_duracion->setText("Duración [hh:mm:ss]:" + QString(" 0") + QString("%1").arg(nucleo.signal_hora)+ QString(":")+ QString("0") + QString("%1").arg(nucleo.signal_minutos) + QString(":") + QString("%1").arg(nucleo.signal_seg));
        }

        if(nucleo.signal_hora<10 && nucleo.signal_minutos>=10 && nucleo.signal_seg<10)
        {
            ui->param_signal_duracion->setText("Duración [hh:mm:ss]:" + QString(" 0") + QString("%1").arg(nucleo.signal_hora)+ QString(":")+ QString("%1").arg(nucleo.signal_minutos) + QString(":") + QString("0") + QString("%1").arg(nucleo.signal_seg));
        }
        if(nucleo.signal_hora<10 && nucleo.signal_minutos>=10 && nucleo.signal_seg>=10)
        {
            ui->param_signal_duracion->setText("Duración [hh:mm:ss]:" + QString(" 0") + QString("%1").arg(nucleo.signal_hora)+ QString(":")+ QString("%1").arg(nucleo.signal_minutos) + QString(":") + QString("%1").arg(nucleo.signal_seg));
        }
        if(nucleo.signal_hora>=10 && nucleo.signal_minutos<10 && nucleo.signal_seg<10)
        {
            ui->param_signal_duracion->setText("Duración [hh:mm:ss]:" + QString("%1").arg(nucleo.signal_hora)+ QString(":")+ QString("0") + QString("%1").arg(nucleo.signal_minutos) + QString(":") + QString("0") + QString("%1").arg(nucleo.signal_seg));
        }
        if(nucleo.signal_hora>=10 && nucleo.signal_minutos<10 && nucleo.signal_seg>=10)
        {
            ui->param_signal_duracion->setText("Duración [hh:mm:ss]:" + QString("%1").arg(nucleo.signal_hora)+ QString(":")+ QString("0") + QString("%1").arg(nucleo.signal_minutos) + QString(":") + QString("%1").arg(nucleo.signal_seg));
        }

        if(nucleo.signal_hora>=10 && nucleo.signal_minutos>=10 && nucleo.signal_seg<10)
        {
            ui->param_signal_duracion->setText("Duración [hh:mm:ss]:" + QString("%1").arg(nucleo.signal_hora)+ QString(":")+ QString("%1").arg(nucleo.signal_minutos) + QString(":") + QString("0") + QString("%1").arg(nucleo.signal_seg));
        }
        if(nucleo.signal_hora>=10 && nucleo.signal_minutos>=10 && nucleo.signal_seg>=10)
        {
            ui->param_signal_duracion->setText("Duración [hh:mm:ss]:" + QString("%1").arg(nucleo.signal_hora)+ QString(":")+ QString("%1").arg(nucleo.signal_minutos) + QString(":") + QString("%1").arg(nucleo.signal_seg));
        }

        //Tab Detectar
        //Se reajustan los controles
        ui->param_amp_LE->setText(QString("%1").arg(nucleo.signal_amp));
        ui->detect_SS_GB->setDisabled(false);
        ui->detect_filtro_GB->setDisabled(true);
        ui->detect_met_CB->clear();
        ui->detect_met_CB->addItem("Banco de Filtros");
        ui->detect_met_CB->addItem("Detector de Energía");
        detect_signals_CB_hab=false;
        ui->detect_signals_CB->clear();
        detect_signals_CB_hab=true;
        ui->detect_f0_LE->clear();
        ui->detect_f0_SB->setValue(0);
        ui->detect_f1_LE->clear();
        ui->detect_f1_SB->setValue(0);
        //se deshabilitan controles
        detect_f0_hab=false;
        detect_f1_hab=false;
        detect_f0_LE_hab=false;
        detect_f1_LE_hab=false;
        ui->detect_f0_LE->clear();
        ui->detect_f1_LE->clear();
        ui->detect_f0_SB->setValue(0);
        ui->detect_f1_SB->setValue(0);
        ui->detect_filtro_GB->setDisabled(true);
        //se reinician los controles
        detect_f0_hab=true;
        detect_f1_hab=true;
        detect_f0_LE_hab=true;
        detect_f1_LE_hab=true;
        //se borran los rectángulos
        detect_retangles.clear();

        //Tab Demodulación
        ui->dem_freq_CB->setVisible(false);
    }

    //Pestaña Detección
    if(value==1)
    {
        //se redibuja el espectro y los rectángulos correspondientes
        detect_pintar_fft();
    }


    //Pestaña Estimación
    if(value==0 || value==1)
    {

        //se habilitan los controles de la pestaña
        ui->param_amp_LE->setEnabled(true);
        ui->param_amp_L->setEnabled(true);
        ui->param_SNR_LE->setEnabled(true);
        ui->param_SNR_L->setEnabled(true);
        ui->param_baud_LE->setEnabled(true);
        ui->param_baud_rate_L->setEnabled(true);
        //se completan valores
        ui->param_amp_LE->setText(QString("%1").arg(nucleo.signal_amp));//se visualiza la amplitud estimada;
        ui->param_SNR_LE->setText(QString("%1").arg(nucleo.signal_SNR));//se visualiza el SNR estimado;
        ui->param_baud_LE->setText(QString("%1").arg(nucleo.signal_baud_rate));//se visualiza el baud rate estimado;

        //se completa el CB de clasificación
        //se vacía el comboBox de los métodos
        clasif_modu_habilitar=false;
        ui->clasif_resultado_CB->clear();        
        ui->clasif_metodos_CB->clear();        
        ui->clasif_metodos_CB->addItem("Árbol de decisiones");
        ui->clasif_metodos_CB->addItem("Cruces por cero");
        ui->clasif_metodos_CB->setCurrentIndex(1);//se inicializa en el método de cruces por cero, funciona en la mayoría de los casos
        ui->clasif_mod_GB->setDisabled(false);
        ui->clasif_PB->setDisabled(false);//se habilita el botón de clasificar
        ui->clasif_resultado_CB->setDisabled(false);
        ui->clasif_const_CB->clear();
        ui->clasif_resultado_CB->addItem("");
        ui->clasif_resultado_CB->addItem("BFSK");
        ui->clasif_resultado_CB->addItem("4FSK");
        ui->clasif_resultado_CB->addItem("8FSK");
        ui->clasif_resultado_CB->addItem("16FSK");
        ui->clasif_resultado_CB->addItem("BPSK");
        ui->clasif_resultado_CB->addItem("QPSK");
        ui->clasif_resultado_CB->addItem("DQPSK");
        ui->clasif_resultado_CB->addItem("OQPSK");
        ui->clasif_resultado_CB->addItem("8PSK");
        ui->clasif_resultado_CB->addItem("QAM");

        //se habilitan los resultados de clasificación
        ui->clasif_resultado_CB->setCurrentIndex(0);
        clasif_resultados_habilitar=true;
        clasif_modu_habilitar=true;
        //se borran los métdodos de obtención de la constelación
        clasif_const_hab=false;
        ui->clasif_const_CB->clear();
        clasif_const_hab=true;

        //Constelación
        if(nucleo.signal_I_length!=0)
        {
            delete []nucleo.signal_I;
            delete []nucleo.signal_Q;
        }
        nucleo.signal_I_length=0;
        nucleo.clasif_decision=0;
        ui->clasif_mod_GB->setDisabled(false);
        ui->clasif_const_lines->setChecked(false);
        clasif_graph_const->const_lines=false;
        ui->clasif_const_lines->setDisabled(true);

        //se borra la posible constelación
        if(nucleo.signal_I_length!=0)
        {
            delete[] nucleo.signal_I;
            delete[] nucleo.signal_Q;
            nucleo.signal_I_length=0;
            clasif_graph_const->signal_I_length=0;
        }
        clasif_graph_const->repaint();

        ///se actualiza el espectrograma
        //se borra el anterior espectrograma
        if(nucleo.signal_spect_length!=0)
            delete[] nucleo.signal_spect;
        //se recalcula el espectrograma
        nucleo.nucleo_spect_funct();
        //Tab General
        //se reajustan los controles
        gen_spect_despH_habilitar=false;
        gen_spect_despV_habilitar=false;
        gen_spect_centerH=nucleo.spect_window_length/4;
        gen_spect_windowH=nucleo.spect_window_length/8;
        gen_spect_centerV=nucleo.signal_spect_length/2;
        gen_spect_windowV=nucleo.signal_spect_length/4;
        ui->gen_spect_despH_SB->setRange(0,0);
        ui->gen_spect_despV_SB->setRange(0,0);
        ui->gen_spect_zoomH_SL->setValue(0);
        ui->gen_spect_zoomV_SL->setValue(0);
        gen_graph_spect->x1=0;
        gen_graph_spect->x2=nucleo.spect_window_length/2;
        gen_graph_spect->y1=0;
        gen_graph_spect->y2=nucleo.signal_spect_length;
        gen_graph_spect->spect_max=nucleo.spect_max;
        gen_graph_spect->spect=nucleo.signal_spect;
        gen_graph_spect->spect_length=nucleo.signal_spect_length;
        gen_spect_despH_habilitar=true;
        gen_spect_despV_habilitar=true;
        //se redibuja
        gen_graph_spect->repaint();
        //Tab Detectar
        //se reajustan los controles
        detect_spect_despH_habilitar=false;
        detect_spect_despV_habilitar=false;
        detect_spect_centerH=nucleo.spect_window_length/4;
        detect_spect_windowH=nucleo.spect_window_length/8;
        detect_spect_centerV=nucleo.signal_spect_length/2;
        detect_spect_windowV=nucleo.signal_spect_length/4;
        ui->detect_spect_despH_SB->setRange(0,0);
        ui->detect_spect_despV_SB->setRange(0,0);
        ui->detect_spect_zoomH_SL->setValue(0);
        ui->detect_spect_zoomV_SL->setValue(0);
        detect_graph_spect->x1=0;
        detect_graph_spect->x2=nucleo.spect_window_length/2;
        detect_graph_spect->y1=0;
        detect_graph_spect->y2=nucleo.signal_spect_length;
        detect_graph_spect->spect_max=nucleo.spect_max;
        detect_graph_spect->spect=nucleo.signal_spect;
        detect_graph_spect->spect_length=nucleo.signal_spect_length;
        detect_spect_despH_habilitar=true;
        detect_spect_despV_habilitar=true;
        //se redibuja
        detect_graph_spect->repaint();
        //Tab Estimar
        //se reajustan los controles
        clasif_spect_despH_habilitar=false;
        clasif_spect_despV_habilitar=false;
        clasif_spect_centerH=nucleo.spect_window_length/4;
        clasif_spect_windowH=nucleo.spect_window_length/8;
        clasif_spect_centerV=nucleo.signal_spect_length/2;
        clasif_spect_windowV=nucleo.signal_spect_length/4;
        ui->clasif_spect_despH_SB->setRange(0,0);
        ui->clasif_spect_despV_SB->setRange(0,0);
        ui->clasif_spect_zoomH_SL->setValue(0);
        ui->clasif_spect_zoomV_SL->setValue(0);
        clasif_graph_spect->x1=0;
        clasif_graph_spect->x2=nucleo.spect_window_length/2;
        clasif_graph_spect->y1=0;
        clasif_graph_spect->y2=nucleo.signal_spect_length;
        clasif_graph_spect->spect_max=nucleo.spect_max;
        clasif_graph_spect->spect=nucleo.signal_spect;
        clasif_graph_spect->spect_length=nucleo.signal_spect_length;
        clasif_spect_despH_habilitar=true;
        clasif_spect_despV_habilitar=true;
        //se redibuja
        clasif_graph_spect->repaint();
        //this->on_spect_PB_clicked();

        //se ocultan los opciones de constelación
        ui->clasif_const_CB->setEnabled(false);
        ui->clasif_L3->setEnabled(false);
    }

    //Pestaña Demodular
    if(value==0 || value==1 || value==2)
    {
        //se desactiva el botón demodular
        dem_PB_habilitar=false;
        ui->dem_PB->setEnabled(false);
        dem_PB_habilitar=true;
        //se esconde el panel de frecuencia
        dem_f0_SB_hab=false;
        ui->dem_freq_GB->setDisabled(true);
        dem_f0_SB_hab=true;

        //se borra la posible modulación
        if(nucleo.signal_dem_length!=0)
        {
            delete[] nucleo.signal_dem;
            nucleo.signal_dem_length=0;
            ui->dem_metodos_CB->clear();
            //dem_graph_tiempo->signal_2_length=0;

        }

        //se vacía el comboBox de los métodos de demodulación
        ui->dem_L->setDisabled(false);
        if(ui->dem_metodos_CB->count()!=0)
        {
            int total=ui->dem_metodos_CB->count();
            for(int i=1;i<=total;i++)
            {
                ui->dem_metodos_CB->removeItem(total-i);
            }
        }

        //se borra la señal demodulada
        if(dem_graph_tiempo->signal_2_length!=0)
        {
            dem_graph_tiempo->signal_2_length=0;
        }

        //se borra la señal demodulada
        if(dem_graph_tiempo->signal_3_length!=0)
        {
            dem_graph_tiempo->signal_3_length=0;
        }

        //se actualizan los controles de demodulación
        ui->dem_metodos_CB->setDisabled(true);
        ui->dem_L->setDisabled(true);

        //se reinician los valores de frecuencia
        ui->dem_freq_GB->setDisabled(true);
        ui->dem_f0_L->setVisible(true);
        ui->dem_f0_LE->setVisible(true);
        ui->dem_f0_SB->setVisible(true);
        ui->dem_f0_LE->setText("");
        ui->dem_f1_L->setText("f1 [Hz]:");
        ui->dem_f1_LE->setText("");
        dem_f0_SB_hab=false;
        ui->dem_f0_SB->setValue(0);
        ui->dem_f1_SB->setValue(0);
        dem_f0_SB_hab=true;
        ui->dem_exportar_PB->setDisabled(true);
        ui->dem_freq_CB->setVisible(false);
        ui->dem_freq_L->setVisible(false);
        ui->dem_freq_CB->setVisible(false);

        //se inicializan los valores de frecuencia
        ui->dem_f0_LE->setText("");
        ui->dem_f1_LE->setText("");
        dem_f0_SB_hab=false;//para que no modifique el valor de f0
        ui->dem_f0_SB->setValue(0);
        dem_f0_SB_hab=true;
        ui->dem_f1_SB->setValue(0);
        ui->dem_f0_SB->setRange(0,nucleo.signal_fft_length);
        ui->dem_f1_SB->setRange(0,nucleo.signal_fft_length);


        //se borra el posible raster
        nucleo.patron=0;
        if(nucleo.bits_length!=0)
        {
            nucleo.bits_length=0;
            delete []nucleo.bits;
            dem_graph_raster->bits_length=0;//se borra el posible raster
            nucleo.patron=0;
            nucleo.patrones.clear();
            ui->dem_raster_GB->setDisabled(true);
            dem_patrones_CB_hab=false;
            ui->dem_raster_LE->clear();
            ui->dem_raster_CB->clear();
        }

        //se vacía el LineEdit del raster
        ui->dem_raster_LE->clear();
        dem_patrones_CB_hab=false;
        //se vacía el comboBox de los métodos
        if(ui->dem_raster_CB->count()!=0)
        {
            int total=ui->dem_raster_CB->count();
            for(int i=1;i<=total;i++)
            {
                ui->dem_raster_CB->removeItem(total-i);
            }
        }
        ui->dem_raster_GB->setDisabled(true);
        ui->dem_exportar_PB->setDisabled(true);

        ui->dem_cod_L->setVisible(false);
        ui->dem_cod_CB->setVisible(false);
    }

    //Arreglo de la constelación y Pestaña Demodular por cambio del tipo de modulación
    if(value==2)
    {
        //se borran los métodos de obtención de la constelación
        clasif_const_hab=false;
        ui->clasif_const_CB->clear();
        ui->clasif_const_CB->setDisabled(true);
        ui->clasif_L3->setDisabled(true);
        clasif_const_hab=true;


        //Clasificación BFSK
        if(nucleo.clasif_decision==1)
        {
            //se rellena el comboBox de los métodos de demodulación
            ui->dem_metodos_CB->addItem("Poligonal",1);
            //ui->dem_metodos_CB->addItem("Detector-Envolvente Eigenvalue",1);
            ui->dem_metodos_CB->addItem("Detector-Envolvente Filanovsky",1);
            ui->dem_metodos_CB->addItem("Detector-Envolvente Kaiser",1);
            ui->dem_metodos_CB->addItem("Diferenciador",1);
            ui->dem_metodos_CB->addItem("Cuadricorrelador Balanceado",1);
            ui->dem_metodos_CB->addItem("Cuadricorrelador",1);
            //ui->dem_metodos_CB->addItem("Detector de Envolvente PL",1);
            ui->dem_metodos_CB->setDisabled(false);
            ui->dem_PB->setDisabled(false);
            ui->dem_L->setDisabled(false);

            //se muestran los controles de la 2da frecuencia
            ui->dem_f1_L->setDisabled(false);
            ui->dem_f1_LE->setDisabled(false);
            ui->dem_f1_SB->setDisabled(false);
            ui->dem_freq_GB->setDisabled(false);//se muestra el panel de frecuencia

            //se determina el primer máximo de la FFT
            double signal_fft_max=0;
            for(int i=0; i<nucleo.signal_fft_length; i++)
            {
                if(signal_fft_max<nucleo.signal_fft[i])//se calcula el máximo y la posición de la FFT
                {
                    signal_fft_max=nucleo.signal_fft[i];
                    nucleo.signal_fft_max_pos=i;
                }
            }

            //se detecta la posición del 2do máximo
            nucleo.signal_fft_max_2=0;

            int rango=float(nucleo.signal_baud_rate)/float(nucleo.signal_fm)*float(nucleo.signal_fft_length)/2;
            for(int i=0;i<nucleo.signal_fft_length;i++)
            {
                if(nucleo.signal_fft[i]>nucleo.signal_fft_max_2)
                {
                    if(qAbs(i-nucleo.signal_fft_max_pos)>rango)//garantiza estar fuera del lóbulo principal del primer máximo
                    {
                        nucleo.signal_fft_max_2=nucleo.signal_fft[i];
                        nucleo.signal_fft_max_pos_2=i;
                    }
                }
            }

            //se colocan los valores de frecuencia en los LineEdit de la pestaña demodular
            ui->dem_f0_LE->setText(QString("%1").arg(float(nucleo.signal_fft_max_pos)*float(nucleo.signal_fm/2)/float(nucleo.signal_fft_length)));
            ui->dem_f1_LE->setText(QString("%1").arg(float(nucleo.signal_fft_max_pos_2)*float(nucleo.signal_fm/2)/float(nucleo.signal_fft_length)));

            //se coloca la posición del despH de ambas frecuencias
            dem_f0_SB_hab=false;//se detiene la conección
            ui->dem_f0_SB->setValue(nucleo.signal_fft_max_pos);
            ui->dem_f1_SB->setValue(nucleo.signal_fft_max_pos_2);
            ui->dem_f0_LE->setText(QString("%1").arg(float(nucleo.signal_fft_max_pos)*float(nucleo.signal_fm/2)/float(nucleo.signal_fft_length)));
            dem_f0_SB_hab=true;
            //ui->dem_f0_SB->setEnabled(true);

            //se dibujan las líneas
            dem_graph_fft->signal_lines=new int[2];
            dem_graph_fft->signal_lines_length=2;

            dem_graph_fft->signal_lines[0]=nucleo.signal_fft_max_pos;
            dem_graph_fft->signal_lines[1]=nucleo.signal_fft_max_pos_2;
            dem_graph_fft->signal_lines_selecc=0;
            dem_graph_fft->repaint();

            //se deshabilitan los controles gráficos de la constelación
            ui->clasif_const_zoomH_SL->setValue(0);
            ui->clasif_const_zoomV_SL->setValue(0);
            ui->clasif_const_zoomH_SL->setDisabled(true);
            ui->clasif_const_zoomV_SL->setDisabled(true);
        }

        //Clasificación 4FSK, 8FSK, 16FSK
        if(nucleo.clasif_decision>=2 && nucleo.clasif_decision<=4)
        {
            //se modifican los controles de frecuencia de la pestaña demodular
            ui->dem_f0_L->setVisible(false);
            ui->dem_f0_LE->setVisible(false);
            ui->dem_f0_SB->setVisible(false);
            ui->dem_freq_L->setVisible(true);
            ui->dem_freq_CB->setVisible(true);
            ui->dem_f1_L->setText("f0 [Hz]:");

            //se borra el comboBox de frecuencias
            dem_freq_CB_hab=false;
            ui->dem_freq_CB->clear();
            dem_freq_CB_hab=true;

            //se activan el Combo Box de métodos de demodulación y el botón de Demodular
            ui->dem_metodos_CB->setEnabled(true);
            ui->dem_metodos_CB->addItem("Detector de Envolvente",1);
            ui->dem_PB->setEnabled(true);
            ui->dem_L->setEnabled(true);

        }

        // Lineas de frecuencia
        //Clasificación 4FSK
        if(nucleo.clasif_decision==2)
        {
            //se completa el comboBox de frecuencias
            dem_freq_CB_hab=false;
            ui->dem_freq_CB->addItem("f0",1);
            ui->dem_freq_CB->addItem("f1",1);
            ui->dem_freq_CB->addItem("f2",1);
            ui->dem_freq_CB->addItem("f3",1);
            dem_freq_CB_hab=true;
            ui->dem_freq_CB->setEnabled(true);
            ui->dem_freq_L->setEnabled(true);
            ui->dem_f1_L->setEnabled(true);
            ui->dem_f1_LE->setEnabled(true);
            ui->dem_f1_SB->setEnabled(true);
            ui->dem_freq_GB->setEnabled(true);

            //se asocian las líneas de frecuencia en el gráfico de FFT
            dem_graph_fft->signal_lines_selecc=ui->dem_freq_CB->currentIndex();
            dem_graph_fft->signal_lines_length=nucleo.detect_freq_length;
            dem_graph_fft->signal_lines=nucleo.dem_freq_array;
            dem_graph_fft->signal_lines_selecc=0;
            dem_graph_fft->repaint();


        }

        //Clasificación 8FSK
        if(nucleo.clasif_decision==3)
        {
            //se completa el comboBox de frecuencias
            dem_freq_CB_hab=false;
            ui->dem_freq_CB->addItem("f0",1);
            ui->dem_freq_CB->addItem("f1",1);
            ui->dem_freq_CB->addItem("f2",1);
            ui->dem_freq_CB->addItem("f3",1);
            ui->dem_freq_CB->addItem("f4",1);
            ui->dem_freq_CB->addItem("f5",1);
            ui->dem_freq_CB->addItem("f6",1);
            ui->dem_freq_CB->addItem("f7",1);
            dem_freq_CB_hab=true;
            ui->dem_freq_CB->setEnabled(true);
            ui->dem_freq_L->setEnabled(true);
            ui->dem_f1_L->setEnabled(true);
            ui->dem_f1_LE->setEnabled(true);
            ui->dem_f1_SB->setEnabled(true);
            ui->dem_freq_GB->setEnabled(true);            

            //se asocian las líneas de frecuencia en el gráfico de FFT
            dem_graph_fft->signal_lines_length=nucleo.detect_freq_length;
            dem_graph_fft->signal_lines=nucleo.dem_freq_array;
            dem_graph_fft->signal_lines_selecc=0;
            dem_graph_fft->repaint();


        }

        //Clasificación 16FSK
        if(nucleo.clasif_decision==4)
        {
            //se completa el comboBox de frecuencias
            dem_freq_CB_hab=false;
            ui->dem_freq_CB->addItem("f0",1);
            ui->dem_freq_CB->addItem("f1",1);
            ui->dem_freq_CB->addItem("f2",1);
            ui->dem_freq_CB->addItem("f3",1);
            ui->dem_freq_CB->addItem("f4",1);
            ui->dem_freq_CB->addItem("f5",1);
            ui->dem_freq_CB->addItem("f6",1);
            ui->dem_freq_CB->addItem("f7",1);
            ui->dem_freq_CB->addItem("f8",1);
            ui->dem_freq_CB->addItem("f9",1);
            ui->dem_freq_CB->addItem("f10",1);
            ui->dem_freq_CB->addItem("f11",1);
            ui->dem_freq_CB->addItem("f12",1);
            ui->dem_freq_CB->addItem("f13",1);
            ui->dem_freq_CB->addItem("f14",1);
            ui->dem_freq_CB->addItem("f15",1);
            dem_freq_CB_hab=true;
            ui->dem_freq_CB->setEnabled(true);
            ui->dem_freq_L->setEnabled(true);
            ui->dem_f1_L->setEnabled(true);
            ui->dem_f1_LE->setEnabled(true);
            ui->dem_f1_SB->setEnabled(true);
            ui->dem_freq_GB->setEnabled(true);


            //se asocian las líneas de frecuencia en el gráfico de FFT
            dem_graph_fft->signal_lines_length=nucleo.detect_freq_length;
            dem_graph_fft->signal_lines=nucleo.dem_freq_array;
            dem_graph_fft->signal_lines_selecc=0;
            dem_graph_fft->repaint();
        }


        //si es PSK o QAM se estima y se muestra el valor de frecuencia
        if(nucleo.clasif_decision>=5 && nucleo.clasif_decision<=10)
        {
            //Si es PSK se recalcula la frecuencia de la portadora mediante la determinación de sus máximos y mínimos
            //se recalcula una frecuencia para la constelación y otra para la demodulación

            //Para la constelación
            //se detecta un nuevo valor de frecuencia por el momento de 4to orden de la FFT
            //se crea el eje de frecuencia
            double *freq = new double[nucleo.signal_fft_length];
            for(int i=0; i<nucleo.signal_fft_length; i++)
            {
                freq[i]=double(i)*double(nucleo.signal_fm/2)/double(nucleo.signal_fft_length);
            }
            //se calcula el momento de 4to orden
            double cumul_f_num=0;
            double cumul_f_den=0;
            for(int i=0; i<nucleo.signal_fft_length; i++)
            {
                cumul_f_num+=freq[i]*qPow(nucleo.signal_fft[i],4);
                cumul_f_den+=qPow(nucleo.signal_fft[i],4);
            }

            double f0=cumul_f_num/cumul_f_den;
            //se activa el valor de frecuencia para determinar la constelación
            nucleo.signal_fft_max_pos_const=f0/double(nucleo.signal_fm/2)*nucleo.signal_fft_length;

            //Para la demodulación
            int period_point=0;
            int k=0;//almacena el cruce por cero anterior
            int total=0;
            double p1,p2,p3;
            for(int i=1;i<(nucleo.signal_length-1);i++)
            {
                p1=nucleo.signal_in[i-1]*nucleo.signal_in[i-1];
                p2=nucleo.signal_in[i]*nucleo.signal_in[i];
                p3=nucleo.signal_in[i+1]*nucleo.signal_in[i+1];

                if(p1<=p2 && p2>p3)
                {
                    period_point=period_point+i-k;
                    k=i;
                    total=total+1;
                }
            }

            double signal_fft_max=double(nucleo.signal_fm)*double(total)/double(period_point)/double(2);
            nucleo.signal_fft_max_pos=double(signal_fft_max)*double(nucleo.signal_fft_length)/double(nucleo.signal_fm)*double(2);

            float value=nucleo.signal_fft_max_pos/float(nucleo.signal_fft_length)*float(nucleo.signal_fm/2);
            ui->dem_f0_LE->setText(QString("%1").arg(value));
            ui->dem_freq_GB->setDisabled(false);//se muestra el panel de frecuencia
            ui->dem_f1_L->setDisabled(true);
            ui->dem_f1_LE->setDisabled(true);
            ui->dem_f1_SB->setDisabled(true);

            //se coloca la posición del despH de la frecuencia f0
            dem_f0_SB_hab=false;//se detiene la conección
            ui->dem_f0_SB->setValue(nucleo.signal_fft_max_pos);
            dem_f0_SB_hab=true;

            //se dibujan las líneas
            dem_graph_fft->signal_lines=new int[1];
            dem_graph_fft->signal_lines_length=1;

            dem_graph_fft->signal_lines[0]=nucleo.signal_fft_max_pos;
            dem_graph_fft->signal_lines_selecc=0;
            dem_graph_fft->repaint();
        }

        //Combo Box de Código
        //se borra el comboBox de Código
        dem_cod_CB_hab=false;
        ui->dem_cod_CB->clear();
        dem_cod_CB_hab=true;

        //4FSK, QPSK, OQPSK
        if(nucleo.clasif_decision==2 || nucleo.clasif_decision==6 || nucleo.clasif_decision==8)
        {
            ui->dem_cod_L->setVisible(true);
            ui->dem_cod_L->setEnabled(false);
            ui->dem_cod_CB->setVisible(true);
            ui->dem_cod_CB->setEnabled(false);
            dem_cod_CB_hab=false;
            ui->dem_cod_CB->addItem("00-01-11-10");
            ui->dem_cod_CB->addItem("10-00-01-11");
            ui->dem_cod_CB->addItem("11-10-00-01");
            ui->dem_cod_CB->addItem("01-11-10-00");
            dem_cod_CB_hab=true;
        }

        //4FSK, QPSK, OQPSK
        if(nucleo.clasif_decision==7)
        {
            ui->dem_cod_L->setVisible(true);
            ui->dem_cod_L->setEnabled(false);
            ui->dem_cod_CB->setVisible(true);
            ui->dem_cod_CB->setEnabled(false);
            dem_cod_CB_hab=false;
            ui->dem_cod_CB->addItem("(0->00)-(pi/2->01)-(3pi/2->11)-(5pi/2->10)");
            ui->dem_cod_CB->addItem("(0->10)-(pi/2->00)-(3pi/2->01)-(5pi/2->11)");
            ui->dem_cod_CB->addItem("(0->11)-(pi/2->10)-(3pi/2->00)-(5pi/2->01)");
            ui->dem_cod_CB->addItem("(0->01)-(pi/2->11)-(3pi/2->10)-(5pi/2->00)");
            dem_cod_CB_hab=true;
        }

        //8FSK, 8PSK
        if(nucleo.clasif_decision==3 || nucleo.clasif_decision==9)
        {
            //se activa y completa el comboBox de Código
            ui->dem_cod_L->setVisible(true);
            ui->dem_cod_L->setEnabled(false);
            ui->dem_cod_CB->setVisible(true);
            ui->dem_cod_CB->setEnabled(false);
            dem_cod_CB_hab=false;
            ui->dem_cod_CB->addItem("000-001-011-010-110-111-101-100");
            ui->dem_cod_CB->addItem("100-000-001-011-010-110-111-101");
            ui->dem_cod_CB->addItem("101-100-000-001-011-010-110-111");
            ui->dem_cod_CB->addItem("111-101-100-000-001-011-010-110");
            ui->dem_cod_CB->addItem("110-111-101-100-000-001-011-010");
            ui->dem_cod_CB->addItem("010-110-111-101-100-000-001-011");
            ui->dem_cod_CB->addItem("011-010-110-111-101-100-000-001");
            ui->dem_cod_CB->addItem("001-011-010-110-111-101-100-000");
            dem_cod_CB_hab=true;
        }

        //16FSK
        if(nucleo.clasif_decision==4)
        {
            //se activa y completa el comboBox de Código
            ui->dem_cod_L->setVisible(true);
            ui->dem_cod_L->setEnabled(false);
            ui->dem_cod_CB->setVisible(true);
            ui->dem_cod_CB->setEnabled(false);
            dem_cod_CB_hab=false;
            ui->dem_cod_CB->addItem("0000-0001-0011-0010-0110-0111-0101-0100-1100-1101-1111-1110-1010-1011-1001-1000");
            ui->dem_cod_CB->addItem("0001-0011-0010-0110-0111-0101-0100-1100-1101-1111-1110-1010-1011-1001-1000-0000");
            ui->dem_cod_CB->addItem("0011-0010-0110-0111-0101-0100-1100-1101-1111-1110-1010-1011-1001-1000-0000-0001");
            ui->dem_cod_CB->addItem("0010-0110-0111-0101-0100-1100-1101-1111-1110-1010-1011-1001-1000-0000-0001-0011");
            ui->dem_cod_CB->addItem("0110-0111-0101-0100-1100-1101-1111-1110-1010-1011-1001-1000-0000-0001-0011-0010");
            ui->dem_cod_CB->addItem("0111-0101-0100-1100-1101-1111-1110-1010-1011-1001-1000-0000-0001-0011-0010-0110");
            ui->dem_cod_CB->addItem("0101-0100-1100-1101-1111-1110-1010-1011-1001-1000-0000-0001-0011-0010-0110-0111");
            ui->dem_cod_CB->addItem("0100-1100-1101-1111-1110-1010-1011-1001-1000-0000-0001-0011-0010-0110-0111-0101");
            ui->dem_cod_CB->addItem("1100-1101-1111-1110-1010-1011-1001-1000-0000-0001-0011-0010-0110-0111-0101-0100");
            ui->dem_cod_CB->addItem("1101-1111-1110-1010-1011-1001-1000-0000-0001-0011-0010-0110-0111-0101-0100-1100");
            ui->dem_cod_CB->addItem("1111-1110-1010-1011-1001-1000-0000-0001-0011-0010-0110-0111-0101-0100-1100-1101");
            ui->dem_cod_CB->addItem("1110-1010-1011-1001-1000-0000-0001-0011-0010-0110-0111-0101-0100-1100-1101-1111");
            ui->dem_cod_CB->addItem("1010-1011-1001-1000-0000-0001-0011-0010-0110-0111-0101-0100-1100-1101-1111-1110");
            ui->dem_cod_CB->addItem("1011-1001-1000-0000-0001-0011-0010-0110-0111-0101-0100-1100-1101-1111-1110-1010");
            ui->dem_cod_CB->addItem("1001-1000-0000-0001-0011-0010-0110-0111-0101-0100-1100-1101-1111-1110-1010-1011");
            ui->dem_cod_CB->addItem("1000-0000-0001-0011-0010-0110-0111-0101-0100-1100-1101-1111-1110-1010-1011-1001");
            dem_cod_CB_hab=true;
        }

        //Clasificación BPSK
        if(nucleo.clasif_decision==5)
        {
            //se rellena el comboBox de los métodos de demodulación
            ui->dem_metodos_CB->addItem("Lazo Cuadrático",1);
            ui->dem_metodos_CB->addItem("Lazo de Costas",1);
            ui->dem_metodos_CB->setDisabled(false);
            ui->dem_PB->setDisabled(false);
            ui->dem_L->setDisabled(false);

            //se habilitan controles la opción de dibujar líneas
            ui->clasif_const_lines->setEnabled(true);
            ui->clasif_L2->setDisabled(false);
            ui->clasif_L3->setDisabled(false);
            ui->clasif_const_CB->setDisabled(false);
            clasif_const_lines_hab=true;


            //se calcula la constelación
            clasif_const_hab=false;            
            ui->clasif_const_CB->clear();
            ui->clasif_const_CB->addItem(" ",1);
            ui->clasif_const_CB->addItem("Lazo Cuadrático",1);
            ui->clasif_const_CB->addItem("Lazo Costas",1);
            clasif_const_hab=true;
            ui->clasif_const_CB->setCurrentIndex(1);
        }


        //QPSK
        if(nucleo.clasif_decision>=6 && nucleo.clasif_decision<=9)
        {
            //Demodulación
            dem_cod_CB_hab=false;
            dem_PB_habilitar=false;
            //se rellena el comboBox de los métodos de demodulación
            ui->dem_metodos_CB->addItem("Lazo de Costas",1);
            ui->dem_metodos_CB->setDisabled(false);
            ui->dem_PB->setDisabled(false);
            ui->dem_L->setDisabled(false);
            dem_PB_habilitar=true;

            //Constelación QPSK, DQPSK, OQPSK
            if(nucleo.clasif_decision!=9)
            {
                //Constelación
                //se habilitan controles la opción de dibujar líneas
                ui->clasif_const_lines->setEnabled(true);
                ui->clasif_L2->setDisabled(false);
                ui->clasif_L3->setDisabled(false);
                ui->clasif_const_CB->setDisabled(false);
                clasif_const_lines_hab=true;

                //se calcula la constelación
                clasif_const_hab=false;
                ui->clasif_const_CB->clear();
                ui->clasif_const_CB->addItem(" ",1);
                ui->clasif_const_CB->addItem("Lazo Cuadrático",1);
                ui->clasif_const_CB->addItem("Lazo Costas",1);
                clasif_const_hab=true;
                ui->clasif_const_CB->setCurrentIndex(1);
            }
        }

    }



    if(value==3)
    {
        //se deshabilitan los controles del raster
        ui->dem_raster_GB->setEnabled(false);
        dem_patrones_CB_hab=false;
        ui->dem_raster_CB->clear();
        dem_patrones_CB_hab=true;
        ui->dem_raster_LE->clear();

    }


    //accionado por la selección de frecuencia en la pestaña Demodular
    if(value==4)
    {
        //se asocian las líneas de frecuencia en el gráfico de FFT
        dem_graph_fft->signal_lines_length=nucleo.detect_freq_length;
        dem_graph_fft->signal_lines=nucleo.dem_freq_array;
        dem_graph_fft->signal_lines_selecc=ui->dem_freq_CB->currentIndex();
        dem_graph_fft->repaint();

        //se modifica el label de frecuencia
        ui->dem_f1_L->setText("f"+QString("%1").arg(ui->dem_freq_CB->currentIndex())+" Hz:");

        //se edita el line edit con el valor de frecuencia respectiva
        int index=ui->dem_freq_CB->currentIndex();
        float freq_f=float(nucleo.dem_freq_array[index])/float(nucleo.signal_fft_length)*float(nucleo.signal_fm)/float(2);
        ui->dem_f1_LE->setText(QString("%1").arg(freq_f));

        //Se edita el SB con el valor de frecuencia respectiva
        dem_f0_SB_hab=false;
        int freq_pos=float(nucleo.dem_freq_array[index])/float(nucleo.signal_fft_length)*float(ui->dem_f1_SB->maximum());
        ui->dem_f1_SB->setValue(freq_pos);
        dem_f0_SB_hab=true;

    }

}



void MainWindow::detect_pintar_fft()
{
    detect_graph_fft_scene->clear();

    //se confecciona la escena
    QPen painter;
    painter.setBrush(Qt::green);
    painter.setStyle(Qt::SolidLine);
    painter.setCapStyle(Qt::SquareCap);
    painter.setJoinStyle(Qt::MiterJoin);

    //se plotea el espectro en la pestaña Detectar
    int diezm=nucleo.signal_fft_length/40000;
    if (diezm==0)
        diezm=1;
    QPolygonF polygon(nucleo.signal_fft_length-diezm);
    int x1=0;
    int y1=0;
    //se detecta el máximo de esta transformada
    float max_fft=nucleo.signal_fft_max;
    //se construye el polígono
    int width=ui->detect_graph_fft_GV->width();
    int height=ui->detect_graph_fft_GV->height();
    for(int i=0; i<nucleo.signal_fft_length-diezm; i+=diezm)
    {

        x1=(float(i)/float(nucleo.signal_fft_length-diezm))*float(width);
        y1=-(float(nucleo.signal_fft[i])/float(max_fft))*float(height);

        polygon << QPointF(x1,y1);


    }

    detect_graph_fft_scene->addPolygon(polygon,painter);

    //se plotean los rectángulos
    detect_retangles.clear();
    //se sombrean los rangos de frecuencia
    x1=0;
    float x2=0;
    width=ui->detect_graph_fft_GV->width()-5;
    height=ui->detect_graph_fft_GV->height()-40;

    painter.setStyle(Qt::SolidLine);
    painter.setColor(Qt::white);
    QBrush brush;
    brush.setStyle(Qt::Dense7Pattern);
    brush.setColor(Qt::white);

    QRectF rectangle;

    for(int i=1; i<=nucleo.detect_freq_length; i+=2)
    {
        x1=float(nucleo.detect_freq[i-1])/float(nucleo.signal_fft_length)*float(width);
        x2=float(nucleo.detect_freq[i])/float(nucleo.signal_fft_length)*float(width);
        detect_retangles.push_back(detect_graph_fft_scene->addRect(x1,-height,x2-x1+1,height,painter,brush));
    }

    //se redibuja en rojo el rectángulo correspondiente
    if(ui->detect_signals_CB->currentIndex()!=0)
    {
        painter.setStyle(Qt::SolidLine);
        painter.setColor(Qt::red);
        brush.setStyle(Qt::Dense7Pattern);
        brush.setColor(Qt::red);
        detect_retangles[ui->detect_signals_CB->currentIndex()-1]->setPen(painter);
        detect_retangles[ui->detect_signals_CB->currentIndex()-1]->setBrush(brush);
    }


    else
    {
        painter.setStyle(Qt::SolidLine);
        painter.setColor(Qt::white);
        brush.setStyle(Qt::Dense7Pattern);
        brush.setColor(Qt::white);
        for(int i=0;i<detect_retangles.size();i++)
        {
            detect_retangles[i]->setPen(painter);
            detect_retangles[i]->setBrush(brush);

        }
    }

    //se plotean las coordenadas

    painter.setColor(Qt::gray);
    painter.setStyle(Qt::DashLine);
    painter.setCapStyle(Qt::SquareCap);
    painter.setJoinStyle(Qt::MiterJoin);
    width=ui->detect_graph_fft_GV->width();
    height=ui->detect_graph_fft_GV->height()-10;

    //líneas horizontales
    double y2=-double(height)/double(2);
    y1=y2+double(height)/double(4);
    double y3=y2-double(height)/double(4);
    detect_graph_fft_scene->addLine(0,y1,width,y1,painter);
    detect_graph_fft_scene->addLine(0,y2,width,y2,painter);
    detect_graph_fft_scene->addLine(0,y3,width,y3,painter);
    //líneas verticales
    x2=double(width)/double(2);
    x1=x2+double(width)/double(4);
    double x3=x2-double(width)/double(4);
    detect_graph_fft_scene->addLine(x1,0,x1,-height,painter);
    detect_graph_fft_scene->addLine(x2,0,x2,-height,painter);
    detect_graph_fft_scene->addLine(x3,0,x3,-height,painter);

    //texto
    //texto horizontal
    double y3_value=nucleo.signal_fft_max*double(3)/double(4);
    QGraphicsTextItem *text1=detect_graph_fft_scene->addText(QString("%1").arg(y3_value));
    text1->setDefaultTextColor(Qt::white);
    text1->setPos(0,y3-17);
    double y2_value=nucleo.signal_fft_max/double(2);
    QGraphicsTextItem *text2=detect_graph_fft_scene->addText(QString("%1").arg(y2_value));
    text2->setDefaultTextColor(Qt::white);
    text2->setPos(0,y2-17);
    double y1_value=nucleo.signal_fft_max/double(4);
    QGraphicsTextItem *text3=detect_graph_fft_scene->addText(QString("%1").arg(y1_value));
    text3->setDefaultTextColor(Qt::white);
    text3->setPos(0,y1-17);

    //texto vertical
    double x1_value=nucleo.signal_fm*double(3)/double(8);
    QGraphicsTextItem *text1_x=detect_graph_fft_scene->addText(QString("%1").arg(x1_value));
    text1_x->setDefaultTextColor(Qt::white);
    text1_x->setPos(x1,-17);
    double x2_value=nucleo.signal_fm/double(4);
    QGraphicsTextItem *text2_x=detect_graph_fft_scene->addText(QString("%1").arg(x2_value));
    text2_x->setDefaultTextColor(Qt::white);
    text2_x->setPos(x2,-17);
    double x3_value=nucleo.signal_fm/double(8);
    QGraphicsTextItem *text3_x=detect_graph_fft_scene->addText(QString("%1").arg(x3_value));
    text3_x->setDefaultTextColor(Qt::white);
    text3_x->setPos(x3,-17);

}





void MainWindow::on_gen_spect_zoomV_SL_valueChanged(int value)
{
    int range=ui->gen_spect_zoomV_SL->maximum();

    if(value==0)
    {
        gen_spect_centerV=nucleo.signal_spect_length/2;
        gen_spect_windowV=nucleo.signal_spect_length/4;
        gen_spect_despV_habilitar=false;
        ui->gen_spect_despV_SB->setRange(0,0);
        ui->gen_spect_despV_SB->setValue(0);
        gen_spect_despV_habilitar=true;
    }
    else
    {
        //se recalcula la ventana
        gen_spect_windowV=nucleo.signal_spect_length/2*(1-double(value)/double(range));
        //se actualiza la coordenada
        gen_graph_spect->y1=gen_spect_centerV-gen_spect_windowV;
        gen_graph_spect->y2=gen_spect_centerV+gen_spect_windowV;

        if(gen_graph_spect->y1<0)
            gen_graph_spect->y1=0;
        if(gen_graph_spect->y2>nucleo.signal_spect_length)
            gen_graph_spect->y2=nucleo.signal_spect_length;

        //se recalcula el despH
        gen_spect_despV_habilitar=false;
        range=nucleo.signal_spect_length-gen_graph_spect->y2+gen_graph_spect->y1;
        ui->gen_spect_despV_SB->setRange(0,range);
        int value_V=double(gen_spect_centerV)*double(range)/double(nucleo.signal_spect_length);
        ui->gen_spect_despV_SB->setValue(value_V);
        gen_spect_despV_habilitar=true;
    }

    gen_graph_spect->repaint();
}

void MainWindow::on_gen_spect_despH_SB_valueChanged(int value)
{
    if(gen_spect_despH_habilitar)
    {
        int range=ui->gen_spect_despH_SB->maximum();
        gen_spect_centerH=double(value)*double(nucleo.spect_window_length/2)/double(range);
        gen_graph_spect->x1=gen_spect_centerH-gen_spect_windowH;
        gen_graph_spect->x2=gen_spect_centerH+gen_spect_windowH;

        if(gen_graph_spect->x1<0)
            gen_graph_spect->x1=0;
        if(gen_graph_spect->x2>nucleo.spect_window_length/2)
            gen_graph_spect->x2=nucleo.spect_window_length/2;

        gen_graph_spect->repaint();
    }

}

void MainWindow::on_gen_spect_despV_SB_valueChanged(int value)
{
    if(gen_spect_despV_habilitar)
    {
        int range=ui->gen_spect_despV_SB->maximum();
        gen_spect_centerV=double(value)*double(nucleo.signal_spect_length)/double(range);
        gen_graph_spect->y1=gen_spect_centerV-gen_spect_windowV;
        gen_graph_spect->y2=gen_spect_centerV+gen_spect_windowV;

        if(gen_graph_spect->y1<0)
            gen_graph_spect->y1=0;
        if(gen_graph_spect->y2>nucleo.signal_spect_length)
            gen_graph_spect->y2=nucleo.signal_spect_length;

        gen_graph_spect->repaint();
    }
}


void MainWindow::on_spect_PB_clicked()
{

    nucleo.spect_window_length=ui->spect_window_length_LE->text().toInt();

    int value=ui->spect_window_solap_LE->text().toInt();

    if(value<=0 || value>5000)
    {
        information(0,0,2);
        //se pone el valor anterior
        ui->spect_window_solap_LE->setText(QString("%1").arg(nucleo.spect_window_solapamiento));
        return;
    }

    else
    {
        nucleo.spect_window_solapamiento=value;
        ui->spect_solap_SpinB->setValue(value);

    }

    nucleo.spect_window=ui->spect_window_CB->currentIndex();

    //se borra el anterior espectrograma
    if(nucleo.signal_spect_length!=0)
        delete[] nucleo.signal_spect;
    //se recalcula el espectrograma
    nucleo.nucleo_spect_funct();

    ///Tab General
    //se reajustan los controles
    gen_spect_despH_habilitar=false;
    gen_spect_despV_habilitar=false;
    gen_spect_centerH=nucleo.spect_window_length/4;
    gen_spect_windowH=nucleo.spect_window_length/8;
    gen_spect_centerV=nucleo.signal_spect_length/2;
    gen_spect_windowV=nucleo.signal_spect_length/4;
    ui->gen_spect_despH_SB->setRange(0,0);
    ui->gen_spect_despV_SB->setRange(0,0);
    ui->gen_spect_zoomH_SL->setValue(0);
    ui->gen_spect_zoomV_SL->setValue(0);
    gen_graph_spect->x1=0;
    gen_graph_spect->x2=nucleo.spect_window_length/2;
    gen_graph_spect->y1=0;
    gen_graph_spect->y2=nucleo.signal_spect_length;
    gen_graph_spect->spect_max=nucleo.spect_max;
    gen_graph_spect->spect=nucleo.signal_spect;
    gen_graph_spect->spect_length=nucleo.signal_spect_length;
    gen_spect_despH_habilitar=true;
    gen_spect_despV_habilitar=true;
    //se redibuja
    gen_graph_spect->repaint();

    ///Tab Detectar
    //se reajustan los controles
    detect_spect_despH_habilitar=false;
    detect_spect_despV_habilitar=false;
    detect_spect_centerH=nucleo.spect_window_length/4;
    detect_spect_windowH=nucleo.spect_window_length/8;
    detect_spect_centerV=nucleo.signal_spect_length/2;
    detect_spect_windowV=nucleo.signal_spect_length/4;
    ui->detect_spect_despH_SB->setRange(0,0);
    ui->detect_spect_despV_SB->setRange(0,0);
    ui->detect_spect_zoomH_SL->setValue(0);
    ui->detect_spect_zoomV_SL->setValue(0);
    detect_graph_spect->x1=0;
    detect_graph_spect->x2=nucleo.spect_window_length/2;
    detect_graph_spect->y1=0;
    detect_graph_spect->y2=nucleo.signal_spect_length;
    detect_graph_spect->spect_max=nucleo.spect_max;
    detect_graph_spect->spect=nucleo.signal_spect;
    detect_graph_spect->spect_length=nucleo.signal_spect_length;
    detect_spect_despH_habilitar=true;
    detect_spect_despV_habilitar=true;
    //se redibuja
    detect_graph_spect->repaint();

    ///Tab Estimar
    //se reajustan los controles
    clasif_spect_despH_habilitar=false;
    clasif_spect_despV_habilitar=false;
    clasif_spect_centerH=nucleo.spect_window_length/4;
    clasif_spect_windowH=nucleo.spect_window_length/8;
    clasif_spect_centerV=nucleo.signal_spect_length/2;
    clasif_spect_windowV=nucleo.signal_spect_length/4;
    ui->clasif_spect_despH_SB->setRange(0,0);
    ui->clasif_spect_despV_SB->setRange(0,0);
    ui->clasif_spect_zoomH_SL->setValue(0);
    ui->clasif_spect_zoomV_SL->setValue(0);
    clasif_graph_spect->x1=0;
    clasif_graph_spect->x2=nucleo.spect_window_length/2;
    clasif_graph_spect->y1=0;
    clasif_graph_spect->y2=nucleo.signal_spect_length;
    clasif_graph_spect->spect_max=nucleo.spect_max;
    clasif_graph_spect->spect=nucleo.signal_spect;
    clasif_graph_spect->spect_length=nucleo.signal_spect_length;
    clasif_spect_despH_habilitar=true;
    clasif_spect_despV_habilitar=true;
    //se redibuja
    clasif_graph_spect->repaint();
    information(0,0,0);

}

void MainWindow::on_spect_solap_SpinB_valueChanged(int arg1)
{
   ui->spect_window_solap_LE->setText(QString("%1").arg(arg1));
   information(0,0,0);
}

void MainWindow::on_detect_spect_zoomH_SL_valueChanged(int value)
{
    int range=ui->detect_spect_zoomH_SL->maximum();

    if(value==0)
    {
        detect_spect_centerH=nucleo.spect_window_length/4;
        detect_spect_windowH=nucleo.spect_window_length/8;
        detect_spect_despH_habilitar=false;
        ui->detect_spect_despH_SB->setRange(0,0);
        ui->detect_spect_despH_SB->setValue(0);
        detect_spect_despH_habilitar=true;
    }
    else
    {
        //se recalcula la ventana
        detect_spect_windowH=nucleo.spect_window_length/4*(1-double(value)/double(range));
        //se actualiza la coordenada
        detect_graph_spect->x1=detect_spect_centerH-detect_spect_windowH;
        detect_graph_spect->x2=detect_spect_centerH+detect_spect_windowH;

        if(detect_graph_spect->x1<0)
            detect_graph_spect->x1=0;
        if(detect_graph_spect->x2>nucleo.spect_window_length/2)
            detect_graph_spect->x2=nucleo.spect_window_length/2;

        //se recalcula el despH
        detect_spect_despH_habilitar=false;
        range=nucleo.spect_window_length/2-detect_graph_spect->x2+detect_graph_spect->x1;
        ui->detect_spect_despH_SB->setRange(0,range);
        int value_H=double(detect_spect_centerH)*double(range)/double(nucleo.spect_window_length/2);
        ui->detect_spect_despH_SB->setValue(value_H);
        detect_spect_despH_habilitar=true;
    }

    detect_graph_spect->repaint();
}

void MainWindow::on_detect_spect_despH_SB_valueChanged(int value)
{
    if(detect_spect_despH_habilitar)
    {
        int range=ui->detect_spect_despH_SB->maximum();
        detect_spect_centerH=double(value)*double(nucleo.spect_window_length/2)/double(range);
        detect_graph_spect->x1=detect_spect_centerH-detect_spect_windowH;
        detect_graph_spect->x2=detect_spect_centerH+detect_spect_windowH;

        if(detect_graph_spect->x1<0)
            detect_graph_spect->x1=0;
        if(detect_graph_spect->x2>nucleo.spect_window_length/2)
            detect_graph_spect->x2=nucleo.spect_window_length/2;

        detect_graph_spect->repaint();
    }
}

void MainWindow::on_detect_spect_zoomV_SL_valueChanged(int value)
{
    int range=ui->detect_spect_zoomV_SL->maximum();

    if(value==0)
    {
        detect_spect_centerV=nucleo.signal_spect_length/2;
        detect_spect_windowV=nucleo.signal_spect_length/4;
        detect_spect_despV_habilitar=false;
        ui->detect_spect_despV_SB->setRange(0,0);
        ui->detect_spect_despV_SB->setValue(0);
        detect_spect_despV_habilitar=true;
    }
    else
    {
        //se recalcula la ventana
        detect_spect_windowV=nucleo.signal_spect_length/2*(1-double(value)/double(range));
        //se actualiza la coordenada
        detect_graph_spect->y1=detect_spect_centerV-detect_spect_windowV;
        detect_graph_spect->y2=detect_spect_centerV+detect_spect_windowV;

        if(detect_graph_spect->y1<0)
            detect_graph_spect->y1=0;
        if(detect_graph_spect->y2>nucleo.signal_spect_length)
            detect_graph_spect->y2=nucleo.signal_spect_length;

        //se recalcula el despH
        detect_spect_despV_habilitar=false;
        range=nucleo.signal_spect_length-detect_graph_spect->y2+detect_graph_spect->y1;
        ui->detect_spect_despV_SB->setRange(0,range);
        int value_V=double(detect_spect_centerV)*double(range)/double(nucleo.signal_spect_length);
        ui->detect_spect_despV_SB->setValue(value_V);
        detect_spect_despV_habilitar=true;
    }

    detect_graph_spect->repaint();
}

void MainWindow::on_detect_spect_despV_SB_valueChanged(int value)
{
    if(detect_spect_despV_habilitar)
    {
        int range=ui->detect_spect_despV_SB->maximum();
        detect_spect_centerV=double(value)*double(nucleo.signal_spect_length)/double(range);
        detect_graph_spect->y1=detect_spect_centerV-detect_spect_windowV;
        detect_graph_spect->y2=detect_spect_centerV+detect_spect_windowV;

        if(detect_graph_spect->y1<0)
            detect_graph_spect->y1=0;
        if(detect_graph_spect->y2>nucleo.signal_spect_length)
            detect_graph_spect->y2=nucleo.signal_spect_length;

        detect_graph_spect->repaint();
    }
}

void MainWindow::on_clasif_spect_zoomH_SL_valueChanged(int value)
{
    int range=ui->clasif_spect_zoomH_SL->maximum();

    if(value==0)
    {
        clasif_spect_centerH=nucleo.spect_window_length/4;
        clasif_spect_windowH=nucleo.spect_window_length/8;
        clasif_spect_despH_habilitar=false;
        ui->clasif_spect_despH_SB->setRange(0,0);
        ui->clasif_spect_despH_SB->setValue(0);
        clasif_spect_despH_habilitar=true;
    }
    else
    {
        //se recalcula la ventana
        clasif_spect_windowH=nucleo.spect_window_length/4*(1-double(value)/double(range));
        //se actualiza la coordenada
        clasif_graph_spect->x1=clasif_spect_centerH-clasif_spect_windowH;
        clasif_graph_spect->x2=clasif_spect_centerH+clasif_spect_windowH;

        if(clasif_graph_spect->x1<0)
            clasif_graph_spect->x1=0;
        if(clasif_graph_spect->x2>nucleo.spect_window_length/2)
            clasif_graph_spect->x2=nucleo.spect_window_length/2;

        //se recalcula el despH
        clasif_spect_despH_habilitar=false;
        range=nucleo.spect_window_length/2-clasif_graph_spect->x2+clasif_graph_spect->x1;
        ui->clasif_spect_despH_SB->setRange(0,range);
        int value_H=double(clasif_spect_centerH)*double(range)/double(nucleo.spect_window_length/2);
        ui->clasif_spect_despH_SB->setValue(value_H);
        clasif_spect_despH_habilitar=true;
    }

    clasif_graph_spect->repaint();
}

void MainWindow::on_clasif_spect_despH_SB_valueChanged(int value)
{
    if(clasif_spect_despH_habilitar)
    {
        int range=ui->clasif_spect_despH_SB->maximum();
        clasif_spect_centerH=double(value)*double(nucleo.spect_window_length/2)/double(range);
        clasif_graph_spect->x1=clasif_spect_centerH-clasif_spect_windowH;
        clasif_graph_spect->x2=clasif_spect_centerH+clasif_spect_windowH;

        if(clasif_graph_spect->x1<0)
            clasif_graph_spect->x1=0;
        if(clasif_graph_spect->x2>nucleo.spect_window_length/2)
            clasif_graph_spect->x2=nucleo.spect_window_length/2;

        clasif_graph_spect->repaint();
    }
}

void MainWindow::on_clasif_spect_zoomV_SL_valueChanged(int value)
{
    int range=ui->clasif_spect_zoomV_SL->maximum();

    if(value==0)
    {
        clasif_spect_centerV=nucleo.signal_spect_length/2;
        clasif_spect_windowV=nucleo.signal_spect_length/4;
        clasif_spect_despV_habilitar=false;
        ui->clasif_spect_despV_SB->setRange(0,0);
        ui->clasif_spect_despV_SB->setValue(0);
        clasif_spect_despV_habilitar=true;
    }
    else
    {
        //se recalcula la ventana
        clasif_spect_windowV=nucleo.signal_spect_length/2*(1-double(value)/double(range));
        //se actualiza la coordenada
        clasif_graph_spect->y1=clasif_spect_centerV-clasif_spect_windowV;
        clasif_graph_spect->y2=clasif_spect_centerV+clasif_spect_windowV;

        if(clasif_graph_spect->y1<0)
            clasif_graph_spect->y1=0;
        if(clasif_graph_spect->y2>nucleo.signal_spect_length)
            clasif_graph_spect->y2=nucleo.signal_spect_length;

        //se recalcula el despH
        clasif_spect_despV_habilitar=false;
        range=nucleo.signal_spect_length-clasif_graph_spect->y2+clasif_graph_spect->y1;
        ui->clasif_spect_despV_SB->setRange(0,range);
        int value_V=double(clasif_spect_centerV)*double(range)/double(nucleo.signal_spect_length);
        ui->clasif_spect_despV_SB->setValue(value_V);
        clasif_spect_despV_habilitar=true;
    }

    clasif_graph_spect->repaint();
}

void MainWindow::on_clasif_spect_despV_SB_valueChanged(int value)
{
    if(clasif_spect_despV_habilitar)
    {
        int range=ui->clasif_spect_despV_SB->maximum();
        clasif_spect_centerV=double(value)*double(nucleo.signal_spect_length)/double(range);
        clasif_graph_spect->y1=clasif_spect_centerV-clasif_spect_windowV;
        clasif_graph_spect->y2=clasif_spect_centerV+clasif_spect_windowV;

        if(clasif_graph_spect->y1<0)
            clasif_graph_spect->y1=0;
        if(clasif_graph_spect->y2>nucleo.signal_spect_length)
            clasif_graph_spect->y2=nucleo.signal_spect_length;

        clasif_graph_spect->repaint();
    }
}

void MainWindow::on_spect_window_length_SpinB_valueChanged(int arg1)
{
    ui->spect_window_length_LE->setText(QString("%1").arg(qPow(2,arg1)));
    information(0,0,0);
}



void MainWindow::on_spect_color_clicked(bool checked)
{

    gen_graph_spect->spect_color_scale=checked;
    gen_graph_spect->repaint();

    detect_graph_spect->spect_color_scale=checked;
    detect_graph_spect->repaint();

    clasif_graph_spect->spect_color_scale=checked;
    clasif_graph_spect->repaint();

    information(0,0,0);

}

void MainWindow::on_dem_f0_LE_editingFinished()
{
    if(dem_f0_SB_hab)
    {
        //validación
        bool ok=0;
        float value=ui->dem_f0_LE->text().toFloat(&ok);
        if(ok==0)
        {            
            //se pone el valor anterior
            ui->dem_f0_LE->setText(QString("%1").arg(float(nucleo.signal_fft_max_pos)*float(nucleo.signal_fm/2)/float(nucleo.signal_fft_length)));
            information(3,0,1);
            return;
        }

        if(value<0 || value>(nucleo.signal_fm/2) || qIsNaN(value))
        {
            //se pone el valor anterior
            ui->dem_f0_LE->setText(QString("%1").arg(float(nucleo.signal_fft_max_pos)*float(nucleo.signal_fm/2)/float(nucleo.signal_fft_length)));
            information(3,0,2);
            return;
        }

        //conversión a muestras
        value= value*float(nucleo.signal_fft_length)/float(nucleo.signal_fm/2);

        //se actualiza el scroll y el resto de las variables relacionadas
        dem_f0_SB_hab=false;
        ui->dem_f0_SB->setValue(value);
        nucleo.signal_fft_max_pos=value;
        dem_graph_fft->signal_lines[0]=float(nucleo.signal_fft_max_pos);
        dem_f0_SB_hab=true;

        dem_graph_fft->repaint();

        information(0,0,0);
    }
}

void MainWindow::on_dem_f1_LE_editingFinished()
{
    if(dem_f0_SB_hab)
    {

        //validación
        bool ok;
        float value=ui->dem_f1_LE->text().toFloat(&ok);

        if(ok==0)
        {
            int index=ui->clasif_resultado_CB->currentIndex();
            if(index==2 || index==4)
            {
                int index2=ui->dem_freq_CB->currentIndex();
                value=nucleo.dem_freq_array[index2];
            }
            else
            {
                value=nucleo.signal_fft_max_pos_2;
            }
            //se pone el valor anterior
            ui->dem_f1_LE->setText(QString("%1").arg(float(value)*float(nucleo.signal_fm/2)/float(nucleo.signal_fft_length)));
            information(3,0,1);
            return;
        }

        if(value<0 || value>(nucleo.signal_fm/2) || qIsNaN(value))
        {
            int index=ui->clasif_resultado_CB->currentIndex();
            if(index==2 || index==4)
            {
                int index2=ui->dem_freq_CB->currentIndex();
                value=nucleo.dem_freq_array[index2];
            }
            else
            {
                value=nucleo.signal_fft_max_pos_2;
            }
            //se pone el valor anterior
            ui->dem_f1_LE->setText(QString("%1").arg(float(value)*float(nucleo.signal_fm/2)/float(nucleo.signal_fft_length)));
            information(3,0,2);
            return;
        }

        //conversión a muestras
        value= value*float(nucleo.signal_fft_length)/float(nucleo.signal_fm/2);

        //se actualiza el scroll y el resto de las variables relacionadas
        dem_f0_SB_hab=false;
        ui->dem_f1_SB->setValue(value);
        int index=ui->clasif_resultado_CB->currentIndex();
        //si es modulación
        if(index==2 || index==3)
        {
            int index2=ui->dem_freq_CB->currentIndex();
            nucleo.dem_freq_array[index2]=value;
            //se asocian las líneas de frecuencia en el gráfico de FFT
            dem_graph_fft->signal_lines_length=nucleo.detect_freq_length;
            dem_graph_fft->signal_lines=nucleo.dem_freq_array;
            dem_graph_fft->signal_lines_selecc=ui->dem_freq_CB->currentIndex();

        }
        else
        {
            nucleo.signal_fft_max_pos_2=value;
            dem_graph_fft->signal_lines[1]=float(nucleo.signal_fft_max_pos_2);
        }


        //restart_graphics(5);
        dem_graph_fft->repaint();

        dem_f0_SB_hab=true;

        information(0,0,0);
    }
}

void MainWindow::on_dem_raster_LE_editingFinished()
{
    //validación del valor del patrón
    bool ok=0;
    int value=ui->dem_raster_LE->text().toInt(&ok);

    if(ok==0)
    {

        ui->dem_raster_LE->setText(QString("%1").arg(nucleo.patron));
        return;
    }

    if(value<=0 || value>nucleo.bits_length || qIsNaN(ui->dem_raster_LE->text().toFloat()))
    {
        ui->dem_raster_LE->setText(QString("%1").arg(nucleo.patron));
        information(3,0,4);
        return;
    }

    else
    {
        nucleo.patron=ui->dem_raster_LE->text().toFloat();

        //se manda a dibujar el nuevo raster
        //se reinician el zoom y desplazamiento
        dem_raster_despH_habilitar=false;
        dem_raster_despV_habilitar=false;
        ui->dem_raster_despH_SB->setRange(0,0);
        ui->dem_raster_despV_SB->setRange(0,0);
        ui->dem_raster_zoomH_SL->setValue(0);
        ui->dem_raster_zoomV_SL->setValue(0);
        dem_raster_despH_habilitar=true;
        dem_raster_despV_habilitar=true;

        //se manda a dibujar el raster
        dem_graph_raster->bits=nucleo.bits;
        dem_graph_raster->bits_length=nucleo.bits_length;
        dem_graph_raster->patron=nucleo.patron;
        dem_graph_raster->x1=0;
        dem_graph_raster->x2=nucleo.patron;
        dem_graph_raster->y1=0;
        dem_graph_raster->y2=ceil(double(nucleo.bits_length)/double(nucleo.patron));

        //se coloca el centro del dibujo
        dem_raster_centerH=(dem_graph_raster->x1+dem_graph_raster->x2)/2;
        dem_raster_windowH=(dem_graph_raster->x2-dem_graph_raster->x1)/2;
        dem_raster_centerV=(dem_graph_raster->y1+dem_graph_raster->y2)/2;
        dem_raster_windowV=(dem_graph_raster->y2-dem_graph_raster->y1)/2;

        dem_graph_raster->repaint();

        information(0,0,0);
    }
}

void MainWindow::on_spect_window_solap_LE_editingFinished()
{
    bool ok;

    int value=ui->spect_window_solap_LE->text().toInt(&ok,10);

    if(ok==0)
    {

        //se pone el valor anterior
        ui->spect_window_solap_LE->setText(QString("%1").arg(nucleo.spect_window_solapamiento));

        information(0,1,1);
        return;
    }

    if(value<=0 || value>5000 || qIsNaN(ui->spect_window_solap_LE->text().toFloat()))
    {
        //se pone el valor anterior
        ui->spect_window_solap_LE->setText(QString("%1").arg(nucleo.spect_window_solapamiento));

        information(0,1,2);
        return;
    }

    else
    {
        nucleo.spect_window_solapamiento=value;
        ui->spect_solap_SpinB->setValue(value);
        information(0,0,0);

    }
}

void MainWindow::on_detect_f0_LE_editingFinished()
{
    if(detect_f0_hab)
    {
        detect_f0_hab=false;
        //float value_2=ui->detect_f1_LE->text().toFloat();//=ui->detect_f1_LE->text().toFloat()-1/100*float(nucleo.signal_fm)/2;
        int index_2 = 2*(ui->detect_signals_CB->currentIndex()-1);

        bool ok=0;
        float center_f=ui->detect_f0_LE->text().toFloat(&ok);
        float f0_f=float(nucleo.detect_freq[index_2])*1/float(nucleo.signal_fft_length)*float(nucleo.signal_fm)/2;
        float f1_f=float(nucleo.detect_freq[index_2+1])*1/float(nucleo.signal_fft_length)*float(nucleo.signal_fm)/2;

        //validación del campo numérico
        if(ok==0)
        {
            //se pone el valor anterior

            ui->detect_f0_LE->setText(QString("%1").arg((f0_f+f1_f)/2));
            detect_f0_hab=true;
            information(1,1,1);
            return;
        }


        //Comprobación de rangos
        float ancho_f=(f1_f-f0_f)/2;

        if((center_f-ancho_f)<=0 || (center_f+ancho_f)>float(nucleo.signal_fm)/float(2) || qIsNaN(center_f))
        {            
            //se pone el valor anterior
            ui->detect_f0_LE->setText(QString("%1").arg((f0_f+f1_f)/2));
            restart_graphics(1);
            //detect_pintar_fft();
            detect_f0_hab=true;
            information(1,1,2);
            return;
        }

        //conversión a muestras
        int center=center_f/float(nucleo.signal_fm/2)*float(nucleo.signal_fft_length);
        int ancho=ancho_f/float(nucleo.signal_fm/2)*float(nucleo.signal_fft_length);


        //se actualiza el scroll y el resto de las variables relacionadas
        detect_f0_hab=false;       
        nucleo.detect_freq[index_2]=center-ancho;
        nucleo.detect_freq[index_2+1]=center+ancho;

        restart_graphics(1);

        //se actualiza el centro en el SB
        int f0=nucleo.detect_freq[index_2];
        int f1=nucleo.detect_freq[index_2+1];
        center=(f0+f1)/2;
        ancho=min(center-f0,f1-center);
        int center_pos=float(center-ancho-10)*float(100)/float(nucleo.signal_fft_length-2*(ancho+10));
        ui->detect_f0_SB->setValue(center_pos);
        detect_f0_hab=true;

        //se actualiza el valor del SB de banda
        int ancho_min=3;
        int ancho_max=min(center,nucleo.signal_fft_length-center);
        int ancho_pos=float(ancho-ancho_min)*float(100)/float(ancho_max-ancho_min);
        detect_f1_hab=false;
        ui->detect_f1_SB->setValue(ancho_pos);
        detect_f1_hab=true;





        restart_graphics(1);

    }
}

void MainWindow::on_detect_f1_LE_editingFinished()
{

    if(detect_f1_hab)
    {
        detect_f1_hab=false;
        //float value_2=ui->detect_f1_LE->text().toFloat();//=ui->detect_f1_LE->text().toFloat()-1/100*float(nucleo.signal_fm)/2;
        int index_2 = 2*(ui->detect_signals_CB->currentIndex()-1);

        bool ok=0;
        float ancho_f=ui->detect_f1_LE->text().toFloat(&ok)/2;
        float f0_f=float(nucleo.detect_freq[index_2])*1/float(nucleo.signal_fft_length)*float(nucleo.signal_fm)/2;
        float f1_f=float(nucleo.detect_freq[index_2+1])*1/float(nucleo.signal_fft_length)*float(nucleo.signal_fm)/2;

        //validación del campo numérico
        if(ok==0)
        {
            //se pone el valor anterior

            ui->detect_f1_LE->setText(QString("%1").arg((f1_f-f0_f)));
            detect_f1_hab=true;
            information(1,1,1);
            return;
        }

        //Comprobación de rangos
        //valor positivo
        if(ancho_f<0 || qIsNaN(ancho_f))
        {
            //se pone el valor anterior
            ui->detect_f1_LE->setText(QString("%1").arg((f1_f-f0_f)));
            restart_graphics(1);
            //detect_pintar_fft();
            detect_f1_hab=true;
            information(1,1,3);
            return;
        }

        //valor mínimo
        int ancho_min=3;
        float ancho_min_f=float(ancho_min)/float(nucleo.signal_fft_length)*float(nucleo.signal_fm)/float(2);
        if(ancho_f<ancho_min_f)
        {
            //se pone el valor anterior
            ui->detect_f1_LE->setText(QString("%1").arg((f1_f-f0_f)));
            restart_graphics(1);
            //detect_pintar_fft();
            detect_f1_hab=true;
            information(1,1,3);
            return;
        }

        float center_f=(f1_f+f0_f)/2;

        if((center_f-ancho_f)<=0 || (center_f+ancho_f)>float(nucleo.signal_fm)/float(2))
        {
            //se pone el valor anterior
            ui->detect_f1_LE->setText(QString("%1").arg((f1_f-f0_f)));
            restart_graphics(1);
            //detect_pintar_fft();
            detect_f1_hab=true;
            information(1,1,3);
            return;
        }

        //conversión a muestras
        int center=center_f/float(nucleo.signal_fm/2)*float(nucleo.signal_fft_length);
        int ancho=ancho_f/float(nucleo.signal_fm/2)*float(nucleo.signal_fft_length);


        //se actualiza el scroll y el resto de las variables relacionadas

        nucleo.detect_freq[index_2]=center-ancho;
        nucleo.detect_freq[index_2+1]=center+ancho;

        restart_graphics(1);

        //se actualiza el centro en el SB
        int f0=nucleo.detect_freq[index_2];
        int f1=nucleo.detect_freq[index_2+1];
        center=(f0+f1)/2;
        ancho=min(center-f0,f1-center);
        ancho_min=3;
        int ancho_max=min(center,nucleo.signal_fft_length-center);


        int ancho_pos=float(ancho-ancho_min)*float(100)/float(ancho_max-ancho_min);

        detect_f1_hab=false;
        ui->detect_f1_SB->setValue(ancho_pos);
        detect_f1_hab=true;
        information(0,0,0);
        restart_graphics(1);

    }

}

void MainWindow::on_param_amp_LE_editingFinished()
{
    //validación
    bool ok=0;
    float value=ui->param_amp_LE->text().toFloat(&ok);

    if(ok==0)
    {        
        ui->param_amp_LE->setText(QString("%1").arg(nucleo.signal_amp));
        information(2,0,1);
        return;
    }

    //validación
    if(value<qPow(10,-100) || qIsNaN(value))
    {        
        ui->param_amp_LE->setText(QString("%1").arg(nucleo.signal_amp));
        information(2,0,2);
    }
    else
    {
        information(0,0,0);
        nucleo.signal_amp=ui->param_amp_LE->text().toFloat();
    }
}

void MainWindow::on_param_SNR_LE_editingFinished()
{
    //validación
    bool ok=0;
    float value=ui->param_SNR_LE->text().toFloat(&ok);

    if(ok==0 || qIsNaN(value))
    {        
        ui->param_SNR_LE->setText(QString("%1").arg(nucleo.signal_SNR));
        information(2,0,1);
        return;
    }

    nucleo.signal_SNR=value;

}

void MainWindow::on_param_baud_LE_editingFinished()
{
    bool ok=0;
    float value=ui->param_baud_LE->text().toFloat(&ok);

    if(ok==0)
    {        
        ui->param_baud_LE->setText(QString("%1").arg(nucleo.signal_baud_rate));
        information(2,0,1);
        return;
    }

    //validación
    if(value<1 || value>nucleo.signal_fm/2 || qIsNaN(value))
    {        
        ui->param_baud_LE->setText(QString("%1").arg(nucleo.signal_baud_rate));
        information(2,1,2);
        return;
    }

    else
        nucleo.signal_baud_rate = value;//se copia el baud_rate

    //se modifica todo lo relacionado con el raster y la constelación
    if(nucleo.bits_length!=0)
    {
        //se modifica el raster
        delete[] nucleo.bits;
        nucleo.bits=NULL;
        nucleo.bits_length=0;

    }

    //se modifican los gráficos de demodulación
    restart_graphics(2);

    //se modifican los gráficos de la constelación
    //se completa el CB de clasificación
    //se vacía el comboBox de los métodos
    clasif_modu_habilitar=false;
    ui->clasif_resultado_CB->clear();
    clasif_modu_habilitar=true;
    ui->clasif_metodos_CB->clear();
    ui->clasif_metodos_CB->addItem("Árbol de decisiones");
    ui->clasif_metodos_CB->addItem("Cruces por cero");
    ui->clasif_metodos_CB->setCurrentIndex(1);//se inicializa en el método de cruces por cero, funciona en la mayoría de los casos
    ui->clasif_mod_GB->setDisabled(false);

    ui->clasif_PB->setDisabled(false);//se habilita el botón de clasificar
    ui->clasif_resultado_CB->setDisabled(false);
    clasif_modu_habilitar=false;
    ui->clasif_const_CB->clear();
    ui->clasif_resultado_CB->addItem("");
    ui->clasif_resultado_CB->addItem("BFSK");
    ui->clasif_resultado_CB->addItem("4FSK");
    ui->clasif_resultado_CB->addItem("8FSK");
    ui->clasif_resultado_CB->addItem("16FSK");
    ui->clasif_resultado_CB->addItem("BPSK");
    ui->clasif_resultado_CB->addItem("QPSK");
    ui->clasif_resultado_CB->addItem("DQPSK");
    ui->clasif_resultado_CB->addItem("OQPSK");
    ui->clasif_resultado_CB->addItem("8PSK");
    ui->clasif_resultado_CB->addItem("QAM");
    clasif_modu_habilitar=true;
    clasif_resultados_habilitar=true; //se habilitan los resultados de clasificación

    clasif_resultados_habilitar=false;
    ui->clasif_resultado_CB->setCurrentIndex(0);
    clasif_resultados_habilitar=true;
    //se borran los métdodos de obtención de la constelación
    clasif_const_hab=false;
    ui->clasif_const_CB->clear();
    clasif_const_hab=true;

    //Constelación
    if(nucleo.signal_I_length!=0)
    {
        delete []nucleo.signal_I;
        delete []nucleo.signal_Q;
    }
    nucleo.signal_I_length=0;
    nucleo.clasif_decision=0;
    ui->clasif_mod_GB->setDisabled(false);
    ui->clasif_const_lines->setChecked(false);
    clasif_graph_const->const_lines=false;
    ui->clasif_const_lines->setDisabled(true);

    //se borra la posible constelación
    if(nucleo.signal_I_length!=0)
    {
        delete[] nucleo.signal_I;
        delete[] nucleo.signal_Q;
        nucleo.signal_I_length=0;
        clasif_graph_const->signal_I_length=0;
    }
    clasif_graph_const->repaint();


    //se ocultan los opciones de constelación
    ui->clasif_const_CB->setEnabled(false);
    ui->clasif_L3->setEnabled(false);
}



void MainWindow::information(int tab, int campo, int error)
{
    int index_2 = 2*(ui->detect_signals_CB->currentIndex()-1);


    if(tab==0 && campo==0 && error==0)
        ui->info_L->setText("Información");

    if(tab==0)
    {

        if(error==1)            
            ui->info_L->setText("Error: Debe escribir valores enteros");

        if(error==2)
            ui->info_L->setText("Error: El rango permisible es [1 5000]");

        if(error==3)
            ui->info_L->setText("Error: Archivo vacío");

        if(error==4)
            ui->info_L->setText("Error: No se pudo abrir el archivo");

        if(error==5)
            ui->info_L->setText("Error: Escoja una señal");

        if(error==6)
            ui->info_L->setText("Error: El archivo no es procesable");

    }

    if(tab==1)
    {
        if(error==1)
            ui->info_L->setText("Error: Debe introducir un valor numérico.");


        //error en el centro del filtro
        if(campo==1 && error==2)
        {

            float f0=float(nucleo.detect_freq[index_2])*1/float(nucleo.signal_fft_length)*float(nucleo.signal_fm)/2;
            float f1=float(nucleo.detect_freq[index_2+1])*1/float(nucleo.signal_fft_length)*float(nucleo.signal_fm)/2;
            float ancho=(f1-f0)/2;
            float x1_f=ancho;
            float x2_f=float(nucleo.signal_fm)/float(2)-ancho;

            ui->info_L->setText("Error: El rango permisible es \n ("+QString("%1").arg(x1_f)+"  "+QString("%1").arg(x2_f)+")");
        }

        //error por la banda de detección
        if(campo==1 && error==3)
        {
            //se actualiza el centro en el SB
            int f0=nucleo.detect_freq[index_2];
            int f1=nucleo.detect_freq[index_2+1];
            int center=(f0+f1)/2;
            int ancho_min=3;
            int ancho_max=min(center,nucleo.signal_fft_length-center);
            float ancho_min_f=float(2*ancho_min)/float(nucleo.signal_fft_length)*float(nucleo.signal_fm)/float(2);
            float ancho_max_f=float(2*ancho_max)/float(nucleo.signal_fft_length)*float(nucleo.signal_fm)/float(2);

            ui->info_L->setText("Error: El rango permisible es \n ("+QString("%1").arg(ancho_min_f)+"  "+QString("%1").arg(ancho_max_f)+")");
        }

        if(campo==2 && error==2)
        {
            float value=ui->detect_f0_LE->text().toFloat();            
            ui->info_L->setText("Error: El rango permisible es ["+QString("%1").arg(value)+"  "+QString("%1").arg(float(nucleo.signal_fm/2))+"]");
        }
    }

    if(tab==2)
    {
        if(error==1)
            ui->info_L->setText("Error: Debe introducir un valor numérico.");

        if(campo==0 && error==2)
            ui->info_L->setText("Error: La amplitud debe ser mayor que cero.");

        if(campo==1 && error==2)
            ui->info_L->setText("Error: El Baud rate debe ser mayor que cero.");

        if(campo==2 && error==2)
        {
            ui->info_L->setText("Error: No se pudo implementar el \n el filtro para los valores de frequencia especificados.");
        }

        if(error==3)
        {
            ui->info_L->setText("Error: El baud rate debe estar en el rango [1 "+QString("%1").arg(nucleo.signal_fm/2)+"]");

        }

        if(error==4)
        {

           ui->info_L->setText("Error: Con el valor de Baud rate asociado \n no se pudo obtener la constelación");

        }

        if(error==5)
            ui->info_L->setText("Error: Inserte el Baud Rate");

    }

    if(tab==3)
    {
        if(error==1)
            ui->info_L->setText("Error: Debe introducir un valor numérico.");

        if(error==2)
            ui->info_L->setText("Error: El rango permisible es [0 "+QString("%1").arg(float(nucleo.signal_fm/2))+QString("] Hz"));

        if(error==3)
            ui->info_L->setText("Error: Debe escribir valores naturales");

        if(error==4)
            ui->info_L->setText("Error: El Patrón debe estar en el \n rango [0  "+QString("%1").arg(nucleo.bits_length)+QString("]"));

        if(error==5)
            ui->info_L->setText("Error: No se encontró un patrón para \n esta secuencia");

        if(error==6)
            ui->info_L->setText("Error: Inserte el Baud Rate");

        if(error==7)
            ui->info_L->setText("Error: No se pudo implementar el demodulador");

    }
}


void MainWindow::on_detect_f0_SB_valueChanged(int value)
{

    if(nucleo.detect_freq_length!=0 && detect_f0_hab)
    {
        //se calculan el centro y el ancho de la banda en muestras
        int index=ui->detect_signals_CB->currentIndex();
        int index_2=2*(index-1);
        int f0=nucleo.detect_freq[index_2];
        int f1=nucleo.detect_freq[index_2+1];
        int center=(f0+f1)/2;
        int ancho=min(center-f0,f1-center);

        //nuevos valores de center, f0 y f1
        center=float(value)/float(100)*float(nucleo.signal_fft_length-2*(ancho+10))+ancho+10;
        f0=center-ancho;
        f1=center+ancho;
        nucleo.detect_freq[index_2]=f0;
        nucleo.detect_freq[index_2+1]=f1;
        //se actualiza el line edit
        float center_f=float(center)/float(nucleo.signal_fft_length)*float(nucleo.signal_fm)/float(2);
        detect_f0_LE_hab=false;
        ui->detect_f0_LE->setText(QString("%1").arg(center_f));
        detect_f0_LE_hab=true;

        //se actualiza el valor del SB de banda
        int ancho_min=3;
        int ancho_max=min(center,nucleo.signal_fft_length-center);
        int ancho_pos=float(ancho-ancho_min)*float(100)/float(ancho_max-ancho_min);
        detect_f1_hab=false;
        ui->detect_f1_SB->setValue(ancho_pos);
        detect_f1_hab=true;

        restart_graphics(1);
    }


}



void MainWindow::on_clasif_metodos_CB_currentIndexChanged(int index)
{
    if(clasif_modu_habilitar)
    {
        //se reinicializan controles
        clasif_const_hab=false;
        ui->clasif_const_CB->clear();
        ui->clasif_const_lines->setChecked(false);
        clasif_graph_const->const_lines=false;
        ui->clasif_const_lines->setDisabled(true);
        clasif_modu_habilitar=false;
        nucleo.clasif_decision=0;
        ui->clasif_resultado_CB->setCurrentIndex(0);
        clasif_const_hab=true;
        clasif_modu_habilitar=true;
        restart_graphics(2);

        //se borra la posible constelación
        //Constelación
        if(nucleo.signal_I_length!=0)
        {
            delete []nucleo.signal_I;
            delete []nucleo.signal_Q;
        }
        nucleo.signal_I_length=0;
        nucleo.clasif_decision=0;
        ui->clasif_mod_GB->setDisabled(false);
        clasif_graph_const->const_lines=false;
        clasif_graph_const->signal_I_length=0;
        clasif_graph_const->repaint();
    }
}


void MainWindow::on_dem_metodos_CB_currentIndexChanged(int index)
{
    information(0,0,0);
}



void MainWindow::on_spect_window_CB_currentIndexChanged(int index)
{
    information(0,0,0);
}



void MainWindow::on_dem_freq_CB_currentIndexChanged(int index)
{
    restart_graphics(4);
    information(0,0,0);
}

void MainWindow::on_dem_cod_CB_currentIndexChanged(int index)
{
    if(dem_cod_CB_hab)
    {
        //se borra el posible raster
        if(nucleo.bits_length!=0)
        {
            delete [] nucleo.bits;
            nucleo.bits_length=0;
        }
        nucleo.code_dem=index;

        if(!nucleo.nucleo_raster_funct())
        {
            //error en la determinación del raster

            //se borra el posible raster
            if(nucleo.bits_length!=0)
            {
                delete[] nucleo.bits;
                nucleo.bits_length=0;

            }

            restart_graphics(3);

            information(3,0,5);
        }

        qDebug()<<"bits_length"<<nucleo.bits_length;

        //se manda a dibujar el raster
        dem_graph_raster->bits=nucleo.bits;
        dem_graph_raster->bits_length=nucleo.bits_length;
        dem_graph_raster->patron=nucleo.patron;
        dem_graph_raster->x1=0;
        dem_graph_raster->x2=nucleo.patron;
        dem_graph_raster->y1=0;
        dem_graph_raster->y2=ceil(double(nucleo.bits_length)/double(nucleo.patron));

        //se coloca el centro del dibujo
        dem_raster_centerH=(dem_graph_raster->x1+dem_graph_raster->x2)/2;
        dem_raster_windowH=(dem_graph_raster->x2-dem_graph_raster->x1)/2;
        dem_raster_centerV=(dem_graph_raster->y1+dem_graph_raster->y2)/2;
        dem_raster_windowV=(dem_graph_raster->y2-dem_graph_raster->y1)/2;

        dem_graph_raster->repaint();
    }
}




