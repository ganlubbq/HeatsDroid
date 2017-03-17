#include "graph_1D.h"

#include <QDebug>

graph_1D::graph_1D(QWidget *parent) :
    QWidget(parent)//ejecuta el constructor de la clase base
{
    x_p1=0;
    x_p2=0;
    y_p1=0;
    y_p2=0;
    diezm_factor=1;
    signal_2=NULL;
    signal_3=NULL;
    signal_2_length=0;
    signal_3_length=0;
    signal_lines=NULL;
    signal_lines_length=0;
    fm=0;

    //constelación
    signal_I=NULL;
    signal_Q=NULL;
    signal_I_length=0;
    diezm_const=0;
    const_lines=false;

    //pintar coordenadas
    pintar_coord_hab=false;

    //Configuración del Pen
    //se escoge el color y tipo de línea del pencil
    QVector<qreal> dashes;
    qreal space = 2;
    dashes << 2 << space << 2 << space << 2 << space
               << 2 << space << 2 << space;
    pen_coord.setDashPattern(dashes);
    pen_coord.setWidth(2);
    pen_coord.setColor(Qt::gray);

    //Para las señales
    pen_curv.setStyle(Qt::SolidLine);
    pen_curv.setCapStyle(Qt::SquareCap);
    pen_curv.setJoinStyle(Qt::MiterJoin);
    pen_curv.setColor(Qt::green);

    //Para el texto
    pen_text.setColor(Qt::white);
}

void graph_1D::inicializar(void)
{
    x_p1=0;
    x_p2=0;
    y_p1=0;
    y_p2=0;
    diezm_factor=1;
    signal_2_length=0;
    signal_3_length=0;
    signal_3=NULL;
    signal_2=NULL;
    signal_lines=NULL;
    signal_lines_length=0;
    fm=0;

    //constelación
    signal_I=NULL;
    signal_Q=NULL;
    signal_I_length=0;
    diezm_const=0;
    const_lines=false;

    //pintar coordenadas
    pintar_coord_hab=false;

    //Configuración del Pen
    //se escoge el color y tipo de línea del pencil
    QVector<qreal> dashes;
    qreal space = 2;
    dashes << 2 << space << 2 << space << 2 << space
               << 2 << space << 2 << space;
    pen_coord.setDashPattern(dashes);
    pen_coord.setWidth(2);
    pen_coord.setColor(Qt::gray);

    //Para las señales
    pen_curv.setStyle(Qt::SolidLine);
    pen_curv.setCapStyle(Qt::SquareCap);
    pen_curv.setJoinStyle(Qt::MiterJoin);
    pen_curv.setColor(Qt::green);

    //Para el texto
    pen_text.setColor(Qt::white);

    this->repaint();
}

graph_1D::~graph_1D()
{
    if(signal_lines_length!=0)
        delete []signal_lines;
    if(signal_2_length==0)
        delete []signal_2;
    if(signal_3_length==0)
        delete []signal_3;
}

