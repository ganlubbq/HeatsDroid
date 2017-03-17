// RasterPolinomio.cpp : main project file.

#include "patron.h"


///
/// \brief Patron::Patron
/// \note   Constructor de la clase
Patron::Patron()
{

}

///
/// \brief Patron::eliminarSubPatron
/// \param patrones lista de patrones
/// \param pos      posicion en la lista de patrones
/// \note           Elimina el elemento en la posicion "pos" en la lista "patrones"
///
void Patron::eliminarSubPatron(QVector <PatronEncontradoRaster> &patrones, int pos)
{
        int i;
        for (i = pos; i < patrones.size() - 1; i++)
                patrones[i] = patrones[i + 1];
        patrones.pop_back();  // remove last elt
}

///
/// \brief Patron::BuscarPatronEnRasterPeriodicidad2222
/// \param patrones     lista de patrones encontrados
/// \param patronActual secuencia del patron que se esta analizando
/// \param error        margen de error permitido
/// \return             Devuelve la posicion del patron "patronActual" dentro de la lista "patrones"
///
int Patron::BuscarPatronEnRasterPeriodicidad2222(const QVector <PatronEncontradoRaster> &patrones, QVector<char> patronActual, int error) //OK, REVISADO Y PROBADO
{
    int existe=-1;
    int margenError=0;
    //int comparar=0;
    int i=0;
    int l = 0;
    if (!patrones.empty())
    {
        while(i < patrones.size() && existe == -1)
        {
            //busco patrones del mismo tamano
            if (patrones[i].cantBit==patronActual.size())
            {
                margenError=0;
                l = 0;
                while(l < patronActual.size() && margenError <= error)
                {
                    if(patrones[i].secuencia[l] != patronActual[l])
                        margenError++;
                    //comparar = l + 1;
                    l++;
                }
                if (margenError <= error)/* && comparar==patronActual.size()*/
                    existe = i;
                i++;
            }
            else
                i++;
        }
    }
    return existe;
}

///
/// \brief Patron::BuscarPatronMasRepetido
/// \param patrones lista de patrones encontrados
/// \return         Devuelve el patron con mayor frecencia de aparicion
///
PatronEncontradoRaster Patron::BuscarPatronMasRepetido(const QVector <PatronEncontradoRaster> &patrones)
{
        PatronEncontradoRaster patronMasRepetido;
        if(!patrones.empty())
                {
                        patronMasRepetido=patrones[0];
                        int i=1;
                        while (i<patrones.size())
                        {
                                if (patrones[i].frecuenciaAparicion.size()>patronMasRepetido.frecuenciaAparicion.size())
                                        patronMasRepetido=patrones[i];
                                i++;
                        }
                }

        return patronMasRepetido;

}

///
/// \brief Patron::VerificarCadenaDeFrecuencia
/// \param frecuencia   lista con las frecuencias de las apariciones de un patron
/// \return             me devuelve cuantas veces una frecuencia (mayor que 60) se repite ej: 424, 24 veces; 12, 44 veces.
/// \note               Se seleccionan las frecuencias de aparicion "si son mayores que 60 bits de diferencia"
///
QVector<SecuenciaCantidad> Patron::VerificarCadenaDeFrecuencia(const QVector <int> &frecuencia)
{
    QVector<SecuenciaCantidad>temp;
//    int c=0;
    int pos=-1;
    int i=0;
    if (!frecuencia.empty())
    {
        while (i < frecuencia.size())
        {
            if (frecuencia[i] >= 60)
            {
                int f=0;
                if (!temp.empty())
                {
                    bool found=false;
                    while(f<temp.size() && found==false)
                    {
                        if (temp[f].numeroDondeSeEncuentra==frecuencia[i])
                            found=true;
                        if (found==false)
                            f++;
                    }
                    if (found==false)
                        pos = -1;
                    else
                        pos = f;
                }
                if (pos==-1)
                {
                    SecuenciaCantidad sc;
                    sc.numeroDondeSeEncuentra = frecuencia[i];
                    sc.cantVecesEncontrado = 1;
                    sc.primeraVezDondeSeEncuentra = i;
                    temp.push_back(sc);
                }
                else
                    temp[pos].cantVecesEncontrado = temp[pos].cantVecesEncontrado + 1;
            }
            i++;
        }
    }
    return temp;
}

