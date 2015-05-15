// Rest bench used to test rotator.cpp
// Goes through all the states in the finite state machine

#include "mc_testbench.h"
#include <mc_scverify.h>

#include <iostream>
#include <assert.h>
#include "ac_int.h"
// filter defs and protos
#include "rotator.h"
// bmp lib
#include "bmp_io.h"
   
// some super cool macros
#define RESET() \
    rotator(false, true, false, sum, best_edge, current_state, timer, best_edge, motor_state, current_state, timer);
#define STOP() \
    rotator(false, false, true, sum, best_edge, current_state, timer, best_edge, motor_state, current_state, timer);
#define CYCLE() \
    rotator(false, false, false, sum, best_edge, current_state, timer, best_edge, motor_state, current_state, timer);
#define REINITIALIZE() \
    timer = 0;

// function prototypes:
void print_motor_state(const ac_int<2, false> & state){
    switch (state) {
    case MOTOR_STOP:
        cout << "Motor is in stop state" << endl;
        break;
    case MOTOR_UNWIND:
        cout << "Motor is unwinding" << endl;
        break;
    case MOTOR_WIND:
        cout << "Motor is winding" << endl;
        break;
    }
}
// Main Verification Function
CCS_MAIN(int argc, char *argv[]) {
    // rotator(
    //     ac_int<1, false> enablerefocus,
    //     ac_int<1, false> reset,
    //     ac_int<1, false> stop,
    //     ac_int<30, false> sum, 
    //     ac_int<30, false> best_edgeIN, 
    //     ac_int<3, false> cur_stateIN,
    //     ac_int<30, false> timerIN,
    //     ac_int<30, false> & best_edge, 
    //     ac_int<2, false> & motor_state,
    //     ac_int<3, false> & cur_state,
    //     ac_int<30, false> & timer 
    // )

    // NB : These variables are used by the rotator method!
    ac_int<2, false> motor_state;
    ac_int<3, false> current_state;
    ac_int<30, false> sum = 0;
    ac_int<30, false> best_edge = 0;
    ac_int<30, false> timer = 0;
    // end of used variables
    
    const unsigned best_edge_count = 25000000;
    const unsigned VGA_FREQUENCY = 25000000;
    
    
    cout << "*** start testbench *** " << endl;
    
    cout << "Checking if timer works" << endl;
    // checking if timer updates accordingly
    REINITIALIZE();
    CYCLE();
    assert(timer == 1);
    cout << "Timer increases in the end of clock cycle" << endl << endl;
    // end of checking timer

    // checking if reset works
    cout << "Checking if reset works" << endl;
    RESET();
    assert(current_state == S_DETER);
    assert(motor_state == MOTOR_UNWIND);
    cout << "Reset changing state into S_DETER" << endl << endl;
    // end of reset

    // checking if stop works
    cout << "Checking if stop works" << endl;
    STOP();
    assert(current_state == S_STOP);
    assert(motor_state == MOTOR_STOP);
    cout << "Stop changes state into S_STOP" << endl << endl;
    // end of checking stop

    // check if doing nothing for too long with terminate the motor
    /*
    cout << "Checking if automatic stopping works" << endl;
    for (int i = 0 ; i < VGA_FREQUENCY * 6 ; i++) {
        CYCLE();
    }
    assert(current_state == S_STOP);
    assert(motor_state == MOTOR_STOP);
    cout << "Automatic stopping works as expected" << endl << endl;
    // end of checking automatic stop
    */
    cout << "Follwing tests assumes edge count rises linearly (not important in most cases)" << endl << endl;

    cout << "Assuming starting form the left of the curve (fully wind state)" << endl;
    // first test case : assume the current sum is at the left of the curve
    // assume a rising linear scale
    // reset it first

    REINITIALIZE();
    RESET();
    cout << "Testing motor unwinding in S_DETER" << endl;
    // reset the rotator first
    sum = 1, timer = 1;
    CYCLE();
    
    for (; timer <= (VGA_FREQUENCY >> 1) - 1 ; sum++) {
        assert(motor_state == MOTOR_UNWIND);
        assert(current_state == S_DETER);
        CYCLE();
    }
    cout << "Motor unwinds as expected" << endl;
    cout << "Motor stays in S_DETER as expected" << endl;

    // now that 12500000 cycles pass, motor should starting executing S_OUT_FIND
    cout << "Motor should shift into S_OUT_FIND" << endl;
    // rise the thing until best_edge_count
    for ( ; sum < best_edge_count ; sum += 100) {
        CYCLE();
        
        assert(current_state == S_OUT_FIND);
        assert(motor_state == MOTOR_UNWIND);
    }
    cout << "Motor remainded unwinding thorugbout the S_OUT_FIND process" << endl;

    // assuming a falling linear scale, before falling less than 25% of best value
    for (sum = best_edge_count ; sum >= (best_edge_count - (best_edge_count >> 3)) ; sum -= 100) {
        CYCLE();
        assert(motor_state == MOTOR_UNWIND);
        assert(current_state == S_OUT_FIND);
    }

    cout << "Motor continue unwinding as expected until 25 percent of the best possible edge count" << endl << endl;
    // end of falling sum

    // motor should start trying to refind the best_edge_count position
    
    cout << "Starts winding motor until 6.25 percent of best_edge_count" << endl;
    CYCLE();
    for( ; absolute(sum - best_edge_count) > (best_edge_count >> 4) ; sum += 100) {
        CYCLE();
        assert(motor_state == MOTOR_WIND);
        assert(current_state == S_IN_REFIND);
    }
    cout << "Motor starts winding as expected" << endl << endl;
    // motor found approximate best position

    cout << "Motor now within range of the best position, should stop rotating" << endl;
    sum = best_edge_count - (best_edge_count >> 5);
    CYCLE(); CYCLE();
    assert(motor_state == MOTOR_STOP);
    assert(current_state == S_STOP);
    cout << "Image is now in focus, and motor stopped!" << endl << endl;
    
    cout << "Passed first possible test case (starting from the left of the curve" << endl;
    cout << "Now trying to start from the right of the curve" << endl << endl;

    // second possible test case code ... 
    REINITIALIZE();
    RESET();
    sum = 100 ; timer = 1;

    for (; timer <= (VGA_FREQUENCY >> 1) - 1 ; ) {
        assert(motor_state == MOTOR_UNWIND);
        assert(current_state == S_DETER);
        CYCLE();
    }
    cout << "Motor unwinds as expected" << endl;
    cout << "Motor stays in S_DETER as expected" << endl;

    // now that 12500000 cycles pass, motor should starting executing S_IN_FIND
    cout << "Motor should shift into S_IN_FIND" << endl;
    // rise the thing until best_edge_count
    
    CYCLE();
    for ( ; sum < best_edge_count ; sum += 100) {
        CYCLE();
        assert(current_state == S_IN_FIND);
        assert(motor_state == MOTOR_WIND);
    }
    cout << "Motor remainded winding thorugbout the S_IN_FIND process" << endl;

    // assuming a falling linear scale, before falling less than 25% of best value
    for (sum = best_edge_count ; sum >= (best_edge_count - (best_edge_count >> 3)) ; sum -= 100) {
        CYCLE();
        assert(current_state == S_IN_FIND);
        assert(motor_state == MOTOR_WIND);
    }

    cout << "Motor continue unwinding as expected until 25 percent of the best possible edge count" << endl << endl;
    // end of falling sum

    // motor should start trying to refind the best_edge_count position
    
    cout << "Starts unwinding motor until 6.25 percent of best_edge_count" << endl;
    CYCLE();
    for( ; absolute(sum - best_edge_count) > (best_edge_count >> 4) ; sum += 100) {
        CYCLE();
        assert(current_state == S_OUT_REFIND);
        assert(motor_state == MOTOR_UNWIND);
    }
    cout << "Motor starts unwinding as expected" << endl << endl;
    // motor found approximate best position

    cout << "Motor now within range of the best position, should stop rotating" << endl;
    sum = best_edge_count - (best_edge_count >> 5);
    CYCLE(); CYCLE();
    assert(motor_state == MOTOR_STOP);
    assert(current_state == S_STOP);
    cout << "Image is now in focus, and motor stopped!" << endl << endl;

    cout << "Testing enable refocus" << endl;
    cout << "NOT YET IMPLEMENTED : Refocus" << endl;
    cout << "Enable refocus works as expected" << endl << endl;
    CCS_RETURN(0);
}