#ifndef _MOUSE_BUFFER
#define _MOUSE_BUFFER

#include <ac_int.h>
#include <iostream>

// consants
const unsigned MOUSE_X_MIN = 243;
const unsigned MOUSE_X_MAX = 683;
const unsigned MOUSE_Y_MIN = 135;
const unsigned MOUSE_Y_MAX = 415;
const unsigned THRESHOLD_DELTA_VALUE = 100;

// total number of pixels from screen frame/image read in testbench

ac_int<10, false> calculate_coordinate(
	int prev, int cur, int on_screen
);

void mouse_buffer(
	ac_int<10, false> input_x, ac_int<10, false> input_y,
	ac_int<10, false> & output_x, ac_int<10, false> & output_y
);

#endif
