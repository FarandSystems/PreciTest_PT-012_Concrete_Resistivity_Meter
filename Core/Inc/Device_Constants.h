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
//Serial No.: 01201
//Date/Time: 8/19/2025 5:16:57 PM

//Temperature Calibration Coefficients:
#define TEMP_G		000.081
#define TEMP_O		000.000

//Temperature Calibration Data Points:
//Physical Value (C)	Measured Levels
//015.00	
//030.00	


//Resistance Calibration Coefficients:

// Concrete_LowRange_Line_Gain X10000
#define CON_LOW_L1_G		001.134
#define CON_LOW_L2_G		001.004
#define CON_LOW_L3_G		000.997
#define CON_LOW_L4_G		001.004

// Concrete_LowRange_Line_Offset X10000
#define CON_LOW_L1_O		-924.714
#define CON_LOW_L2_O		-327.986
#define CON_LOW_L3_O		261.837
#define CON_LOW_L4_O		-7517.623

// Concrete_HighRange_Line_Gain X100
#define CON_HIGH_L1_G		002.224
#define CON_HIGH_L2_G		002.254
#define CON_HIGH_L3_G		002.270
#define CON_HIGH_L4_G		002.372

// Concrete_HighRange_Line_Offset X100
#define CON_HIGH_L1_O		1812.031
#define CON_HIGH_L2_O		094.630
#define CON_HIGH_L3_O		-1387.348
#define CON_HIGH_L4_O		-82371.742


//Resistance Calibration Data Points:
//Physical Value (Ohm)	Measured Levels
//000.97	9295 9437 9311 9617 9156 9100 9373 9407 
//002.20	20093 20381 20232 20226 20160 20245 20236 20206 
//005.09	50944 50837 50856 50938 51275 51442 51141 50564 
//010.00	100612 100110 98831 100204 98971 100038 99653 100627 
//020.80	208524 208212 208420 208029 207409 208531 208442 208505 
//050.40	505305 505229 504772 504957 505121 505274 504954 504397 
//100.00	1003931 1003681 1003411 1003606 1003518 1003588 1003624 1003546 
//203.00	2028953 2029732 2029788 2029585 2029461 2029869 2029769 2029815 
//203.00	8249 8261 8286 8482 8304 8341 8298 8289 
//500.00	21765 21660 21497 21754 21569 21870 21713 21527 
//998.90	43560 44512 44323 44419 44422 44203 44487 44313 
//1995.00	88726 88469 88480 88734 88787 87239 88626 88759 
//5138.00	227584 227359 227117 226642 227088 227398 227073 225599 
//9999.00	441235 441169 441186 441131 441096 441136 440937 441307 
//20070.00	880966 880562 880441 880861 880849 880772 880784 880771 
//52165.00	2232344 2233979 2233367 2234010 2233833 2233981 2234120 2233796 

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

