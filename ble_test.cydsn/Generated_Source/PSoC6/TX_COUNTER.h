/*******************************************************************************
* File Name: TX_COUNTER.h
* Version 1.0
*
* Description:
*  This file provides constants and parameter values for the TX_COUNTER
*  component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(TX_COUNTER_CY_TCPWM_COUNTER_PDL_H)
#define TX_COUNTER_CY_TCPWM_COUNTER_PDL_H

#include "cyfitter.h"
#include "tcpwm/cy_tcpwm_counter.h"

   
/*******************************************************************************
* Variables
*******************************************************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t  TX_COUNTER_initVar;
extern cy_stc_tcpwm_counter_config_t const TX_COUNTER_config;
/** @} group_globals */


/***************************************
*  Conditional Compilation Parameters
***************************************/

#define TX_COUNTER_INIT_COMPARE_OR_CAPTURE    (2UL)


/***************************************
*        Function Prototypes
****************************************/
/**
* \addtogroup group_general
* @{
*/
void TX_COUNTER_Start(void);
__STATIC_INLINE cy_en_tcpwm_status_t TX_COUNTER_Init(cy_stc_tcpwm_counter_config_t const *config);
__STATIC_INLINE void TX_COUNTER_DeInit(void);
__STATIC_INLINE void TX_COUNTER_Enable(void);
__STATIC_INLINE void TX_COUNTER_Disable(void);
__STATIC_INLINE uint32_t TX_COUNTER_GetStatus(void);

#if(CY_TCPWM_COUNTER_MODE_CAPTURE == TX_COUNTER_INIT_COMPARE_OR_CAPTURE)
    __STATIC_INLINE uint32_t TX_COUNTER_GetCapture(void);
    __STATIC_INLINE uint32_t TX_COUNTER_GetCaptureBuf(void);
#else
    __STATIC_INLINE void TX_COUNTER_SetCompare0(uint32_t compare0);
    __STATIC_INLINE uint32_t TX_COUNTER_GetCompare0(void);
    __STATIC_INLINE void TX_COUNTER_SetCompare1(uint32_t compare1);
    __STATIC_INLINE uint32_t TX_COUNTER_GetCompare1(void);
    __STATIC_INLINE void TX_COUNTER_EnableCompareSwap(bool enable);
#endif /* (CY_TCPWM_COUNTER_MODE_CAPTURE == TX_COUNTER_INIT_COMPARE_OR_CAPTURE) */

__STATIC_INLINE void TX_COUNTER_SetCounter(uint32_t count);
__STATIC_INLINE uint32_t TX_COUNTER_GetCounter(void);
__STATIC_INLINE void TX_COUNTER_SetPeriod(uint32_t period);
__STATIC_INLINE uint32_t TX_COUNTER_GetPeriod(void);
__STATIC_INLINE void TX_COUNTER_TriggerStart(void);
__STATIC_INLINE void TX_COUNTER_TriggerReload(void);
__STATIC_INLINE void TX_COUNTER_TriggerStop(void);
__STATIC_INLINE void TX_COUNTER_TriggerCapture(void);
__STATIC_INLINE uint32_t TX_COUNTER_GetInterruptStatus(void);
__STATIC_INLINE void TX_COUNTER_ClearInterrupt(uint32_t source);
__STATIC_INLINE void TX_COUNTER_SetInterrupt(uint32_t source);
__STATIC_INLINE void TX_COUNTER_SetInterruptMask(uint32_t mask);
__STATIC_INLINE uint32_t TX_COUNTER_GetInterruptMask(void);
__STATIC_INLINE uint32_t TX_COUNTER_GetInterruptStatusMasked(void);
/** @} general */


/***************************************
*           API Constants
***************************************/

/**
* \addtogroup group_macros
* @{
*/
/** This is a ptr to the base address of the TCPWM instance */
#define TX_COUNTER_HW                 (TX_COUNTER_TCPWM__HW)

/** This is a ptr to the base address of the TCPWM CNT instance */
#define TX_COUNTER_CNT_HW             (TX_COUNTER_TCPWM__CNT_HW)

/** This is the counter instance number in the selected TCPWM */
#define TX_COUNTER_CNT_NUM            (TX_COUNTER_TCPWM__CNT_IDX)

