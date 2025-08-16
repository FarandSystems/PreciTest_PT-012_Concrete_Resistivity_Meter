/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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
#include "spi.h"

/* USER CODE BEGIN 0 */
void Read_ADC_24Bit(void);
void Init_Input_From_ADC_As_EXTI(void);
void Init_Input_From_ADC_As_MISO(void);
void Start_SPI_Capture(void);

// SUB_SAMPLE_DIVISOR is "3" for concrete project and is "1" for soil project
// SUB_SAMPLE_DIVISOR is used to set number of calling Add_One_Sample_to_ADCBuffer() function
//for Soil this is equevalent to 20 Ksamples/Sec. (40 KSps/(1+1))  
//for Concrete this is equevalent to 10 Ksamples/Sec. (40 KSps/(3+1))
//#define SUB_SAMPLE_DIVISOR       1  //Soil: 1, Concrete: 3

	
uint8_t Command[3]; // SPI Command Bytes which should be sent to ADC 24 Bit (MAX1140)
uint8_t SPI_RxBuffer[3];
/* USER CODE END 0 */

SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi2_rx;
DMA_HandleTypeDef hdma_spi2_tx;

/* SPI2 init function */
void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
    /* SPI2 clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI2 GPIO Configuration
    PI1     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    */
    GPIO_InitStruct.Pin = ARDUINO_SCK_D13_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(ARDUINO_SCK_D13_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ARDUINO_MISO_D12_Pin|ARDUINO_MOSI_PWM_D11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* SPI2 DMA Init */
    /* SPI2_RX Init */
    hdma_spi2_rx.Instance = DMA1_Stream3;
    hdma_spi2_rx.Init.Channel = DMA_CHANNEL_0;
    hdma_spi2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_spi2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi2_rx.Init.Mode = DMA_NORMAL;
    hdma_spi2_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_spi2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(spiHandle,hdmarx,hdma_spi2_rx);

    /* SPI2_TX Init */
    hdma_spi2_tx.Instance = DMA1_Stream4;
    hdma_spi2_tx.Init.Channel = DMA_CHANNEL_0;
    hdma_spi2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi2_tx.Init.Mode = DMA_NORMAL;
    hdma_spi2_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_spi2_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(spiHandle,hdmatx,hdma_spi2_tx);

  /* USER CODE BEGIN SPI2_MspInit 1 */

  /* USER CODE END SPI2_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspDeInit 0 */

  /* USER CODE END SPI2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI2_CLK_DISABLE();

    /**SPI2 GPIO Configuration
    PI1     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    */
    HAL_GPIO_DeInit(ARDUINO_SCK_D13_GPIO_Port, ARDUINO_SCK_D13_Pin);

    HAL_GPIO_DeInit(GPIOB, ARDUINO_MISO_D12_Pin|ARDUINO_MOSI_PWM_D11_Pin);

    /* SPI2 DMA DeInit */
    HAL_DMA_DeInit(spiHandle->hdmarx);
    HAL_DMA_DeInit(spiHandle->hdmatx);
  /* USER CODE BEGIN SPI2_MspDeInit 1 */

  /* USER CODE END SPI2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void Start_SPI_Capture(void)
{
	if(systemMode == FINISH_MEASUREMENT_START_PROCESS_MODE)
	{
		Init_Input_From_ADC_As_MISO();		
		Read_ADC_24Bit();	

	}	
}
void Read_ADC_24Bit(void)
{
	//Set NSS High
	HAL_GPIO_WritePin(SPIx_NSS_PORT, SPIx_NSS_PIN, GPIO_PIN_SET); 
	Command[0] = 0x00;
	Command[1] = 0x00;
	Command[2] = 0x00;
	HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t*)Command, (uint8_t*)SPI_RxBuffer, 3);
}

// Define Input Pin as External Interrupt to recieve not_DRDY from ADC
void Init_Input_From_ADC_As_EXTI(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;	
	SPIx_MISO_GPIO_CLK_ENABLE();
	HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT,SPIx_MISO_PIN);	
  /* EXT_IT MISO GPIO pin configuration  */
  GPIO_InitStruct.Pin       = SPIx_MISO_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);
		
  /* Enable and set Touch screen EXTI Interrupt to the lowest priority */
  HAL_NVIC_SetPriority((IRQn_Type)(EXTI15_10_IRQn), 7, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)(EXTI15_10_IRQn));		
}

// Define Input Pin as MISO of the SPI to recieve DOut from ADC
void Init_Input_From_ADC_As_MISO(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;	
	SPIx_MISO_GPIO_CLK_ENABLE();
	HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT,SPIx_MISO_PIN);
  /* EXT_IT MISO GPIO pin configuration  */
  GPIO_InitStruct.Pin       = SPIx_MISO_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = SPIx_MISO_AF;	
  HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
	// SPI Process is started
	if(GPIO_Pin == GPIO_PIN_14)
	{
		Start_SPI_Capture();
	}
	if(GPIO_Pin == SW_Sense_Pin)
	{
		HAL_GPIO_WritePin(Sustain_Power_GPIO_Port,Sustain_Power_Pin,GPIO_PIN_RESET);
	}

}
/**
  * @brief Tx and Rx Transfer completed callback
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) //Move this function
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_TxRxCpltCallback should be implemented in the user file
   */
	
		/*
			ADS1252 XTAL freq. = 16MHz
			so Data output rate is 41.667KHz(According to dataSheet Pg.8)
			We could only get half of it due to code speed limit so this callback is executed at 20.8335KHz
			SOIL : SUB_SAMPLE_DIVISOR = 1
	    All of samples are saved (20.8335/1 = 20.8335KHz or 48uSec.)(20833 / 97 = 215 Samples/Capture);
	    CONCRETE : SUB_SAMPLE_DIVISOR = 3
			some samples are ignored for concrete (1 out of 3 samples) (20.8335/3 = 6.94KHz or 144uSec.) (6940 / 40 = 173.5 Samples/Capture);
			if 1 out of 2 samples was captured  (20.8335KHz / 2) /40Hz = 260 samples would be needed 
	 */
		if(divisor == 0)
		{			
			Add_One_Sample_to_ADCBuffer();
		}
		
		divisor++; 
		if(divisor == SUB_SAMPLE_DIVISOR)
		{
			divisor = 0;
		}
		
		//Change Pin Function to EXTI to Read DRDY
		HAL_GPIO_WritePin(SPIx_NSS_PORT, SPIx_NSS_PIN, GPIO_PIN_RESET);
		Init_Input_From_ADC_As_EXTI();		
		
		
		

}
/* USER CODE END 1 */
