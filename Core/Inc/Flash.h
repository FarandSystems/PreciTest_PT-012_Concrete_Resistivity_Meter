/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __flash_H
#define __flash_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"



extern void Save_In_Four_Words(uint16_t flash_Address, uint8_t* parameter_Array) ;
extern uint32_t ReadParameter(uint16_t parameterIndex);
extern void Send_Via_USB(uint8_t* Buf, uint32_t Len);
extern void Send_Project_Data_To_PC(uint8_t projNumb);
extern void SendCapturedSignalToPC(void);
extern void Erase_one_Project_On_Flash(uint8_t projNumb);
extern void Erase_All_Projects_On_Flash(void);
extern void Update_Project_Info(void);
extern void Save_Record_Data_On_Flash(void);
extern void Save_Device_ID(uint8_t year, uint8_t SN);
extern void Send_Device_ID_To_PC(void);
extern void Force_RecordNum0_And_ProjectNum1(void);

extern uint8_t projectNumber ;
extern uint8_t recordNumber;
extern uint8_t latestProjectInfo[16];
extern uint32_t projectRecordNumber;
extern uint32_t dataBuffer;


/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base address of Sector 0, 32 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08008000) /* Base address of Sector 1, 32 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08010000) /* Base address of Sector 2, 32 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x08018000) /* Base address of Sector 3, 32 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08020000) /* Base address of Sector 4, 128 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08040000) /* Base address of Sector 5, 256 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08070000) /* Base address of Sector 6, 256 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x080A0000) /* Base address of Sector 7, 256 Kbytes */


#ifdef __cplusplus
}
#endif
#endif

