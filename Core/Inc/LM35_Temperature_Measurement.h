/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __lm35_temp_measurement_H
#define __lm35_temp_measurement_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern uint16_t blockEndAdd;
extern uint16_t blockEndAdd;
extern float temp_Deg_Display;
extern void Caibrate_And_Filter_Temperature(void);
extern uint8_t tempFilteringMode;
	 
#define TEMP_FILTER_FAST   (uint8_t)0;
#define TEMP_FILTER_STABLE (uint16_t)1;

	 
#ifdef __cplusplus
}
#endif
#endif
