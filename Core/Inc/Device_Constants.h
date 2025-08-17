/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __soil_const_H
#define __soil_const_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/*Flash Index*/
#define DEVICE_ID_FLASH_INDEX 						0
#define PROJECT_RECORD_NUMBER_FLASH_INDEX 1
#define DATA_BUFFER_FLASH_INDEX   				0

//Calibration Report for PT012 
//Serial No.: 012001
//Date/Time: 8/17/2025 5:38:48 PM

//Temperature Calibration Coefficients:
#define TEMP_G		000.081
#define TEMP_O		000.000

////015.00	
//030.00	


//Resistance Calibration Coefficients:

// Concrete_LowRange_Line_Gain X10000
#define CON_LOW_L1_G		009.508
#define CON_LOW_L2_G		005.497
#define CON_LOW_L3_G		005.056
#define CON_LOW_L4_G		005.011

// Concrete_LowRange_Line_Offset X10000
#define CON_LOW_L1_O		-13479.639
#define CON_LOW_L2_O		-3531.818
#define CON_LOW_L3_O		5991.561
#define CON_LOW_L4_O		13671.104

// Concrete_HighRange_Line_Gain X100
#define CON_HIGH_L1_G		004.576
#define CON_HIGH_L2_G		005.119
#define CON_HIGH_L3_G		005.169
#define CON_HIGH_L4_G		005.347

// Concrete_HighRange_Line_Offset X100
#define CON_HIGH_L1_O		3723.532
#define CON_HIGH_L2_O		4908.571
#define CON_HIGH_L3_O		4300.993
#define CON_HIGH_L4_O		-51412.690


//Resistance Calibration Data Points:
//Physical Value (Ohm)	Measured Levels
//000.97	3214 2859 2228 1920 1947 
//002.20	3576 3955 4048 4479 2599 
//005.09	10099 10148 9415 9548 10303 
//010.00	19268 18635 19327 19185 17761 
//020.80	40258 39651 39957 39940 39951 
//050.40	99698 98845 98578 97368 97969 
//100.00	197797 196410 195994 197378 196489 
//203.00	402677 402720 401210 402579 402521 
//203.00	3620 4090 4092 2848 3463 
//500.00	10511 10396 10430 9867 9362 
//998.90	18656 18793 18381 18199 18745 
//1995.00	38011 38169 38826 37709 37354 
//5138.00	98464 100145 97727 98867 97682 
//9999.00	190260 193349 193102 192841 193582 
//20070.00	386322 380195 386805 381782 389652 
//52165.00	970068 984471 993129 979511 998678 

// Soil_MiddleRange_Line_Gain
#define WEN_LOW_L1_G    0.1243E-4f
#define WEN_LOW_L2_G    0.1243E-4f
#define WEN_LOW_L3_G    0.1243E-4f
#define WEN_LOW_L4_G    0.1243E-4f

// Soil_MiddleRange_Line_Offset
#define WEN_LOW_L1_O	  -0.0077f  
#define WEN_LOW_L2_O	  -0.0077f  
#define WEN_LOW_L3_O	  -0.0077f  
#define WEN_LOW_L4_O	  -0.0077f  

// Soil_HighRange_Line_Gain
#define WEN_HIGH_L1_G		1.0399E-1f
#define WEN_HIGH_L2_G		1.0399E-1f
#define WEN_HIGH_L3_G		1.0399E-1f
#define WEN_HIGH_L4_G		1.0399E-1f

// Soil_HighRange_Line_Offset
#define WEN_HIGH_L1_O		-1.4671f
#define WEN_HIGH_L2_O		-1.4671f
#define WEN_HIGH_L3_O		-1.4671f
#define WEN_HIGH_L4_O		-1.4671f

#ifdef __cplusplus
}
#endif
#endif

