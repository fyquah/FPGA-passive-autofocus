////////////////////////////////////////////////////////////////////////////////
// Catapult Project options
// Constraint Editor:
//  Frequency: 50 MHz
//  Top design: vga_blur
//  clk>reset sync: disable; reset async: enable; enable: enable
// Architecture Constraints:
//  interface>vin: wordlength = 150, streaming = 150 (90 if using a smaller kernel width maybe?)
//  interface>vout: wordlength = 30, streaming = 30
//  core>main: pipeline + distributed + merged
//  core>main>frame: merged
//  core>main>frame>ACC1, ACC1_1, ACC2 : unroll + merged
//  core>main>frame>shift, mac1, mac2: unroll + merged
////////////////////////////////////////////////////////////////////////////////

#include <ac_fixed.h>
#include "rotator.h"

// shift_class: page 119 HLS Blue Book
#include "shift_class.h"

#define RESET_TO_S_DETER() \
cur_stateIN = S_DETER; \
best_edgeIN = sum; \
timerIN = 0;

#define RESET_TO_S_STOP() \
cur_stateIN = S_STOP; \
best_edgeIN = sum; \
timerIN = 0;


inline int max(int a, int b) {
    return a > b ? a : b;
}

unsigned absolute(int a) {
    return a < 0 ? -a : a;
}

#pragma hls_design top
void rotator(
    ac_int<1, false> enablerefocus,
    ac_int<1, false> slow,
    ac_int<1, false> start,
    ac_int<1, false> stop,
    ac_int<30, false> sum, 
    ac_int<30, false> & best_edge, 
    ac_int<2, false> & motor_state,
    ac_int<3, false> & cur_state,
    ac_int<30, false> & timer 
) {
    //0 = determining direction
    //1 = screwing out to find best
    //2 = screwing in to find best
    //3 = screwing in to re-find best
    //4 = screwing out to re-find best
    //5 = stopped
    
    static ac_int<30, false> best_edgeIN = 0;
    static ac_int<3, false> cur_stateIN = S_STOP;
    static ac_int<30, false> timerIN = 0;
    
    //pathways are 0,1,3,5 or 0,2,4,5 depending on whether it's behind or not behind.
    //if
    timerIN = timerIN+1;
    
    if (timerIN > (25000000 * 5  * (slow ? SLOW_MO_MULTIPIER : 1)) && cur_stateIN != S_STOP && cur_stateIN != S_EVALUATE_DELTA) {
        RESET_TO_S_STOP();
        
    } else if (start && cur_stateIN == S_STOP) {
        RESET_TO_S_DETER();
    } else if (stop && cur_stateIN != S_STOP) {
        RESET_TO_S_STOP();
    } else {
        switch (cur_stateIN){
        case S_DETER:
            motor_state = MOTOR_UNWIND;
            if (absolute(sum - best_edgeIN) > (max(sum, best_edgeIN) >> 3) || timerIN > 12500000 * (slow ? SLOW_MO_MULTIPIER : 1)) {
                if (sum<best_edgeIN) {
                    //turn around!
                    best_edgeIN = 0;
                    cur_stateIN = S_IN_FIND;
                } else {
                    best_edgeIN = 0;
                    cur_stateIN = S_OUT_FIND;
                }
            }
        break;
        case S_OUT_FIND:
            motor_state = MOTOR_UNWIND;
            best_edgeIN = max(best_edgeIN, sum);
            if (best_edgeIN >= MINIMUM_FOCUSED_EDGE_COUNT && sum < (best_edgeIN - (best_edgeIN >> 3)) ) {
                cur_stateIN = S_IN_REFIND;
            }
        break;
        case S_IN_FIND:
            motor_state = MOTOR_WIND;
            best_edgeIN = max(best_edgeIN, sum);
            if ((best_edgeIN >= MINIMUM_FOCUSED_EDGE_COUNT) && (sum < (best_edgeIN - (best_edgeIN >> 3)))) {
                cur_stateIN = S_OUT_REFIND;
            }
        break;
        case S_IN_REFIND:
            motor_state = MOTOR_WIND;
            if (absolute(sum - best_edgeIN) < (best_edgeIN >> 4) ) {
                // if the difference between current sum and best_edge is within 6.25% of the actually amount
                // then stop the motor (should be a good enough state)
                RESET_TO_S_STOP();
            } else if (sum < (best_edgeIN - (best_edgeIN >> 2))) {
                // if the difference falls out of more than 12.5% from the identified best edge, the best edge is 
                // probably at the other side
                RESET_TO_S_DETER();
            }
            break;
        case S_OUT_REFIND:
            motor_state = MOTOR_UNWIND;
            if (absolute(sum - best_edgeIN) < (best_edgeIN >> 4)) {
                // if the difference between current sum and best_edge is within 6.25% of the actually amount
                // then stop the motor (should be a good enough state)
                RESET_TO_S_STOP();
            } else if (sum < (best_edgeIN - (best_edgeIN >> 2))) { 
                RESET_TO_S_DETER();
            }
            break;
        case S_STOP:
            motor_state = MOTOR_STOP;
            
            if (enablerefocus && 
                (timerIN > REFOCUS_WAIT_TIME * (slow ? SLOW_MO_MULTIPIER : 1)) &&
                ((absolute(sum-best_edgeIN) > (best_edgeIN >> 2)))) {
                // start evluating the delta
                timerIN = 0;
                cur_stateIN = S_EVALUATE_DELTA;
                best_edgeIN = sum; 
            }
            
            break;
        case S_EVALUATE_DELTA:
            static int left_sum = 0;
            static int right_sum = 0;
            
            if (timerIN < LEFT_TIME * (slow ? SLOW_MO_MULTIPIER : 1)) {
                motor_state = MOTOR_WIND;
                left_sum = sum;
                
            } else if (timerIN < 3 * LEFT_TIME * (slow ? SLOW_MO_MULTIPIER : 1)) {
                motor_state = MOTOR_UNWIND;
                right_sum = sum;
                
            } else if (timerIN < 4 * LEFT_TIME * (slow ? SLOW_MO_MULTIPIER : 1)) {
                motor_state = MOTOR_WIND;
                // just keep going back 
                
            } else {
                if((left_sum < best_edgeIN) && (right_sum < best_edgeIN)) {
                    RESET_TO_S_STOP(); // there is no slope, stop;
                } else {
                    RESET_TO_S_DETER(); // there is a slope, recalculate!
                }
            }
            break;
            
        default:
            RESET_TO_S_STOP();
            break;
        }
    }
        
    best_edge = best_edgeIN;
    cur_state = cur_stateIN;
    timer = timerIN;
 }

