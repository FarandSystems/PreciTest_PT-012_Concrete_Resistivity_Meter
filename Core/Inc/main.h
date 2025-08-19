/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "spi.h"
#include <stdlib.h>
#include <stdio.h>	
#include "ProcessData.h"
#include "Device_Constants.h"
#include "Flash.h"
#include "LM35_Temperature_Measurement.h"
#include "math.h"
#include "tim.h"
#include "i2c.h"
#include "Alarm.h"
#include "ltdc.h"
#include "gpio.h"
#include "dma.h"
#include "crc.h"
#include "fmc.h"
#include "dma2d.h"
#include "command_RX.h"
#include "quadspi.h"
#include "PowerManagement.h"
#include <stm32746g_discovery_qspi.h>
#include <touchgfx/hal/Config.hpp>	


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
extern void Check_USB_Connection(void);
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern uint8_t usb_connection_state;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define QSPI_D2_Pin GPIO_PIN_2
#define QSPI_D2_GPIO_Port GPIOE
#define FMC_NBL1_Pin GPIO_PIN_1
#define FMC_NBL1_GPIO_Port GPIOE
#define FMC_NBL0_Pin GPIO_PIN_0
#define FMC_NBL0_GPIO_Port GPIOE
#define ARDUINO_SCL_D15_Pin GPIO_PIN_8
#define ARDUINO_SCL_D15_GPIO_Port GPIOB
#define Charging_Pin GPIO_PIN_4
#define Charging_GPIO_Port GPIOB
#define HIGH_CURRENT_EN_Pin GPIO_PIN_15
#define HIGH_CURRENT_EN_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define ARDUINO_SDA_D14_Pin GPIO_PIN_9
#define ARDUINO_SDA_D14_GPIO_Port GPIOB
#define QSPI_NCS_Pin GPIO_PIN_6
#define QSPI_NCS_GPIO_Port GPIOB
#define FMC_SDNCAS_Pin GPIO_PIN_15
#define FMC_SDNCAS_GPIO_Port GPIOG
#define OTG_FS_VBUS_Pin GPIO_PIN_12
#define OTG_FS_VBUS_GPIO_Port GPIOJ
#define FMC_D2_Pin GPIO_PIN_0
#define FMC_D2_GPIO_Port GPIOD
#define OTG_FS_P_Pin GPIO_PIN_12
#define OTG_FS_P_GPIO_Port GPIOA
#define LCD_DE_Pin GPIO_PIN_7
#define LCD_DE_GPIO_Port GPIOK
#define LCD_B7_Pin GPIO_PIN_6
#define LCD_B7_GPIO_Port GPIOK
#define LCD_B6_Pin GPIO_PIN_5
#define LCD_B6_GPIO_Port GPIOK
#define LCD_B4_Pin GPIO_PIN_12
#define LCD_B4_GPIO_Port GPIOG
#define OTG_FS_PowerSwitchOn_Pin GPIO_PIN_5
#define OTG_FS_PowerSwitchOn_GPIO_Port GPIOD
#define FMC_D3_Pin GPIO_PIN_1
#define FMC_D3_GPIO_Port GPIOD
#define LED_RED_Pin GPIO_PIN_3
#define LED_RED_GPIO_Port GPIOI
#define LED_BLUE_Pin GPIO_PIN_2
#define LED_BLUE_GPIO_Port GPIOI
#define OTG_FS_N_Pin GPIO_PIN_11
#define OTG_FS_N_GPIO_Port GPIOA
#define FMC_A0_Pin GPIO_PIN_0
#define FMC_A0_GPIO_Port GPIOF
#define LCD_HSYNC_Pin GPIO_PIN_10
#define LCD_HSYNC_GPIO_Port GPIOI
#define LCD_B5_Pin GPIO_PIN_4
#define LCD_B5_GPIO_Port GPIOK
#define LCD_BL_CTRL_Pin GPIO_PIN_3
#define LCD_BL_CTRL_GPIO_Port GPIOK
#define LCD_B3_Pin GPIO_PIN_15
#define LCD_B3_GPIO_Port GPIOJ
#define OTG_FS_OverCurrent_Pin GPIO_PIN_4
#define OTG_FS_OverCurrent_GPIO_Port GPIOD
#define ARDUINO_SCK_D13_Pin GPIO_PIN_1
#define ARDUINO_SCK_D13_GPIO_Port GPIOI
#define FMC_A1_Pin GPIO_PIN_1
#define FMC_A1_GPIO_Port GPIOF
#define LCD_DISP_Pin GPIO_PIN_12
#define LCD_DISP_GPIO_Port GPIOI
#define LCD_VSYNC_Pin GPIO_PIN_9
#define LCD_VSYNC_GPIO_Port GPIOI
#define ARDUINO_PWM_CS_D5_Pin GPIO_PIN_0
#define ARDUINO_PWM_CS_D5_GPIO_Port GPIOI
#define LCD_G6_Pin GPIO_PIN_1
#define LCD_G6_GPIO_Port GPIOK
#define LCD_G7_Pin GPIO_PIN_2
#define LCD_G7_GPIO_Port GPIOK
#define HIGH_CURRENT_NOTEN_Pin GPIO_PIN_8
#define HIGH_CURRENT_NOTEN_GPIO_Port GPIOA
#define OSC_25M_Pin GPIO_PIN_0
#define OSC_25M_GPIO_Port GPIOH
#define FMC_A2_Pin GPIO_PIN_2
#define FMC_A2_GPIO_Port GPIOF
#define LCD_INT_Pin GPIO_PIN_13
#define LCD_INT_GPIO_Port GPIOI
#define LCD_G4_Pin GPIO_PIN_11
#define LCD_G4_GPIO_Port GPIOJ
#define LCD_G5_Pin GPIO_PIN_0
#define LCD_G5_GPIO_Port GPIOK
#define FMC_A3_Pin GPIO_PIN_3
#define FMC_A3_GPIO_Port GPIOF
#define LCD_CLK_Pin GPIO_PIN_14
#define LCD_CLK_GPIO_Port GPIOI
#define LCD_G3_Pin GPIO_PIN_10
#define LCD_G3_GPIO_Port GPIOJ
#define FMC_SDCLK_Pin GPIO_PIN_8
#define FMC_SDCLK_GPIO_Port GPIOG
#define Full_Charge_Pin GPIO_PIN_6
#define Full_Charge_GPIO_Port GPIOC
#define FMC_A4_Pin GPIO_PIN_4
#define FMC_A4_GPIO_Port GPIOF
#define FMC_SDNME_Pin GPIO_PIN_5
#define FMC_SDNME_GPIO_Port GPIOH
#define FMC_SDNE0_Pin GPIO_PIN_3
#define FMC_SDNE0_GPIO_Port GPIOH
#define LCD_G2_Pin GPIO_PIN_9
#define LCD_G2_GPIO_Port GPIOJ
#define BUZZER_Pin GPIO_PIN_7
#define BUZZER_GPIO_Port GPIOG
#define LED_GREEN_Pin GPIO_PIN_6
#define LED_GREEN_GPIO_Port GPIOG
#define Sustain_Power_Pin GPIO_PIN_7
#define Sustain_Power_GPIO_Port GPIOF
#define SW_Sense_Pin GPIO_PIN_6
#define SW_Sense_GPIO_Port GPIOF
#define SW_Sense_EXTI_IRQn EXTI9_5_IRQn
#define FMC_A5_Pin GPIO_PIN_5
#define FMC_A5_GPIO_Port GPIOF
#define LCD_R7_Pin GPIO_PIN_6
#define LCD_R7_GPIO_Port GPIOJ
#define FMC_D1_Pin GPIO_PIN_15
#define FMC_D1_GPIO_Port GPIOD
#define FMC_D15_Pin GPIO_PIN_10
#define FMC_D15_GPIO_Port GPIOD
#define Temperature_Sense_Pin GPIO_PIN_10
#define Temperature_Sense_GPIO_Port GPIOF
#define Battery_Sense_Pin GPIO_PIN_9
#define Battery_Sense_GPIO_Port GPIOF
#define FMC_SDCKE0_Pin GPIO_PIN_3
#define FMC_SDCKE0_GPIO_Port GPIOC
#define FMC_D0_Pin GPIO_PIN_14
#define FMC_D0_GPIO_Port GPIOD
#define FMC_D14_Pin GPIO_PIN_9
#define FMC_D14_GPIO_Port GPIOD
#define FMC_D13_Pin GPIO_PIN_8
#define FMC_D13_GPIO_Port GPIOD
#define FMC_A6_Pin GPIO_PIN_12
#define FMC_A6_GPIO_Port GPIOF
#define FMC_A11_Pin GPIO_PIN_1
#define FMC_A11_GPIO_Port GPIOG
#define FMC_A9_Pin GPIO_PIN_15
#define FMC_A9_GPIO_Port GPIOF
#define LCD_R5_Pin GPIO_PIN_4
#define LCD_R5_GPIO_Port GPIOJ
#define QSPI_D1_Pin GPIO_PIN_12
#define QSPI_D1_GPIO_Port GPIOD
#define QSPI_D3_Pin GPIO_PIN_13
#define QSPI_D3_GPIO_Port GPIOD
#define EXT_RST_Pin GPIO_PIN_3
#define EXT_RST_GPIO_Port GPIOG
#define LCD_R6_Pin GPIO_PIN_5
#define LCD_R6_GPIO_Port GPIOJ
#define WakeUpPin_Pin GPIO_PIN_0
#define WakeUpPin_GPIO_Port GPIOA
#define FMC_A7_Pin GPIO_PIN_13
#define FMC_A7_GPIO_Port GPIOF
#define FMC_A10_Pin GPIO_PIN_0
#define FMC_A10_GPIO_Port GPIOG
#define LCD_R4_Pin GPIO_PIN_3
#define LCD_R4_GPIO_Port GPIOJ
#define FMC_D5_Pin GPIO_PIN_8
#define FMC_D5_GPIO_Port GPIOE
#define QSPI_D0_Pin GPIO_PIN_11
#define QSPI_D0_GPIO_Port GPIOD
#define FMC_BA1_Pin GPIO_PIN_5
#define FMC_BA1_GPIO_Port GPIOG
#define FMC_BA0_Pin GPIO_PIN_4
#define FMC_BA0_GPIO_Port GPIOG
#define LCD_SCL_Pin GPIO_PIN_7
#define LCD_SCL_GPIO_Port GPIOH
#define FMC_A8_Pin GPIO_PIN_14
#define FMC_A8_GPIO_Port GPIOF
#define LCD_R3_Pin GPIO_PIN_2
#define LCD_R3_GPIO_Port GPIOJ
#define FMC_SDNRAS_Pin GPIO_PIN_11
#define FMC_SDNRAS_GPIO_Port GPIOF
#define FMC_D6_Pin GPIO_PIN_9
#define FMC_D6_GPIO_Port GPIOE
#define FMC_D8_Pin GPIO_PIN_11
#define FMC_D8_GPIO_Port GPIOE
#define FMC_D11_Pin GPIO_PIN_14
#define FMC_D11_GPIO_Port GPIOE
#define RANGE_RELAY_Pin GPIO_PIN_6
#define RANGE_RELAY_GPIO_Port GPIOH
#define LCD_SDA_Pin GPIO_PIN_8
#define LCD_SDA_GPIO_Port GPIOH
#define FMC_D4_Pin GPIO_PIN_7
#define FMC_D4_GPIO_Port GPIOE
#define FMC_D7_Pin GPIO_PIN_10
#define FMC_D7_GPIO_Port GPIOE
#define FMC_D9_Pin GPIO_PIN_12
#define FMC_D9_GPIO_Port GPIOE
#define FMC_D12_Pin GPIO_PIN_15
#define FMC_D12_GPIO_Port GPIOE
#define FMC_D10_Pin GPIO_PIN_13
#define FMC_D10_GPIO_Port GPIOE
#define ARDUINO_MISO_D12_Pin GPIO_PIN_14
#define ARDUINO_MISO_D12_GPIO_Port GPIOB
#define ARDUINO_MOSI_PWM_D11_Pin GPIO_PIN_15
#define ARDUINO_MOSI_PWM_D11_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define CONCRETE
//#define SOIL

