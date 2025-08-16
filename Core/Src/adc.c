/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
void SendCapturedSignalToPC(void);
void EraseADCBuffer(void);

extern uint8_t systemMode;
extern uint8_t SPI_RxBuffer[3];
extern uint8_t spikeLess_flag;

uint8_t sendData[SEND_DATA_BUFFER_SIZE] __attribute__((at(SEND_DATA_BASE_ADDRESS)));
int32_t	signal_Captured [CAPTURE_COUNT][256] __attribute__((at(ADC_DATA_BASE_ADDRESS)));
extern float signal_SubSampled_SpikeLess [CAPTURE_COUNT][256]__attribute__((at(CORRECTED_VALUE_DATA_BASE_ADDRESS)));
extern float signal_SubSampled_LPF [CAPTURE_COUNT][256]__attribute__((at(FILTERED_VALUE_DATA_BASE_ADDRESS)));
extern float signal_Median[256]__attribute__((at(FILTERED_MEDIAN_DATA_BASE_ADDRESS)));
extern float signal_LPF_And_Cut[256]__attribute__((at(FILTERED_MEDIAN_LOWPASS_DATA_BASE_ADDRESS)));

uint16_t captureIndex;
uint16_t sampleIndex;

__IO uint16_t ADC_ConvertedValue[2];

// ADC_ConvertedValue[0] -> Internal Battery Sense , internal Li-Polymer (3.7V) sense
// ADC_ConvertedValue[1] -> Input Power Sense, Voltage sopplyed to Switching Power Supply (3...5 V)
// ADC_ConvertedValue[2] -> Input Charger Sense, Inpu Jack Charger
// ADC_ConvertedValue[3] -> Temperature Sensor
/* USER CODE END 0 */

ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc3;