///
/// \brief Patron::EncontrarPatronesEnLista
/// \param patronesEncontrados  lista con patrones
/// \return                     lista con patrones actualizada
/// \note                       Verifica si el patron es de solo "1s" o "0s", si lo es lo borra; luego resta las posiciones de aparicion
///                             del patron con la anterior, y lo guarda como patron
///
QVector <PatronEncontradoRaster> Patron::EncontrarPatronesEnLista(QVector <PatronEncontradoRaster> &patronesEncontrados) //IT'S OK
{
    //QVector<PatronEncontradoRaster> patronesReales;
    QVector<SecuenciaCantidad> cantidadPorSecuenciaEncontrada;

    if (!patronesEncontrados.empty())
    {
        int i=0;
        QVector<int>frecuencias;
        //analiza cada patron encontrado
        while(i<patronesEncontrados.size())
        {
            int u=0;
            int contador1=0;
            int contador2=0;
            int hasta=patronesEncontrados[i].cantBit;
            while(u<hasta) //cuenta cuantos 1 y 0 hay
            {
                    if(patronesEncontrados[i].secuencia[u] == '1')
                            contador1++;
                    else
                            contador2++;
                    u++;
            }
            //descarta todos los que tengas 1s o 0s en una secuencia
            if (contador1!=hasta && contador2!=hasta)//entra si la secuencia es diferente de unos o ceros
            {
                if (patronesEncontrados[i].frecuenciaAparicion.size() != 1)
                {
                    if (!frecuencias.empty())
                            frecuencias.clear();

                    //2.2.2.2.4.4.4.18.18.28
                    for (int p = 0; p < patronesEncontrados[i].frecuenciaAparicion.size() - 1; p++)
                        frecuencias.push_back(patronesEncontrados[i].frecuenciaAparicion[p + 1] - patronesEncontrados[i].frecuenciaAparicion[p]);

                    SecuenciaCantidad varTemp;
                    if (!frecuencias.empty())
                    {
                        //devuelve las diferencias de posiciones de los patrones (18, 28) y cuantas veces se repiten
                        cantidadPorSecuenciaEncontrada = VerificarCadenaDeFrecuencia(frecuencias);
                        int primeraVez = -1;

                        if (cantidadPorSecuenciaEncontrada.size()==1)
                        {
                            primeraVez = cantidadPorSecuenciaEncontrada[0].primeraVezDondeSeEncuentra + 1;
                            cantidadPorSecuenciaEncontrada[0].primeraVezDondeSeEncuentra=patronesEncontrados[i].frecuenciaAparicion[primeraVez];
                            varTemp=cantidadPorSecuenciaEncontrada[0];
                            patronesEncontrados[i].sec=varTemp;
                        }
                        else if (cantidadPorSecuenciaEncontrada.empty())
                        {
                                eliminarSubPatron(patronesEncontrados,i);
                                i--;
                        }
                        else
                        {
                            varTemp=cantidadPorSecuenciaEncontrada[0];
                            for (int yyy=1;yyy<cantidadPorSecuenciaEncontrada.size();yyy++)//mayor que (busca la mayor frecuencia de aparicion de una secuencia)
                            {
                                if (varTemp.cantVecesEncontrado < cantidadPorSecuenciaEncontrada[yyy].cantVecesEncontrado)
                                {
                                        varTemp=cantidadPorSecuenciaEncontrada[yyy];
                                }

                            }
                            primeraVez = varTemp.primeraVezDondeSeEncuentra+1;
                            varTemp.primeraVezDondeSeEncuentra=patronesEncontrados[i].frecuenciaAparicion[primeraVez];
                            patronesEncontrados[i].sec=varTemp;

                        }
                    }
                }
                else
                {
                    eliminarSubPatron(patronesEncontrados,i);//elimina el atron si solo se encuentra una sols vez
                    i--;
                }
            }
            else
            {
                eliminarSubPatron(patronesEncontrados,i);//elimina el patrones que sea solo 1s y 0s
                i--;
            }
            i++;
        }
    }
    return patronesEncontrados;
}

