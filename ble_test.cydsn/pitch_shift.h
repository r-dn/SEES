/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

// functions related to pitch detection & correction

#ifndef PITCH_SHIFT_H
#define PITCH_SHIFT_H

#include "project.h"

//period of a4 (440 Hz) at sample rate 48 kHz
#define PERIOD_A4	(109.090909090909090909090909)

// detects the period of a frame using autocorrelation
float detect_period(int16_t data[], int length);

// get the closest actual pitch to the input frequency
// the result is in semitones (+/-) away from a4
int approx_semitones_f(float freq_hz);

// same, but for a period
int approx_semitones_p(float period);

// get the actual period of a pitch
float period(int semitones);

// same, but for frequency
float freq_hz(int semitones);

#endif
/* [] END OF FILE */
