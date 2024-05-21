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
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

#include "project.h"

void ble_callback(uint32_t event, void * eventParam);

#define printf(x)	Cy_SCB_UART_PutString(UART_DEBUG_HW, x)
#define NOTIFICATION_PKT_SIZE	160
#define CUSTOM_SERV0_CHAR0_HANDLE           cy_ble_customConfig.customs[0]\
                                        .customServInfo[0].customServCharHandle 
#define AUDIO_SERV_HANDLE	 CY_BLE_AUDIO_SERVICE_SERVICE_HANDLE

#define SUCCESS                     (0u)


static cy_stc_ble_conn_handle_t appConnHandle;
static uint8_t transmit_buffer[NOTIFICATION_PKT_SIZE];
static cy_stc_ble_gatts_handle_value_ntf_t notificationPacket;

int main(void) {
	cy_en_ble_api_result_t          apiResult;
	
    __enable_irq(); /* Enable global interrupts. */
	
	
	apiResult = Cy_BLE_Start(ble_callback);
	if(apiResult == CY_BLE_SUCCESS) {
		/* Enable CM4 only if BLE Controller started successfully.
		* CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout
		* is changed. */
		Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR);
	} else {
		/* Halt CPU */
		CY_ASSERT(0u != 0u);
	} 

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
	/* UART initialization status */
    cy_en_scb_uart_status_t uart_status ;
     /* Initialize UART operation. Config and Context structure is copied from Generated source. */
    uart_status  = Cy_SCB_UART_Init(UART_DEBUG_HW, &UART_DEBUG_config, &UART_DEBUG_context);
    if(uart_status != CY_SCB_UART_SUCCESS)
    {
        exit(1);
    }	
    Cy_SCB_UART_Enable(UART_DEBUG_HW);
	Cy_SCB_UART_PutString(UART_DEBUG_HW, "uart working\n");
	
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
	
	
	while (Cy_BLE_GetState() != CY_BLE_STATE_ON) {
		Cy_BLE_ProcessEvents();
	}
	
	for(uint8_t i=0; i < NOTIFICATION_PKT_SIZE; i++) {
        transmit_buffer[i]=i; 
    }
	printf("trm buffer ready\n");
	
    for(;;)
    {
        /* Place your application code here. */
		Cy_BLE_ProcessEvents();
		
		if ((Cy_BLE_GATT_GetBusyStatus(appConnHandle.attId) == CY_BLE_STACK_STATE_FREE) && (Cy_BLE_GetConnectionState(appConnHandle) >= CY_BLE_CONN_STATE_CONNECTED) /*&& (Cy_BLE_IsDevicePaired(&appConnHandle) == true)*/) {
			printf("notification\n");
			cy_en_ble_api_result_t apiResult;
    		apiResult = Cy_BLE_GATTS_Notification(&notificationPacket);
   			if(apiResult == CY_BLE_ERROR_INVALID_PARAMETER) {
				printf("Couldn't send notification. [CY_BLE_ERROR_INVALID_PARAMETER]\n");
			} else if(apiResult != CY_BLE_SUCCESS) {
				printf("Attrhandle = 0x  Cy_BLE_GATTS_Notification API Error: 0x\n");
			}
		}
    }
}

void ble_callback(uint32_t event, void * eventParam) {
	cy_stc_ble_gatts_write_cmd_req_param_t *writeReqParameter;
	
	switch (event) {
	case CY_BLE_EVT_STACK_ON:
        printf("Start advertising\n");
        Cy_BLE_GAPP_StartAdvertisement(CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX);
		break;
		
    case CY_BLE_EVT_GAP_DEVICE_DISCONNECTED:
        printf("Device disconnected!\n");
        Cy_BLE_GAPP_StartAdvertisement(CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX);
        printf(">>Start advertising!\n");
        break;

    case CY_BLE_EVT_GATT_CONNECT_IND:
		{
			appConnHandle = *(cy_stc_ble_conn_handle_t *) eventParam;
			
			char interp[50]; 
			snprintf(interp, 50, "val rec: %s\n", appConnHandle);
            printf(interp);
			
		notificationPacket.connHandle = appConnHandle;
        notificationPacket.handleValPair.attrHandle = cy_ble_customConfig.customs[0].customServInfo[0].customServCharHandle;
        notificationPacket.handleValPair.value.val = transmit_buffer;
        notificationPacket.handleValPair.value.len = NOTIFICATION_PKT_SIZE;
        printf("Device connected!\n");
        	
		
		Cy_BLE_GetPhy(appConnHandle.bdHandle);    
		}
		
		
		break;
		
	/* This event is triggered when there is a change to either the maximum Payload 
        length or the maximum transmission time of Data Channel PDUs in either direction */
        case CY_BLE_EVT_DATA_LENGTH_CHANGE:
        {
            printf("CY_BLE_EVT_DATA_LENGTH_CHANGE \r\n");
			cy_en_ble_api_result_t apiResult;
            cy_stc_ble_set_phy_info_t phyParam;
            
            /* Configure the BLE Component for 2Mbps data rate */
            phyParam.bdHandle = appConnHandle.bdHandle;
            phyParam.allPhyMask = CY_BLE_PHY_NO_PREF_MASK_NONE;
            phyParam.phyOption = 0;
            phyParam.rxPhyMask = CY_BLE_PHY_MASK_LE_2M;
            phyParam.txPhyMask = CY_BLE_PHY_MASK_LE_2M;
            
            Cy_BLE_EnablePhyUpdateFeature();
            apiResult = Cy_BLE_SetPhy(&phyParam);
            if(apiResult != CY_BLE_SUCCESS)
            {
                printf("Failed to set PHY..[bdHandle 0x%02X] : 0x%4x\r\n");
            }
            else
            {
                printf("Setting PHY.[bdHandle 0x%02X] \r\n");
            }
            
            break;
        }
		
		/* This event indicates completion of the Cy_BLE_SetPhy API*/
		case CY_BLE_EVT_SET_PHY_COMPLETE:
        {
            printf("Updating the Phy.....\r\n");
            cy_stc_ble_events_param_generic_t * param =\
            (cy_stc_ble_events_param_generic_t *) eventParam;
            if(param->status == SUCCESS)
            {
                printf("SET PHY updated to 2 Mbps\r\n");
                Cy_BLE_GetPhy(appConnHandle.bdHandle);
            }
            else
            {
                printf("SET PHY Could not update to 2 Mbps\r\n");
                Cy_BLE_GetPhy(appConnHandle.bdHandle);
            }
            
            break;
        }

    default:
		{
        	char interp[50]; 
			snprintf(interp, 50, "val rec: %.04X\n", event);
            printf(interp);
		}
        break;
	}
}

/* [] END OF FILE */
