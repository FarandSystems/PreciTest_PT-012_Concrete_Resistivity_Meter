

#include "Flash.h"

uint8_t recordNumber;
uint8_t projectNumber = 0;
uint8_t projectRecordNum[4];
uint8_t data_Buffer[4];
uint32_t device_ID;
uint32_t projectRecordNumber;
uint32_t dataBuffer;
uint32_t flash_Data [FLASH_DATA_BUFFER_SIZE] __attribute__((at(FLASH_DATA_BASE_ADDRESS)));
uint8_t flash_Buff [SEND_FLASH_DATA_BUFFER_SIZE] __attribute__((at(SEND_FLASH_DATA_BASE_ADDRESS)));

void SaveParameter(uint16_t parameterIndex, uint32_t param);
uint32_t ReadParameter(uint16_t parameterIndex);
void Save_Device_ID(uint8_t year, uint8_t SN);
void Save_Record_and_Project_Number(void);
void Send_Device_ID_To_PC(void);
void Send_Project_Data_To_PC(uint8_t projNumb);
void Erase_one_Project_On_Flash(uint8_t projNumb);
void Erase_All_Projects_On_Flash(void);
void Force_RecordNum0_And_ProjectNum1(void);

uint32_t ReadParameter(uint16_t parameterIndex)
{
	__IO uint32_t data32;

	data32 = *(__IO uint32_t *)(ADDR_FLASH_SECTOR_4 + 4 * parameterIndex);
	
	return data32;	
}


