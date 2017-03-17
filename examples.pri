################################################################
# Qwt Widget Library
# Copyright (C) 1997   Josef Wilgen
# Copyright (C) 2002   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
###################################################################

QWT_ROOT = /usr/local/qwt-6.1.0
include( qwtconfig.pri )
include( qwtbuild.pri )
include( qwtfunctions.pri )

TEMPLATE     = app

INCLUDEPATH += $${QWT_ROOT}/src
DEPENDPATH  += $${QWT_ROOT}/src

!debug_and_release {

    DESTDIR      = /home/jorge/Desktop/UDP_QT_2.3_raspberry/UDP_QT_raspberry
}
else {
    CONFIG(debug, debug|release) {

        DESTDIR      = /home/jorge/Desktop/UDP_QT_2.3_raspberry/build-UDP_QT_raspberry-Desktop_Qt_5_2_1_GCC_64bit-Debug
    }
    else {

        DESTDIR      = /home/jorge/Desktop/UDP_QT_2.3_raspberry/build-UDP_QT_raspberry-Desktop_Qt_5_2_1_GCC_64bit-Release
    }
}

QMAKE_RPATHDIR *= $${QWT_ROOT}/lib

contains(QWT_CONFIG, QwtFramework) {

    LIBS      += -F$${QWT_ROOT}/lib
}
else {

    LIBS      += -L$${QWT_ROOT}/lib
}

qwtAddLibrary(qwt)

greaterThan(QT_MAJOR_VERSION, 4) {

    QT += printsupport
    QT += concurrent
}

contains(QWT_CONFIG, QwtOpenGL ) {

    QT += opengl
}
else {

    DEFINES += QWT_NO_OPENGL
}

contains(QWT_CONFIG, QwtSvg) {

    QT += svg
}
else {

    DEFINES += QWT_NO_SVG
}


win32 {
    contains(QWT_CONFIG, QwtDll) {
        DEFINES    += QT_DLL QWT_DLL
    }
}
