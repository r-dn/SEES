/*******************************************************************************
* File Name: PDM_PCM.h
* Version 2.0
*
* Description:
*  This file provides constants, parameter values, and API definition for the
*  PDM_PCM Component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PDMPCM_PDM_PCM_H)
#define CY_PDMPCM_PDM_PCM_H

#include "cyfitter.h"
#include "pdm_pcm/cy_pdm_pcm.h"


/***************************************
*    Initial Parameter Constants
***************************************/

#define PDM_PCM_GAIN_LEFT              (CY_PDM_PCM_GAIN_10_5_DB)
#define PDM_PCM_GAIN_RIGHT             (CY_PDM_PCM_BYPASS)
#define PDM_PCM_CHANNELS               (CY_PDM_PCM_OUT_CHAN_LEFT)
#define PDM_PCM_WORD_LEN               (CY_PDM_PCM_WLEN_16_BIT)
#define PDM_PCM_RX_FIFO_TRG_LVL        (240U)

/* Interrupt mask */
#define PDM_PCM_INT_MASK               (_BOOL2FLD(PDM_INTR_RX_TRIGGER,   false)     | \
                                                 _BOOL2FLD(PDM_INTR_RX_NOT_EMPTY, false) | \
                                                 _BOOL2FLD(PDM_INTR_RX_OVERFLOW,  false) | \
                                                 _BOOL2FLD(PDM_INTR_RX_UNDERFLOW, false))


/***************************************
*           API Constants
***************************************/
    
/** The pointer to the base address of the HW PDM-PCM instance. */
#define PDM_PCM_HW                     (PDM)
/** The Rx FIFO Read register pointer for DMA initialization. */
#define PDM_PCM_RX_FIFO_RD_PTR         ((volatile const uint32_t *) &(PDM_PCM_HW->RX_FIFO_RD))
    
/***************************************
*        Function Prototypes
***************************************/

__STATIC_INLINE void     PDM_PCM_Init(void);
__STATIC_INLINE void     PDM_PCM_Deinit(void);
                void     PDM_PCM_Start(void);
__STATIC_INLINE void     PDM_PCM_Stop(void);
__STATIC_INLINE void     PDM_PCM_Enable(void);
__STATIC_INLINE void     PDM_PCM_Disable(void);

__STATIC_INLINE uint8_t  PDM_PCM_GetNumInFifo(void);
__STATIC_INLINE uint32_t PDM_PCM_ReadFifo(void);
__STATIC_INLINE void     PDM_PCM_ClearFifo(void);
__STATIC_INLINE uint32_t PDM_PCM_ReadFifoSilent(void);
__STATIC_INLINE void     PDM_PCM_FreezeFifo(void);
__STATIC_INLINE void     PDM_PCM_UnfreezeFifo(void);

__STATIC_INLINE void     PDM_PCM_SetGain(cy_en_pdm_pcm_chan_select_t chan, cy_en_pdm_pcm_gain_t gain);
__STATIC_INLINE cy_en_pdm_pcm_gain_t PDM_PCM_GetGain(cy_en_pdm_pcm_chan_select_t chan);

__STATIC_INLINE void     PDM_PCM_EnableSoftMute(void);
__STATIC_INLINE void     PDM_PCM_DisableSoftMute(void);

__STATIC_INLINE uint32_t PDM_PCM_GetInterruptStatus(void);
__STATIC_INLINE void     PDM_PCM_ClearInterrupt(uint32_t interrupt);
__STATIC_INLINE void     PDM_PCM_SetInterrupt(uint32_t interrupt);
__STATIC_INLINE uint32_t PDM_PCM_GetInterruptMask(void);
__STATIC_INLINE void     PDM_PCM_SetInterruptMask(uint32_t interrupt);
__STATIC_INLINE uint32_t PDM_PCM_GetInterruptStatusMasked(void);


/***************************************
*   Variable with external linkage
***************************************/

extern uint8 PDM_PCM_initVar;
extern const  cy_stc_pdm_pcm_config_t PDM_PCM_config;


/***************************************
*    In-line Functions Implementation
***************************************/

