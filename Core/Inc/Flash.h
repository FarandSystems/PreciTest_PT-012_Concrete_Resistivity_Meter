/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __flash_H
#define __flash_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"



extern uint8_t Save_In_Four_Words(uint32_t wordIndex, const uint8_t *parameter_Array);
extern uint32_t ReadParameter(uint32_t wordIndex);
extern void Send_Via_USB(uint8_t* Buf, uint32_t Len);
extern void Send_Project_Data_To_PC(uint8_t projNumb);
extern void SendCapturedSignalToPC(void);
extern uint8_t Erase_one_Project_On_Flash(uint8_t projNumb);
extern uint8_t Erase_All_Projects_On_Flash(void);
extern void Update_Project_Info(void);
extern void Save_Record_Data_On_Flash(void);
extern void Save_Device_ID(uint8_t year, uint8_t SN);
extern void Send_Device_ID_To_PC(void);
extern void Force_RecordNum0_And_ProjectNum1(void);
extern void App_InitLatestInfo(void);

extern uint8_t projectNumber ;
extern uint8_t recordNumber;
extern uint8_t latestProjectInfo[16];
extern uint32_t projectRecordNumber;
extern uint32_t dataBuffer;


/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0   ((uint32_t)0x08000000) //  0 KB
#define ADDR_FLASH_SECTOR_1   ((uint32_t)0x08008000) // 32 KB
#define ADDR_FLASH_SECTOR_2   ((uint32_t)0x08010000) // 64 KB
#define ADDR_FLASH_SECTOR_3   ((uint32_t)0x08018000) // 96 KB
#define ADDR_FLASH_SECTOR_4   ((uint32_t)0x08020000) // 128 KB
#define ADDR_FLASH_SECTOR_5   ((uint32_t)0x08040000) // 256 KB
#define ADDR_FLASH_SECTOR_6   ((uint32_t)0x08080000) // 512 KB
#define ADDR_FLASH_SECTOR_7   ((uint32_t)0x080C0000) // 768 KB

/* ====== Flash layout config (ADJUST to your map) ====== */
#define FLASH_BASE_ADDR_SECTOR7   0x080C0000u     /* F746 1MB: sector 7 base */
#define FLASH_SECTOR_TO_USE       FLASH_SECTOR_7
#define WORDS_TOTAL               2504u           /* total words stored in sector */

#define PROJECTS_COUNT            25u
#define RECORDS_PER_PROJECT       25u
#define WORDS_PER_RECORD          4u
#define WORDS_PER_PROJECT         (RECORDS_PER_PROJECT * WORDS_PER_RECORD) /* 100 */

#define BASE_OFFSET_WORDS         0u   /* if you keep a header before projects, put its word count here */
#define KEEP_HEAD_WORDS           25u  /* for Erase_All_Projects: how many words to preserve (your “Project 0 / Device Settings”) */


#ifdef __cplusplus
}
#endif
#endif