#ifdef CONCRETE

	#define CCR1_VAL_MEASUREMENT                                8000//8000->400ms (=CAPTURE_COUNT * 25ms(40Hz))
	#define CCR1_VAL_PROCESS                                    1200 //1200->60ms
	#define CCR1_VAL_ERASE_BUFFER                               800//800->40ms
	#define USB_TRANSMIT_SIZE                                   8192//24576 //
	#define CAPTURE_COUNT		                                   	16	
	#define TIP_CUT_SAMPLE                                      30 
	#define TAIL_CUT_SAMPLE                                     85
	#define SAMPLES_PER_CAPTURE                                 174
	#define AVE_START_SAMPLE                                    30
	#define AVE_END_SAMPLE                                      80
	#define CONCRETE_LOW_RANGE_THERESHOLD                                8000
	#define CONCRETE_HIGH_RANGE_THERESHOLD                               1500000
	#define DISCONNECTED_THERESHOLD                             1.5f  // This parameter is < 2 when probe is connected, (This Parameter was 2.5 previously. and caused poor performance of disconnect detection)
	#define LOWER_DISCONNECTED_THERESHOLD												0.005f
	#define PROBE_SIZE_AFFECTED_VARIABLE                        probeTypeIndex
	#define RX_DATA_SIZE                                        16384
	#define	SEND_DATA_BUFFER_SIZE																8192 //24576
	#define	ADC_DATA_BUFFER_SIZE																16384
	#define	CORRECTED_VALUE_DATA_BUFFER_SIZE										16384
	#define	FILTERED_VALUE_DATA_BUFFER_SIZE											16384
	#define SUB_SAMPLE_DIVISOR                                  3  
	#define EXCITATION_FREQUENCY 									              40.0
	#define TIM5_Period																					24999 
	#define TIM5_Prescaler																			107
	#define TIM5_Pulse																					TIM5_Period/2.0

