#ifndef _PIXEL_CORRECTOR
#define _PIXEL_CORRECTOR

#include <ac_int.h>
#include <iostream>
#include <ac_fixed.h>
#include "shift_class.h"

// total number of pixels from screen frame/image read in testbench
#define NUM_PIXELS (800*525)

#define KERNEL_WIDTH      3
#define KERNEL_NUMEL      (KERNEL_WIDTH * KERNEL_WIDTH)
#define COLOR_WL         10
#define PIXEL_WL          (3 * COLOR_WL)

#define  COORD_WL          10

void pixel_corrector(ac_int<PIXEL_WL, false> * vin, ac_int<PIXEL_WL, false> * vout);
#endif
