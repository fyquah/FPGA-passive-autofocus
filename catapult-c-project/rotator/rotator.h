////////////////////////////////////////////////////////////////////////////////
//  _____                           _       _    _____      _ _
// |_   _|                         (_)     | |  / ____|    | | |
//   | |  _ __ ___  _ __   ___ _ __ _  __ _| | | |     ___ | | | ___  __ _  ___
//   | | | '_ ` _ \| '_ \ / _ \ '__| |/ _` | | | |    / _ \| | |/ _ \/ _` |/ _ \
//  _| |_| | | | | | |_) |  __/ |  | | (_| | | | |___| (_) | | |  __/ (_| |  __/
// |_____|_| |_| |_| .__/ \___|_|  |_|\__,_|_|  \_____\___/|_|_|\___|\__, |\___|
//                 | |                                                __/ |
//                 |_|                                               |___/
//  _                     _
// | |                   | |
// | |     ___  _ __   __| | ___  _ __
// | |    / _ \| '_ \ / _` |/ _ \| '_ \
// | |___| (_) | | | | (_| | (_) | | | |
// |______\___/|_| |_|\__,_|\___/|_| |_|
//
////////////////////////////////////////////////////////////////////////////////
//  File:           rotator.h
//  Description:    vga blur - real-time processing
//  By:             rad09
////////////////////////////////////////////////////////////////////////////////
// this hardware block receives the VGA stream and then produces a blured output
////////////////////////////////////////////////////////////////////////////////


#ifndef _ROTATOR
#define _ROTATOR

#include <ac_int.h>
#include <iostream>

// total number of pixels from screen frame/image read in testbench
#define NUM_PIXELS (640*480)

#define KERNEL_WIDTH      3
#define KERNEL_NUMEL      (KERNEL_WIDTH * KERNEL_WIDTH)
#define COLOUR_WL         10
#define PIXEL_WL          (3 * COLOUR_WL)

#define  COORD_WL          10

// NB : FOR MOTOR STATE:
// LSB => UNWIND
// MSB => WIND
// (and (not LSB) (not MSB)) => HI
// (and LSB MSB) => UNDEFINED
const unsigned int MOTOR_STOP = 0;
const unsigned int MOTOR_UNWIND = 1;
const unsigned int MOTOR_WIND = 2;
const unsigned int MINIMUM_FOCUSED_EDGE_COUNT = 10000;

const unsigned int S_DETER = 0;
const unsigned int S_OUT_FIND = 1;
const unsigned int S_IN_FIND = 2;
const unsigned int S_IN_REFIND = 3;
const unsigned int S_OUT_REFIND = 4;
const unsigned int S_STOP = 5;
const unsigned int S_EVALUATE_DELTA = 6;

// some constants for gradient readjustment
const unsigned LEFT_TIME = 10000000;
const unsigned REFOCUS_WAIT_TIME = 1 * 25000000;
const unsigned SLOW_MO_MULTIPIER = 5;

void rotator(
    ac_int<1, false> enablerefocus,
    ac_int<1, false> start,
    ac_int<1, false> stop,
    ac_int<30, false> sum, 
    ac_int<30, false> best_edgeIN, 
    ac_int<3, false> cur_stateIN,
    ac_int<30, false> timerIN,
    ac_int<30, false> & best_edge, 
    ac_int<2, false> & motor_state,
    ac_int<3, false> & cur_state,
    ac_int<30, false> & timer 
);

unsigned absolute(int a);
int max(int a, int b);
#endif
