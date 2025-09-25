

#include "Flash.h"

uint8_t recordNumber;
uint8_t projectNumber = 0;
uint8_t latestProjectInfo[16];
uint8_t Record_Buffer[16];
uint32_t device_ID;
uint32_t projectRecordNumber;
uint32_t record_Data_Word;
uint32_t flash_Data [FLASH_DATA_BUFFER_SIZE] __attribute__((at(FLASH_DATA_BASE_ADDRESS)));
uint8_t tx_Buff [SEND_FLASH_DATA_BUFFER_SIZE] __attribute__((at(SEND_FLASH_DATA_BASE_ADDRESS)));


static uint32_t g_paramWords[WORDS_TOTAL];

uint8_t Save_In_Four_Words(uint32_t wordIndex, const uint8_t *parameter_Array);
uint32_t ReadParameter(uint32_t wordIndex);
void Save_Device_ID(uint8_t year, uint8_t SN);
void Update_Project_Info(void);
void Send_Device_ID_To_PC(void);
void Send_Project_Data_To_PC(uint8_t projNumb);
uint8_t Erase_one_Project_On_Flash(uint8_t projNumb);
uint8_t Erase_All_Projects_On_Flash(void);
void Force_RecordNum0_And_ProjectNum1(void);
void App_InitLatestInfo(void);

uint32_t ReadParameter(uint32_t wordIndex)
{
    const uint32_t *addr = (const uint32_t *)(FLASH_BASE_ADDR_SECTOR7 + 4u * wordIndex);
    return *addr;
}

/* ====== Utilities ====== */
static void Flash_ClearErrors(void)
{
    uint32_t mask = 0;
    mask |= FLASH_FLAG_EOP;
	#ifdef FLASH_FLAG_OPERR
			mask |= FLASH_FLAG_OPERR;
	#endif
	#ifdef FLASH_FLAG_WRPERR
			mask |= FLASH_FLAG_WRPERR;
	#endif
	#ifdef FLASH_FLAG_PGAERR
			mask |= FLASH_FLAG_PGAERR;
	#endif
	#ifdef FLASH_FLAG_PGPERR
			mask |= FLASH_FLAG_PGPERR;
	#endif
	#ifdef FLASH_FLAG_PGSERR
			mask |= FLASH_FLAG_PGSERR;
	#endif
	#ifdef FLASH_FLAG_RDERR
			mask |= FLASH_FLAG_RDERR;
	#endif
			__HAL_FLASH_CLEAR_FLAG(mask);
}

static void Flash_CacheBarrier(void)
{
	#ifdef SCB_CleanInvalidateDCache
			SCB_CleanInvalidateDCache();
	#endif
	#ifdef SCB_InvalidateICache
			SCB_InvalidateICache();
	#endif
}

static void Flash_SnapshotSector(void)
{
    uint32_t i;
    for (i = 0; i < WORDS_TOTAL; i++) {
        const uint32_t *src = (const uint32_t *)(FLASH_BASE_ADDR_SECTOR7 + 4u*i);
        g_paramWords[i] = *src;
    }
}

/* Program whole RAM image back to flash */
static uint8_t Flash_ProgramWholeImage(void)
{
    uint32_t i;
    HAL_StatusTypeDef st;
    for (i = 0; i < WORDS_TOTAL; i++) {
        uint32_t addr = FLASH_BASE_ADDR_SECTOR7 + 4u*i;
        st = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, g_paramWords[i]);
        if (st != HAL_OK) return 0;
    }
    return 1;
}

static uint8_t Flash_EraseSector(void)
{
    FLASH_EraseInitTypeDef EraseInit;
    uint32_t SectorError = 0xFFFFFFFFu;

    EraseInit.TypeErase    = FLASH_TYPEERASE_SECTORS;
#ifdef FLASH_VOLTAGE_RANGE_3
    EraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;
#else
    EraseInit.VoltageRange = VOLTAGE_RANGE_3;
#endif
    EraseInit.Sector       = FLASH_SECTOR_TO_USE;
    EraseInit.NbSectors    = 1;

    if (HAL_FLASHEx_Erase(&EraseInit, &SectorError) != HAL_OK) return 0;
    if (SectorError != 0xFFFFFFFFu) return 0;
    return 1;
}