/*******************************************************************************
* Function Name: PDM_PCM_Init
****************************************************************************//**
* Invokes the Cy_PDM_PCM_Init() PDL driver function.
*******************************************************************************/
__STATIC_INLINE void PDM_PCM_Init(void)
{
    (void) Cy_PDM_PCM_Init(PDM_PCM_HW, &PDM_PCM_config);
}


/*******************************************************************************
* Function Name: PDM_PCM_Deinit
****************************************************************************//**
* Invokes the Cy_PDM_PCM_DeInit() PDL driver function.
*******************************************************************************/
__STATIC_INLINE void PDM_PCM_Deinit(void)
{
    PDM_PCM_initVar = 0U;
    Cy_PDM_PCM_DeInit(PDM_PCM_HW);
}


/*******************************************************************************
* Function Name: PDM_PCM_Stop
********************************************************************************
* Invokes the Cy_PDM_PCM_Disable() PDL driver function.
*******************************************************************************/
__STATIC_INLINE void PDM_PCM_Stop(void)
{
    Cy_PDM_PCM_Disable(PDM_PCM_HW);
}


/*******************************************************************************
* Function Name: PDM_PCM_Enable
****************************************************************************//**
* Invokes the Cy_PDM_PCM_Enable() PDL driver function.
*******************************************************************************/
__STATIC_INLINE void PDM_PCM_Enable(void)
{
    Cy_PDM_PCM_Enable(PDM_PCM_HW);
}


/*******************************************************************************
* Function Name: PDM_PCM_Disable
****************************************************************************//**
* Invokes the Cy_PDM_PCM_Disable() PDL driver function.
*******************************************************************************/
__STATIC_INLINE void PDM_PCM_Disable(void)
{
    Cy_PDM_PCM_Disable(PDM_PCM_HW);
}


/*******************************************************************************
* Function Name: PDM_PCM_GetNumInFifo
****************************************************************************//**
* Invokes the Cy_PDM_PCM_GetNumInFifo() PDL driver function.
*******************************************************************************/
__STATIC_INLINE uint8_t PDM_PCM_GetNumInFifo(void)
{
    return (Cy_PDM_PCM_GetNumInFifo(PDM_PCM_HW));
}


/*******************************************************************************
* Function Name: PDM_PCM_ReadFifo
****************************************************************************//**
* Invokes the Cy_PDM_PCM_ReadFifo() PDL driver function.
*******************************************************************************/
__STATIC_INLINE uint32_t PDM_PCM_ReadFifo(void)
{
    return (Cy_PDM_PCM_ReadFifo(PDM_PCM_HW));
}


/*******************************************************************************
* Function Name: PDM_PCM_ClearFifo
****************************************************************************//**
* Invokes the Cy_PDM_PCM_ClearFifo() PDL driver function.
*******************************************************************************/
__STATIC_INLINE void PDM_PCM_ClearFifo(void)
{
    Cy_PDM_PCM_ClearFifo(PDM_PCM_HW);
}


/*******************************************************************************
* Function Name: PDM_PCM_SetGain
****************************************************************************//**
* Invokes the Cy_PDM_PCM_SetGain() PDL driver function.
*******************************************************************************/
__STATIC_INLINE void PDM_PCM_SetGain(cy_en_pdm_pcm_chan_select_t chan, cy_en_pdm_pcm_gain_t gain)
{
    Cy_PDM_PCM_SetGain(PDM_PCM_HW, chan, gain);
}


/*******************************************************************************
* Function Name: PDM_PCM_GetGain
****************************************************************************//**
* Invokes the Cy_PDM_PCM_GetGain() PDL driver function.
*******************************************************************************/
__STATIC_INLINE cy_en_pdm_pcm_gain_t PDM_PCM_GetGain(cy_en_pdm_pcm_chan_select_t chan)
{
    return (Cy_PDM_PCM_GetGain(PDM_PCM_HW, chan));
}


/*******************************************************************************
* Function Name: PDM_PCM_EnableSoftMute
****************************************************************************//**
* Invokes the Cy_PDM_PCM_EnableSoftMute() PDL driver function.
*******************************************************************************/
__STATIC_INLINE void PDM_PCM_EnableSoftMute(void)
{
    Cy_PDM_PCM_EnableSoftMute(PDM_PCM_HW);
}


