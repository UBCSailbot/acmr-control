/* 
 * File:   PID.cpp
 * Author: areksredzki
 * 
 * Created on January 24, 2014, 3:18 PM
 */

#include "PID.h"


/*
 * Core PID Methods
 *
 * Important: dt is required for I & D terms
 */

// init or zero P(ID)
PID::PID(float p_gain, float i_gain, float d_gain, int windup_guard, int correction_guard)
: P_GAIN(p_gain), I_GAIN(i_gain), D_GAIN(d_gain), WINDUP_GUARD(windup_guard), CORRECTION_GUARD(correction_guard)
{
    // Set prev and integrated error to zero
    prev_error_ = 0;
    int_error_ = 0;

    // S no change by default
    output_ = 0;
}

// Perform PID calculations
void PID::Compute(double current_error, double dt) {
    double p_term, i_term, d_term, diff, s_terms; //summed terms for range checking

    // integration with windup guarding
    int_error_ += (current_error * dt);
    if (int_error_ < -(WINDUP_GUARD))
        int_error_ = -(WINDUP_GUARD);
    else if (int_error_ > WINDUP_GUARD)
        int_error_ = WINDUP_GUARD;

    // differentiation
    diff = ((current_error - prev_error_) / dt);

    // scaling
    p_term = (P_GAIN * current_error);
    i_term = (I_GAIN * int_error_);
    d_term = (D_GAIN * diff);


    // summation of terms
    s_terms = p_term + i_term + d_term;

    //output range checking
    if (s_terms < -(CORRECTION_GUARD))
        s_terms = -(CORRECTION_GUARD);
    else if (s_terms > CORRECTION_GUARD)
        s_terms = CORRECTION_GUARD;

    output_ = s_terms;

    // save current error as previous error for next iteration
    prev_error_ = current_error;
}