/** This is the bit field representing the counter instance in the selected TCPWM */
#define TX_COUNTER_CNT_MASK           (1UL << TX_COUNTER_CNT_NUM)
/** @} group_macros */

#define TX_COUNTER_INPUT_MODE_MASK    (0x3U)
#define TX_COUNTER_INPUT_DISABLED     (7U)


/*******************************************************************************
* Function Name: TX_COUNTER_Init
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_tcpwm_status_t TX_COUNTER_Init(cy_stc_tcpwm_counter_config_t const *config)
{
    return(Cy_TCPWM_Counter_Init(TX_COUNTER_HW, TX_COUNTER_CNT_NUM, config));
}


/*******************************************************************************
* Function Name: TX_COUNTER_DeInit
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void TX_COUNTER_DeInit(void)                   
{
    Cy_TCPWM_Counter_DeInit(TX_COUNTER_HW, TX_COUNTER_CNT_NUM, &TX_COUNTER_config);
}

/*******************************************************************************
* Function Name: TX_COUNTER_Enable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Enable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void TX_COUNTER_Enable(void)                   
{
    Cy_TCPWM_Enable_Multiple(TX_COUNTER_HW, TX_COUNTER_CNT_MASK);
}


/*******************************************************************************
* Function Name: TX_COUNTER_Disable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Disable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void TX_COUNTER_Disable(void)                  
{
    Cy_TCPWM_Disable_Multiple(TX_COUNTER_HW, TX_COUNTER_CNT_MASK);
}


/*******************************************************************************
* Function Name: TX_COUNTER_GetStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_GetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t TX_COUNTER_GetStatus(void)                
{
    return(Cy_TCPWM_Counter_GetStatus(TX_COUNTER_HW, TX_COUNTER_CNT_NUM));
}


#if(CY_TCPWM_COUNTER_MODE_CAPTURE == TX_COUNTER_INIT_COMPARE_OR_CAPTURE)
    /*******************************************************************************
    * Function Name: TX_COUNTER_GetCapture
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCapture() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t TX_COUNTER_GetCapture(void)               
    {
        return(Cy_TCPWM_Counter_GetCapture(TX_COUNTER_HW, TX_COUNTER_CNT_NUM));
    }


    /*******************************************************************************
    * Function Name: TX_COUNTER_GetCaptureBuf
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCaptureBuf() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t TX_COUNTER_GetCaptureBuf(void)            
    {
        return(Cy_TCPWM_Counter_GetCaptureBuf(TX_COUNTER_HW, TX_COUNTER_CNT_NUM));
    }
#else
    /*******************************************************************************
    * Function Name: TX_COUNTER_SetCompare0
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_SetCompare0() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE void TX_COUNTER_SetCompare0(uint32_t compare0)      
    {
        Cy_TCPWM_Counter_SetCompare0(TX_COUNTER_HW, TX_COUNTER_CNT_NUM, compare0);
    }


    /*******************************************************************************
    * Function Name: TX_COUNTER_GetCompare0
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCompare0() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t TX_COUNTER_GetCompare0(void)              
    {
        return(Cy_TCPWM_Counter_GetCompare0(TX_COUNTER_HW, TX_COUNTER_CNT_NUM));
    }


    /*******************************************************************************
    * Function Name: TX_COUNTER_SetCompare1
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_SetCompare1() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE void TX_COUNTER_SetCompare1(uint32_t compare1)      
    {
        Cy_TCPWM_Counter_SetCompare1(TX_COUNTER_HW, TX_COUNTER_CNT_NUM, compare1);
    }


    /*******************************************************************************
    * Function Name: TX_COUNTER_GetCompare1
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCompare1() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t TX_COUNTER_GetCompare1(void)              
    {
        return(Cy_TCPWM_Counter_GetCompare1(TX_COUNTER_HW, TX_COUNTER_CNT_NUM));
    }


    /*******************************************************************************
    * Function Name: TX_COUNTER_EnableCompareSwap
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_EnableCompareSwap() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE void TX_COUNTER_EnableCompareSwap(bool enable)  
    {
        Cy_TCPWM_Counter_EnableCompareSwap(TX_COUNTER_HW, TX_COUNTER_CNT_NUM, enable);
    }
#endif /* (CY_TCPWM_COUNTER_MODE_CAPTURE == TX_COUNTER_INIT_COMPARE_OR_CAPTURE) */


