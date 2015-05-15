#include "interface_block.h"

#pragma hls_design top
void interface_block(
	ac_int<PIXEL_WL,false> vin, 
	ac_int<COOR_WL + COOR_WL, false> vga_xy,
	ac_int<COOR_WL + COOR_WL, false> mouse_xy,
	ac_int<3, false> rotator_state,
    ac_int<1, false> is_refocus_mode,
    ac_int<1, false> is_macro_mode,
	ac_int<PIXEL_WL,false> * vout
){
    ac_int<COOR_WL, false> vga_x = vga_xy.slc<COOR_WL>(COOR_WL);
    ac_int<COOR_WL, false> vga_y = vga_xy.slc<COOR_WL>(0);
    ac_int<COOR_WL, false> mouse_x = mouse_xy.slc<COOR_WL>(COOR_WL);
    ac_int<COOR_WL, false> mouse_y = mouse_xy.slc<COOR_WL>(0);
    
    
    if (is_macro_mode && mouse::is_target_pixel(vga_x, vga_y, mouse_x, mouse_y)) {
        *vout = ((1023 << (2 * COLOR_WL)) | (1023 << COLOR_WL) | 1023 );
        
    } else if (statusBar::is_relevant_coordinate(vga_x, vga_y)) {
        *vout = statusBar::render(vga_x, vga_y, mouse_x, mouse_y, is_refocus_mode, is_macro_mode); 
              
    } else {
        *vout = vin;
    }
}