/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __power_management_H
#define __power_management_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define Farand_RED           ((uint16_t)0xF000)
#define Farand_GRN           ((uint16_t)0x0F00)
#define Farand_ORG           ((uint16_t)0x8A00)
#define Farand_BLU           ((uint16_t)0xA000)

typedef enum{
Fast_Operation, 
Stable_Operation
}Voltage_Filter_Mode_TypeDef;

typedef enum{
FULL_CHARGE, 
CHARGING,
UNPLUGGED,	
}Charging_Mode_TypeDef;

extern Charging_Mode_TypeDef Charging_Mode;

extern uint16_t v_Bat_Levels;
extern float v_Bat_Volts;
extern float v_Bat_Percentage;
extern float v_Bat_Volts_Filtered;

extern uint16_t v_Input_Power_Levels0;
extern float	v_Input_Power_Volts;
extern float v_Input_Power_Volts_Filtered;
extern uint16_t input_Power_Voltage_Filter_Counter;

extern uint16_t v_Input_Jack_Charger_Levels;
extern float	v_Input_Jack_Charger_Volts;
extern float v_Input_Jack_Charger_Filtered;
extern uint16_t input_Jack_Charger_Voltage_Filter_Counter;

extern float I_Charge_mA;
extern uint16_t power_LED_Color;



extern void Check_Power_And_Charging(void);
extern void Set_LED_Color(void);

#define POWER_IN_5V_WITH_Li_ION_INTERNAL_BATTERY
//#define POWER_IN_5V_WITH_NO_INTERNAL_BATTERY

#define V_REF 																	3.321f
#define CURRENT_SENSE_RESISTOR									0.1f // Ohm
#define DIVIDER_INTERNAL_BATTERY_RATIO  				0.67326f // 0.67326 = 68.0/(68.0 + 33.0) 	divider resistors are 68k and 33k, for a more accurate result it should be measured
#define DIVIDER_INPUT_POWER_RATIO  							0.67326f // 0.67326 = 68.0/(68.0 + 33.0) 	divider resistors are 68k and 33k, for a more accurate result it should be measured
#define DIVIDER_INPUT_JACK_CHARGER_RATIO  			0.67326f // 0.67326 = 68.0/(68.0 + 33.0) 	divider resistors are 68k and 33k, for a more accurate result it should be measured

#ifdef __cplusplus
}
#endif
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
