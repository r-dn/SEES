/*******************************************************************************
* File Name: PDM_PCM.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the
*  PDM_PCM component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PDM_PCM.h"


/** An instance-specific configuration structure.
*   It should be used in the associated Cy_PDM_PCM_Init() function.
*/
const  cy_stc_pdm_pcm_config_t PDM_PCM_config =
{
    .clkDiv             = CY_PDM_PCM_CLK_DIV_1_4,
    .mclkDiv            = CY_PDM_PCM_CLK_DIV_BYPASS,
    .ckoDiv             = 3U,
    .ckoDelay           = 0U,
    .sincDecRate        = 32U,
    .chanSelect         = CY_PDM_PCM_OUT_CHAN_LEFT,
    .chanSwapEnable     = false,
    .highPassFilterGain = 1U,
    .highPassDisable    = false,
    .softMuteCycles     = CY_PDM_PCM_SOFT_MUTE_CYCLES_96,
    .softMuteFineGain   = 1UL,
    .softMuteEnable     = false,
    .wordLen            = CY_PDM_PCM_WLEN_16_BIT,
    .signExtension      = true,
    .gainLeft           = CY_PDM_PCM_GAIN_10_5_DB,
    .gainRight          = CY_PDM_PCM_BYPASS,
    .rxFifoTriggerLevel = 240U,
    .dmaTriggerEnable   = true,
    .interruptMask      = 0UL
};

/* An internal variable indicates whether the PDM_PCM has been
*  initialized. The variable is initialized to 0 and set to 1 the first time
*  PDM_PCM_Start() is called. This allows the Component to
*  restart without reinitialization after the first call to the
*  PDM_PCM_Start() routine. The variable is cleared by the
*  PDM_PCM_DeInit() routine call.
*/
uint8_t PDM_PCM_initVar = 0U;

/*******************************************************************************
* Function Name: PDM_PCM_Start
********************************************************************************
*
* Calls the Cy_PDM_PCM_Init() function when called the first time or after the
* \ref PDM_PCM_DeInit() call, then clears FIFO and enables the PDM-PCM 
* data coversion. For subsequent calls (after the
* \ref PDM_PCM_Stop() call) the configuration is left unchanged, the
* FIFO is cleared and the PDM-PCM data coversion is enabled.
*
*******************************************************************************/
void PDM_PCM_Start(void)
{
    if(0U == PDM_PCM_initVar)
    {
        (void) Cy_PDM_PCM_Init(PDM_PCM_HW, &PDM_PCM_config);
        PDM_PCM_initVar = 1U;
    }

    Cy_PDM_PCM_ClearFifo(PDM_PCM_HW);
    Cy_PDM_PCM_Enable(PDM_PCM_HW);
}


/* [] END OF FILE */