void SaveParameter(uint16_t parameterIndex, uint32_t paramFloat) 
{
	//Read All Words, Change the Intended Word, Erase All Sector, Write again All ParamWords
	int i;
  uint32_t paramWords[626];
	
	 /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();   

  /* Clear pending flags (if any) */  
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR );	
	
	//Read FlashBytes to ParamByte Array (626 Word 25x25)
	for (i = 0 ; i < 626 ; i++)
	{
		paramWords[i] = ReadParameter(i);
	}	
	
	// Change ParamWord at parameterIndex
	paramWords[parameterIndex] = paramFloat; // Write one Word

	/* Erase the whole Sector*/
	FLASH_Erase_Sector(FLASH_SECTOR_4, VOLTAGE_RANGE_3);

	
	//Write All paramWords on Flash
	if (HAL_FLASH_Program(TYPEPROGRAM_WORD,ADDR_FLASH_SECTOR_4 ,paramWords[0])== HAL_OK)
	{
		for(i = 1; i < 626 ; i++)
		{
			HAL_FLASH_Program(TYPEPROGRAM_WORD,ADDR_FLASH_SECTOR_4 + 4 * i ,paramWords[i]);
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
  uint32_t paramWords[626];
	
	 /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();   

  /* Clear pending flags (if any) */  
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR );
	
	
	//Read FlashBytes to ParamWord Array 
	for (i = 0 ; i < 626 ; i++)
	{
		paramWords[i] = ReadParameter(i);
	}	
	
	//Erase one Project (25 Record)
	for(int recordNum = 0 ; recordNum < 25; recordNum++)
	{
		// Change ParamWord at parameterIndex
		paramWords [(25 * projNumb) + recordNum] = 0xFFFF;
	}

	/* Erase the whole Sector*/
	FLASH_Erase_Sector(FLASH_SECTOR_4, VOLTAGE_RANGE_3);

	
	//Write Again All 625 paramWords on Flash
	if (HAL_FLASH_Program(TYPEPROGRAM_WORD,ADDR_FLASH_SECTOR_4 ,paramWords[0])== HAL_OK)
	{
		for(i = 1; i < 626 ; i++)
		{
			HAL_FLASH_Program(TYPEPROGRAM_WORD,ADDR_FLASH_SECTOR_4 + 4 * i ,paramWords[i]);
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
  uint32_t paramWords[626];

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
	FLASH_Erase_Sector(FLASH_SECTOR_4, VOLTAGE_RANGE_3);

	
	//Write Again Project0(25 paramWords) on Flash
	if (HAL_FLASH_Program(TYPEPROGRAM_WORD,ADDR_FLASH_SECTOR_4 ,paramWords[0])== HAL_OK)
	{
		for(i = 1; i < 25 ; i++)
		{
			HAL_FLASH_Program(TYPEPROGRAM_WORD,ADDR_FLASH_SECTOR_4 + 4 * i ,paramWords[i]);			
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

#ifdef CONCRETE
	// Save Device ID
	// 4111802 (FW411, 2018, SN:02)
	device_ID = ((uint32_t)(04) << 24)+
										((uint32_t)(11) << 16)+
										((uint32_t)(year) << 8)+
										((uint32_t)(SN) << 0);
#endif	
	
#ifdef SOIL
	// 4021802 (FW402, 2018, SN:02)
	device_ID = ((uint32_t)(04) << 24)+
										((uint32_t)(02) << 16)+
										((uint32_t)(year) << 8)+
										((uint32_t)(SN) << 0);
#endif	
	
	SaveParameter(DEVICE_ID_FLASH_INDEX,device_ID);
}

void Save_Record_and_Project_Number(void) //Each time we press the Save key, one Word is saved
{
	projectRecordNumber = ReadParameter(PROJECT_RECORD_NUMBER_FLASH_INDEX);
	projectRecordNum[0] = (uint8_t)((projectRecordNumber & 0xFF000000) >> 24);  // Project Number
	projectRecordNum[1] = (uint8_t)((projectRecordNumber & 0x00FF0000) >> 16);  // Record Number

	projectNumber = projectRecordNum[0];
	
	if(electrical_Connection_Status == Connected || ((electrical_Connection_Status == Disconnected) && (hold_flag == 1)))
	{
		recordNumber = projectRecordNum[1] + 1;
	}
	else if(electrical_Connection_Status == Disconnected && hold_flag == 0)
	{
		recordNumber = projectRecordNum[1];		
	}
	if(recordNumber == 25)
	{
		recordNumber = 0;
		
		projectNumber++;
		if(projectNumber == 25) //1...24
		{
			projectNumber = 1;
		}	
		
		Erase_one_Project_On_Flash(projectNumber);
	}
	
	projectRecordNum[0] = projectNumber ;
	projectRecordNum[1] = recordNumber ;

		
	projectRecordNumber = ((uint32_t)(projectRecordNum[0]) << 24)+
												((uint32_t)(projectRecordNum[1]) << 16)+
												((uint32_t)(projectRecordNum[2]) << 8) +
												((uint32_t)(projectRecordNum[3]) << 0);
	
	SaveParameter(PROJECT_RECORD_NUMBER_FLASH_INDEX, projectRecordNumber);
}

void Force_RecordNum0_And_ProjectNum1(void)
{
	//Force Project Number to be One & Record Number to be Zero
	projectNumber = 1;
	recordNumber = 0;
	
	projectRecordNum[0] = projectNumber ;
	projectRecordNum[1] = recordNumber ;

		
	projectRecordNumber = ((uint32_t)(projectRecordNum[0]) << 24)+
												((uint32_t)(projectRecordNum[1]) << 16)+
												((uint32_t)(projectRecordNum[2]) << 8)+
												((uint32_t)(projectRecordNum[3]) << 0);
	
	SaveParameter(PROJECT_RECORD_NUMBER_FLASH_INDEX,projectRecordNumber);
}
//Each time we press the Save key, one Word (1 Record) is saved
//Each Project have 25 Param Record 
void Save_Data_On_Flash(void) //Each time we press the Save key, one Word is saved
{	
	data_Buffer[0] = (uint8_t)(resistance_Or_conductivityX10 >> 8);		
	data_Buffer[1] = (uint8_t)(resistance_Or_conductivityX10 - ((resistance_Or_conductivityX10 >> 8) << 8));			
	data_Buffer[2] = (uint8_t)(temp_Deg_Display);			


	#ifdef CONCRETE
		data_Buffer[3] = (uint8_t)((((uint8_t)probeTypeIndex) << 4)  | (((uint8_t)(resistance_conductivity_Range + 6)) & 0x0F)); //resistance_conductivity_Range can be : -6, -3, 0, 3, 6 ...> 0,3,9,12
	#endif	
		
	#ifdef SOIL
		data_Buffer[3] =  (uint8_t)(resistance_conductivity_Range + 6); //bulk_cond_unit_PrefixNumber can be : -6, -3, 0, 3, 6 ...> 0,3,9,12
	#endif
	
	if(electrical_Connection_Status == Connected || ((electrical_Connection_Status == Disconnected)&&(hold_flag == 1)))
	{
		dataBuffer =  (uint8_t)(((uint32_t)(data_Buffer[0]) << 24)+
															((uint32_t)(data_Buffer[1]) << 16)+
															((uint32_t)(data_Buffer[2]) << 8) +
															((uint32_t)(data_Buffer[3]) << 0));
		
		SaveParameter(DATA_BUFFER_FLASH_INDEX + (25 * (projectNumber)) + recordNumber ,dataBuffer);	//25 : each project has 25 record
	}	
}

// This function send one Project Parameters to PC to analysis them.
void Send_Project_Data_To_PC(uint8_t projNumb)
{
	//Read 1 Project, Project Number is received from PC
	for(uint8_t i = 0 ; i < 25; i++ )
	{
		flash_Data[i] = ReadParameter(DATA_BUFFER_FLASH_INDEX + 25 * projNumb + i);
	}
	
	for(uint8_t sample_Index = 0 ; sample_Index < 25; sample_Index++ )
	{
		flash_Buff[(sample_Index) * 8 + 0] = 0; // code
		flash_Buff[(sample_Index) * 8 + 1] = (uint8_t) ((flash_Data[(sample_Index)] &0xFF000000) >> 24);
		flash_Buff[(sample_Index) * 8 + 2] = (uint8_t) ((flash_Data[(sample_Index)] &0x00FF0000) >> 16);
		flash_Buff[(sample_Index) * 8 + 3] = (uint8_t) ((flash_Data[(sample_Index)] &0x0000FF00) >> 8 );
		flash_Buff[(sample_Index) * 8 + 4] = (uint8_t) ((flash_Data[(sample_Index)] &0x000000FF) >> 0 );
		flash_Buff[(sample_Index) * 8 + 5] = 0;
		flash_Buff[(sample_Index) * 8 + 6] = 0;
		flash_Buff[(sample_Index) * 8 + 7] = 0;
	}
	
	Send_Via_USB(flash_Buff,512);	
}

// This function send one System Settings to PC 
void Send_Device_ID_To_PC(void)
{
	uint32_t device_ID_On_Flash;
	uint8_t buff[8];	

	device_ID_On_Flash = ReadParameter(DEVICE_ID_FLASH_INDEX);

	buff[0] = 0; // code
	buff[1] = (uint8_t) ((device_ID_On_Flash &0xFF000000) >> 24);
	buff[2] = (uint8_t) ((device_ID_On_Flash &0x00FF0000) >> 16);
	buff[3] = (uint8_t) ((device_ID_On_Flash &0x0000FF00) >> 8 );
	buff[4] = (uint8_t) ((device_ID_On_Flash &0x000000FF) >> 0 );
	buff[5] = 0;
	buff[6] = 0;
	buff[7] = 0;

	
	Send_Via_USB(buff,8);	
}

