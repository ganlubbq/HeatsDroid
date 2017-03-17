#ifndef RASTER_H
#define RASTER_H
#include <math.h>
#include <QtWidgets>//para abrir ventanas de diálogo, //define malloc

bool raster(bool* &bits, int *bits_length, double *signal_dem, int signal_dem_length, double signal_dem_threshold, double baud_rate, double signal_fm);

bool raster_M(bool* &bits, int *bits_length, int *symbols, int symbols_length, int M, int code, bool diferencial,double baud_rate, double signal_fm);

#endif // RASTER_H