/* ADC3 init function */
void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc3.Init.ContinuousConvMode = ENABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_TRGO;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 2;
  hadc3.Init.DMAContinuousRequests = ENABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC3_MspInit 0 */

  /* USER CODE END ADC3_MspInit 0 */
    /* ADC3 clock enable */
    __HAL_RCC_ADC3_CLK_ENABLE();

    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**ADC3 GPIO Configuration
    PF10     ------> ADC3_IN8
    PF9     ------> ADC3_IN7
    */
    GPIO_InitStruct.Pin = Temperature_Sense_Pin|Battery_Sense_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* ADC3 DMA Init */
    /* ADC3 Init */
    hdma_adc3.Instance = DMA2_Stream1;
    hdma_adc3.Init.Channel = DMA_CHANNEL_2;
    hdma_adc3.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc3.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc3.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc3.Init.Mode = DMA_CIRCULAR;
    hdma_adc3.Init.Priority = DMA_PRIORITY_LOW;
    hdma_adc3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc3) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc3);

  /* USER CODE BEGIN ADC3_MspInit 1 */

  /* USER CODE END ADC3_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC3_MspDeInit 0 */

  /* USER CODE END ADC3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC3_CLK_DISABLE();

    /**ADC3 GPIO Configuration
    PF10     ------> ADC3_IN8
    PF9     ------> ADC3_IN7
    */
    HAL_GPIO_DeInit(GPIOF, Temperature_Sense_Pin|Battery_Sense_Pin);

    /* ADC3 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
  /* USER CODE BEGIN ADC3_MspDeInit 1 */

  /* USER CODE END ADC3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void SendCapturedSignalToPC(void)
{
	uint16_t capture_Index = 0;
	uint16_t sample_Index = 0;
	uint16_t packetNumber = 0;
	uint16_t waveformIndex = 0;
	uint16_t maximum_capture_index;
	
	#ifdef CONCRETE
		maximum_capture_index = CAPTURE_COUNT;
	#endif
	#ifdef SOIL
	if(CAPTURE_COUNT > 32)
	{
		maximum_capture_index = CAPTURE_COUNT/4;
	}
	else
	{
		maximum_capture_index = CAPTURE_COUNT/2;
	}
	#endif

		
	//Pack Captured WaveForms into 8Bytes Packet Format in a total 32(16)x1024 bytes buffer
	
		/* each Sample is fragmented to 3 bytes & 2 Samples (6Bytes) are packed in 1 packet(8Bytes)
	8N indexed Bytes are used for coding(0x1A) & 8N+7 indexed Bytes are reserved
	so for sending 256 samples in (8x256/2) = 1024 bytes are needed
	8N: 0(Coding)
	8N+1: HHk(Samplek MSB) 
	8N+2: HLk(Samplek Middle Byte) 
	8N+3: LHk(Samplek LSB) 
	8N+4: HH(k+1)(Sample(k+1) MSB) 
	8N+5: HL(k+1)(Sample(k+1) Middle Byte) 
	8N+6: LH(k+1)(Sample(k+1) LSB) 
	8N+7: Reserved 
	
	each capture is buffered in 1024 bytes which start at capture_Index * 10
	
	32 Capturede Signal + 8 Auxilary Signal (for Monitoring Process Signal)
	
	*/
	#ifdef CONCRETE
		if(CAPTURE_COUNT >= 16)
		{		
			Correct_Sixteenth_Captured_Signal();
		}
	#endif
	spikeLess_flag = 0;
	
	if(spikeLess_flag == 0)
	{
		#ifdef SOIL
		//Each Capture Contains at most 1024 bytes = 256 samples * 8/2 bytes
		for(capture_Index = 0; capture_Index < maximum_capture_index  ; capture_Index++) //for both Soil and Concrete only 16 captured signals are sent to PC for USB memory and speed optimization.
		{
			for(sample_Index = 0; sample_Index < 255; sample_Index += 2)
			{	 
				packetNumber = sample_Index >> 1;	 //each Packet contains 2 samples(8Bytes) ; sample_Index = 0,2,4,...254 & packetNumber = 0,1,2,...,127
				sendData[(capture_Index << 10) + (packetNumber << 3) + 0] = 0x00; //reserved for code 0x1A
				sendData[(capture_Index << 10) + (packetNumber << 3) + 1] = ((signal_Captured[capture_Index][sample_Index]) & 0xFF000000) >> 24; 
				sendData[(capture_Index << 10) + (packetNumber << 3) + 2] = ((signal_Captured[capture_Index][sample_Index]) & 0x00FF0000) >> 16; 
				sendData[(capture_Index << 10) + (packetNumber << 3) + 3] = ((signal_Captured[capture_Index][sample_Index]) & 0x0000FF00) >> 8; 
				sendData[(capture_Index << 10) + (packetNumber << 3) + 4] = ((signal_Captured[capture_Index][sample_Index + 1]) & 0xFF000000) >> 24;
				sendData[(capture_Index << 10) + (packetNumber << 3) + 5] = ((signal_Captured[capture_Index][sample_Index + 1]) & 0x00FF0000) >> 16;
				sendData[(capture_Index << 10) + (packetNumber << 3) + 6] = ((signal_Captured[capture_Index][sample_Index + 1]) & 0x0000FF00) >> 8;
				sendData[(capture_Index << 10) + (packetNumber << 3) + 7] = 0x00;	//reserved for temperature data 				
			}
		}
		#endif
		#ifdef CONCRETE
//		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		
		//Each Capture Contains at most 1024 bytes = 256 samples * 8/2 bytes
		for(capture_Index = 0; capture_Index < maximum_capture_index  ; capture_Index++) //for both Soil and Concrete only 16 captured signals are sent to PC for USB memory and speed optimization.
		{
			for(sample_Index = 0; sample_Index < 255; sample_Index += 2)
			{	 
				packetNumber = sample_Index >> 1;	 //each Packet contains 2 samples(8Bytes) ; sample_Index = 0,2,4,...254 & packetNumber = 0,1,2,...,127
				sendData[(capture_Index << 10) + (packetNumber << 3) + 0] = 0x00; //reserved for code 0x1A
				sendData[(capture_Index << 10) + (packetNumber << 3) + 1] = ((signal_Captured[capture_Index ][sample_Index]) & 0xFF000000) >> 24; 
				sendData[(capture_Index << 10) + (packetNumber << 3) + 2] = ((signal_Captured[capture_Index ][sample_Index]) & 0x00FF0000) >> 16; 
				sendData[(capture_Index << 10) + (packetNumber << 3) + 3] = ((signal_Captured[capture_Index ][sample_Index]) & 0x0000FF00) >> 8; 
				sendData[(capture_Index << 10) + (packetNumber << 3) + 4] = ((signal_Captured[capture_Index ][sample_Index + 1]) & 0xFF000000) >> 24;
				sendData[(capture_Index << 10) + (packetNumber << 3) + 5] = ((signal_Captured[capture_Index ][sample_Index + 1]) & 0x00FF0000) >> 16;
				sendData[(capture_Index << 10) + (packetNumber << 3) + 6] = ((signal_Captured[capture_Index ][sample_Index + 1]) & 0x0000FF00) >> 8;
				sendData[(capture_Index << 10) + (packetNumber << 3) + 7] = 0x00;	//reserved for temperature data 				
			}
		}
		#endif
	}
	
	// All the captured signals, spikes removed
	if(spikeLess_flag == 1)
	{
		for(capture_Index = 0; capture_Index < maximum_capture_index ; capture_Index++) //for both Soil and Concrete only 16 captured signals are sent to PC for USB memory and speed optimization.
		{
			for(sample_Index = 0; sample_Index < 255; sample_Index += 2)
			{
				//Each Capture Contains at most 1024 bytes = 256 samples * 8/2 bytes 
				packetNumber = sample_Index >> 1;	 //each Packet contains 2 samples(8Bytes) ; sample_Index = 0,2,4,...254 & packetNumber = 0,1,2,...,127
				sendData[(capture_Index << 10) + (packetNumber << 3) + 0] = 0x00; //reserved for code 0x1A
				sendData[(capture_Index << 10) + (packetNumber << 3) + 1] = ((int32_t)(signal_SubSampled_SpikeLess[capture_Index][sample_Index]) & 0xFF000000) >> 24; 
				sendData[(capture_Index << 10) + (packetNumber << 3) + 2] = ((int32_t)(signal_SubSampled_SpikeLess[capture_Index][sample_Index]) & 0x00FF0000) >> 16; 
				sendData[(capture_Index << 10) + (packetNumber << 3) + 3] = ((int32_t)(signal_SubSampled_SpikeLess[capture_Index][sample_Index]) & 0x0000FF00) >> 8; 
				sendData[(capture_Index << 10) + (packetNumber << 3) + 4] = ((int32_t)(signal_SubSampled_SpikeLess[capture_Index][sample_Index + 1]) & 0xFF000000) >> 24;
				sendData[(capture_Index << 10) + (packetNumber << 3) + 5] = ((int32_t)(signal_SubSampled_SpikeLess[capture_Index][sample_Index + 1]) & 0x00FF0000) >> 16;
				sendData[(capture_Index << 10) + (packetNumber << 3) + 6] = ((int32_t)(signal_SubSampled_SpikeLess[capture_Index][sample_Index + 1]) & 0x0000FF00) >> 8;
				sendData[(capture_Index << 10) + (packetNumber << 3) + 7] = 0x00;	//reserved for temperature data 
			}
  	}		
	
	}	

	
