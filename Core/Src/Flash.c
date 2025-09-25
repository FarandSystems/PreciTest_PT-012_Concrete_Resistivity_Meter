

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

void Save_In_Four_Words(uint16_t flash_Address, uint8_t* parameter_Array);
uint32_t ReadParameter(uint16_t parameterIndex);
void Save_Device_ID(uint8_t year, uint8_t SN);
void Update_Project_Info(void);
void Send_Device_ID_To_PC(void);
void Send_Project_Data_To_PC(uint8_t projNumb);
void Erase_one_Project_On_Flash(uint8_t projNumb);
void Erase_All_Projects_On_Flash(void);
void Force_RecordNum0_And_ProjectNum1(void);

uint32_t ReadParameter(uint16_t parameterIndex)
{
	__IO uint32_t data32;

	data32 = *(__IO uint32_t *)(ADDR_FLASH_SECTOR_7 + 4 * parameterIndex);
	
	return data32;	
}


void Save_In_Four_Words(uint16_t flash_Address, uint8_t* parameter_Array) 
{
	//Read All Words, Change the Intended Word, Erase All Sector, Write again All ParamWords 
  uint32_t paramWords[2504]; // Totally 25 projects each has 100 words + 1 Device ID
	
	 /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();   

  /* Clear pending flags (if any) */  
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR );	
	
	//Read FlashBytes to ParamByte Array (626 Word 25x25)
	for (int i = 0 ; i < 2504 ; i++)
	{
		paramWords[i] = ReadParameter(i);
	}
	
	
	for (int word_index = 0; word_index < 4; word_index++)
	{
		// Change ParamWord at parameterIndex
		paramWords[flash_Address + word_index] = (uint32_t)((parameter_Array[4 * word_index +0] << 24) 
																											+ (parameter_Array[4 * word_index +1] << 16) 
																											+ (parameter_Array[4 * word_index +2] << 8) 
																											+ (parameter_Array[4 * word_index +3] << 0)); // Write one Word
	}


	/* Erase the whole Sector*/
	FLASH_Erase_Sector(FLASH_SECTOR_7, VOLTAGE_RANGE_3);

	
	//Write All paramWords on Flash
	if (HAL_FLASH_Program(TYPEPROGRAM_WORD,ADDR_FLASH_SECTOR_7 ,paramWords[0])== HAL_OK)
	{
		for(int i = 1; i < 2504 ; i++)
		{
			HAL_FLASH_Program(TYPEPROGRAM_WORD,ADDR_FLASH_SECTOR_7 + 4 * i ,paramWords[i]);
		}
	}
	else
	{
////		panel[SAVE].BackColor = Farand_LIGHT_TOMATO;
////		Farand_DrawPanel(&panel[SAVE]);
	}  

////////////////////////////////////////////////////////////////////////////////////////		
	 /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock(); 
}

// Read All Projects
// Force the Intended Project to be 0
// Erase the whole Sectore4
// Write Again All Projects on Flash
void Erase_one_Project_On_Flash(uint8_t projNumb)
{
	uint16_t i;
  uint32_t paramWords[2504];
	
	 /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();   

  /* Clear pending flags (if any) */  
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR );
	
	
	//Read FlashBytes to ParamWord Array 
	for (i = 0 ; i < 2504 ; i++)
	{
		paramWords[i] = ReadParameter(i);
	}	
	
	//Erase one Project (25 Record)
	for(int recordNum = 0 ; recordNum < 25; recordNum++)
	{
		// Change ParamWord at parameterIndex
		paramWords[(25 * projNumb) + recordNum] = 0xFFFFFFFFu;
	}

	/* Erase the whole Sector*/
	FLASH_Erase_Sector(FLASH_SECTOR_7, VOLTAGE_RANGE_3);

	
	//Write Again All 625 paramWords on Flash
	if (HAL_FLASH_Program(TYPEPROGRAM_WORD,ADDR_FLASH_SECTOR_7 ,paramWords[0])== HAL_OK)
	{
		for(i = 1; i < 2504 ; i++)
		{
			HAL_FLASH_Program(TYPEPROGRAM_WORD,ADDR_FLASH_SECTOR_7 + 4 * i ,paramWords[i]);
		}
	}
	else
	{
////		panel[SAVE].BackColor = Farand_LIGHT_TOMATO;
////		Farand_DrawPanel(&panel[SAVE]);
	}

