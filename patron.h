#ifndef PATRON_H
#define PATRON_H

#include "patronencontradoraster.h"
#include <QObject>
#include <QVector>
#include <math.h>
#include <QDebug>
///
/// \brief The Patron class
/// \package    Modelo
/// \note       Esta clase encuentra los patrones en una secuencia de bits
///
class Patron //: public QObject
{

public:
    Patron();
    QVector<PatronEncontradoRaster> RasterPolinomio(int , QVector<char> , double , int );
    void getData(QVector<int> &);
    ///
    /// \brief patros       Almacena patrones encontrados en la secuencia de bits
    ///                     para el Raster
    ///
    QVector<int>patros;

protected slots:
    void eliminarSubPatron(QVector <PatronEncontradoRaster> &, int);
    int BuscarPatronEnRasterPeriodicidad2222(const QVector <PatronEncontradoRaster> &, QVector<char>, int);
    PatronEncontradoRaster BuscarPatronMasRepetido(const QVector <PatronEncontradoRaster> &);
    QVector<SecuenciaCantidad> VerificarCadenaDeFrecuencia(const QVector <int> &);
    QVector <PatronEncontradoRaster> EncontrarPatronesEnLista(QVector <PatronEncontradoRaster> &);
    void EliminarSubPatrones(QVector <PatronEncontradoRaster> &);
    bool IgualSecuenciaHastaCiertoNumero(PatronEncontradoRaster, PatronEncontradoRaster);
    QVector <PatronEncontradoRaster> BuscarTresPatronesMasRepetidos(const QVector <PatronEncontradoRaster> &);
    void seleccion_lims(QVector<int> &, int, int);
};
#endif // PATRON_H
