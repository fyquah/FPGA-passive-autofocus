#include "mouse.h"

namespace mouse {
	
	bool is_in_cursor (ac_int<COOR_WL, false> vga_x, ac_int<COOR_WL, false> vga_y, ac_int<COOR_WL, false> mouse_x, ac_int<COOR_WL, false> mouse_y) {
		int min_x = mouse_x - CURSOR_SIZE;
		int min_y = mouse_y - CURSOR_SIZE;
		int max_x = mouse_x + CURSOR_SIZE;
		int max_y = mouse_y + CURSOR_SIZE;
		
		return (
			vga_x >= min_x && vga_x <= min_x && 
			vga_y >= min_y && vga_y <= max_y
		);
	}
	
	bool is_plus_pixel (ac_int<COOR_WL, false> vga_x, ac_int<COOR_WL, false> vga_y, ac_int<COOR_WL, false> mouse_x, ac_int<COOR_WL, false> mouse_y) {
		// check if it is in vertical line
		// OR if it is in the horizontal line
		return (
			(mouse_y - PLUS_RADIUS <= vga_y && vga_y <= mouse_y + PLUS_RADIUS && 
				mouse_x - PLUS_WIDTH <= vga_x && vga_x <= mouse_x + PLUS_WIDTH) ||
			(mouse_x - PLUS_RADIUS <= vga_x && vga_x <= mouse_x + PLUS_RADIUS && 
				mouse_y - PLUS_WIDTH <= vga_y && vga_y <= mouse_y + PLUS_WIDTH)
		);
	}
	
	bool is_border_pixel(ac_int<COOR_WL, false> vga_x, ac_int<COOR_WL, false> vga_y, ac_int<COOR_WL, false> mouse_x, ac_int<COOR_WL, false> mouse_y) {
		// (y1 <= y <= y4 AND (x1 <= x <= x2 OR x3 <= x <= x4)) OR
		// (x1 <= x <= x4 AND (y1 <= y <= y2 OR y3 <= y <= y4))
		 
		int x1 = mouse_x - CURSOR_SIZE;
		int x2 = mouse_x - CURSOR_SIZE + BORDER_WIDTH;
		int x3 = mouse_x + CURSOR_SIZE - BORDER_WIDTH;
		int x4 = mouse_x + CURSOR_SIZE;
		
		int y1 = mouse_y - CURSOR_SIZE;
		int y2 = mouse_y - CURSOR_SIZE + BORDER_WIDTH;
		int y3 = mouse_y + CURSOR_SIZE - BORDER_WIDTH;
		int y4 = mouse_y + CURSOR_SIZE;
		
		return (
			(y1 <= vga_y && vga_y <= y4 && (x1 <= vga_x && vga_x <= x2 || x3 <= vga_x && vga_x <= x4)) ||
			(x1 <= vga_x && vga_x <= x4 && (y1 <= vga_y && vga_y <= y2 || y3 <= vga_y && vga_y <= y4))
		);
	}
	
	bool is_target_pixel (ac_int<COOR_WL, false> vga_x, ac_int<COOR_WL, false> vga_y, ac_int<COOR_WL, false> mouse_x, ac_int<COOR_WL, false> mouse_y) {
		// check if it is the border
		return (
			is_border_pixel(vga_x, vga_y, mouse_x, mouse_y) ||
			is_plus_pixel(vga_x, vga_y, mouse_x, mouse_y)
		);
	}
}