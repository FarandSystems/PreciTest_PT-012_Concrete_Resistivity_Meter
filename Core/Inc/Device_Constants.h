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
#define LATEST_PROJECT_INFO_FLASH_INDEX 1
#define DATA_BUFFER_FLASH_INDEX   				64

//Calibration Report for FW411 
//Serial No.: 01201
//Date/Time: 8/19/2025 5:16:57 PM

//Calibration Report for FW411 
//Serial No.: 255255255
//Date/Time: 8/19/2025 6:29:47 PM

//Temperature Calibration Coefficients:
#define TEMP_G		000.081
#define TEMP_O		000.000

//Temperature Calibration Data Points:
//Physical Value (C)	Measured Levels
//015.00	
//030.00	


//Resistance Calibration Coefficients:

// Concrete_LowRange_Line_Gain X10000
#define CON_LOW_L1_G		001.450
#define CON_LOW_L2_G		001.004
#define CON_LOW_L3_G		001.002
#define CON_LOW_L4_G		001.017

// Concrete_LowRange_Line_Offset X10000
#define CON_LOW_L1_O		-6096.400
#define CON_LOW_L2_O		-104.948
#define CON_LOW_L3_O		566.458
#define CON_LOW_L4_O		-12612.689

// Concrete_HighRange_Line_Gain X100
#define CON_HIGH_L1_G		002.499
#define CON_HIGH_L2_G		002.357
#define CON_HIGH_L3_G		002.320
#define CON_HIGH_L4_G		002.432

// Concrete_HighRange_Line_Offset X100
#define CON_HIGH_L1_O		-2825.026
#define CON_HIGH_L2_O		-3293.376
#define CON_HIGH_L3_O		-8072.217
#define CON_HIGH_L4_O		-101214.023


//Resistance Calibration Data Points:
//Physical Value (Ohm)	Measured Levels
//000.97	8232 10350 11532 11453 11634 11546 11460 10729 
//002.20	19459 19659 19274 19222 19249 19555 19274 19330 
//005.09	50858 50759 50809 50877 50777 50791 50902 50805 
//010.00	99853 99873 99890 99808 99599 99920 99929 99099 
//020.80	206693 205796 207342 207338 207166 207325 207288 207289 
//050.40	502950 502930 502515 502213 502126 502223 502370 502299 
//100.00	996133 995908 996105 995842 995960 996456 996293 995916 
//203.00	2009337 2009319 2009357 2008905 2008685 2009540 2009410 2009500 
//203.00	8440 8572 9373 9016 9326 11313 8816 9169 
//500.00	21317 21159 21336 21355 20827 21131 21350 20622 
//998.90	43458 43624 44477 43476 43765 43894 43892 43675 
//1995.00	85979 84897 86479 86176 86318 86317 86003 86223 
//5138.00	225140 224649 225206 224730 225017 224299 225163 225188 
//9999.00	433501 433918 435728 433966 433847 434919 435219 434345 
//20070.00	866640 866696 866988 867076 866885 866952 866708 866703 
//52165.00	2186409 2186621 2186453 2186708 2186640 2186073 2186411 2186493 


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

