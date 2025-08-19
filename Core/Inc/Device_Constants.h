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
#define DATA_BUFFER_FLASH_INDEX   				64

//Calibration Report for FW411 
//Serial No.: 1708500
//Date/Time: 8/18/2025 5:21:46 PM

//Temperature Calibration Coefficients:
#define TEMP_G		000.081
#define TEMP_O		000.000

//Temperature Calibration Data Points:
//Physical Value (C)	Measured Levels
//015.00	
//030.00	


//Resistance Calibration Coefficients:

// Concrete_LowRange_Line_Gain X10000
#define CON_LOW_L1_G		002.219
#define CON_LOW_L2_G		001.736
#define CON_LOW_L3_G		001.664
#define CON_LOW_L4_G		001.686

// Concrete_LowRange_Line_Offset X10000
#define CON_LOW_L1_O		-6052.968
#define CON_LOW_L2_O		-1707.778
#define CON_LOW_L3_O		6966.758
#define CON_LOW_L4_O		8092.695

// Concrete_HighRange_Line_Gain X100
#define CON_HIGH_L1_G		004.312
#define CON_HIGH_L2_G		003.903
#define CON_HIGH_L3_G		003.933
#define CON_HIGH_L4_G		004.100

// Concrete_HighRange_Line_Offset X100
#define CON_HIGH_L1_O		-6495.432
#define CON_HIGH_L2_O		-2395.800
#define CON_HIGH_L3_O		-6074.541
#define CON_HIGH_L4_O		-102897.920


//Resistance Calibration Data Points:
//Physical Value (Ohm)	Measured Levels
//000.97	7839 7886 4971 5870 7846 7861 7156 7223 
//002.20	12679 13133 12410 12574 12449 13247 12448 12203 
//005.09	30213 30311 30215 30156 30288 30277 30501 30473 
//010.00	57169 58019 59953 57803 59566 59822 57348 59023 
//020.80	121430 121599 119701 121607 121085 121643 119407 119924 
//050.40	298722 298375 298711 298988 298675 298763 298676 298401 
//100.00	588394 588088 588120 588187 588206 588118 588622 588609 
//203.00	1198898 1198949 1199125 1198999 1199452 1199260 1199403 1199342 
//203.00	4519 4543 6928 6108 7072 7002 6972 6571 
//500.00	12533 13745 13921 12233 13142 13209 13706 12330 
//998.90	25752 27577 25408 26007 25546 26787 27455 25139 
//1995.00	51841 52522 52072 50671 51097 52206 50970 52478 
//5138.00	132442 131735 132240 132298 132362 132184 132088 132054 
//9999.00	255368 255720 256314 255821 255162 254939 255900 256886 
//20070.00	514529 514837 514689 514175 514612 514918 515050 514511 
//52165.00	1297061 1297463 1297229 1297692 1297551 1297649 1298096 1297699 

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

