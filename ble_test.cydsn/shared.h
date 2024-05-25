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
	
#define DEBUG_PRINT
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
#define BLOCK_SIZE		(BITRATE/8*FRAME_US/1000000)
#define PCM_SBYTES		(PCM_SBITS/8)
	
// #frames in the shared buffer
#define S_FRAMES		(4)
	
#define SEMA_NUM	16u

bool ble_connected __SHARED(".ble_connected");

uint8_t alignas(int32_t) shared_buffer[FRAME_SAMPLES*PCM_SBYTES*S_FRAMES] __SHARED(".shared_buffer");

int current_frame_written __SHARED(".current_frame_written");
int current_frame_reading __SHARED(".current_frame_reading");


#endif /* SHARED_H */
