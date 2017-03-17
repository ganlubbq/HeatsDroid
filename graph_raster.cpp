#include "graph_raster.h"
#include <QPainter>//para objetos con la propiedad de dibujar en un widget
#include <QBrush>
#include <QDebug>
#include <qmath.h>

graph_raster::graph_raster(QWidget *parent) :
    QWidget(parent)
{
    bits=NULL;
    bits_length=0;
    patron=0;
    x1=0;
    x2=0;
    y1=0;
    y2=0;
}

void graph_raster::inicializar()
{
    bits=NULL;
    bits_length=0;
    patron=0;
    x1=0;
    x2=0;
    y1=0;
    y2=0;
}

void graph_raster::paintEvent(QPaintEvent *)
{

    QPainter painter(this);//se le pasa this para libere memoria al declararse el constructor

    painter.setBrush(Qt::black);//establece color de la brocha para el fondo del objeto

    //se pinta el fondo negro
    painter.drawRect(0, 0, width()-1, height()-1);

    //se pintan los bits 1 en blanco
    if(x2>patron)
        x2=patron;

    if(x1<0)
        x1=0;

    if(y2>floor(double(bits_length)/double(patron)))
        y2=floor(double(bits_length)/double(patron));

    if(y1<0)
        y1=0;


    if(bits_length!=0 && bits!=NULL)
    {
        //se calculan las dimensiones de los rectángulos
        double dx=floor(double(width())/double(x2-x1+1));

        double dy=floor(double(height())/double(y2-y1+1));


        //coordenadas del rectángulo
        double x=0;
        double y=0;

        int j_max=patron;
        int i_max=floor(double(bits_length)/double(patron));
        for(int j=0;j<j_max;j++)
        {
            for(int i=0;i<i_max;i++)
            {
                //se pinta si está entre los límites establecidos
                if(j>=x1 && j<=x2 && i>=y1 && i<=y2)
                {
                    x=(double(j-x1)*double(width())/double(x2-x1+1));
                    y=(double(i-y1)*double(height())/double(y2-y1+1));

                    //se pintan solo los blancos                   
                    if(bits[j+i*patron])
                        painter.fillRect(x, y, x+dx, y+dy, Qt::white);
                        //painter.setBrush(Qt::white);//establece color blanco para pintar los unos
                    else
                        painter.fillRect(x, y, x+dx, y+dy, Qt::black);

                }
            }
        }

    }
}