// Median applied  signal
	waveformIndex = 17;
	for(sample_Index = 0; sample_Index < 255; sample_Index += 2)
	{
		//Each Capture Contains at most 1024 bytes = 256 samples * 8/2 bytes 
		packetNumber = sample_Index >> 1;	 //each Packet contains 2 samples(8Bytes) ; sample_Index = 0,2,4,...254 & packetNumber = 0,1,2,...,127
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 0] = 0x00; //reserved for code 0x1A
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 1] = ((int32_t)(signal_Median[sample_Index]) & 0xFF000000) >> 24; 
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 2] = ((int32_t)(signal_Median[sample_Index]) & 0x00FF0000) >> 16; 
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 3] = ((int32_t)(signal_Median[sample_Index]) & 0x0000FF00) >> 8; 
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 4] = ((int32_t)(signal_Median[sample_Index + 1]) & 0xFF000000) >> 24;
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 5] = ((int32_t)(signal_Median[sample_Index + 1]) & 0x00FF0000) >> 16;
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 6] = ((int32_t)(signal_Median[sample_Index + 1]) & 0x0000FF00) >> 8;
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 7] = 0x00;	//reserved for temperature data 
	}

	// LPF applied and transients cut out signal

	waveformIndex = 18;
	for(sample_Index = 0; sample_Index < 255; sample_Index += 2)
	{
		//Each Capture Contains at most 1024 bytes = 256 samples * 8/2 bytes 
		packetNumber = sample_Index >> 1;	 //each Packet contains 2 samples(8Bytes) ; sample_Index = 0,2,4,...254 & packetNumber = 0,1,2,...,127
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 0] = 0x00; //reserved for code 0x1A
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 1] = ((int32_t)(signal_LPF_And_Cut[sample_Index]) & 0xFF000000) >> 24; 
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 2] = ((int32_t)(signal_LPF_And_Cut[sample_Index]) & 0x00FF0000) >> 16; 
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 3] = ((int32_t)(signal_LPF_And_Cut[sample_Index]) & 0x0000FF00) >> 8; 
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 4] = ((int32_t)(signal_LPF_And_Cut[sample_Index + 1]) & 0xFF000000) >> 24;
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 5] = ((int32_t)(signal_LPF_And_Cut[sample_Index + 1]) & 0x00FF0000) >> 16;
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 6] = ((int32_t)(signal_LPF_And_Cut[sample_Index + 1]) & 0x0000FF00) >> 8;
		sendData[(waveformIndex << 10) + (packetNumber << 3) + 7] = 0x00;	//reserved for temperature data 
		
	}

	//Since sendData[(waveformIndex << 10) + k] = 0 for k > 214(in Soil) && k > 174 (in Concrete), They can be used for sending other data
	// Other system patrameters sent in the end of 18'th signal
	
	waveformIndex = 19;
	// Data index = 216 in C# App, contains samplePlus  (216 = 864/4)
	sendData[(waveformIndex << 10) + 864] = 0x00;	 // Reserved
	sendData[(waveformIndex << 10) + 865] = (((int32_t)samplePlus) & 0x00FF0000) >> 16;	
	sendData[(waveformIndex << 10) + 866] = (((int32_t)samplePlus) & 0x0000FF00) >> 8;	
	sendData[(waveformIndex << 10) + 867] = (((int32_t)samplePlus) & 0x000000FF) >> 0;
	// Data index = 217 in C# App, contains sampleMinus (217=868/4)
	sendData[(waveformIndex << 10) + 868] = (((int32_t)sampleMinus) & 0x00FF0000) >> 16;	
	sendData[(waveformIndex << 10) + 869] = (((int32_t)sampleMinus) & 0x0000FF00) >> 8;	
	sendData[(waveformIndex << 10) + 870] = (((int32_t)sampleMinus) & 0x000000FF) >> 0;	
	sendData[(waveformIndex << 10) + 871] = 0x00;	 // Reserved
	
	// Data index = 218 in C# App, contains Average Amplitude (Temperature Correctd)  (218 = 872/4)
	sendData[(waveformIndex << 10) + 872] = 0x00;	 // Reserved
	sendData[(waveformIndex << 10) + 873] = (((int32_t)average_Amplitude_Temp_Corrected) & 0x00FF0000) >> 16;	
	sendData[(waveformIndex << 10) + 874] = (((int32_t)average_Amplitude_Temp_Corrected) & 0x0000FF00) >> 8;	
	sendData[(waveformIndex << 10) + 875] = (((int32_t)average_Amplitude_Temp_Corrected) & 0x000000FF) >> 0;			
	// Data index = 219 in C# App, contains ADC Levels for temperature sensor  (219=876/4)
	sendData[(waveformIndex << 10) + 876] = 0x00;	
	sendData[(waveformIndex << 10) + 877] = ( ADC_ConvertedValue[1] & 0xFF00) >> 8;	
	sendData[(waveformIndex << 10) + 878] = ( ADC_ConvertedValue[1] & 0x00FF) >> 0;	
	sendData[(waveformIndex << 10) + 879] = 0x00;	 // Reserved
	
	// Data index = 220 in C# App, contains displayed temperature  (220 = 880/4)
	sendData[(waveformIndex << 10) + 880] = 0x00;	 // Reserved
	sendData[(waveformIndex << 10) + 881] = (((uint32_t)(temp_Deg_Display * 100.0f)) & 0x00FF0000) >> 16;	
	sendData[(waveformIndex << 10) + 882] = (((uint32_t)(temp_Deg_Display * 100.0f)) & 0x0000FF00) >> 8;	
	sendData[(waveformIndex << 10) + 883] = (((uint32_t)(temp_Deg_Display * 100.0f)) & 0x000000FF) >> 0;			
	// Data index = 221 in C# App, contains ADC Levels for temperature sensor  (221 = 884 / 4)	
	sendData[(waveformIndex << 10) + 884] = 0x00;	 // Reserved			
	sendData[(waveformIndex << 10) + 885] = (((uint32_t)(displayed_Resistance_Or_Conductivity * 100.0f)) & 0x00FF0000) >> 16;	
	sendData[(waveformIndex << 10) + 886] = (((uint32_t)(displayed_Resistance_Or_Conductivity * 100.0f)) & 0x0000FF00) >> 8;	
	sendData[(waveformIndex << 10) + 887] = (((uint32_t)(displayed_Resistance_Or_Conductivity * 100.0f)) & 0x000000FF) >> 0;	
	// Data index = 222 in C# App, contains ADC Levels for temperature sensor  (221 = 888 / 4)	
	sendData[(waveformIndex << 10) + 888] = 0x00;	 // Reserved			
	sendData[(waveformIndex << 10) + 889] = (((uint32_t)(probeTypeIndex)) & 0x000000FF) >> 0;	
	#ifdef CONCRETE
		sendData[(waveformIndex << 10) + 890] = (((uint32_t)(electrical_Connection_Status)) & 0x000000FF) >> 0;					
	#endif
	#ifdef SOIL
		sendData[(waveformIndex << 10) + 890] = (((uint32_t)(electrical_Connection_Status)) & 0x000000FF) >> 0;		
	#endif
	sendData[(waveformIndex << 10) + 891] = 0x00;	 // Reserved	
	
	// Data index = 223 in C# App, contains ADC Levels for temperature sensor  (223 = 892/4)
	sendData[(waveformIndex << 10) + 892] = 0x00;	 // Reserved
	sendData[(waveformIndex << 10) + 893] = (((uint32_t)(resistance_conductivity_Range + 6)) & 0x000000FF) >> 0;	
	sendData[(waveformIndex << 10) + 894] = 0x00;
	sendData[(waveformIndex << 10) + 895] = 0x00;
	
	// Data index = 224 in C# App, contains ADC Levels for temperature sensor  (224 = 896/4)
	sendData[(waveformIndex << 10) + 896] = 0x00;	 // Reserved
	sendData[(waveformIndex << 10) + 897] = (((int32_t)varPlus) & 0x00FF0000) >> 16;	
	sendData[(waveformIndex << 10) + 898] = (((int32_t)varPlus) & 0x0000FF00) >> 8;	
	sendData[(waveformIndex << 10) + 899] = (((int32_t)varPlus) & 0x000000FF) >> 0;
	
	// Data index = 225 in C# App, contains ADC Levels for temperature sensor  (225 = 900/4)
	sendData[(waveformIndex << 10) + 900] = (((int32_t)varMinus) & 0x00FF0000) >> 16;	
	sendData[(waveformIndex << 10) + 901] = (((int32_t)varMinus) & 0x0000FF00) >> 8;
	sendData[(waveformIndex << 10) + 902] = (((int32_t)varMinus) & 0x000000FF) >> 0;
	sendData[(waveformIndex << 10) + 903] = 0x00;	 // Reserved
	
	// Data index = 226 in C# App, contains ADC Levels for temperature sensor  (226 = 904/4)
	sendData[(waveformIndex << 10) + 904] = 0x00;	//Reserved
	sendData[(waveformIndex << 10) + 905] = 0x00;
	sendData[(waveformIndex << 10) + 906] = 0x00;
	sendData[(waveformIndex << 10) + 907] = (((int32_t)OutputFilterOperation) & 0x000000FF) >> 0;
	
	
		
