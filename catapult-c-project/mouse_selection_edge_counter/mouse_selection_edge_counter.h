#ifndef _MOUSE_SELECTION_EDGE_COUNTER
#define _MOUSE_SELECTION_EDGE_COUNTER

#include <ac_int.h>
#include <iostream>

// total number of pixels from screen frame/image read in testbench
#define NUM_PIXELS (800*525)

#define KERNEL_WIDTH      3
#define KERNEL_NUMEL      (KERNEL_WIDTH * KERNEL_WIDTH)
#define COLOUR_WL         10
#define PIXEL_WL          (3 * COLOUR_WL)

#define  COORD_WL          10

#endif
