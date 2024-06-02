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

#include <math.h>
#include "pitch_shift.h"
//#include "project.h"

#define ARM_MATH_LOOPUNROLL

#include <arm_math.h>

float detect_period(int16_t data[], int length) {
	// source: https://github.com/akellyirl/AutoCorr_Freq_detect
	int64_t sum = 0;
	int64_t sum_old = 0;
	int64_t sum_very_old = 0;
	int64_t thresh = 0;
	int pd_state = 0;
	
	pd_state = 0;
	
	// skip the first few values of autocorr
	// this removes false positives & speeds up the pitch detection
	for (int i = 48; i < length; i++) {
		sum_very_old = sum_old;
		sum_old = sum;
		sum = 0;
		
		// inefficient method
//		for (int k=0; k < length-i; k++) {
//			sum += (data[k])*(data[(k+i)]);
//		}
		
		// a lot faster
		// q15_t is just int16_t in disguise
		arm_dot_prod_q15(&data[0], &data[i], length-i, &sum);
		
		
		// Peak Detect State Machine
		if (pd_state == 2 && (sum-sum_old) <=0) {
			pd_state = 3;
			
			// quadratic interpolation
			// https://ccrma.stanford.edu/~jos/sasp/Quadratic_Interpolation_Spectral_Peaks.html
			float num = (float) (sum_very_old - sum);
			float den = (float) ((sum_very_old - (sum_old >> 1) + sum) >> 1);
			float period_shift = num/den;
			return (float) (i-1) + period_shift;
			break;
		}
		if (pd_state == 1 && (sum > thresh) && (sum-sum_old) > 0) {
			pd_state = 2;
		}
		if (!i) {
			thresh = sum >> 1;
			pd_state = 1;
		}
	}
	
	//no period found
	return NAN;
}

int approx_semitones_f(float freq_hz) {
	// a pitch s semitones away from a4 has frequency 440*2^(s/12) Hz
	float r_in = freq_hz/440;
	float s_in = 12*log2(r_in);
	
	return round(s_in);
}

int approx_semitones_p(float period) {
	// a pitch s semitones away from a4 has frequency 440*2^(s/12) Hz
	float r_in = PERIOD_A4/period;
	float s_in = 12*log2(r_in);
	
	return round(s_in);
}

float freq_hz(int semitones) {
	float r_rounded = exp2((float) semitones/12);
	return 440*r_rounded;
}

float period(int semitones) {
	float r_rounded = exp2((float) semitones/12);
	return PERIOD_A4/r_rounded;
}


/* [] END OF FILE */
