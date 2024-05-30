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

#include "shared.h"

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <lc3.h>

#define NOTIFICATION_PKT_SIZE	BLOCK_SIZE+1

typedef LC3_ENCODER_MEM_T(FRAME_US, SRATE_HZ) lc3_encoder_mem_t;

lc3_encoder_t enc;
int16_t dummy[FRAME_SAMPLES];
lc3_encoder_mem_t enc_mem;

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
	
	memset(shared_buffer, 0, sizeof(shared_buffer));
	
	// setup encoding
	enc = lc3_setup_encoder(FRAME_US, SRATE_HZ, SRATE_HZ, &enc_mem);
		
	if (!enc) {
		printf("Encoder initialization failed");
	}
	
	
	current_frame_written = 0;
	
	srand(time(NULL)); 
	
	// fill with random stuff for testing
		for (int16_t i = 0; i < FRAME_SAMPLES; i++) {
			
			int16_t val = ((i ) - 240) << 6;
			
			dummy[i] = val;
		}
	
	do {
		__WFE();
	} while (!ble_connected);
	printf("cm4 woken up!\n");
		
    for(;;) {
		// only start writing the next frame if that frame is not being read by cm0p
		if (!ble_connected) {
			// setup encoding
			enc = lc3_setup_encoder(FRAME_US, SRATE_HZ, SRATE_HZ, &enc_mem);
		
			if (!enc) {
				printf("Encoder initialization failed");
			}
			continue;
		} else if (current_frame_reading == (current_frame_written+1) % S_BLOCKS) {
			continue;
		}
		
		int current_frame_writing = (current_frame_written+1) % S_BLOCKS;
		
		
		
//		printf("encoding frame\n");
		uint8_t* out_ptr = &shared_buffer[BLOCK_SIZE*current_frame_writing];
		int result = lc3_encode(enc, LC3_PCM_FORMAT_S16, dummy, 1, BLOCK_SIZE, out_ptr);
		
		char str[50];
		snprintf(str, 50, "%d, 0x%X\n", result, shared_buffer[current_frame_writing*BLOCK_SIZE]);
//		printf(str);
		
		
		current_frame_written = current_frame_writing;
    }
}


/* [] END OF FILE */
