#include <ac_fixed.h>
#include "mouse_buffer.h"

int absolute(int a) {
    return a < 0 ? -a : a;
}

ac_int<10, false> calculate_coordinate(
	int prev, int cur, int on_screen, int low_end, int high_end
) {
	int delta = (cur - prev);
	if (absolute(delta) > THRESHOLD_DELTA_VALUE) {
	   delta = delta < 0 ? 1 : -1;
	}
	int tmp = on_screen + delta;
	if(tmp < low_end) {
		return low_end;
	} else if(tmp > high_end) {
		return high_end;
	} else {
		return tmp;
	}
}

#pragma hls_design top
void mouse_buffer(
	ac_int<10, false> input_x, ac_int<10, false> input_y,
	ac_int<10, false> & output_x, ac_int<10, false> & output_y
) {
	static ac_int<10, false> previous_x = 0;
	static ac_int<10, false> previous_y = 0;
	
	static ac_int<12, true> on_screen_x = 0;
	static ac_int<12, true> on_screen_y = 0;

	// update the values of previous_x and previous_y
	on_screen_x = calculate_coordinate(previous_x, input_x, on_screen_x, MOUSE_X_MIN, MOUSE_X_MAX);
	on_screen_y = calculate_coordinate(previous_y, input_y, on_screen_y, MOUSE_Y_MIN, MOUSE_Y_MAX); 
	
	// wire the output 
	output_x = on_screen_x;
	output_y = on_screen_y;
	 
	// save the the previous values
	previous_x = input_x;
	previous_y = input_y;
}