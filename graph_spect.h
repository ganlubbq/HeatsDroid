#ifndef GRAPH_SPECT_H
#define GRAPH_SPECT_H

#include <QWidget>
#include <QPainter>//para objetos con la propiedad de dibujar en un widget
#include <QBrush>

class graph_SPECT : public QWidget
{
    Q_OBJECT
public:
    explicit graph_SPECT(QWidget *parent = 0);

    double **spect;
    int spect_length;
    double spect_max;//almacena el máximo del espectrograma para dibujar en escala de grises
    //límites a dibujar en los ejes x e y
    int x1;
    int x2;
    int y1;
    int y2;
    bool spect_color_scale;
    QRgb estimate_color(double value);

    //pen coordenadas
    QPen pen_coord;
    QPen pen_text;
    bool pintar_coord_hab;
    double fm_tiempo;
    double fm_freq;
    double Tiempo;//alamacena la duración de la señal


signals:

public slots:
    void inicializar(void);

protected:
    void paintEvent(QPaintEvent *);

};

#endif // GRAPH_SPECT_H
