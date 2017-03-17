#ifndef PATRONENCONTRADORASTER_H
#define PATRONENCONTRADORASTER_H

#include "secuenciacantidad.h"
#include <QVector>

///
/// \brief The PatronEncontradoRaster class
/// \package    Modelo
/// \note       Esta clase guarda informacion de un patron
///
class PatronEncontradoRaster
{
public:
    PatronEncontradoRaster();
    void ConstructorSecuencia(int);
    ///
    /// \brief frecuenciaAparicion      Ubicaciones donde se ha encontrado el patron
    ///
    QVector<int> frecuenciaAparicion;
    ///
    /// \brief secuencia                Secuencia de bits del patron
    ///
    char* secuencia;
    ///
    /// \brief patronNum                Numero decimal correspondiente a la secuencia
    ///                                 del patron
    ///
    unsigned long long patronNum;
    ///
    /// \brief cantBit                  Cantidad de bits en el patron
    ///
    int cantBit;
    ///
    /// \brief sec                      Instancia de la clase SecuenciaCantidad
    ///
    SecuenciaCantidad sec;
};

#endif // PATRONENCONTRADORASTER_H
