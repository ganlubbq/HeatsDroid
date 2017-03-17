#include "patronencontradoraster.h"

/////////////
/// \brief PatronEncontradoRaster::PatronEncontradoRaster
/// \package    Procesamiento
/// \todo       Constructor de la clase
///
PatronEncontradoRaster::PatronEncontradoRaster()
{
}

///////////////
/// \brief PatronEncontradoRaster::ConstructorSecuencia
/// \param e    Tamano de la secuencia
/// \package    Procesamiento
/// \todo       Reserva memoria para almacenar una secuencia de bits
///
void PatronEncontradoRaster::ConstructorSecuencia(int e)
{
    secuencia = (char *) malloc(e * sizeof(char));
}