////////////////////////////////////////////////////////////////////////////////////////		
	 /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock(); 
}

//Read the ProjectNumber0(Device Settings)
//Erase the whole Sector4
//Write Again the project Number 0 
void Erase_All_Projects_On_Flash(void)
{
	int i;
  uint32_t paramWords[2504];

	 /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();   

  /* Clear pending flags (if any) */  
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR );
	
	//Read FlashBytes to ParamWord Array 
	for (i = 0 ; i < 25 ; i++)
	{
		paramWords[i] = ReadParameter(i);
	}	

	/* Erase the whole Sector*/
	FLASH_Erase_Sector(FLASH_SECTOR_7, VOLTAGE_RANGE_3);

	
	//Write Again Project0(25 paramWords) on Flash
	if (HAL_FLASH_Program(TYPEPROGRAM_WORD,ADDR_FLASH_SECTOR_7 ,paramWords[0])== HAL_OK)
	{
		for(i = 1; i < 25 ; i++)
		{
			HAL_FLASH_Program(TYPEPROGRAM_WORD,ADDR_FLASH_SECTOR_7 + 4 * i ,paramWords[i]);			
		}
	}
	else
	{
////		panel[SAVE].BackColor = Farand_LIGHT_TOMATO;
////		Farand_DrawPanel(&panel[SAVE]);
	}

////////////////////////////////////////////////////////////////////////////////////////		
	 /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock(); 
	
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
void Save_Record_Data_On_Flash(void) //Each time we press the Save key, one Word is saved
{	
	Record_Buffer[0]  = (uint8_t)((resistance_Or_conductivityX10 & 0xFF00) >> 8);		
	Record_Buffer[1]  = (uint8_t)((resistance_Or_conductivityX10 & 0x00FF) >> 0);			
	Record_Buffer[2]  = (uint8_t)(resistance_conductivity_Range + 6); //resistance_conductivity_Range can be : -6, -3, 0, 3, 6 ...> 0,3,9,12	
	Record_Buffer[3]  = (uint8_t)(probeTypeIndex); //0 = 38mm 1 = 50mm 2 = Bulk
									  
	Record_Buffer[4]  = (uint8_t)(Minute);
	Record_Buffer[5]  = (uint8_t)(Hour);
	Record_Buffer[6]  = (uint8_t)(0x00); //Reserved
	Record_Buffer[7]  = (uint8_t)(0x00); //Reserved
									  
	Record_Buffer[8]  = (uint8_t)(Day);
	Record_Buffer[9]  = (uint8_t)(Month);
	Record_Buffer[10] = (uint8_t)(Year);
	Record_Buffer[11] = (uint8_t)(0x00); //Reserved
	
	Record_Buffer[12] = (uint8_t)(temp_Deg_Display);
	Record_Buffer[13] = (uint8_t)(0x00); //Reserved
	Record_Buffer[14] = (uint8_t)(0x00); //Don't Use
	Record_Buffer[15] = (uint8_t)(0x00); //Don't Use
	
	if(electrical_Connection_Status == Connected || ((electrical_Connection_Status == Disconnected)&&(hold_flag == 1)))
	{
			Save_In_Four_Words(DATA_BUFFER_FLASH_INDEX + (25 * 4 * (projectNumber)) + recordNumber * 4 , Record_Buffer);	//each project contains 25 records , each record contains 4 Words
	}	
	

}