/*******************************************************************************
* Function Name: TX_COUNTER_SetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_SetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void TX_COUNTER_SetCounter(uint32_t count)          
{
    Cy_TCPWM_Counter_SetCounter(TX_COUNTER_HW, TX_COUNTER_CNT_NUM, count);
}


/*******************************************************************************
* Function Name: TX_COUNTER_GetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_GetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t TX_COUNTER_GetCounter(void)               
{
    return(Cy_TCPWM_Counter_GetCounter(TX_COUNTER_HW, TX_COUNTER_CNT_NUM));
}


/*******************************************************************************
* Function Name: TX_COUNTER_SetPeriod
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_SetPeriod() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void TX_COUNTER_SetPeriod(uint32_t period)          
{
    Cy_TCPWM_Counter_SetPeriod(TX_COUNTER_HW, TX_COUNTER_CNT_NUM, period);
}


/*******************************************************************************
* Function Name: TX_COUNTER_GetPeriod
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_GetPeriod() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t TX_COUNTER_GetPeriod(void)                
{
    return(Cy_TCPWM_Counter_GetPeriod(TX_COUNTER_HW, TX_COUNTER_CNT_NUM));
}


/*******************************************************************************
* Function Name: TX_COUNTER_TriggerStart
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void TX_COUNTER_TriggerStart(void)             
{
    Cy_TCPWM_TriggerStart(TX_COUNTER_HW, TX_COUNTER_CNT_MASK);
}


/*******************************************************************************
* Function Name: TX_COUNTER_TriggerReload
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerReloadOrIndex() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void TX_COUNTER_TriggerReload(void)     
{
    Cy_TCPWM_TriggerReloadOrIndex(TX_COUNTER_HW, TX_COUNTER_CNT_MASK);
}


/*******************************************************************************
* Function Name: TX_COUNTER_TriggerStop
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStopOrKill() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void TX_COUNTER_TriggerStop(void)
{
    Cy_TCPWM_TriggerStopOrKill(TX_COUNTER_HW, TX_COUNTER_CNT_MASK);
}


/*******************************************************************************
* Function Name: TX_COUNTER_TriggerCapture
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerCaptureOrSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void TX_COUNTER_TriggerCapture(void)     
{
    Cy_TCPWM_TriggerCaptureOrSwap(TX_COUNTER_HW, TX_COUNTER_CNT_MASK);
}


/*******************************************************************************
* Function Name: TX_COUNTER_GetInterruptStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t TX_COUNTER_GetInterruptStatus(void)       
{
    return(Cy_TCPWM_GetInterruptStatus(TX_COUNTER_HW, TX_COUNTER_CNT_NUM));
}


/*******************************************************************************
* Function Name: TX_COUNTER_ClearInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_ClearInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void TX_COUNTER_ClearInterrupt(uint32_t source)     
{
    Cy_TCPWM_ClearInterrupt(TX_COUNTER_HW, TX_COUNTER_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: TX_COUNTER_SetInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void TX_COUNTER_SetInterrupt(uint32_t source)
{
    Cy_TCPWM_SetInterrupt(TX_COUNTER_HW, TX_COUNTER_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: TX_COUNTER_SetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void TX_COUNTER_SetInterruptMask(uint32_t mask)     
{
    Cy_TCPWM_SetInterruptMask(TX_COUNTER_HW, TX_COUNTER_CNT_NUM, mask);
}


/*******************************************************************************
* Function Name: TX_COUNTER_GetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t TX_COUNTER_GetInterruptMask(void)         
{
    return(Cy_TCPWM_GetInterruptMask(TX_COUNTER_HW, TX_COUNTER_CNT_NUM));
}


/*******************************************************************************
* Function Name: TX_COUNTER_GetInterruptStatusMasked
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatusMasked() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t TX_COUNTER_GetInterruptStatusMasked(void)
{
    return(Cy_TCPWM_GetInterruptStatusMasked(TX_COUNTER_HW, TX_COUNTER_CNT_NUM));
}

#endif /* TX_COUNTER_CY_TCPWM_COUNTER_PDL_H */


/* [] END OF FILE */
