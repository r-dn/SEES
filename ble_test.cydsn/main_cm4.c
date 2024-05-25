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

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
	
	memset(shared_buffer, 0, sizeof(shared_buffer));
	
	current_frame_written = 0;
	
	do {
		__WFE();
	} while (!ble_connected);
	printf("woken up!\n");
		
    for(;;) {
		// only start writing the next frame if that frame is not being read by cm0p
		if (!ble_connected || current_frame_reading == (current_frame_written+1) % S_FRAMES) {
			continue;
		}
//		printf("started writing\n");
		
		char str[50];
            snprintf(str, 50, "cur_fr_r: %d, written: %d \n", current_frame_reading, current_frame_written);
//			printf(str);
		
		int current_frame_writing = (current_frame_written+1) % S_FRAMES;
		
		// fill with random stuff for testing
		for (uint16_t i = 0; i < FRAME_SAMPLES; i++) {
			shared_buffer[2*i + FRAME_SAMPLES*PCM_SBYTES*current_frame_writing] = i & 0xFF;
			shared_buffer[2*i+1 + FRAME_SAMPLES*PCM_SBYTES*current_frame_writing] = i >> 8;
		}
		
		current_frame_written = current_frame_writing;
    }
	
	
}


/* [] END OF FILE */
