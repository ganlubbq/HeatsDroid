#-------------------------------------------------
#
# Project created by QtCreator 2015-02-26T12:45:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HEATS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    nucleo.cpp \
    clasificador.cpp \
    fft.cpp \
    graph_1D.cpp \
    psk.cpp \
    fsk.cpp \
    raster.cpp \
    graph_raster.cpp \
    patronencontradoraster.cpp \
    secuenciacantidad.cpp \
    patron.cpp \
    graph_spect.cpp \
    detection.cpp \
    filters.cpp

HEADERS  += mainwindow.h \
    nucleo.h \
    clasificador.h \
    fft.h \
    graph_1D.h \
    psk.h \
    fsk.h \
    raster.h \
    graph_raster.h \
    patronencontradoraster.h \
    secuenciacantidad.h \
    patron.h \
    graph_spect.h \
    detection.h \
    filters.h

FORMS    += mainwindow.ui

#adición de bibliotecas para filtros de programación lineal
#LIBS += -L"C:\Program Files\glpk-4.55\w32" -lglpk_4_55
#INCLUDEPATH += "C:\Program Files\glpk-4.55\src"

#adición de bibliotecas para filtros de eigenvalue
#LIBS += -L$$PWD/libraries/ -leigeniir
#LIBS += -L$$PWD/libraries/ -lLAPACKE
INCLUDEPATH += $$PWD/libraries
DEPENDPATH += $$PWD/libraries