//		waveformIndex = 19;
//			sprintf(panel[SAVE].Text  , "%2.2E" , absolute_DC_Level);
//		for(sample_Index = 0; sample_Index < 255; sample_Index += 2)
//		{
//			//Each Capture Contains at most 1024 bytes = 256 samples * 8/2 bytes 
//			packetNumber = sample_Index >> 1;	 //each Packet contains 2 samples(8Bytes) ; sample_Index = 0,2,4,...254 & packetNumber = 0,1,2,...,127
//			sendData[(waveformIndex << 10) + (packetNumber << 3) + 0] = 0x00; //reserved for code 0x1A
//			sendData[(waveformIndex << 10) + (packetNumber << 3) + 1] = ((int32_t)(signal_LPF_And_Cut[sample_Index] - 20000) & 0xFF000000) >> 24; 
//			sendData[(waveformIndex << 10) + (packetNumber << 3) + 2] = ((int32_t)(signal_LPF_And_Cut[sample_Index] - 20000) & 0x00FF0000) >> 16; 
//			sendData[(waveformIndex << 10) + (packetNumber << 3) + 3] = ((int32_t)(signal_LPF_And_Cut[sample_Index] - 20000) & 0x0000FF00) >> 8; 
//			sendData[(waveformIndex << 10) + (packetNumber << 3) + 4] = ((int32_t)(signal_LPF_And_Cut[sample_Index + 1] - 20000) & 0xFF000000) >> 24;
//			sendData[(waveformIndex << 10) + (packetNumber << 3) + 5] = ((int32_t)(signal_LPF_And_Cut[sample_Index + 1] - 20000) & 0x00FF0000) >> 16;
//			sendData[(waveformIndex << 10) + (packetNumber << 3) + 6] = ((int32_t)(signal_LPF_And_Cut[sample_Index + 1] - 20000) & 0x0000FF00) >> 8;
//			sendData[(waveformIndex << 10) + (packetNumber << 3) + 7] = 0x00;	//reserved for temperature data 

