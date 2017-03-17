#ifndef SECUENCIACANTIDAD_H
#define SECUENCIACANTIDAD_H

///
/// \brief The SecuenciaCantidad class
/// \package    Modelo
/// \note       Almacena informacion sobre ubicaciones de un patrons
///
class SecuenciaCantidad
{
public:
    SecuenciaCantidad();
    ///
    /// \brief numeroDondeSeEncuentra       Periodo que se emplea para visualizar el Raster
    ///
    int numeroDondeSeEncuentra;
    ///
    /// \brief cantVecesEncontrado          cantida de veces que se ha encontrado el patron
    ///
    int cantVecesEncontrado;
    ///
    /// \brief primeraVezDondeSeEncuentra   QUITAR
    ///
    int primeraVezDondeSeEncuentra;
};

#endif // SECUENCIACANTIDAD_H