/* Word-index helpers (DATA_BUFFER_FLASH_INDEX is the word index of Project 0 start) */
static inline uint32_t ProjectBaseWordIndex(uint32_t projN)  /* projN: 0..24 */
{
    return (uint32_t)DATA_BUFFER_FLASH_INDEX + projN * WORDS_PER_PROJECT;
}

static inline uint32_t RecordWordIndex(uint32_t projN, uint32_t recN) /* recN: 0..24 */
{
    return ProjectBaseWordIndex(projN) + recN * WORDS_PER_RECORD;
}

/* Clamp helpers */
static inline uint32_t clampu32(uint32_t v, uint32_t lo, uint32_t hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}


/* Returns 1 on success, 0 on failure. Writes 4 words starting at wordIndex. */
uint8_t Save_In_Four_Words(uint32_t wordIndex, const uint8_t *parameter_Array)
{
    uint32_t i, k;
    HAL_StatusTypeDef st;
    uint32_t SectorError = 0xFFFFFFFFu;
    FLASH_EraseInitTypeDef EraseInit;

    if (parameter_Array == 0) return 0;
    if (wordIndex + 4u > WORDS_TOTAL) return 0;

    /* Snapshot sector into RAM */
    for (i = 0; i < WORDS_TOTAL; i++) {
        const uint32_t *src = (const uint32_t *)(ADDR_FLASH_SECTOR_7 + i * 4u);
        g_paramWords[i] = *src;
    }

    /* Patch 4 words from 16 bytes (big-endian) */
    for (k = 0; k < 4u; k++) {
        uint32_t w = ((uint32_t)parameter_Array[4u*k + 0u] << 24) |
                     ((uint32_t)parameter_Array[4u*k + 1u] << 16) |
                     ((uint32_t)parameter_Array[4u*k + 2u] <<  8) |
                     ((uint32_t)parameter_Array[4u*k + 3u] <<  0);
        g_paramWords[wordIndex + k] = w;
    }

    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR  |
                           FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR |
                           FLASH_FLAG_PGPERR
#ifdef FLASH_FLAG_RDERR
                           | FLASH_FLAG_RDERR
#endif
                           );

    EraseInit.TypeErase    = FLASH_TYPEERASE_SECTORS;
    EraseInit.VoltageRange = VOLTAGE_RANGE_3;  /* 2.7–3.6 V */
    EraseInit.Sector       = FLASH_SECTOR_TO_USE;
    EraseInit.NbSectors    = 1;

    __disable_irq();
    st = HAL_FLASHEx_Erase(&EraseInit, &SectorError);
    if (st != HAL_OK || SectorError != 0xFFFFFFFFu) {
        __enable_irq();
        HAL_FLASH_Lock();
        return 0;
    }

    for (i = 0; i < WORDS_TOTAL; i++) {
        uint32_t addr = ADDR_FLASH_SECTOR_7 + i * 4u;
        st = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, g_paramWords[i]);
        if (st != HAL_OK) {
            __enable_irq();
            HAL_FLASH_Lock();
            return 0;
        }
    }
    __enable_irq();

    Flash_CacheBarrier();
    HAL_FLASH_Lock();
    return 1;
}

// Read All Projects
// Force the Intended Project to be 0
// Erase the whole Sectore4
// Write Again All Projects on Flash
uint8_t Erase_one_Project_On_Flash(uint8_t projNumb)
{
    uint32_t start, i;

    /* bounds: if you number 1..24 and keep 0 for settings, adjust here */
    if (projNumb >= PROJECTS_COUNT) return 0;

    HAL_FLASH_Unlock();
    Flash_ClearErrors();

    /* Read current sector image */
    Flash_SnapshotSector();

    /* Compute project start index (in words) */
    start = BASE_OFFSET_WORDS + (uint32_t)projNumb * WORDS_PER_PROJECT;
    if (start + WORDS_PER_PROJECT > WORDS_TOTAL) { HAL_FLASH_Lock(); return 0; }

    /* Wipe that project’s words */
    for (i = 0; i < WORDS_PER_PROJECT; i++) {
        g_paramWords[start + i] = 0xFFFFFFFFu;
    }

    /* Erase & re-program */
    __disable_irq();
    if (!Flash_EraseSector()) { __enable_irq(); HAL_FLASH_Lock(); return 0; }
    if (!Flash_ProgramWholeImage()) { __enable_irq(); HAL_FLASH_Lock(); return 0; }
    __enable_irq();

    Flash_CacheBarrier();
    HAL_FLASH_Lock();
    return 1;
}

