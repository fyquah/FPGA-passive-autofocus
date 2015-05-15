#ifndef _MOUSE_H
#define _MOUSE_H

#include <ac_int.h>
#include "constants.h"

namespace mouse {
	const unsigned CURSOR_SIZE = 99;
	const unsigned BORDER_WIDTH = 3;
	const unsigned PLUS_RADIUS = 20;
	const unsigned PLUS_WIDTH = 1;
	
	bool is_in_cursor (ac_int<COOR_WL, false> vga_x, ac_int<COOR_WL, false> vga_y, ac_int<COOR_WL, false> mouse_x, ac_int<COOR_WL, false> mouse_y);
	bool is_plus_pixel (ac_int<COOR_WL, false> vga_x, ac_int<COOR_WL, false> vga_y, ac_int<COOR_WL, false> mouse_x, ac_int<COOR_WL, false> mouse_y);
	bool is_border_pixel(ac_int<COOR_WL, false> vga_x, ac_int<COOR_WL, false> vga_y, ac_int<COOR_WL, false> mouse_x, ac_int<COOR_WL, false> mouse_y);
	bool is_target_pixel (ac_int<COOR_WL, false> vga_x, ac_int<COOR_WL, false> vga_y, ac_int<COOR_WL, false> mouse_x, ac_int<COOR_WL, false> mouse_y);
}

#endif