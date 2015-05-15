#include <ac_fixed.h>
#include "edge_counter.h"
#include <iostream>

// shift_class: page 119 HLS Blue Book
#include "shift_class.h"

const int MASK_X[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

const int MASK_Y[3][3] = {
    {1, 2, 1},
    {0, 0, 0},
    {-1, -2,-1}
};
 
ac_int<PIXEL_WL> getGrayScalePixel(const ac_int<16, true> & res, const ac_int<1, false> & flag1, const ac_int<1, false> & flag2) {
    unsigned int a = flag1;
    unsigned int b = flag2;
    a = a << 10;
    b = b << 11;
    
    return (((ac_int<PIXEL_WL, false>) a) | ((ac_int<PIXEL_WL, false>) b) | ((ac_int<PIXEL_WL, false>) a) | ((ac_int<PIXEL_WL, false>) res));
}

ac_int<PIXEL_WL * KERNEL_WIDTH, false> getPixel (shift_class<ac_int<PIXEL_WL*KERNEL_WIDTH,false>, KERNEL_WIDTH> & regs, int i, int j) {
    return ((regs[i].slc<COLOUR_WL>(2 * COLOUR_WL + j * PIXEL_WL)) + (regs[i].slc<COLOUR_WL>(COLOUR_WL + j * PIXEL_WL)) + (regs[i].slc<COLOUR_WL>(0 + j * PIXEL_WL)));
}

#pragma hls_design top
void edge_counter(ac_int<PIXEL_WL*KERNEL_WIDTH, false> vin[NUM_PIXELS], ac_int<PIXEL_WL, false> vout[NUM_PIXELS], ac_int<30, false> & sum, ac_int<1, false> * flag)
{
    // Declrea and initialize variables and registers
    ac_int<16, true> x_arr[KERNEL_WIDTH], y_arr[KERNEL_WIDTH];
    ac_int<16, true> gx, gy, res;
    static int tmp_sum = 0;
    static shift_class<ac_int<PIXEL_WL*KERNEL_WIDTH,false>, KERNEL_WIDTH> regs;
    
    // declare variables
    int i = 0;
    
    // initialize the variables
    gx = gy = res = 0;

    // Initialize sum as previous accumulated value and initialize flag as true (rised edge)
    sum = tmp_sum;
    tmp_sum = 0;
    *flag = 1;

    FRAME: for(int p = 0; p < NUM_PIXELS; p++) {
        *flag = (p <= (NUM_PIXELS / 2));

        // init gx and accumulators
        gx = 0;
        gy = 0;

        RESET: for(i = 0; i < KERNEL_WIDTH; i++) {
            x_arr[i] = 0;
            y_arr[i] = 0;
        }

        // shift input data in the filter fifo
        regs << vin[p]; // advance the pointer address by the pixel number (testbench/simulation only)
        // accumulate

        ACC1: for(i = 0; i < KERNEL_WIDTH; i++) {
            // current line
            // NB : Unroll this loop!
            ACC1_1: for (int j = 0 ; j < 3 ; j++) {
                x_arr[j] += MASK_X[i][j] * getPixel(regs, i, j);
                y_arr[j] += MASK_Y[i][j] * getPixel(regs, i, j);
            }
        }

        // add the accumualted value for all processed lines
        ACC2: for(i = 0; i < KERNEL_WIDTH; i++) {
            gx += x_arr[i];
            gy += y_arr[i];
        }
        // normalize result
        gx /= 3;
        gy /= 3;

        // absolute value
        if(gx < 0) { gx = -gx; }
        if(gy < 0) { gy = -gy; }

        // Compute absolute sum
        res = gx + gy;

        if (res >= 1023) { res = 1023; }

        // group the RGB components into a single signal
        vout[p] = getGrayScalePixel(res, p % 2, *flag);

        // add the original value rather than the capped-value
        tmp_sum += (gx + gy);
    }

    sum = tmp_sum;
    *flag = 1; // Trigger the flag on completion
}