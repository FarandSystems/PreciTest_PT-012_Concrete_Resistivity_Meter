
/* Includes ------------------------------------------------------------------*/


#include "LM35_Temperature_Measurement.h"

float temp_Deg_Display = 0;
float temp_Deg_Raw = 0;
float temp_Deg_Median = 0;
float temp_Deg_LPF = 0;
uint8_t temp_Filter_Counter = 0;
float medPrev[3];
uint16_t tempCounter = 0;
uint8_t medCounter = 0;
uint8_t Temp_Invalid_Flag = 0;
uint8_t temp_Filtering_Mode = 0;

void Apply_Median_On_Temperature(void);
void Caibrate_And_Filter_Temperature(void);


// Calibrate LM35 Sensor(convert Level to Degree)
// Calibrate &  Apply Low Pass Filter to omit noise then Apply High pass filter on low pass result to compensate speed by lead compensation.
void Caibrate_And_Filter_Temperature(void)
{
	//Calibrate temperature
	temp_Deg_Raw = (TEMP_G * ADC_ConvertedValue[1] + TEMP_O);
	
	//Apply Median_3
	Apply_Median_On_Temperature();
	
	
	// Apply a fast Low Pass Filter
	
	if(tempCounter <= 50)
	{
		tempCounter++;
		temp_Deg_LPF = temp_Deg_Median;
	}
	else
	{
		temp_Deg_LPF = (0.02f * temp_Deg_Median + 0.98f * temp_Deg_LPF) ;
	//temp_Deg_LPF = (1 * temp_Deg_Median + 0 * temp_Deg_LPF) ;
		Temp_Invalid_Flag = 1;
	}
	
	switch(temp_Filtering_Mode)
	{
		case 0 : // TEMP_FILTER_FAST
			if(Temp_Invalid_Flag == 1)
			{			
				temp_Deg_Display = temp_Deg_LPF;
				
				// Remain in fast filtering mode for 16 successive measurements
				temp_Filter_Counter ++;
				if(temp_Filter_Counter == 16)
				{
					temp_Filter_Counter = 0;
					temp_Filtering_Mode = 1; // TEMP_FILTER_STABLE
				}
			}
			break;
			
		case 1 : // TEMP_FILTER_STABLE
			//Apply another LPF, this one is much stronger
			temp_Deg_Display = 0.06f * temp_Deg_LPF + 0.94f * temp_Deg_Display;
			// If the stable filter output is more than 1.0 C different from the Fast filter output, then switch back to fast filtering mode
			if(fabs(temp_Deg_Display - temp_Deg_LPF) > 1.0)
			{
				temp_Filtering_Mode = 0; // TEMP_FILTER_FAST
			}
			break;
	}
	
}

void Apply_Median_On_Temperature(void)
{
	
	// Save Previous values of calibrated temperature
	medPrev[medCounter] = temp_Deg_Raw;
	medCounter++;
	if(medCounter == 3)
	{
		medCounter = 0;
	}
	
	if(medPrev[0] >= medPrev[1] && medPrev[1] >= medPrev[2])
	{
		temp_Deg_Median = medPrev[1];
	}
	if(medPrev[0] >= medPrev[2] && medPrev[2] >= medPrev[1])
	{
		temp_Deg_Median = medPrev[2];
	}
	if(medPrev[1] >= medPrev[0] && medPrev[0] >= medPrev[2])
	{
		temp_Deg_Median = medPrev[0];
	}
	if(medPrev[1] >= medPrev[2] && medPrev[2] >= medPrev[0])
	{
		temp_Deg_Median = medPrev[2];
	}
	if(medPrev[2] >= medPrev[1] && medPrev[1] >= medPrev[0])
	{
		temp_Deg_Median = medPrev[1];
	}
	if(medPrev[2] >= medPrev[0] && medPrev[0] >= medPrev[1])
	{
		temp_Deg_Median = medPrev[0];
	}	
}

