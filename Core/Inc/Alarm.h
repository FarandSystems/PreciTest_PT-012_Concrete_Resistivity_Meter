/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __alarm_H
#define __alarm_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


typedef struct
{
	uint32_t Pattern;
	uint32_t AlarmCount;
	uint8_t BeepOnOff;
	uint8_t TimePeriod;
  
} Message_TypeDef;


// Alarm Message Definition
#define MEDIUM_BEEP_X1 0x000000FF
#define MEDIUM_BEEP_X2 0x00FF00FF
#define LONG_BEEP 0x0000FFFF
#define VERY_LONG_BEEP 0xFFFFFFFF
#define EMERGENCY_BEEP 0x0F0F0F0F
#define PULSE_TRAIN_BEEP 0x33333333
#define SHORT_BEEP_X1 0x00000003
#define SHORT_BEEP_X2 0x00000033
#define SHORT_BEEP_X3 0x00000333
#define SHORT_BEEP_X4 0x00003333
#define VERYSHORT_BEEP_X1 0x00000001
#define BEEP_ON 0x01
#define BEEP_OFF 0x00
#define MESSAGE_QUEUE_SIZE 32

extern void Alarm(uint32_t PatternInit, uint8_t NumberInit, uint8_t AlarmTime , uint8_t BeepOnOffInit);
extern void UpdateAlarm(void);


#ifdef __cplusplus
}
#endif
#endif



