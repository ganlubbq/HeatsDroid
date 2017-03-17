#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "nucleo.h"
#include "graph_1D.h"
#include "graph_spect.h"
#include "graph_raster.h"
#include<QtMath>
#include <QPainter>//para objetos con la propiedad de dibujar en un widget
#include <QBrush>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Nucleo nucleo;

    //General
    //Tiempo
    graph_1D *gen_graph_tiempo;//grafica la forma de onda temporal
    int gen_tiempo_despH_range;
    bool gen_tiempo_despH_habilitar;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
    int gen_tiempo_centerH;//almacena el centro de la ventana que se visualiza
    int gen_tiempo_windowH;//almacena el ancho de la ventana que da el zoom horizontal
    int gen_tiempo_despV_range;
    bool gen_tiempo_despV_habilitar;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
    float gen_tiempo_centerV;//almacena el centro de la ventana que se visualiza verticalmente
    float gen_tiempo_windowV;//almacena el ancho de la ventana que da el zoom vertical    

    //Espectrograma
    graph_SPECT *gen_graph_spect;
    bool gen_spect_despH_habilitar;
    bool gen_spect_despV_habilitar;
    int gen_spect_centerH;
    int gen_spect_windowH;
    int gen_spect_centerV;
    int gen_spect_windowV;

    //Detección
    //Frecuencia
    //graph_1D *detect_graph_fft;//grafica la forma de onda temporal
    QGraphicsScene *detect_graph_fft_scene;
    QVector<QGraphicsRectItem*> detect_retangles;//almacena los rectángulos a pintar en la escena
    QGraphicsPolygonItem* detect_graph_fft_polygon;
    bool detect_PB_hab;//true para determinar que es la primera vez se oprime el botón detectar
    bool detect_f0_hab;
    bool detect_f1_hab;
    bool detect_f0_LE_hab;
    bool detect_f1_LE_hab;
    bool detect_signals_CB_hab;
    int detect_fft_despH_range;
    bool detect_fft_despH_habilitar;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
    int detect_fft_centerH;//almacena el centro de la ventana que se visualiza
    int detect_fft_windowH;//almacena el ancho de la ventana que da el zoom horizontal
    int detect_fft_despV_range;
    bool detect_fft_despV_habilitar;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
    float detect_fft_centerV;//almacena el centro de la ventana que se visualiza verticalmente
    float detect_fft_windowV;//almacena el ancho de la ventana que da el zoom vertical
    //Espectrograma
    graph_SPECT *detect_graph_spect;
    bool detect_spect_despH_habilitar;
    bool detect_spect_despV_habilitar;
    int detect_spect_centerH;
    int detect_spect_windowH;
    int detect_spect_centerV;
    int detect_spect_windowV;


    //Estimación
    //Frecuencia
    graph_1D *clasif_graph_fft;//grafica la forma de onda frecuencial
    int clasif_fft_despH_range;
    bool clasif_fft_despH_habilitar;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
    int clasif_fft_centerH;//almacena el centro de la ventana que se visualiza
    int clasif_fft_windowH;//almacena el ancho de la ventana que da el zoom horizontal
    int clasif_fft_despV_range;
    bool clasif_fft_despV_habilitar;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
    float clasif_fft_centerV;//almacena el centro de la ventana que se visualiza verticalmente
    float clasif_fft_windowV;//almacena el ancho de la ventana que da el zoom vertical
    //Panel de control
    bool clasif_resultados_habilitar;
    bool clasif_modu_habilitar;//permite modificar el combo box de las modulaciones de forma manual

    //Constelación
    graph_1D *clasif_graph_const;
    float clasif_const_centerH;
    float clasif_const_windowH;
    bool clasif__const_despH_habilitar;
    float clasif_const_centerV;
    float clasif_const_windowV;
    bool clasif__const_despV_habilitar;
    bool clasif_const_hab;
    bool clasif_const_lines_hab;

    //Espectrograma
    graph_SPECT *clasif_graph_spect;
    bool clasif_spect_despH_habilitar;
    bool clasif_spect_despV_habilitar;
    int clasif_spect_centerH;
    int clasif_spect_windowH;
    int clasif_spect_centerV;
    int clasif_spect_windowV;

    //Demodulación
    //Tiempo
    graph_1D *dem_graph_tiempo;//grafica la forma de onda temporal
    int dem_tiempo_despH_range;
    bool dem_tiempo_despH_habilitar;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
    int dem_tiempo_centerH;//almacena el centro de la ventana que se visualiza
    int dem_tiempo_windowH;//almacena el ancho de la ventana que da el zoom horizontal
    int dem_tiempo_despV_range;
    bool dem_tiempo_despV_habilitar;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
    float dem_tiempo_centerV;//almacena el centro de la ventana que se visualiza verticalmente
    float dem_tiempo_windowV;//almacena el ancho de la ventana que da el zoom vertical
    bool dem_PB_habilitar;

    //Frecuencia
    graph_1D *dem_graph_fft;//grafica la forma de onda temporal
    int dem_fft_despH_range;
    bool dem_fft_despH_habilitar;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
    int dem_fft_centerH;//almacena el centro de la ventana que se visualiza
    int dem_fft_windowH;//almacena el ancho de la ventana que da el zoom horizontal
    int dem_fft_despV_range;
    bool dem_fft_despV_habilitar;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
    float dem_fft_centerV;//almacena el centro de la ventana que se visualiza verticalmente
    float dem_fft_windowV;//almacena el ancho de la ventana que da el zoom vertical
    bool dem_f0_SB_hab; //habilita la conección del slider
    bool dem_freq_CB_hab; //habilita la selección del comboBox de frecuencia

    //Raster
    graph_raster *dem_graph_raster;
    int dem_raster_despH_range;
    bool dem_raster_despH_habilitar;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
    int dem_raster_centerH;//almacena el centro de la ventana que se visualiza
    int dem_raster_windowH;//almacena el ancho de la ventana que da el zoom horizontal
    int dem_raster_despV_range;
    bool dem_raster_despV_habilitar;//deshabilita el desplazamiento horizontal para que el zoom horizontal modifique sus datos
    float dem_raster_centerV;//almacena el centro de la ventana que se visualiza verticalmente
    float dem_raster_windowV;//almacena el ancho de la ventana que da el zoom vertical
    bool dem_patrones_CB_hab;
    bool dem_cod_CB_hab;