///
/// \brief Patron::IgualSecuenciaHastaCiertoNumero
/// \param patronTemp   patron
/// \param patronTemp2  patron
/// \return             true si las dos secuencias de bits son iguales
///
bool Patron::IgualSecuenciaHastaCiertoNumero(PatronEncontradoRaster patronTemp,PatronEncontradoRaster patronTemp2)
{
    bool igual=true;
    int i = 0, cant=0;
    if (patronTemp.cantBit <= patronTemp2.cantBit)
        cant = patronTemp.cantBit;
    else
        if (patronTemp.cantBit>patronTemp2.cantBit)
            cant = patronTemp2.cantBit;
    while (i < cant && igual==true)
    {
        if (patronTemp.secuencia[i] != patronTemp2.secuencia[i])
            igual = false;
        i++;
    }
    return igual;
}

///
/// \brief Patron::EliminarSubPatrones
/// \param patrones Lista de patrones
/// \note           Elimina un patron si este se encuentra dentro de otro
///
void Patron::EliminarSubPatrones(QVector <PatronEncontradoRaster> &patrones)
{
    PatronEncontradoRaster patronTemp;
    PatronEncontradoRaster patronTemp2;
    int e = 0;
    int pos=0;
    while(e < patrones.size())
    {
        pos = e + 1;
        while (pos < patrones.size())
        {
            patronTemp=patrones[e];
            patronTemp2=patrones[pos];
            if(patronTemp.cantBit != patronTemp2.cantBit)
            {
                bool NoPatron=false;
                bool igualSecuencia=IgualSecuenciaHastaCiertoNumero(patronTemp, patronTemp2); //funciona fucion
                if (igualSecuencia==true)
                {
                    int cant=patronTemp.frecuenciaAparicion.size();
                    if (patronTemp2.frecuenciaAparicion.size() < cant)
                            cant=patronTemp2.frecuenciaAparicion.size();
                    int t=0;
                    bool CambiarEstado1=false;
                    bool CambiarEstado2=false;
                    bool CambiarEstado3=false;

                    if (patronTemp2.frecuenciaAparicion[t]==patronTemp.frecuenciaAparicion[t])
                            CambiarEstado1=true;
                    else if (patronTemp2.frecuenciaAparicion[t+1]==patronTemp.frecuenciaAparicion[t])
                            CambiarEstado2=true;
                    else if (patronTemp2.frecuenciaAparicion[t]==patronTemp.frecuenciaAparicion[t+1])
                            CambiarEstado3=true;
                    t++;
                    if (CambiarEstado1==true)
                    {
                        while(t<cant-50 && NoPatron==false)//resto -50 pork es la longitud maxima k voy a buscar un patron
                        {
                            if (patronTemp2.frecuenciaAparicion[t]!=patronTemp.frecuenciaAparicion[t])
                                NoPatron=true;
                            t++;
                        }
                    }
                    if (CambiarEstado2==true)
                    {
                        while(t<cant-50 && NoPatron==false)
                        {
                            if (patronTemp2.frecuenciaAparicion[t + 1]!=patronTemp.frecuenciaAparicion[t])
                                NoPatron=true;
                            t++;
                        }
                    }
                    if (CambiarEstado3==true)
                    {
                        while(t<cant-50 && NoPatron==false)
                        {
                            if (patronTemp2.frecuenciaAparicion[t]!=patronTemp.frecuenciaAparicion[t+1])
                                NoPatron=true;
                            t++;
                        }
                    }
                    if (NoPatron==false)
                    {
                        if(patronTemp.cantBit<patronTemp2.cantBit)
                        {
                            eliminarSubPatron(patrones, e);
                            pos = e + 1;
                            patronTemp = patrones[0];
                        }
                        else
                            eliminarSubPatron(patrones, pos);
                    }
                    else
                        pos++;
                }
                else
                    pos++;
            }
            else
                pos++;
        }
        e++;
    }
}

