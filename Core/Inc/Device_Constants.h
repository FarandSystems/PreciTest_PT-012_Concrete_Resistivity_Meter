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

//Calibration Report for FW411 
//Serial No.: 4112001
//Date/Time: 2/17/2020 12:37:16 PM

//Temperature Calibration Coefficients:
#define TEMP_G		000.053f
#define TEMP_O		006.928f

//Temperature Calibration Data Points:
//Physical Value (C)	Measured Levels
//025.00	313 303 325 369 350 359 312 312 366 390 
//029.70	420 445 452 428 429 434 418 458 394 405 


//Resistance Calibration Coefficients:

// Concrete_LowRange_Line_Gain X10000
#define CON_LOW_L1_G		002.168f
#define CON_LOW_L2_G		002.107f
#define CON_LOW_L3_G		002.065f
#define CON_LOW_L4_G		002.085f

// Concrete_LowRange_Line_Offset X10000
#define CON_LOW_L1_O		1052.273f
#define CON_LOW_L2_O		-317.971f
#define CON_LOW_L3_O		1050.898f
#define CON_LOW_L4_O		-11851.107f

// Concrete_HighRange_Line_Gain X100
#define CON_HIGH_L1_G		002.084f
#define CON_HIGH_L2_G		002.102f
#define CON_HIGH_L3_G		002.090f
#define CON_HIGH_L4_G		002.090f

// Concrete_HighRange_Line_Offset X100
#define CON_HIGH_L1_O		969.117f
#define CON_HIGH_L2_O		-138.468f
#define CON_HIGH_L3_O		1735.559f
#define CON_HIGH_L4_O		-1813.253f


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

