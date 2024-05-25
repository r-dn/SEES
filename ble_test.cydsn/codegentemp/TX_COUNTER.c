/*******************************************************************************
* File Name: TX_COUNTER.c
* Version 1.0
*
* Description:
*  This file provides the source code to the API for the TX_COUNTER
*  component
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "TX_COUNTER.h"

/** Indicates whether or not the TX_COUNTER has been initialized. 
*  The variable is initialized to 0 and set to 1 the first time 
*  TX_COUNTER_Start() is called. This allows the Component to 
*  restart without reinitialization after the first call to 
*  the TX_COUNTER_Start() routine.
*/
uint8_t TX_COUNTER_initVar = 0U;

/** The instance-specific configuration structure. This should be used in the 
*  associated TX_COUNTER_Init() function.
*/ 
cy_stc_tcpwm_counter_config_t const TX_COUNTER_config =
{
        .period = 10000UL,
        .clockPrescaler = 0UL,
        .runMode = 0UL,
        .countDirection = 0UL,
        .compareOrCapture = 2UL,
        .compare0 = 16384UL,
        .compare1 = 16384UL,
        .enableCompareSwap = false,
        .interruptSources = 0UL,
        .captureInputMode = 3UL,
        .captureInput = CY_TCPWM_INPUT_CREATOR,
        .reloadInputMode = 3UL,
        .reloadInput = CY_TCPWM_INPUT_CREATOR,
        .startInputMode = 3UL,
        .startInput = CY_TCPWM_INPUT_CREATOR,
        .stopInputMode = 3UL,
        .stopInput = CY_TCPWM_INPUT_CREATOR,
        .countInputMode = 3UL,
        .countInput = CY_TCPWM_INPUT_CREATOR,
};


/*******************************************************************************
* Function Name: TX_COUNTER_Start
****************************************************************************//**
*
*  Calls the TX_COUNTER_Init() when called the first time and enables 
*  the TX_COUNTER. For subsequent calls the configuration is left 
*  unchanged and the component is just enabled.
*
* \globalvars
*  \ref TX_COUNTER_initVar
*
*******************************************************************************/
void TX_COUNTER_Start(void)
{
    if (0U == TX_COUNTER_initVar)
    {
        (void)Cy_TCPWM_Counter_Init(TX_COUNTER_HW, TX_COUNTER_CNT_NUM, &TX_COUNTER_config); 

        TX_COUNTER_initVar = 1U;
    }

    Cy_TCPWM_Enable_Multiple(TX_COUNTER_HW, TX_COUNTER_CNT_MASK);
    
    #if (TX_COUNTER_INPUT_DISABLED == 7UL)
        Cy_TCPWM_TriggerStart(TX_COUNTER_HW, TX_COUNTER_CNT_MASK);
    #endif /* (TX_COUNTER_INPUT_DISABLED == 7UL) */    
}


/* [] END OF FILE */