///
/// \brief Patron::BuscarTresPatronesMasRepetidos
/// \param patrones Lista con los patrones encontrados
/// \return         Devuelve los 3 patrones que mas se repitieron
/// \note           Se van posicionando los patrones mas repetidos y se llena la lista "patros" con todos los patrones
///
QVector <PatronEncontradoRaster> Patron::BuscarTresPatronesMasRepetidos(const QVector <PatronEncontradoRaster> &patrones)
{
    QVector <PatronEncontradoRaster> devolver(3);
    SecuenciaCantidad temp;
    patros.clear();
    int contador = 0;
    if(patrones.size() > 25)
    {

        devolver[0]=patrones[0];
        int i=1;
        while(i < patrones.size())
        {
            temp=patrones[i].sec;

            if (temp.cantVecesEncontrado >= 10)
                patros.push_back(patrones[i].sec.numeroDondeSeEncuentra);

            if ((devolver[0].sec.numeroDondeSeEncuentra == temp.numeroDondeSeEncuentra) && (devolver[0].sec.cantVecesEncontrado < temp.cantVecesEncontrado))
            {
                devolver[0] = patrones[i];
                //patros.push_back(patrones[i].sec.numeroDondeSeEncuentra);
            }
            else
                if ((devolver[0].sec.numeroDondeSeEncuentra != temp.numeroDondeSeEncuentra) && (devolver[0].sec.cantVecesEncontrado < temp.cantVecesEncontrado))
                {
                    devolver[2]=devolver[1];
                    devolver[1]=devolver[0];
                    devolver[0]=patrones[i];
                    //patros.push_back(patrones[i].sec.numeroDondeSeEncuentra);
                }
                else
                    if ((devolver[1].sec.numeroDondeSeEncuentra == temp.numeroDondeSeEncuentra) && (devolver[1].sec.cantVecesEncontrado < temp.cantVecesEncontrado) && (devolver[0].sec.numeroDondeSeEncuentra != temp.numeroDondeSeEncuentra))
                    {
                        //patros.push_back(patrones[i].sec.numeroDondeSeEncuentra);
                        devolver[1]=patrones[i];
                    }
                    else
                        if ((devolver[1].sec.numeroDondeSeEncuentra != temp.numeroDondeSeEncuentra) && (devolver[1].sec.cantVecesEncontrado < temp.cantVecesEncontrado)&& (devolver[0].sec.numeroDondeSeEncuentra != temp.numeroDondeSeEncuentra))
                        {
                                devolver[2]=devolver[1];
                                devolver[1]=patrones[i];
                                //patros.push_back(patrones[i].sec.numeroDondeSeEncuentra);
                        }
                        else
                            if ((devolver[2].sec.numeroDondeSeEncuentra == temp.numeroDondeSeEncuentra) && (devolver[2].sec.cantVecesEncontrado < temp.cantVecesEncontrado)&& (devolver[0].sec.numeroDondeSeEncuentra != temp.numeroDondeSeEncuentra)&& (devolver[1].sec.numeroDondeSeEncuentra != temp.numeroDondeSeEncuentra))
                            {
                                devolver[2]=patrones[i];
                                //patros.push_back(patrones[i].sec.numeroDondeSeEncuentra);
                            }
                        else
                            if ((devolver[2].sec.numeroDondeSeEncuentra != temp.numeroDondeSeEncuentra) && (devolver[2].sec.cantVecesEncontrado < temp.cantVecesEncontrado)&&(devolver[0].sec.numeroDondeSeEncuentra != temp.numeroDondeSeEncuentra)&&(devolver[1].sec.numeroDondeSeEncuentra != temp.numeroDondeSeEncuentra))
                            {
                                devolver[2]=patrones[i];
                                //patros.push_back(patrones[i].sec.numeroDondeSeEncuentra);
                            }
            i++;
        }

        return devolver;
    }
    else
    {
        //qDebug("pocos");
        patros.clear();
        QVector <PatronEncontradoRaster> devolver2;
        for (int ll=0;ll<patrones.size();ll++)
        {
            devolver2.push_back(patrones[ll]);
            patros.push_back(patrones[ll].sec.numeroDondeSeEncuentra);
        }
        return devolver2;
    }
}

///
/// \brief Patron::getData
/// \param asd  Vector que almacenara los periodos para el raster
/// \note       Si hay un numero repetido, se eliminan las restantes repeticiones quedando
///             una sola
///
void Patron::getData(QVector<int>& asd)
{
    QVector<int> devolver;

    //si no se encuentra patrón
    if(patros.size()==0)
    {
        asd.clear();
        return;
    }

    else
    {
        int pa, j = 0;
        bool finded= false;
        for (int i = 0; i < patros.size(); i++)
        {
            j = 0;
            pa = patros[i];
            while (!finded && j < devolver.size())
            {
                if (devolver[j] == pa)
                    finded = true;
                else
                    j++;
            }
            if (!finded)
                devolver.push_back(pa);
            finded = false;
        }

        seleccion_lims(devolver, 0, devolver.size());

        asd = devolver;
    }
}

