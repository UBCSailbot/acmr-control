/* 
 * File:   PID.h
 * Author: joshandrews
 *
 * Created on January 24, 2014, 3:18 PM
 */

#ifndef PID_HPP
#define    PID_HPP

class PID {

public:

    PID(float p_gain, float i_gain, float d_gain, int windup_guard, int correction_guard);

    double Output() { return output_; }

    void Compute(double current_error, double dt);

private:
    int WINDUP_GUARD;          // cap on the maximum value that the integrated error can be
    int CORRECTION_GUARD;      // cap on the maximum/min value that can be outputted
    float P_GAIN;              // kp
    float I_GAIN;              // ki
    float D_GAIN;              // kd
    double prev_error_;              // previous error
    double int_error_;               // integrated error
    double output_;                  // pid output

};

#endif	/* PID_HPP */

