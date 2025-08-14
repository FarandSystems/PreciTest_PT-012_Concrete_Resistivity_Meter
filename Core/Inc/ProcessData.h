/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __processData_H
#define __processData_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

typedef enum {FastOperation = 1, StableOperation = 2}FinalShow_TypeDef;
extern void Process_Captured_Data(void);
extern void Apply_Filter_After_N_Measurements(void);
extern void Display_Resistivity(float n);
extern void Display_Resistance(float n);
extern void Update_Temperature_Battery_Range_Panels(void);
extern void Correct_Sixteenth_Captured_Signal(void);

extern uint16_t resistivityX1000;
extern uint16_t resistance_Or_conductivityX10;
extern int bulk_cond_unit_PrefixNumber;
extern int resistance_conductivity_Range;
extern int bulk_conRange;
extern float connection_Criteria;
extern uint16_t num_off_blocks;
extern uint32_t sd_BuffSize;
extern double bulk_Or_cond_Final; 
extern double resistivity_Final;
extern float probeTypeIndex;
extern double resistivity_Fast;
extern double resistance_Ohm_Final;
extern uint8_t fast_Output_Counter;
extern FinalShow_TypeDef OutputFilterOperation;
extern float average_Amplitude_Temp_Corrected;
extern float displayed_Resistance_Or_Conductivity;
extern uint8_t change_sign_counter;
extern uint8_t first_time_disconnection_flag;

extern uint8_t temp_Correction_flag;
extern uint8_t save_flag;
extern uint8_t hold_flag;
extern uint8_t createPrj_flag;

typedef enum {HighRange, MiddleRange, LowRange}Measurement_Range_TypeDef;
typedef enum {AutoRange = 0, Force_MiddleRange = 1, Force_HighRange = 2, Force_LowRange = 3}ForceRange_TypeDef;
typedef enum {Connected = 1, Disconnected = 0}Electrical_Connection_TypeDef;
extern Measurement_Range_TypeDef measurementRange;

extern ForceRange_TypeDef  range_ForcingMode;
extern float samplePlus;
extern float sampleMinus;
extern float average_Amplitude;
extern float amplitude_RMS;
extern double disconnected_Thereshold;
extern float Ave;
extern float Ave_Plus;
extern float Ave_Minus;
extern float varMinus;	
extern float varPlus;
extern float absolute_DC_Level;
extern float displayedResistivity;
extern float displayed_Resistance_Or_Conductivity;

extern void Go_to_Middle_Range(void);
extern void Go_to_High_Range(void);
extern void Go_to_Low_Range(void);
extern void Select_Range(void);
extern void Calculate_Signal_Amplitude_Levels(void);
extern void Restart_Output_Filter(void);
extern void Restart_Output_Filter_in_Save_And_CreatingProj(void);
extern void Check_Electrical_Connection(void);
extern Electrical_Connection_TypeDef electrical_Connection_Status;

#ifdef __cplusplus
}
#endif
#endif