#endif

#ifdef SOIL

	#define CCR1_VAL_MEASUREMENT                                6800//6800->340ms (= CAPTURE_COUNT * 10.3ms(97Hz))
	#define CCR1_VAL_PROCESS                                    3600//3600-> 180ms
	#define CCR1_VAL_ERASE_BUFFER                               800//800->40ms
	#define USB_TRANSMIT_SIZE                                   24576
	#define CAPTURE_COUNT		                             				32//16//8	
	#define TIP_CUT_SAMPLE                                      30
	#define TAIL_CUT_SAMPLE                                     104
	#define SAMPLES_PER_CAPTURE                                 214
	#define AVE_START_SAMPLE                                    30
	#define AVE_END_SAMPLE                                      74
	
	#define SOIL_LOW_RANGE_LOW_THERESHOLD                            2684
	#define SOIL_LOW_RANGE_HIGH_THERESHOLD                           2684355
	#define SOIL_MIDDLE_RANGE_LOW_THERESHOLD                         5906
	#define SOIL_MIDDLE_RANGE_HIGH_THERESHOLD                        5905580
	#define SOIL_HIGH_RANGE_LOW_THERESHOLD                           13422	
	#define SOIL_HIGH_RANGE_HIGH_THERESHOLD                          6710886

	
	#define DISCONNECTED_THERESHOLD                             1E9
	#define PROBE_SIZE_AFFECTED_VARIABLE                        NullVariable
	#define RX_DATA_SIZE                                        16384
	#define	SEND_DATA_BUFFER_SIZE																24576 //40960
	#define	ADC_DATA_BUFFER_SIZE																4 * CAPTURE_COUNT * 256 // 32768
	#define	CORRECTED_VALUE_DATA_BUFFER_SIZE										4 * CAPTURE_COUNT * 256 // 32768
	#define	FILTERED_VALUE_DATA_BUFFER_SIZE											4 * CAPTURE_COUNT * 256 // 32768
	#define SUB_SAMPLE_DIVISOR                                  1 
	#define EXCITATION_FREQUENCY 				              					97.0
	#define TIM5_Period																					10308
	#define TIM5_Prescaler																			107
	#define TIM5_Pulse																					TIM5_Period/2.0

