#ifndef _EDGE_COUNTER
#define _EDGE_COUNTER

#include <ac_int.h>
#include <iostream>

// total number of pixels from screen frame/image read in testbench
#define NUM_PIXELS (800*525)

#define KERNEL_WIDTH      3
#define KERNEL_NUMEL      (KERNEL_WIDTH * KERNEL_WIDTH)
#define COLOUR_WL         10
#define PIXEL_WL          (3 * COLOUR_WL)

#define  COORD_WL          10


// void mean_vga(ac_int<PIXEL_WL*KERNEL_WIDTH,false> vin[NUM_PIXELS], ac_int<PIXEL_WL,false> vout[NUM_PIXELS]);
void mean_vga(ac_int<PIXEL_WL*KERNEL_WIDTH, false> vin[NUM_PIXELS], ac_int<PIXEL_WL, false> vout[1 + NUM_PIXELS], ac_int<30, false> & sum, ac_int<1, false> * flag);

#endif