///
/// \brief Patron::seleccion_lims
/// \param T            Lista que se desea ordenar
/// \param inicial      Posicion inicial de la lista
/// \param final        hasta donde se desea ordenar
/// \note               Ordena una lista
///
void Patron::seleccion_lims(QVector<int> &T, int inicial, int final)
{
    int i, j, indice_menor;
    int menor, aux;
    for (i = inicial; i < final - 1; i++)
    {
        indice_menor = i;
        menor = T[i];
        for (j = i; j < final; j++)
            if (T[j] > menor)
            {
                indice_menor = j;
                menor = T[j];
            }
        aux = T[i];
        T[i] = T[indice_menor];
        T[indice_menor] = aux;
    }
}

///
/// \brief Patron::RasterPolinomio
/// \param cant         Cantidad de bits
/// \param arraySenales Bits
/// \param error        Margen de error
/// \param longFinal    Tamano maximo de una secuencia de un patron
/// \return             3 mejores patrones
/// \note               Va buscando a partir de un tamano de 3 bits, los patrones existentes, hasta llegar a un tamano de 8
///                     bits, luego, filtra los repetidos, los que estan uno dentro de otros, y luego selecciona los mas repetidos
///
QVector<PatronEncontradoRaster> Patron::RasterPolinomio(int cant, QVector<char> arraySenales, double error, int longFinal)
{
    QVector<PatronEncontradoRaster> retornar;
    QVector<PatronEncontradoRaster> patronesEncontrados;
    QVector<PatronEncontradoRaster> patrones;
    QVector<PatronEncontradoRaster>ListTemp;
    int e=3;//patron de 3 elementos
    //a es la posicion donde estoy parado
    int a=0;
    int m=0;
    int existe=0;
    //patron que estoy analizando
    QVector<char> buscarPatron;//(bp)

    //este for me dice el tamano de la secuencia del patron
    for (e=5; e<longFinal + 1;e++)
    {
        a=0;

        //este while me controla que analice todos los bits del arreglo
        while (a < cant - e)
        {
            if(!buscarPatron.empty())
                    buscarPatron.clear();
            //m toma la proxima posicion
            m=a;
            //se llena con la primera secuencia de bits de "e" elementos
            for(int t=0; t<e; t++)
            {
                    buscarPatron.push_back(arraySenales[m]);
                    m++;
            }
            //se busca si buscarPatron esta dentro de patronesEncontrados con un margen de error ej: 010 == 011, 010 != 101
            existe = BuscarPatronEnRasterPeriodicidad2222(patronesEncontrados, buscarPatron, error);
            //se incrementa la lista de los patron con el nuevo patron encontrado, o sea
            //una nueva cadena de bits
            if (existe == -1)
            {
                PatronEncontradoRaster insertarPatron;
                //guaro la cantidad de elementos del patron
                insertarPatron.cantBit = e;
                //guardo en memo
                insertarPatron.ConstructorSecuencia(e);
                //guardo la posicion donde aparecio
                insertarPatron.frecuenciaAparicion.push_back(a);
                //guardo la secuencia
                for (int j=0;j<e;j++)
                {
                    insertarPatron.secuencia[j]=buscarPatron[j];
                }
                //insertarPatron.patronNum=BinarioDecimal(buscarPatron, e);
                //guardo el patron
                patronesEncontrados.push_back(insertarPatron);
            }
            else
            {
                //se encontro el patron con un margen de error y se agrega la posicion donde se encontro
                //patron y posiciones de aparicion
                patronesEncontrados[existe].frecuenciaAparicion.push_back(a);
            }
            a++;
        }


        if (!ListTemp.empty())
            ListTemp.clear();

        ListTemp= EncontrarPatronesEnLista(patronesEncontrados);        

        if (!ListTemp.empty())
            for (int s=0; s<ListTemp.size(); s++)
                patrones.push_back(ListTemp[s]);

        if (!patrones.empty() && e > 3)
            EliminarSubPatrones(patrones);
    }

    if (!patrones.empty())
        retornar = BuscarTresPatronesMasRepetidos(patrones);    

    return retornar;
}
