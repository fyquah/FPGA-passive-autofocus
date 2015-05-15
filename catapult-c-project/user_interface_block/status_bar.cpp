#include "status_bar.h"

namespace statusBar {

	bool is_refocus_mode_coordinate(int x, int y) {
		// nb : x and y are relative to status bar, not vga!!
		return (
			BORDER_WIDTH <= x && x < BORDER_WIDTH + ICON_SIZE &&
			BORDER_WIDTH <= y && y < BORDER_WIDTH + ICON_SIZE
		);
	}

	bool is_macro_mode_coordinate(int x, int y) {
		// nb : x and y are relative to status bar, not vga!!
		return (
			2 * BORDER_WIDTH + ICON_SIZE <= x && x < 2 * BORDER_WIDTH + 2 * ICON_SIZE && 
			BORDER_WIDTH <= y && y < BORDER_WIDTH + ICON_SIZE  
		);
	}

	bool is_relevant_coordinate(ac_int<COOR_WL, false> vga_x, ac_int<COOR_WL, false> vga_y) {
		int x = vga_x - TOP_LEFT_X;
		int y = vga_y - TOP_LEFT_Y;

        // check if it is in the VGA Status bar
        // then check if it is in the 
		return (
			(is_refocus_mode_coordinate(x, y) || is_macro_mode_coordinate(x, y))
		);
	}
	
	ac_int<PIXEL_WL, false> render_icon(int x, int y, const unsigned char * icon) {
	    // array sequence is [ R G B R G B ... ] 
	    return (
	    (((ac_int<PIXEL_WL, false>) icon[y * ICON_SIZE * 3 + x * 3 + 0]) << (2 * COLOR_WL + 2)) |
	    (((ac_int<PIXEL_WL, false>) icon[y * ICON_SIZE * 3 + x * 3 + 1]) << (1 * COLOR_WL + 2)) | 
	    (((ac_int<PIXEL_WL, false>) icon[y * ICON_SIZE * 3 + x * 3 + 2]) << (0 * COLOR_WL + 2)));
	}
	
	int width() {
		return BORDER_WIDTH * 3 + ICON_SIZE * 2;
	}

	int height() {
		return BORDER_WIDTH * 2 + ICON_SIZE;
	}

    ac_int<PIXEL_WL, false> render(ac_int<COOR_WL, false> vga_x, ac_int<COOR_WL, false> vga_y, 
        ac_int<COOR_WL, false> mouse_x, ac_int<COOR_WL, false> mouse_y, 
        ac_int<1, false> is_refocus_mode, ac_int<1, false> is_macro_mode) {
            
		int relative_x = vga_x - TOP_LEFT_X;
		int relative_y = vga_y - TOP_LEFT_Y;
		
		if (is_refocus_mode_coordinate(relative_x, relative_y)) {
		    // substract top left pixel of the icon's coordinate
            int x = relative_x - BORDER_WIDTH; 
            int y = relative_y - BORDER_WIDTH;

			if (is_refocus_mode) {
			    return render_icon(x, y, refocus::icon);
			} else {
				return render_icon(x, y, manual::icon);
			}

		} else if (is_macro_mode_coordinate(relative_x, relative_y)) {
		    // substract top left pixel of the icon's coordinate
		    int x = relative_x - (2 * BORDER_WIDTH + ICON_SIZE);
		    int y = relative_y - BORDER_WIDTH;
                
            if (is_macro_mode) {
                return render_icon(x, y, selectedFrame::icon);
            } else {
                return render_icon(x, y, fullFrame::icon);
            }
		}
	}
}