void graph_1D::paintEvent(QPaintEvent *)
{

    QPainter painter(this);//se le pasa this para libere memoria al declararse el constructor

    painter.setBrush(Qt::black);//establece color de la brocha para el fondo del objeto

    //se pinta el fondo negro
    painter.drawRect(0, 0, width()-1, height()-1);

    //se escoge el color y tipo de línea del pencil para las señales
    painter.setPen(pen_curv);

    //painter.drawLine(0,0,width(),height());
    //se pintan solo 100 puntos por un efecto de diezmado
    int diezm=(x_p2-x_p1)/diezm_factor;
    if(diezm==0)
    {
        diezm=1;
    }

    //se pinta signal
    if(x_p2!=0)
    {
        float x1=0, x2=0, y1=0, y2=0;

        for(int i=x_p1; i<(x_p2-diezm); i+=diezm)
        {
            x1=(float(i-x_p1)/float(x_p2-x_p1))*float(width());
            x2=(float(i+diezm-x_p1)/float(x_p2-x_p1))*float(width());

            //dentro del marco
            if(signal[i]>=y_p1 && signal[i]<=y_p2)
            {

                y1=((y_p2-signal[i])/(y_p2-y_p1))*height();
                y2=((y_p2-signal[i+diezm])/(y_p2-y_p1))*height();
                painter.drawLine(x1,y1,x2,y2);

            }

            //fuera del marco
            if(signal[i]>=y_p1 && signal[i]<=y_p2 && signal[i+diezm]>=y_p2)
            {

                y1=((y_p2-signal[i])/(y_p2-y_p1))*height();
                y2=0;
                painter.drawLine(x1,y1,x2,y2);
            }

            //fuera del marco
            if(signal[i+diezm]>=y_p1 && signal[i+diezm]<=y_p2 && signal[i]>=y_p2)
            {

                y1=0;
                y2=((y_p2-signal[i+diezm])/(y_p2-y_p1))*height();
                painter.drawLine(x1,y1,x2,y2);
            }

            //fuera del marco
            if(signal[i]>=y_p1 && signal[i]<=y_p2 && signal[i+diezm]<=y_p1)
            {

                y1=((y_p2-signal[i])/(y_p2-y_p1))*height();
                y2=height();
                painter.drawLine(x1,y1,x2,y2);
            }

            //fuera del marco
            if(signal[i+diezm]>=y_p1 && signal[i+diezm]<=y_p2 && signal[i]<=y_p1)
            {

                y1=height();
                y2=((y_p2-signal[i+diezm])/(y_p2-y_p1))*height();
                painter.drawLine(x1,y1,x2,y2);
            }

            //fuera del marco
            if(signal[i]>=y_p2 && signal[i+diezm]<=y_p1)
            {

                y1=0;
                y2=height();
                painter.drawLine(x1,y1,x2,y2);
            }

            //fuera del marco
            if(signal[i]<=y_p1 && signal[i+diezm]>=y_p2)
            {

                y1=height();
                y2=0;
                painter.drawLine(x1,y1,x2,y2);
            }

        }



    }



    //se pinta signal_2
    if(x_p2!=0 && signal_2_length!=0)
    {
        //se escoge color rojo
        painter.setPen(Qt::red);

        float x1=0, x2=0, y1=0, y2=0;

        for(int i=x_p1; i<(x_p2-diezm); i+=diezm)
        {
            x1=(float(i-x_p1)/float(x_p2-x_p1))*float(width());
            x2=(float(i+diezm-x_p1)/float(x_p2-x_p1))*float(width());

            //dentro del marco
            if(signal_2[i]>=y_p1 && signal_2[i]<=y_p2)
            {

                y1=((y_p2-signal_2[i])/(y_p2-y_p1))*height();
                y2=((y_p2-signal_2[i+diezm])/(y_p2-y_p1))*height();
                painter.drawLine(x1,y1,x2,y2);

            }

            //fuera del marco
            if(signal_2[i]>=y_p1 && signal_2[i]<=y_p2 && signal_2[i+diezm]>=y_p2)
            {

                y1=((y_p2-signal_2[i])/(y_p2-y_p1))*height();
                y2=0;
                painter.drawLine(x1,y1,x2,y2);
            }

            //fuera del marco
            if(signal_2[i+diezm]>=y_p1 && signal_2[i+diezm]<=y_p2 && signal_2[i]>=y_p2)
            {

                y1=0;
                y2=((y_p2-signal_2[i+diezm])/(y_p2-y_p1))*height();
                painter.drawLine(x1,y1,x2,y2);
            }

            //fuera del marco
            if(signal_2[i]>=y_p1 && signal_2[i]<=y_p2 && signal_2[i+diezm]<=y_p1)
            {

                y1=((y_p2-signal_2[i])/(y_p2-y_p1))*height();
                y2=height();
                painter.drawLine(x1,y1,x2,y2);
            }

            //fuera del marco
            if(signal_2[i+diezm]>=y_p1 && signal_2[i+diezm]<=y_p2 && signal_2[i]<=y_p1)
            {

                y1=height();
                y2=((y_p2-signal_2[i+diezm])/(y_p2-y_p1))*height();
                painter.drawLine(x1,y1,x2,y2);
            }

            //fuera del marco
            if(signal_2[i]>=y_p2 && signal_2[i+diezm]<=y_p1)
            {

                y1=0;
                y2=height();
                painter.drawLine(x1,y1,x2,y2);
            }

            //fuera del marco
            if(signal_2[i]<=y_p1 && signal_2[i+diezm]>=y_p2)
            {

                y1=height();
                y2=0;
                painter.drawLine(x1,y1,x2,y2);
            }

        }

    }

    //se pinta signal_3
    if(x_p2!=0 && signal_3_length!=0)
    {
        //se escoge color amarillo
        painter.setPen(Qt::yellow);

        float x1=0, x2=0, y1=0, y2=0;

        for(int i=x_p1; i<(x_p2-diezm); i+=diezm)
        {
            x1=(float(i-x_p1)/float(x_p2-x_p1))*float(width());
            x2=(float(i+diezm-x_p1)/float(x_p2-x_p1))*float(width());

            //dentro del marco
            if(signal_3[i]>=y_p1 && signal_3[i]<=y_p2)
            {

                y1=((y_p2-signal_3[i])/(y_p2-y_p1))*height();
                y2=((y_p2-signal_3[i+diezm])/(y_p2-y_p1))*height();
                painter.drawLine(x1,y1,x2,y2);

            }

            //fuera del marco
            if(signal_3[i]>=y_p1 && signal_3[i]<=y_p2 && signal_3[i+diezm]>=y_p2)
            {

                y1=((y_p2-signal_3[i])/(y_p2-y_p1))*height();
                y2=0;
                painter.drawLine(x1,y1,x2,y2);
            }

            //fuera del marco
            if(signal_3[i+diezm]>=y_p1 && signal_3[i+diezm]<=y_p2 && signal_3[i]>=y_p2)
            {

                y1=0;
                y2=((y_p2-signal_3[i+diezm])/(y_p2-y_p1))*height();
                painter.drawLine(x1,y1,x2,y2);
            }

            //fuera del marco
            if(signal_3[i]>=y_p1 && signal_3[i]<=y_p2 && signal_3[i+diezm]<=y_p1)
            {

                y1=((y_p2-signal_3[i])/(y_p2-y_p1))*height();
                y2=height();
                painter.drawLine(x1,y1,x2,y2);
            }

            //fuera del marco
            if(signal_3[i+diezm]>=y_p1 && signal_3[i+diezm]<=y_p2 && signal_3[i]<=y_p1)
            {

                y1=height();
                y2=((y_p2-signal_3[i+diezm])/(y_p2-y_p1))*height();
                painter.drawLine(x1,y1,x2,y2);
            }

            //fuera del marco
            if(signal_3[i]>=y_p2 && signal_3[i+diezm]<=y_p1)
            {

                y1=0;
                y2=height();
                painter.drawLine(x1,y1,x2,y2);
            }

            //fuera del marco
            if(signal_3[i]<=y_p1 && signal_3[i+diezm]>=y_p2)
            {

                y1=height();
                y2=0;
                painter.drawLine(x1,y1,x2,y2);
            }

        }

    }


    //se pintan las líneas signal_3 de frecuencia en el espectro
    if(signal_lines_length!=0)
    {


        if(x_p2!=0)
        {
            float x1=0;

            for(int i=0; i<signal_lines_length; i++)
            {
                if(i==signal_lines_selecc)
                    painter.setPen(Qt::red);
                else
                    painter.setPen(Qt::white);

                //si está en el marco
                if(signal_lines[i]>x_p1 && signal_lines[i]<x_p2)
                {


                    x1=(float(signal_lines[i]-x_p1)/float(x_p2-x_p1))*float(width());
                    //se pinta la línea si está en el marco
                    painter.drawLine(x1,height(),x1,0);
                }
            }
        }
    }

    //se pintan las coordenadas
    if(pintar_coord_hab)
    {
        //se pintan las líneas
        painter.setPen(pen_coord);
        //horizontales
        painter.drawLine(0,height()/4,width(),height()/4);
        painter.drawLine(0,height()/2,width(),height()/2);        
        painter.drawLine(0,height()-height()/4,width(),height()-height()/4);

        //verticales
        painter.drawLine(double(width())/double(4),0,double(width())/double(4),double(height()));
        painter.drawLine(double(width())/double(2),0,double(width())/double(2),double(height()));
        painter.drawLine(double(3)*double(width())/double(4),0,double(3)*double(width())/double(4),double(height()));

        //se coloca el texto
        double y1,y2,y3,x1,x2,x3;
        y1=double(y_p2+y_p1)/double(2)+double(y_p2-y_p1)/double(4);
        y2=double(y_p2+y_p1)/double(2);
        y3=double(y_p2+y_p1)/double(2)-double(y_p2-y_p1)/double(4);
        x1=double(x_p2+x_p1)/double(2)/fm-double(x_p2+x_p1)/double(4)/fm;
        x2=double(x_p2+x_p1)/double(2)/fm;
        x3=double(x_p2+x_p1)/double(2)/fm+double(x_p2+x_p1)/double(4)/fm;

        painter.setPen(pen_text);
        painter.drawText(0,height()/4,QString("%1").arg(y1));
        painter.drawText(0,height()/2,QString("%1").arg(y2));
        painter.drawText(0,height()-height()/4,QString("%1").arg(y3));
        painter.drawText(width()/4,height(),QString("%1").arg(x1));
        painter.drawText(width()/2,height(),QString("%1").arg(x2));
        painter.drawText(3*width()/4,height(),QString("%1").arg(x3));
    }


    if(signal_I_length!=0)
    {
        float x1=0, x2=0, y1=0, y2=0;

        painter.setBrush(Qt::green);

        //dibujo de puntos de la constelación
        for(int i=0; i<signal_I_length-diezm_const; i+=diezm_const)
        {

            //dentro del marco
            if(signal_I[i]>=x_const_p1 && signal_I[i]<=x_const_p2 && signal_Q[i]>=y_const_p1 && signal_Q[i]<=y_const_p2)
            {

                //se pinta la constelación

                x1=(float(signal_I[i]-x_const_p1)/float(x_const_p2-x_const_p1))*float(width());
                y1=((y_const_p2-signal_Q[i])/(y_const_p2-y_const_p1))*height();
                x2=(float(signal_I[i+1]-x_const_p1)/float(x_const_p2-x_const_p1))*float(width());
                y2=((y_const_p2-signal_Q[i+1])/(y_const_p2-y_const_p1))*height();

                painter.drawEllipse(x1,y1,2,2);
                //se dibujan las líneas
                if(const_lines)
                    painter.drawLine(x1,y1,x2,y2);

            }

        }

        //dibujo de las líneas de la constelación
        for(int i=0; i<signal_I_length-diezm_const; i+=diezm_const)
        {

            //dentro del marco
            if(signal_I[i]>=x_const_p1 && signal_I[i]<=x_const_p2 && signal_Q[i]>=y_const_p1 && signal_Q[i]<=y_const_p2)
            {

                x1=(float(signal_I[i]-x_const_p1)/float(x_const_p2-x_const_p1))*float(width());
                y1=((y_const_p2-signal_Q[i])/(y_const_p2-y_const_p1))*height();

                painter.drawEllipse(x1,y1,2,2);

            }

        }
    }
}
