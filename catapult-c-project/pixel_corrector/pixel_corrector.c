#include "pixel_corrector.h"

// shift_class: page 119 HLS Blue Book

ac_int<10, false> get_value(int x){
    x = x + (x >> 2) + (x >> 3) + (x >> 4); 
    
    if(x >= 1023) x = 1023;
        
    return ((ac_int<10, false>) x);
}

#pragma hls_design top
void pixel_corrector(ac_int<PIXEL_WL, false> * vin, ac_int<PIXEL_WL, false> * vout) {
    ac_int<10, false> r,g,b;
    
    r = vin->slc<COLOR_WL>(20);
    g = vin->slc<COLOR_WL>(10);
    b = vin->slc<COLOR_WL>(0);
    
    r = get_value(r);
    g = get_value(g);
    b = get_value(b);
    
    *vout = ((((ac_int<PIXEL_WL, false>)r) << 20) | (((ac_int<PIXEL_WL, false>)g) << 10) | (ac_int<PIXEL_WL, false>)b);
}