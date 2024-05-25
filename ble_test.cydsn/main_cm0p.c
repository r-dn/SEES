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
#include <lc3.h>



#define NOTIFICATION_PKT_SIZE	BLOCK_SIZE
#define SUCCESS					(0u)

typedef LC3_ENCODER_MEM_T(FRAME_US, SRATE_HZ) lc3_encoder_mem_t;

void ble_callback(uint32_t event, void * eventParam);

static cy_stc_ble_conn_handle_t appConnHandle;
static cy_stc_ble_gatts_handle_value_ntf_t notificationPacket;
static uint8_t tx_buffer[NOTIFICATION_PKT_SIZE];
lc3_encoder_t enc[NCHANNELS];

int main(void) {
	cy_en_ble_api_result_t          apiResult;
	
    __enable_irq(); /* Enable global interrupts. */
	
	apiResult = Cy_BLE_Start(ble_callback);
	
	// UART initialization
	cy_en_scb_uart_status_t uart_status ;
	// Initialize UART operation. Config and Context structure is copied from Generated source.
	uart_status  = Cy_SCB_UART_Init(UART_DEBUG_HW, &UART_DEBUG_config, &UART_DEBUG_context);
	if(uart_status != CY_SCB_UART_SUCCESS) {
        exit(1);
    }	
    Cy_SCB_UART_Enable(UART_DEBUG_HW);
	printf("\nUART working\n");
	
	// BLE initialization
	while (Cy_BLE_GetState() != CY_BLE_STATE_ON) {
		Cy_BLE_ProcessEvents();
	}
	
	if(apiResult == CY_BLE_SUCCESS) {
		/* Enable CM4 only if BLE Controller started successfully.
		* CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout
		* is changed. */
		ble_connected = false;
		Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR);
	} else {
		/* Halt CPU */
		CY_ASSERT(0u != 0u);
	} 
	
	current_frame_reading = S_FRAMES-1;
	
	// setup encoding
	for (int ich = 0; ich < NCHANNELS; ich++) {
		enc[ich] = lc3_setup_encoder(FRAME_US, SRATE_HZ, SRATE_HZ, malloc(sizeof(lc3_encoder_mem_t)));
		
		if (!enc[ich]) {
			printf("Encoder initialization failed");
		}
	}
	
    for(;;) {
		Cy_BLE_ProcessEvents();
		
		// only start reading the next frame if that frame is not being written by cm4 (curr being written to frame is c_fr_wr + 1)
		if (!ble_connected || current_frame_reading == current_frame_written) {
			continue;
		}
		current_frame_reading = (current_frame_reading + 1) % S_FRAMES;
		
		printf("sending frame\n");
		
		// encode frame
		uint8_t* out_ptr = tx_buffer;
		for (int ich = 0; ich < NCHANNELS; ich++) {
			int frame_bytes = BLOCK_SIZE / NCHANNELS
				+ (ich < BLOCK_SIZE % NCHANNELS);
			
			// channels interleaved!
			lc3_encode(enc[ich], LC3_PCM_FORMAT_S16,
			&shared_buffer[current_frame_reading*PCM_SBYTES*FRAME_SAMPLES] + ich * PCM_SBYTES,
			NCHANNELS, frame_bytes, out_ptr);
		
			out_ptr += frame_bytes;
		}
		
		notificationPacket.handleValPair.value.val = tx_buffer;
		
		if ((Cy_BLE_GATT_GetBusyStatus(appConnHandle.attId) == CY_BLE_STACK_STATE_FREE) && (Cy_BLE_GetConnectionState(appConnHandle) >= CY_BLE_CONN_STATE_CONNECTED) /*&& (Cy_BLE_IsDevicePaired(&appConnHandle) == true)*/) {
			printf("notification\n");
			
    		apiResult = Cy_BLE_GATTS_Notification(&notificationPacket);
			if(apiResult == CY_BLE_ERROR_INVALID_PARAMETER) {
				printf("Couldn't send notification. [CY_BLE_ERROR_INVALID_PARAMETER]\n");
				continue;
			} else if(apiResult != CY_BLE_SUCCESS) {
				printf("Attrhandle = 0x  Cy_BLE_GATTS_Notification API Error: 0x\n");
				continue;
			}
			
			
		}
    }
}

void ble_callback(uint32_t event, void * eventParam) {
//	cy_stc_ble_gatts_write_cmd_req_param_t *writeReqParameter;
	
	switch (event) {
	case CY_BLE_EVT_STACK_ON:
        printf("Start advertising\n");
        Cy_BLE_GAPP_StartAdvertisement(CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX);
		break;
		
    case CY_BLE_EVT_GAP_DEVICE_DISCONNECTED:
        printf("Device disconnected; start advertising\n");
		ble_connected = false;
        Cy_BLE_GAPP_StartAdvertisement(CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX);
        break;

    case CY_BLE_EVT_GATT_CONNECT_IND:
		{
			appConnHandle = *(cy_stc_ble_conn_handle_t *) eventParam;
			
		notificationPacket.connHandle = appConnHandle;
        notificationPacket.handleValPair.attrHandle = cy_ble_customConfig.customs[0].customServInfo[0].customServCharHandle;
//		notificationPacket.handleValPair.value.val = tx_buffer;
        notificationPacket.handleValPair.value.len = NOTIFICATION_PKT_SIZE;
        printf("Device connected!\n");
		
		Cy_BLE_GetPhy(appConnHandle.bdHandle);    
		
		// wake up CM4
//		ble_connected = true;
//		__SEV();
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
            if(apiResult != CY_BLE_SUCCESS) {
                printf("Failed to set PHY..[bdHandle 0x%02X] : 0x%4x\r\n");
            } else {
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
            if(param->status == SUCCESS) {
                printf("SET PHY updated to 2 Mbps\r\n");
                Cy_BLE_GetPhy(appConnHandle.bdHandle);
				
				// wake up CM4
		ble_connected = true;
		__SEV();
            } else {
                printf("SET PHY Could not update to 2 Mbps\r\n");
                Cy_BLE_GetPhy(appConnHandle.bdHandle);
            }
            break;
        }

    default:
		{
			char str[50];
            snprintf(str, 50, "event: %X\n", (int) event);
//			printf(str);
		}
        break;
	}
}

/* [] END OF FILE */
