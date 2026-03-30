// Maidenhead / QTH Locator - header-only implementatie
// Gebaseerd op sp6q/Maidenhead library, static bug gefixt
#ifndef MAIDENHEAD_H
#define MAIDENHEAD_H

#include <math.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static char letterize(int x) {
    return (char) x + 65;
}

static char* get_mh(double lat, double lon, int size) {
    static char locator[11];
    double LON_F[]={20,2.0,0.083333,0.008333,0.0003472083333333333};
    double LAT_F[]={10,1.0,0.0416665,0.004166,0.0001735833333333333};
    int i;
    lon += 180;
    lat += 90;
    if (size <= 0 || size > 10) size = 6;
    size/=2; size*=2;
    for (i = 0; i < size/2; i++){
        if (i % 2 == 1) {
            locator[i*2] = (char) (lon/LON_F[i] + '0');
            locator[i*2+1] = (char) (lat/LAT_F[i] + '0');
        } else {
            locator[i*2] = letterize((int) (lon/LON_F[i]));
            locator[i*2+1] = letterize((int) (lat/LAT_F[i]));
        }
        lon = fmod(lon, LON_F[i]);
        lat = fmod(lat, LAT_F[i]);
    }
    locator[i*2]=0;
    return locator;
}

#ifdef __cplusplus
}
#endif

#endif