//Read the ProjectNumber0(Device Settings)
//Erase the whole Sector4
//Write Again the project Number 0 
uint8_t Erase_All_Projects_On_Flash(void)
{
    uint32_t i;

    HAL_FLASH_Unlock();
    Flash_ClearErrors();

    /* Preserve only the first KEEP_HEAD_WORDS words; others -> 0xFFFFFFFF */
    Flash_SnapshotSector();

    /* Fill the tail with erased value */
    for (i = KEEP_HEAD_WORDS; i < WORDS_TOTAL; i++) {
        g_paramWords[i] = 0xFFFFFFFFu;
    }

    /* Erase & re-program */
    __disable_irq();
    if (!Flash_EraseSector()) { __enable_irq(); HAL_FLASH_Lock(); return 0; }
    if (!Flash_ProgramWholeImage()) { __enable_irq(); HAL_FLASH_Lock(); return 0; }
    __enable_irq();

    Flash_CacheBarrier();
    HAL_FLASH_Lock();
    return 1;
}

void Save_Device_ID(uint8_t year, uint8_t SN)
{
	// Save Device ID
	// 4111802 (PT012, 2018, SN:02)
//	device_ID = ((uint32_t)(0) << 24)+
//										((uint32_t)(12) << 16)+
//										((uint32_t)(year) << 8)+
//										((uint32_t)(SN) << 0);
	
	uint8_t four_words[16];
	four_words[0] = 0;
	four_words[1] = 12;
	four_words[2] = year;
	four_words[3] = SN;
	
	Save_In_Four_Words(DEVICE_ID_FLASH_INDEX, four_words);
}

static uint8_t crc8_07(const uint8_t *p, uint32_t n)
{
    uint8_t crc = 0;
    for (uint32_t i = 0; i < n; i++) {
        crc ^= p[i];
        for (uint8_t b = 0; b < 8; b++) {
            crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x07) : (uint8_t)(crc << 1);
        }
    }
    return crc;
}

static void Save_LatestInfo_4Words(uint8_t project, uint8_t record)
{
    uint8_t blk[16] = {0};
    blk[0] = project;                 /* proj */
    blk[1] = record;                  /* rec  */
    blk[2] = 1;                       /* version */
    /* blk[3] = 0 */
    blk[4] = 0x4C; blk[5] = 0x50; blk[6] = 0x49; blk[7] = 0x46;  /* 'L','P','I','F' */
    /* blk[8..11] = 0 */
    blk[12] = crc8_07(blk, 12);       /* CRC of first 12 bytes */
    /* blk[13..15] = 0 */

    Save_In_Four_Words(LATEST_PROJECT_INFO_FLASH_INDEX, blk);
}

