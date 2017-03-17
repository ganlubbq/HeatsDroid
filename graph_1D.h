#ifndef GRAPH_TEMPORAL_H
#define GRAPH_TEMPORAL_H

#include <QWidget>
#include <QPainter>//para objetos con la propiedad de dibujar en un widget
#include <QBrush>

class graph_1D : public QWidget
{
    Q_OBJECT//para que la clase sea detectada como un objeto
public:
    explicit graph_1D(QWidget *parent = 0);
    ~graph_1D();
    //señal 1
    double *signal;
    int x_p1;//límite horizontal izquierdo
    int x_p2;//límite horizontal derecho
    float y_p1;//límite vertical inferior
    float y_p2;//límite vertical superior
    int diezm_factor;//total de puntos a pintar en pantalla
    double fm;//determina la frecuencia de muestreo de la señal

    //señal 2 y 3 para la demodulación
    double *signal_2;
    int signal_2_length;
    double *signal_3;
    int signal_3_length;

    //señal 3 para líneas de frecuencia en gráfico de espectro
    int *signal_lines=NULL;
    int signal_lines_length=0;
    int signal_lines_selecc=-1;

    //para pintar coordenadas
    bool pintar_coord_hab;

    //señal 4 para la constelación
    double *signal_I;
    double *signal_Q;
    bool const_lines;
    int signal_I_length;
    int diezm_const;
    double x_const_p1;//límite horizontal izquierdo
    double x_const_p2;//límite horizontal derecho
    double y_const_p1;//límite vertical inferior
    double y_const_p2;//límite vertical superior

    //Pencil
    QPen pen_coord;
    QPen pen_curv;
    QPen pen_text;

signals:

public slots:
    void inicializar(void);

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // GRAPH_TEMPORAL_H