//		}
//		
//		
//		sendData[(waveformIndex << 10) + 881] = (((int32_t)(Ave_Plus / 256.0)) & 0x00FF0000) >> 16;	
//		sendData[(waveformIndex << 10) + 882] = (((int32_t)(Ave_Plus / 256.0)) & 0x0000FF00) >> 8;	
//		sendData[(waveformIndex << 10) + 883] = (((int32_t)(Ave_Plus / 256.0)) & 0x000000FF) >> 0;
		
		/*fill reserved bytes with 20 recent values of temperature data 
		8N+7: Temperature (Hbyte) 
		8*(N+1)+7: Temperature (Hbyte) 
		*/	
		
		
//		static uint8_t k = 0;
//		for(uint32_t i = 0; i < 24576 ; i++)
//		{
//			sendData[i] = 0 + k;
//		}
//		k++;		
		
		Send_Via_USB(sendData,USB_TRANSMIT_SIZE);	
	
}

void Add_One_Sample_to_ADCBuffer(void)
{
	if(captureIndex < CAPTURE_COUNT && sampleIndex < 256)
	{
		if(captureIndex != 0)
		{
			signal_Captured[captureIndex][sampleIndex] = 1e9; //(SPI_RxBuffer[0] << 24) + (SPI_RxBuffer[1] << 16) + (SPI_RxBuffer[2] << 8);	
		}
		else
		{
			if(sampleIndex < 174)
			{
				signal_Captured[0][sampleIndex] = -1e9 + (2e9 / 200) * sampleIndex;
			}
			else
			{
				signal_Captured[0][sampleIndex] = 0;
			}
		}
		
		sampleIndex++;
	}	
}

void EraseADCBuffer(void)
{
	uint16_t m = 0;
	uint16_t n = 0;
	for(m = 0; m < CAPTURE_COUNT; m++)
	{
		for(n = 0; n < 256; n++)
		{
			signal_Captured[m][n] = 0;
		}		
	}
}
/* USER CODE END 1 */
