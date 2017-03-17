#include "graph_spect.h"
#include <QDebug>
#include <qmath.h>

graph_SPECT::graph_SPECT(QWidget *parent) :
    QWidget(parent)
{
    spect=NULL;
    spect_length=0;
    spect_max=0;//almacena el máximo del espectrograma para dibujar en escala de grises
    //límites a dibujar en los ejes x e y
    x1=0;
    x2=0;
    y1=0;
    y2=0;
    spect_color_scale=0;

    //Configuración del Pen
    pintar_coord_hab=false;
    //se escoge el color y tipo de línea del pencil
    QVector<qreal> dashes;
    qreal space = 2;
    dashes << 2 << space << 2 << space << 2 << space
               << 2 << space << 2 << space;
    pen_coord.setDashPattern(dashes);
    pen_coord.setWidth(2);
    pen_coord.setColor(Qt::gray);

    //Para el texto
    pen_text.setColor(Qt::white);
    fm_tiempo=0;
    fm_freq=0;
    Tiempo=0;
}


void graph_SPECT::inicializar(void)
{
    spect=NULL;
    spect_length=0;
    spect_max=0;//almacena el máximo del espectrograma para dibujar en escala de grises
    //límites a dibujar en los ejes x e y
    x1=0;
    x2=0;
    y1=0;
    y2=0;
    spect_color_scale=0;

    //Configuración del Pen
    pintar_coord_hab=false;
    //se escoge el color y tipo de línea del pencil
    QVector<qreal> dashes;
    qreal space = 2;
    dashes << 2 << space << 2 << space << 2 << space
               << 2 << space << 2 << space;
    pen_coord.setDashPattern(dashes);
    pen_coord.setWidth(2);
    pen_coord.setColor(Qt::gray);

    //Para el texto
    pen_text.setColor(Qt::white);
    fm_tiempo=0;
    fm_freq=0;
    Tiempo=0;

    this->repaint();
}


void graph_SPECT::paintEvent(QPaintEvent *)
{
    QPainter painter(this);//se le pasa this para libere memoria al declararse el constructor

    painter.setBrush(Qt::black);//establece color de la brocha para el fondo del objeto

    //se pinta el fondo negro
    painter.drawRect(0, 0, width()-1, height()-1);

    //se pasa a pintar la matriz
    if(spect_length!=0)
    {

        QColor color;
        //coordenadas del rectángulo
        qreal x;
        qreal y;
        //dimensiones del rectángulo
        qreal dx=double(this->width())/double(x2-x1);
        qreal dy=double(this->height())/double(y2-y1);

        QRectF rectangle;

        for(int i=x1; i<x2;i++)
        {
            for(int j=y1; j<y2;j++)
            {
                x=double(i-x1)/double(x2-x1)*double(this->width());
                y=double(j-y1)/double(y2-y1)*double(this->height());
                rectangle.setRect(x,y,dx,dy);
                double c=spect[j][i]/spect_max;
                if (spect_color_scale)
                {
                    color.setRgb(estimate_color(c));
                }
                else
                {
                    color.setRgbF(c,c,c,1);
                }
                painter.fillRect(rectangle, color);
            }
        }
    }

    if(pintar_coord_hab)
    {
        //se pintan las líneas
        painter.setPen(pen_coord);
        //horizontales
        painter.drawLine(0,height()/4,width(),height()/4);
        painter.drawLine(0,height()/2,width(),height()/2);
        painter.drawLine(0,height()-height()/4,width(),height()-height()/4);

        //verticales
        painter.drawLine(double(width())/(4),0,double(width())/(4),double(height()));
        painter.drawLine(double(width())/double(2),0,double(width())/double(2),double(height()));
        painter.drawLine(double(3)*double(width())/double(4),0,double(3)*double(width())/double(4),double(height()));

        //se coloca el texto
        double yp1,yp2,yp3,xp1,xp2,xp3;
        yp1=(double(y2+y1)/double(2)-double((y2-y1))/double(4))*Tiempo/double(spect_length);
        yp2=(double(y2+y1)/double(2))*Tiempo/double(spect_length);
        yp3=(double(y2+y1)/double(2)+double((y2-y1))/double(4))*Tiempo/double(spect_length);
        xp1=double(x2+x1)/double(2)/fm_freq-double(x2+x1)/double(4)/fm_freq;
        xp2=double(x2+x1)/double(2)/fm_freq;
        xp3=double(x2+x1)/double(2)/fm_freq+double(x2+x1)/double(4)/fm_freq;

        painter.setPen(pen_text);
        painter.drawText(0,height()/4,QString("%1").arg(yp1)+QString(" s"));
        painter.drawText(0,height()/2,QString("%1").arg(yp2)+QString(" s"));
        painter.drawText(0,height()-height()/4,QString("%1").arg(yp3)+QString(" s"));
        painter.drawText(double(width())/double(4),height(),QString("%1").arg(xp1)+QString(" Hz"));
        painter.drawText(width()/2,height(),QString("%1").arg(xp2)+QString(" Hz"));
        painter.drawText(3*width()/4,height(),QString("%1").arg(xp3)+QString(" Hz"));
    }


}


QRgb graph_SPECT::estimate_color(double value)
{
    QRgb color;
    int v , r , a;
    if( value <= 0.25 )
    {
        r = 10;
        v = (int) (255 * value / 0.33);
        a = 140;
    }else
    if( value <= 0.55 )
    {
        r = 10;
        v = 220;
        a = (int)(140 * (2 - value / 0.33));
    }
    else
    {
        r = (int)(255 * (value / 0.33 - 2) );
        a = 0;
        v = (int)(220 * (1 - value) / 0.33);
    }
    color = qRgb(r , v , a);

    return color;
}
