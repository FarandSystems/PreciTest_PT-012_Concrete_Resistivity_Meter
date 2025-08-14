/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c1;

extern I2C_HandleTypeDef hi2c3;

/* USER CODE BEGIN Private defines */
#define I2C_DEVICE_ADDRESS        0xD0
#define I2C_RECIEVE_BUFFER_SIZE      8
#define I2C_TRANSMIT_BUFFER_SIZE     2
/* USER CODE END Private defines */

void MX_I2C1_Init(void);
void MX_I2C3_Init(void);

/* USER CODE BEGIN Prototypes */
extern uint8_t I2C_Rx_Data[I2C_RECIEVE_BUFFER_SIZE];
extern uint8_t I2C_Tx_Data[8];

extern uint8_t Second;
extern uint8_t Minute;
extern uint8_t Hour;

extern uint8_t Day;
extern uint8_t Month;
extern uint8_t Year;

extern void Set_Colck_and_Date(uint8_t hour, uint8_t min, uint8_t sec, uint8_t year, uint8_t mon, uint8_t day);
extern uint8_t Decimal_To_BCD(uint8_t num);
extern void Initialize_RTC(void);
extern void Sending_Request_to_RTC(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