static uint8_t Load_LatestInfo_4Words(uint8_t *project, uint8_t *record)
{
    /* Read first word (bytes 0..3) */
    uint32_t w0 = ReadParameter(LATEST_PROJECT_INFO_FLASH_INDEX);
    /* Read next words to rebuild the 16B block for CRC/magic check */
    uint32_t w1 = ReadParameter(LATEST_PROJECT_INFO_FLASH_INDEX + 1u);
    uint32_t w2 = ReadParameter(LATEST_PROJECT_INFO_FLASH_INDEX + 2u);
    uint32_t w3 = ReadParameter(LATEST_PROJECT_INFO_FLASH_INDEX + 3u);

    uint8_t blk[16];
    blk[0]  = (uint8_t)((w0 >> 24) & 0xFF);
    blk[1]  = (uint8_t)((w0 >> 16) & 0xFF);
    blk[2]  = (uint8_t)((w0 >>  8) & 0xFF);
    blk[3]  = (uint8_t)((w0 >>  0) & 0xFF);
    blk[4]  = (uint8_t)((w1 >> 24) & 0xFF);
    blk[5]  = (uint8_t)((w1 >> 16) & 0xFF);
    blk[6]  = (uint8_t)((w1 >>  8) & 0xFF);
    blk[7]  = (uint8_t)((w1 >>  0) & 0xFF);
    blk[8]  = (uint8_t)((w2 >> 24) & 0xFF);
    blk[9]  = (uint8_t)((w2 >> 16) & 0xFF);
    blk[10] = (uint8_t)((w2 >>  8) & 0xFF);
    blk[11] = (uint8_t)((w2 >>  0) & 0xFF);
    blk[12] = (uint8_t)((w3 >> 24) & 0xFF);
    blk[13] = (uint8_t)((w3 >> 16) & 0xFF);
    blk[14] = (uint8_t)((w3 >>  8) & 0xFF);
    blk[15] = (uint8_t)((w3 >>  0) & 0xFF);

    /* Quick invalid cases: fully erased or bad magic */
    if ((w0 == 0xFFFFFFFFu && w1 == 0xFFFFFFFFu && w2 == 0xFFFFFFFFu && w3 == 0xFFFFFFFFu) ||
        !(blk[4] == 0x4C && blk[5] == 0x50 && blk[6] == 0x49 && blk[7] == 0x46)) {
        return 0; /* invalid */
    }

    /* CRC check */
    if (blk[12] != crc8_07(blk, 12)) {
        return 0; /* invalid */
    }
		    *project = blk[0];
    *record  = blk[1];
    return 1; /* valid */
}

void App_InitLatestInfo(void)
{
    uint8_t proj, rec;
    if (!Load_LatestInfo_4Words(&proj, &rec) ||
        proj < 1 || proj > 24 || rec > 24)
    {
        /* Seed sane defaults and store */
        proj = 1; rec = 0;
        Save_LatestInfo_4Words(proj, rec);
    }

    projectNumber = proj;
    recordNumber  = rec;
}

void Update_Project_Info(void) //Each time we press the Save key, one Word is saved
{
	projectRecordNumber = ReadParameter(LATEST_PROJECT_INFO_FLASH_INDEX);
	latestProjectInfo[0] = (uint8_t)((projectRecordNumber & 0xFF000000) >> 24);  // Project Number
	latestProjectInfo[1] = (uint8_t)((projectRecordNumber & 0x00FF0000) >> 16);  // Record Number

	projectNumber = latestProjectInfo[0];
	
	// Calculate current record number (Each record is a measurement).
	if(electrical_Connection_Status == Connected || ((electrical_Connection_Status == Disconnected) && (hold_flag == 1)))
	{
		recordNumber = latestProjectInfo[1] + 1; // Possible Bug
	}
	else if(electrical_Connection_Status == Disconnected && hold_flag == 0)
	{
		recordNumber = latestProjectInfo[1];		
	}
	
	// Bound number of records to 24 (1...24)
	if(recordNumber == 25)
	{
		recordNumber = 0;
		
		projectNumber++; // Automatically increase project number.
		if(projectNumber == 25) //Bound projects count to 24 (1...24)
		{
			projectNumber = 1;
			
			
		}
		
		Erase_one_Project_On_Flash(projectNumber); // Erase project memory space to overwrite regardless of whether the project is recorded previously or not.
	}
	// Update new Project and record number
	latestProjectInfo[0] = projectNumber ;
	latestProjectInfo[1] = recordNumber ;

//	// Write latest project info for future refrence.
//	projectRecordNumber = ((uint32_t)(latestProjectInfo[0]) << 24)+
//												((uint32_t)(latestProjectInfo[1]) << 16)+
//												((uint32_t)(latestProjectInfo[2]) << 8) +
//												((uint32_t)(latestProjectInfo[3]) << 0);
//	
	Save_In_Four_Words(LATEST_PROJECT_INFO_FLASH_INDEX, latestProjectInfo);
}

