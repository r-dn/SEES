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
#include "pitch_shift.h"

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <lc3.h>

typedef LC3_ENCODER_MEM_T(FRAME_US, SRATE_HZ) lc3_encoder_mem_t;

// the lc3 encoder object & its memory
lc3_encoder_t enc;
lc3_encoder_mem_t enc_mem;

// the DMA frame buffer
// the alignment is necessary for the encoding
int16_t alignas(uint32_t) new_frame[FRAME_SAMPLES];


void PDM_Interrupt_User(void) {
    NVIC_DisableIRQ(DMA_IRQ_cfg.intrSrc);
    Cy_DMA_Channel_ClearInterrupt(DMA_HW, DMA_DW_CHANNEL);
	
	frame_received = true;   
}

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
	
	// init encoder
	enc = lc3_setup_encoder(FRAME_US, SRATE_HZ, SRATE_HZ, &enc_mem);
		
	if (!enc) {
		printf("Encoder initialization failed");
	}
	
	// init shared vars 
	current_frame_written = 0;
	memset(shared_buffer, 0, sizeof(shared_buffer));
	
	// init DMA & PDM-PCM
	Cy_SysInt_Init(&DMA_IRQ_cfg, PDM_Interrupt_User);
    NVIC_EnableIRQ(DMA_IRQ_cfg.intrSrc);
    Cy_PDM_PCM_Init(PDM_PCM_HW, &PDM_PCM_config);
	
	DMA_Init();
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_REC, (void *) &PDM_PCM_HW->RX_FIFO_RD);
    Cy_DMA_Descriptor_SetDstAddress(&DMA_REC, (void *) new_frame);
    DMA_SetInterruptMask(DMA_INTR_MASK);
	
	// this gives problems in release mode
	// only wake up when the device is connected
//	do {
//		__WFE();
//	} while (!ble_connected);
	printf("cm4 woken up!\n");
	
		
    for(;;) {
		// main loop
		
		// only start writing the next frame if that frame is not being read by cm0p
		if (!ble_connected) {
			// lost connection so reinit the encoder
			enc = lc3_setup_encoder(FRAME_US, SRATE_HZ, SRATE_HZ, &enc_mem);
		
			if (!enc) {
				printf("Encoder initialization failed");
			}
			
			/* Enable PDM block */
    		Cy_PDM_PCM_Enable(PDM_PCM_HW);
			
			/* Reset the channel index for the next recording */
			DMA_HW->CH_STRUCT[DMA_DW_CHANNEL].CH_IDX = 0;
    
			/* Clean-up the PDM_PCM FIFO */
			Cy_PDM_PCM_ClearFifo(PDM_PCM_HW);
			
			/* Enable DMA to record from the microphone */
			Cy_DMA_Channel_Enable(DMA_HW, DMA_DW_CHANNEL);
			
			continue;
		} else if (current_frame_reading == (current_frame_written+1) % S_BLOCKS) {
			// make sure the circular shared buffer is available to write to
			continue;
		} else if (!frame_received) {
			continue;
		}
		
		// received frame!
		// start pitch detection & encoding of frame
		
		// update the shared buffer pointer
		int current_frame_writing = (current_frame_written+1) % S_BLOCKS;
		
		// detect the period of the incoming frame
		// this is quite slow
		float curr_period = 0;//detect_period(new_frame, FRAME_SAMPLES);
		freq[current_frame_writing] = SRATE_HZ/curr_period;
		
		if (!isnan(curr_period)) {
			char str[50];
			snprintf(str, 50, "%d mHz\n", (int) (freq[current_frame_writing]*1000));
			printf(str);
		}
		
		// encoding
		uint8_t* out_ptr = &shared_buffer[BLOCK_SIZE*current_frame_writing];
		int result = lc3_encode(enc, LC3_PCM_FORMAT_S16, new_frame, 1, BLOCK_SIZE_R, out_ptr);
		
		// update shared buffer pointers & reënable the DMA interrupt
		current_frame_written = current_frame_writing;
		frame_received = false;
		NVIC_EnableIRQ(DMA_IRQ_cfg.intrSrc);
    }
}



/* [] END OF FILE */