/*******************************************************************************
* Function Name: PDM_PCM_DisableSoftMute
****************************************************************************//**
* Invokes the Cy_PDM_PCM_DisableSoftMute() PDL driver function.
*******************************************************************************/
__STATIC_INLINE void PDM_PCM_DisableSoftMute(void)
{
    Cy_PDM_PCM_DisableSoftMute(PDM_PCM_HW);
}


/*******************************************************************************
* Function Name: PDM_PCM_FreezeFifo
****************************************************************************//**
* Invokes the Cy_PDM_PCM_FreezeFifo() PDL driver function.
*******************************************************************************/
__STATIC_INLINE void PDM_PCM_FreezeFifo(void)
{
    Cy_PDM_PCM_FreezeFifo(PDM_PCM_HW);
}


/*******************************************************************************
* Function Name: PDM_PCM_UnfreezeFifo
****************************************************************************//**
* Invokes the Cy_PDM_PCM_UnfreezeFifo() PDL driver function.
*******************************************************************************/
__STATIC_INLINE void PDM_PCM_UnfreezeFifo(void)
{
    Cy_PDM_PCM_UnfreezeFifo(PDM_PCM_HW);
}


/*******************************************************************************
* Function Name: PDM_PCM_ReadFifoSilent
****************************************************************************//**
* Invokes the Cy_PDM_PCM_ReadFifoSilent() PDL driver function.
*******************************************************************************/
__STATIC_INLINE uint32_t PDM_PCM_ReadFifoSilent(void)
{
    return (Cy_PDM_PCM_ReadFifoSilent(PDM_PCM_HW));
}


/*******************************************************************************
* Function Name: PDM_PCM_GetInterruptStatus
****************************************************************************//**
* Invokes the Cy_PDM_PCM_GetInterruptStatus() PDL driver function.
*******************************************************************************/
__STATIC_INLINE uint32_t PDM_PCM_GetInterruptStatus(void)
{
    return (Cy_PDM_PCM_GetInterruptStatus(PDM_PCM_HW));
}


/*******************************************************************************
* Function Name: PDM_PCM_ClearInterrupt
****************************************************************************//**
* Invokes the Cy_PDM_PCM_ClearInterrupt() PDL driver function.
*******************************************************************************/
__STATIC_INLINE void PDM_PCM_ClearInterrupt(uint32_t interrupt)
{
    Cy_PDM_PCM_ClearInterrupt(PDM_PCM_HW, interrupt);
}


/*******************************************************************************
* Function Name: PDM_PCM_SetInterrupt
****************************************************************************//**
* Invokes the Cy_PDM_PCM_SetInterrupt() PDL driver function.
*******************************************************************************/
__STATIC_INLINE void PDM_PCM_SetInterrupt(uint32_t interrupt)
{
    Cy_PDM_PCM_SetInterrupt(PDM_PCM_HW, interrupt);
}


/*******************************************************************************
* Function Name: PDM_PCM_GetInterruptMask
****************************************************************************//**
* Invokes the Cy_PDM_PCM_GetInterruptMask() PDL driver function.
*******************************************************************************/
__STATIC_INLINE uint32_t PDM_PCM_GetInterruptMask(void)
{
    return (Cy_PDM_PCM_GetInterruptMask(PDM_PCM_HW));
}


/*******************************************************************************
* Function Name: PDM_PCM_SetInterruptMask
****************************************************************************//**
* Invokes the Cy_PDM_PCM_SetInterruptMask() PDL driver function.
*******************************************************************************/
__STATIC_INLINE void PDM_PCM_SetInterruptMask(uint32_t interrupt)
{
    Cy_PDM_PCM_SetInterruptMask(PDM_PCM_HW, interrupt);
}


/*******************************************************************************
* Function Name: PDM_PCM_GetInterruptStatusMasked
****************************************************************************//**
* Invokes the Cy_PDM_PCM_GetInterruptStatusMasked() PDL driver function.
*******************************************************************************/
__STATIC_INLINE uint32_t PDM_PCM_GetInterruptStatusMasked(void)
{
    return (Cy_PDM_PCM_GetInterruptStatusMasked(PDM_PCM_HW));
}


#endif /* CY_PDMPCM_PDM_PCM_H */


/* [] END OF FILE */