void Force_RecordNum0_And_ProjectNum1(void)
{
	//Force Project Number to be One & Record Number to be Zero
	projectNumber = 1;
	recordNumber = 0;
	
	latestProjectInfo[0] = projectNumber ;
	latestProjectInfo[1] = recordNumber ;

		
//	projectRecordNumber = ((uint32_t)(latestProjectInfo[0]) << 24)+
//												((uint32_t)(latestProjectInfo[1]) << 16)+
//												((uint32_t)(latestProjectInfo[2]) << 8)+
//												((uint32_t)(latestProjectInfo[3]) << 0);
	
	Save_In_Four_Words(LATEST_PROJECT_INFO_FLASH_INDEX, latestProjectInfo);
}
//Each time we press the Save key, one Word (1 Record) is saved
//Each Project includes 25 Records (Measurements)
void Save_Record_Data_On_Flash(void) // Each press saves one 16-byte record
{
    /* Build record payload (16 bytes = 4 words) */
    Record_Buffer[0]  = (uint8_t)((resistance_Or_conductivityX10 >> 8) & 0xFF);
    Record_Buffer[1]  = (uint8_t)((resistance_Or_conductivityX10 >> 0) & 0xFF);
    Record_Buffer[2]  = (uint8_t)(resistance_conductivity_Range + 6); /* -6,-3,0,3,6 -> 0,3,6,9,12? (your mapping) */
    Record_Buffer[3]  = (uint8_t)(probeTypeIndex);

    Record_Buffer[4]  = (uint8_t)Minute;
    Record_Buffer[5]  = (uint8_t)Hour;
    Record_Buffer[6]  = 0x00;
    Record_Buffer[7]  = 0x00;

    Record_Buffer[8]  = (uint8_t)Day;
    Record_Buffer[9]  = (uint8_t)Month;
    Record_Buffer[10] = (uint8_t)Year;
    Record_Buffer[11] = 0x00;

    Record_Buffer[12] = (uint8_t)temp_Deg_Display;
    Record_Buffer[13] = 0x00;
    Record_Buffer[14] = 0x00;
    Record_Buffer[15] = 0x00;

    /* Only save when connected or in hold mode */
    if (electrical_Connection_Status == Connected ||
        (electrical_Connection_Status == Disconnected && hold_flag == 1))
    {
        /* UI uses 1..24; project 0 is settings/header. Map 1..24 -> 1..24, clamp inside range. */
        uint32_t proj = clampu32((uint32_t)projectNumber, 1u, (PROJECTS_COUNT - 1u)); /* 1..24 */
        uint32_t rec  = clampu32((uint32_t)recordNumber, 0u, (RECORDS_PER_PROJECT - 1u)); /* 0..24 */

        uint32_t wordIndex = RecordWordIndex(proj, rec); /* DATA_BUFFER_FLASH_INDEX + proj*100 + rec*4 */

        /* Program 4 words (will snapshot, erase, reprogram whole sector safely) */
        if (!Save_In_Four_Words(wordIndex, Record_Buffer)) {
            /* Optional: show a UI error or beep pattern */
            Alarm(EMERGENCY_BEEP, 1, 32, BEEP_ON);
        } else {
//            Alarm(SHORT_BEEP_X1, 1, 8, BEEP_ON); /* success cue */
        }
    }
}