#endif

#define PI_NUMBER																							3.14159265358979f	


#define FINISH_MEASUREMENT_START_PROCESS_MODE          				0
#define FINISH_DISPLAY_START_MEASUREMENT_MODE          	  		1
#define FINISH_PROCESS_START_ERASE_BUFFER_MODE       			    2


// External Memory Base Addresses

#define	FINAL_POINT_DATA_BUFFER_SIZE													1024 // = float[256] -> 4 Byte
#define	FILTERED_MEDIAN_LOWPASS_DATA_BUFFER_SIZE							1024 // = float[256] -> 4 Byte
#define	YN_DATA_BUFFER_SIZE																		1024 // = float[256] -> 4 Byte 
#define	YP_DATA_BUFFER_SIZE																		1024 // = float[256] -> 4 Byte
#define	FILTERED_MEDIAN_DATA_BUFFER_SIZE											1024 // = float[256] -> 4 Byte
#define	FLASH_DATA_BUFFER_SIZE																100//10000
#define SEND_FLASH_DATA_BUFFER_SIZE                           512
#define	GRAPHIC_BUFFER_SIZE																		130560 // = 480 * 272

#define SDRAM_BANK_ADDR                  											((uint32_t)0xC0000000)
#define GRAPHIC_BUFFER1_BASE_ADDRESS													SDRAM_BANK_ADDR
#define GRAPHIC_BUFFER2_BASE_ADDRESS													GRAPHIC_BUFFER1_BASE_ADDRESS + 2 * GRAPHIC_BUFFER_SIZE   // Because each graphic pixel contain 2 bytes
#define	SEND_DATA_BASE_ADDRESS																GRAPHIC_BUFFER2_BASE_ADDRESS + 2 * GRAPHIC_BUFFER_SIZE
#define	ADC_DATA_BASE_ADDRESS																	SEND_DATA_BASE_ADDRESS + SEND_DATA_BUFFER_SIZE // int32_t	signal_Captured [CAPTURE_COUNT][256]
#define	CORRECTED_VALUE_DATA_BASE_ADDRESS											ADC_DATA_BASE_ADDRESS + ADC_DATA_BUFFER_SIZE // float	signal_SubSampled_SpikeLess [CAPTURE_COUNT][256]
#define	FILTERED_VALUE_DATA_BASE_ADDRESS											CORRECTED_VALUE_DATA_BASE_ADDRESS + CORRECTED_VALUE_DATA_BUFFER_SIZE // float	signal_SubSampled_LPF [CAPTURE_COUNT][256]
#define	FINAL_POINT_DATA_BASE_ADDRESS													FILTERED_VALUE_DATA_BASE_ADDRESS + FILTERED_VALUE_DATA_BUFFER_SIZE
#define	FILTERED_MEDIAN_LOWPASS_DATA_BASE_ADDRESS							FINAL_POINT_DATA_BASE_ADDRESS + FINAL_POINT_DATA_BUFFER_SIZE
#define	YN_DATA_BASE_ADDRESS																	FILTERED_MEDIAN_LOWPASS_DATA_BASE_ADDRESS + FILTERED_MEDIAN_LOWPASS_DATA_BUFFER_SIZE
#define	YP_DATA_BASE_ADDRESS																	YN_DATA_BASE_ADDRESS + YN_DATA_BUFFER_SIZE
#define	FILTERED_MEDIAN_DATA_BASE_ADDRESS											YP_DATA_BASE_ADDRESS + YP_DATA_BUFFER_SIZE
#define	FLASH_DATA_BASE_ADDRESS																FILTERED_MEDIAN_DATA_BASE_ADDRESS +	FILTERED_MEDIAN_DATA_BUFFER_SIZE	                          	
#define	SEND_FLASH_DATA_BASE_ADDRESS		                      FLASH_DATA_BASE_ADDRESS + 8 * FLASH_DATA_BUFFER_SIZE

#define DETECION_PC_SENT_BYTES  															{ 0x00, 0xAA, 0x55, 0x00, 0x00, 0x00, 0x00, 0xFF};
#define DETECION_DEVICE_SENT_BYTES 														{ 0x00, 0xAA, 0x55, 0x00, 0x00, 0x00, 0x00, 0xFF};
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
