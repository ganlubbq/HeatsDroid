#ifndef GRAPH_RASTER_H
#define GRAPH_RASTER_H

#include <QWidget>

class graph_raster : public QWidget
{
    Q_OBJECT
public:
    explicit graph_raster(QWidget *parent = 0);
    //marco de dibujo
    bool *bits;
    int bits_length;
    int patron;
    int x1;
    int x2;
    int y1;
    int y2;

signals:

public slots:
    void inicializar(void);
protected:
    void paintEvent(QPaintEvent *event);

};

#endif // GRAPH_RASTER_H
