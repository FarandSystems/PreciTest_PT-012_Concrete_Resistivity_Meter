/**
  ******************************************************************************
  * @file    USB_Device/CDC_Standalone/Src/usbd_cdc_interface.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    25-June-2015
  * @brief   Source file for USBD CDC interface
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "command_RX.h"



void Service_Input_Commands(uint8_t* Rx_Buffer);



uint8_t auto_Detect_PC_Sent_Bytes[8] = DETECION_PC_SENT_BYTES
uint8_t auto_Detect_Device_Sent_Bytes[8] = DETECION_DEVICE_SENT_BYTES
			
uint8_t spikeLess_flag = 0;


uint8_t Capture_Data_Pair_Select = 0;



void Service_Input_Commands(uint8_t* Rx_Buffer)
{	
	uint8_t sequenceDetected = 1;
	int i = 0;

	switch ( Rx_Buffer[0])
	{
		// Serial Port Autodetection ////////////////////////////////////////////////////////////////////////////////////
		case 0x00:
			for(i = 0; i < 8; i++)
			{
				if(Rx_Buffer[i] != auto_Detect_PC_Sent_Bytes[i])
				{
					sequenceDetected = 0;
				}
			}
			
			if(sequenceDetected == 1)
			{
				Send_Via_USB(auto_Detect_Device_Sent_Bytes , 8);
			}
		
			break;	
		
		// Commands ////////////////////////////////////////////////////////////////////////////////////	
		case 0x01:			
		  // Send flash data for project No = Rx_Buffer[1];
			Send_Project_Data_To_PC(Rx_Buffer[1]);
 			break;
		
		case 0x02:				
			Send_Device_ID_To_PC();
 			break;	
		
		case 0x03:
			Erase_All_Projects_On_Flash();
 			break;
		
		case 0x04:	
			// Save Device ID
			Save_Device_ID(Rx_Buffer[1],Rx_Buffer[2]); // production year: (e.g. 18 for 2018) , SN:(0 to 99)
			//sprintf(panel[HELP].Text, "%d, %d", Rx_Buffer[1],Rx_Buffer[2]);
			//Save_Device_ID(18,3);		
 			break;
		
		case 0x05: // Start Sending Captured Data to PC			
			sendFlag = 1; //Setting this flag enables sending the captured data via USB
			if(Rx_Buffer[1] == 0x03)
			{
				spikeLess_flag = 1;
			}
			
			// Selecting which pair of Raw_Captures would send to PC
			Capture_Data_Pair_Select = Rx_Buffer[2];
			
 			break;
		
		case 0x06: // Stop Sending Captured Data to PC			
			sendFlag = 0; //Clearing this flag enables sending the captured data via USB
 			break;	
		
		case 0x07:// Select Operating Range
			switch(Rx_Buffer[1])
			{
				case 0:
					range_ForcingMode = AutoRange;
					break;
				case 1:
					range_ForcingMode = Force_MiddleRange;					
					break;
				case 2:
					range_ForcingMode = Force_HighRange;					
					break;
				case 3:
					range_ForcingMode = Force_LowRange;					
					break;					
			}			
		  Restart_Output_Filter();
 			break;	
	}			
}
	

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