void Send_Project_Data_To_PC(uint8_t project_Number)
{
    uint32_t base = ProjectBaseWordIndex(project_Number);
    uint32_t i;

    /* Read 1 project = 100 words */
    for (i = 0; i < WORDS_PER_PROJECT; i++) {
        flash_Data[i] = ReadParameter(base + i);
    }

    /* Clear the 512B TX buffer (you send 512) */
    for (i = 0; i < sizeof(tx_Buff); i++) {
        tx_Buff[i] = 0x00;
    }

    /* === KEEPING YOUR TX PATTERN AS IS ===
       (same fields and positions you used)
       Each record -> 16 bytes; 25 records -> first 400 bytes filled
    */
    for (uint8_t record_Index = 0; record_Index < RECORDS_PER_PROJECT; record_Index++)
    {
        uint32_t w0 = flash_Data[4u * record_Index + 0u];
        uint32_t w1 = flash_Data[4u * record_Index + 1u];
        uint32_t w2 = flash_Data[4u * record_Index + 2u];
        uint32_t w3 = flash_Data[4u * record_Index + 3u];

        uint32_t o = (uint32_t)record_Index * 16u;
			
			    /* --- ERASED RECORD HANDLING (all words 0xFFFFFFFF) --- */
				if (w0 == 0xFFFFFFFFu && w1 == 0xFFFFFFFFu &&
						w2 == 0xFFFFFFFFu && w3 == 0xFFFFFFFFu)
				{
						/* Keep your TX pattern: we already cleared tx_Buff to 0x00,
							 so leave these 16 bytes as zeros to indicate "empty record". */
						/* If you ever want 0xFF instead, do: memset(&tx_Buff[o], 0xFF, 16); */
						continue;
				}

        tx_Buff[o + 0]  = 0;                                           /* code 0x1A (you kept 0) */
        tx_Buff[o + 1]  = (uint8_t)((w0 & 0xFF000000u) >> 24);         /* Probe Type */
        tx_Buff[o + 2]  = (uint8_t)((w0 & 0x00FF0000u) >> 16);         /* Range (0..12) */
        tx_Buff[o + 3]  = (uint8_t)((w0 & 0x0000FF00u) >> 8 );         /* Resist/Cond ×10 HB */
        tx_Buff[o + 4]  = (uint8_t)((w0 & 0x000000FFu) >> 0 );         /* Resist/Cond ×10 LB */

        tx_Buff[o + 5]  = (uint8_t)((w1 & 0xFF000000u) >> 24);         /* Reserved */
        tx_Buff[o + 6]  = (uint8_t)((w1 & 0x00FF0000u) >> 16);         /* Reserved */
        tx_Buff[o + 7]  = (uint8_t)((w1 & 0x0000FF00u) >> 8 );         /* Time (hh) */

        tx_Buff[o + 8]  = 0;                                           /* code 0x1A (you kept 0) */
        tx_Buff[o + 9]  = (uint8_t)((w1 & 0x000000FFu) >> 0 );         /* Time (mm) */
        tx_Buff[o + 10] = (uint8_t)((w2 & 0xFF000000u) >> 24);         /* Reserved */
        tx_Buff[o + 11] = (uint8_t)((w2 & 0x00FF0000u) >> 16);         /* Date (YY) */
        tx_Buff[o + 12] = (uint8_t)((w2 & 0x0000FF00u) >> 8 );         /* Date (MM) */
        tx_Buff[o + 13] = (uint8_t)((w2 & 0x000000FFu) >> 0 );         /* Date (dd) */
        tx_Buff[o + 14] = (uint8_t)((w3 & 0xFF000000u) >> 24 );         /* Temp */
        tx_Buff[o + 15] = (uint8_t)((w3 & 0x000000FFu) >> 0 );         /* Reserved */
    }

    /* You were sending 512; keep it */
    Send_Via_USB(tx_Buff, 512);
}
// This function send one System Settings to PC 
void Send_Device_ID_To_PC(void)
{
    static uint8_t buff[8];   // persist after function returns

    uint32_t device_ID_On_Flash = ReadParameter(DEVICE_ID_FLASH_INDEX);

    buff[0] = 0; // code
    buff[1] = (uint8_t)((device_ID_On_Flash >> 24) & 0xFF);
    buff[2] = (uint8_t)((device_ID_On_Flash >> 16) & 0xFF);
    buff[3] = (uint8_t)((device_ID_On_Flash >>  8) & 0xFF);
    buff[4] = (uint8_t)((device_ID_On_Flash >>  0) & 0xFF);
    buff[5] = 0;
    buff[6] = 0;
    buff[7] = 0;

    Send_Via_USB(buff, 8);
}

