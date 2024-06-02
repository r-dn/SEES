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

// this header contains macros & variables that are used both by cm0p and cm4

#ifndef SHARED_H
#define SHARED_H

#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdalign.h>

#include "project.h"

// assign the variable to the shared region of memory (see linker scripts)
// to make sure the variable order for both executables is the same, assign each variable to a different section
// these sections need to be declared in the linker scripts
// tedious but works
#define __SHARED(x) 	__attribute__ ((section(x)))
	
// the UART printing can be disabled (hopefully better performance?)
//#define DEBUG_PRINT
#ifdef DEBUG_PRINT
#define printf(x)				Cy_SCB_UART_PutString(UART_DEBUG_HW, x)
#else
#define printf(x)
#endif

// audio information
#define BITRATE			(128000)
#define FRAME_US		(10000)
#define SRATE_HZ 		(48000)
#define NCHANNELS		(1)
#define PCM_SBITS		(16)
#define FRAME_SAMPLES	(SRATE_HZ*FRAME_US/1000000)
#define BLOCK_SIZE_R	(BITRATE/8*FRAME_US/1000000)

// extra padding for block_size
#define BLOCK_SIZE		(BLOCK_SIZE_R + 10)
#define PCM_SBYTES		(PCM_SBITS/8)
	
// #blocks in the shared buffer
#define S_BLOCKS		(10)

// written by cm0p, read by cm4
volatile bool ble_connected __SHARED(".ble_connected");

// written by cm4, read by cm0p
// this contains the encoded frames
// after each block, there is some free space to put additional msg information
// this avoids another memcpy in cm0p
volatile uint8_t alignas(int32_t) shared_buffer[BLOCK_SIZE*S_BLOCKS] __SHARED(".shared_buffer");

// the shared buffer pointers
// cm4's write pointer
volatile int current_frame_written __SHARED(".current_frame_written");
// cm0p's read pointer
volatile int current_frame_reading __SHARED(".current_frame_reading");

// the detected frequency information
// written by cm4, read by cm0p
volatile float freq[S_BLOCKS] __SHARED(".incoming_frame");

volatile int frame_received __SHARED(".frame_received");

#endif /* SHARED_H */