private slots:

    //Interfaz
    void on_actionAbrir_triggered();

    void on_actionSalir_triggered();

    void on_tabWidget_currentChanged(int index);//visualiza las pestañas correspondientes en función del tab seleccionados

    void on_toolBox_currentChanged(int index);//cambia el Tab en función de la herramienta seleccionada

    void restart_graphics(int value);//empleado para reiniciar la interfaz al abrir o detectar una nueva señal, ///value=0 se llama desde el botón abrir, 1 desde el botón detectar, 2 por cambio del tipo de modulación

    void information(int tab, int campo, int error);

    //Pestaña General
    void on_gen_tiempo_zoomH_SL_valueChanged(int value);

    void on_gen_tiempo_despH_SB_valueChanged(int value);

    void on_gen_tiempo_zoomV_SL_valueChanged(int value);

    void on_gen_tiempo_despV_SB_valueChanged(int value);

    void on_gen_spect_zoomH_SL_valueChanged(int value);

    void on_gen_spect_despH_SB_valueChanged(int value);

    void on_gen_spect_zoomV_SL_valueChanged(int value);

    void on_gen_spect_despV_SB_valueChanged(int value);    

    void on_spect_PB_clicked();

    void on_spect_solap_SpinB_valueChanged(int arg1);

    void on_spect_window_length_SpinB_valueChanged(int arg1);

    void on_spect_window_solap_LE_editingFinished();

    void on_spect_window_CB_currentIndexChanged(int index);

    //Detectar
    void on_detect_PB_clicked();

    void on_detect_signals_CB_currentIndexChanged(int index);

    void on_detect_filter_PB_clicked();

    void detect_pintar_fft(void);

    void on_detect_f0_SB_valueChanged(int value);

    void on_detect_f1_SB_valueChanged(int value);

    void on_detect_f0_LE_editingFinished();

    void on_detect_f1_LE_editingFinished();

    void on_detect_fft_zoomH_SL_valueChanged(int value);        

    void on_detect_fft_zoomV_SL_valueChanged(int value);

    void on_detect_spect_zoomH_SL_valueChanged(int value);

    void on_detect_spect_despH_SB_valueChanged(int value);

    void on_detect_spect_zoomV_SL_valueChanged(int value);

    void on_detect_spect_despV_SB_valueChanged(int value);



    //Estimar
    void on_clasif_PB_clicked();//manda a clasificar la señal

    void on_clasif_resultado_CB_currentIndexChanged(int index);//rellena los métodos en función de la modulación detectada

    void on_clasif_metodos_CB_currentIndexChanged(int index);//métodos de clasificación

    void on_clasif_const_CB_currentIndexChanged(int index);//selección entre Lazo de Cotas y Lazo Cuadrático

    void on_clasif_fft_zoomH_SL_valueChanged(int value);

    void on_clasif_fft_despH_SB_valueChanged(int value);

    void on_clasif_fft_zoomV_SL_valueChanged(int value);

    void on_clasif_fft_despV_SB_valueChanged(int value);

    void on_clasif_const_zoomH_SL_valueChanged(int value);

    void on_clasif_const_despH_SB_valueChanged(int value);

    void on_clasif_const_zoomV_SL_valueChanged(int value);

    void on_clasif_const_despV_SB_valueChanged(int value);

    void on_clasif_spect_zoomH_SL_valueChanged(int value);

    void on_clasif_spect_despH_SB_valueChanged(int value);

    void on_clasif_spect_zoomV_SL_valueChanged(int value);

    void on_clasif_spect_despV_SB_valueChanged(int value);

    void on_param_amp_LE_editingFinished();

    void on_param_SNR_LE_editingFinished();

    void on_param_baud_LE_editingFinished();


    //Demodular    
    void on_dem_PB_clicked();

    void on_dem_fft_zoomH_SL_valueChanged(int value);

    void on_dem_fft_despH_SB_valueChanged(int value);

    void on_dem_fft_zoomV_SL_valueChanged(int value);

    void on_dem_fft_despV_SB_valueChanged(int value);

    void on_dem_tiempo_zoomH_SL_valueChanged(int value);

    void on_dem_tiempo_despH_SB_valueChanged(int value);

    void on_dem_tiempo_zoomV_SL_valueChanged(int value);

    void on_dem_tiempo_despV_SB_valueChanged(int value);          

    void on_dem_f0_SB_valueChanged(int value);

    void on_dem_f1_SB_valueChanged(int value); 

    void on_dem_raster_zoomH_SL_valueChanged(int value);

    void on_dem_raster_despH_SB_valueChanged(int value);

    void on_dem_raster_zoomV_SL_valueChanged(int value);

    void on_dem_raster_despV_SB_valueChanged(int value);

    void on_dem_raster_CB_currentIndexChanged(int index);

    void on_clasif_const_lines_clicked(bool checked);

    void on_dem_raster_LE_returnPressed();

    void on_dem_exportar_PB_clicked();       

    void on_spect_color_clicked(bool checked);

    void on_dem_f0_LE_editingFinished();

    void on_dem_f1_LE_editingFinished();

    void on_dem_raster_LE_editingFinished();

    void on_dem_metodos_CB_currentIndexChanged(int index);

    void on_dem_freq_CB_currentIndexChanged(int index);

    void on_dem_cod_CB_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
