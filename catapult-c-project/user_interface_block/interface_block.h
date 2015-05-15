#ifndef _INTERFACE_BLOCK
#define _INTERFACE_BLOCK

#include <ac_int.h>
#include "shift_class.h"
#include "status_bar.h"
#include "mouse.h"
#include "constants.h"

// total number of pixels from screen frame/image read in testbench
void interface_block(
    ac_int<PIXEL_WL,false> vin, 
    ac_int<COOR_WL + COOR_WL, false> vga_xy,
    ac_int<COOR_WL + COOR_WL, false> mouse_xy,
    ac_int<3, false> rotator_state,
    ac_int<1, false> is_refocus_mode,
    ac_int<1, false> is_macro_mode,
    ac_int<PIXEL_WL,false> * vout
);

#endif