// This function send one Project Parameters to PC to analysis them.
void Send_Project_Data_To_PC(uint8_t project_Number)
{
	//Read 1 Project, Project Number is received from PC
	for(uint8_t recordNumber = 0 ; recordNumber < 25; recordNumber++ )
	{
		for(uint8_t word_Number = 0 ; word_Number < 4; word_Number++)
		{
			flash_Data[4 * recordNumber + word_Number] = ReadParameter(DATA_BUFFER_FLASH_INDEX + (25 * 4 * (projectNumber)) + (recordNumber * 4 ) + word_Number); //each project contains 25 records , each record contains 4 Words
		}
		
	}
	
	//Clear the tx buffer
	for(uint16_t index = 0; index < sizeof(tx_Buff); index++)
	{
		tx_Buff[index] = (uint8_t)0x00;
	}
	
	
	// Fill first 14 bytes record data of 1 project
	for(uint8_t record_Index = 0 ; record_Index < 25; record_Index++ ) //16 bytes are sent for 1 record hence 25 records are sent in 25 * 16 = 400 bytes
	{
		tx_Buff[(record_Index) * 16 + 0]  = 0; // code 0x1A
		tx_Buff[(record_Index) * 16 + 1]  = (uint8_t) ((flash_Data[4 * record_Index + 0] & 0xFF000000) >> 24); // Probe Type 0 = 38mm 1 = 50mm 2 = Bulk
		tx_Buff[(record_Index) * 16 + 2]  = (uint8_t) ((flash_Data[4 * record_Index + 0] & 0x00FF0000) >> 16); // Resistence Range 0 ... 12 (-6 ... 6)
		tx_Buff[(record_Index) * 16 + 3]  = (uint8_t) ((flash_Data[4 * record_Index + 0] & 0x0000FF00) >> 8 ); // Resistence/ConductivityX10 HB
		tx_Buff[(record_Index) * 16 + 4]  = (uint8_t) ((flash_Data[4 * record_Index + 0] & 0x000000FF) >> 0 ); // Resistence/ConductivityX10 LB
		tx_Buff[(record_Index) * 16 + 5]  = (uint8_t) ((flash_Data[4 * record_Index + 1] & 0xFF000000) >> 24); // Reserved
		tx_Buff[(record_Index) * 16 + 6]  = (uint8_t) ((flash_Data[4 * record_Index + 1] & 0x00FF0000) >> 16); // Reserved
		tx_Buff[(record_Index) * 16 + 7]  = (uint8_t) ((flash_Data[4 * record_Index + 1] & 0x0000FF00) >> 8 ); // Time(hh)

		
		tx_Buff[(record_Index) * 16 + 8]  = 0; // code 0x1A
		tx_Buff[(record_Index) * 16 + 9]  = (uint8_t) ((flash_Data[4 * record_Index + 1] & 0x000000FF) >> 0 );// Time(mm)
		tx_Buff[(record_Index) * 16 + 10] = (uint8_t) ((flash_Data[4 * record_Index + 2] & 0xFF000000) >> 24); // Reserved
		tx_Buff[(record_Index) * 16 + 11] = (uint8_t) ((flash_Data[4 * record_Index + 2] & 0x00FF0000) >> 16); // Date(YY)
		tx_Buff[(record_Index) * 16 + 12] = (uint8_t) ((flash_Data[4 * record_Index + 2] & 0x0000FF00) >> 8 ); // Date(MM)
		tx_Buff[(record_Index) * 16 + 13] = (uint8_t) ((flash_Data[4 * record_Index + 2] & 0x000000FF) >> 0 ); // Date(dd)
		tx_Buff[(record_Index) * 16 + 14] = (uint8_t) ((flash_Data[4 * record_Index + 3] & 0x0000FF00) >> 8 ); // Reserved
		tx_Buff[(record_Index) * 16 + 15] = (uint8_t) ((flash_Data[4 * record_Index + 3] & 0x000000FF) >> 0 ); // Reserved
	}
	
	
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

