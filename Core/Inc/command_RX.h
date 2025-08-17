/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __command_RX_H
#define __command_RX_H
#ifdef __cplusplus
 extern "C" {
#endif


#include "main.h"


extern void Send_Via_USB(uint8_t* Buf, uint32_t Len);
extern void Code_0x1A(uint8_t* Buf, uint32_t Len);
extern void Farand_USB_Tx (uint8_t* Buf, uint32_t Len);
extern uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint64_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks);


extern uint8_t Capture_Data_Pair_Select;
	 
	 
#ifdef __cplusplus
}
#endif
#endif
