/* Includes ------------------------------------------------------------------*/

#include "ProcessData.h"

float samplePlus;
float sampleMinus;
float varMinus = 0.0f;	
float varPlus = 0.0f;

int bulk_cond_unit_PrefixNumber;
Measurement_Range_TypeDef measurementRange = HighRange;
float connection_Criteria = 0;
float average_Amplitude = 0;
float absolute_DC_Level = 0.0;
double bulk_Average = 0 ;

uint16_t resistance_Or_conductivityX10;
int resistance_conductivity_Range;
double bulk_Or_cond_Final = 0;
uint8_t fast_Output_Counter = 0;
double resistance_Ohm_Final = 0;
FinalShow_TypeDef OutputFilterOperation = FastOperation;
float probeTypeIndex = 1; //0 = 38mm 1 = 50mm 2 = Bulk
double resistivity_Final = 0;
float average_Amplitude_Temp_Corrected = 0.0f;

double resistance_Fast;
ForceRange_TypeDef  range_ForcingMode  = AutoRange;
float Ave_Plus = 0.0f;
float Ave_Minus = 0.0f;
float amplitude_RMS;
float displayedResistivity = 0.0f;
float displayed_Resistance_Or_Conductivity = 0.0f;
double RMS_Plus;
double RMS_Minus;
uint8_t change_sign_counter; 

uint32_t peak_of_median_signal = 0;
extern uint32_t peak_of_median_signal;

uint8_t temp_Correction_flag = 0;
uint8_t save_flag;
uint8_t hold_flag;
uint8_t createPrj_flag;

uint8_t first_time_disconnection_flag = 1;
uint8_t range_number = 0;

extern int32_t	signal_Captured [CAPTURE_COUNT][256] __attribute__((at(ADC_DATA_BASE_ADDRESS)));
float	signal_SubSampled_SpikeLess [CAPTURE_COUNT][256]__attribute__((at(CORRECTED_VALUE_DATA_BASE_ADDRESS)));
float	signal_SubSampled_LPF [CAPTURE_COUNT][256]__attribute__((at(FILTERED_VALUE_DATA_BASE_ADDRESS)));
float finalPoint[256]__attribute__((at(FINAL_POINT_DATA_BASE_ADDRESS)));
float signal_LPF_And_Cut[256]__attribute__((at(FILTERED_MEDIAN_LOWPASS_DATA_BASE_ADDRESS)));
float yN[256]__attribute__((at(YN_DATA_BASE_ADDRESS)));
float yP[256]__attribute__((at(YP_DATA_BASE_ADDRESS)));
float signal_Median[256]__attribute__((at(FILTERED_MEDIAN_DATA_BASE_ADDRESS)));

float Soil_Fast_disconnected_Criteria;
float Soil_Fast_disconnected_Criteria_Prev1;
float Soil_Fast_disconnected_Criteria_Prev2;

float Soil_Stable_disconnected_Criteria;
float Soil_Stable_disconnected_Criteria_Prev1;
float Soil_Stable_disconnected_Criteria_Prev2;

float Concrete_disconnect_Criteria;
float Concrete_disconnect_Criteria_Prev1;
float Concrete_disconnect_Criteria_Prev2;

float average_Amplitude_Temp_Corrected_Prev1;
float average_Amplitude_Temp_Corrected_Prev2;
float average_Amplitude_Temp_Corrected_Prev3;

Electrical_Connection_TypeDef electrical_Connection_Status = Disconnected;

void Process_Captured_Data(void);
void Omit_Far_Points(void);
void Apply_Median_Filter(void);
void Apply_LowPass_Filter_On_Signal_SubSampled(void);
void Apply_LowPass_Filter_And_Cut(void);
void GetSamplePlusAverage(void);
void GetSampleMinusAverage(void);
void Calculate_Resistivity(void);
void Display_Resistivity(float n);
void CalibrateResbyTemperature (void);
void Go_to_Middle_Range(void);
void Go_to_High_Range(void);
void Update_Temperature_Battery_Range_Panels(void);
void Display_Resistance(float n);
void Apply_Filter_After_N_Measurements(void);
void Calibrate_Resistance(void);
void Apply_Piecewise_Linear_for_Soil_LowRange(void);
void Apply_Piecewise_Linear_for_Concrete_LowRange(void);
void Apply_Piecewise_Linear_for_Soil_MiddleRange(void);
void Apply_Piecewise_Linear_for_Concrete_HighRange(void);
void Apply_Piecewise_Linear_for_Soil_HighRange(void);
void Select_Range(void);
void Calculate_Signal_Amplitude_Levels(void);
void Restart_Output_Filter(void);
void Check_Electrical_Connection(void);
void Correct_Sixteenth_Captured_Signal(void);
void Calculate_RMS_Amplitude(void);
void OmitSpikes(void);
void Calculate_RMS_Plus_Minus(void);

void Find_Peak(void);
	

// Signal Processing(Apply Several Filters)
void Process_Captured_Data(void)
{
	#ifdef CONCRETE
	if(CAPTURE_COUNT >= 16)
	{		
		Correct_Sixteenth_Captured_Signal();
	}
	#endif
  //Omit_Far_Points();
	OmitSpikes();
	Apply_LowPass_Filter_On_Signal_SubSampled();	
	Apply_Median_Filter();
	Apply_LowPass_Filter_And_Cut();	

	
	Calculate_Signal_Amplitude_Levels();
	

	Calibrate_Resistance();
	
	// Filter Resistance to get stable value
	Apply_Filter_After_N_Measurements();
	

	Calculate_Resistivity();

	// In the end check if operation range must be changed
	Select_Range();
	Check_Electrical_Connection();
	
}

void Calculate_Signal_Amplitude_Levels(void)
{
	GetSamplePlusAverage();
	GetSampleMinusAverage();
	
	//Calcute Average Amplitude of the Pulse Regarding Both Positive and Negative Cycles
	average_Amplitude = fabs((samplePlus - sampleMinus) / 2);
	absolute_DC_Level = (samplePlus + sampleMinus) / 2;
	
	//Calculate RMS of Signals
	Calculate_RMS_Amplitude();
}

// Remove Spikes using Average and Variance
// Samples/cycle = (1/97) * 20k = 207 samples
void Correct_Sixteenth_Captured_Signal(void)
{
//	for(uint16_t sample_Index = 0; sample_Index < 256; sample_Index++)
//	{
//		signal_Captured[15][sample_Index] = signal_Captured[14][sample_Index];
//	}
}

void Omit_Far_Points(void)
{
	float mean_SubSample = 0;
	float var_SubSample = 0;
	uint8_t mean_Capture_Count = 0;
	
	for (uint16_t sampleIndexF = 0; sampleIndexF < 256; sampleIndexF++)
	{
		mean_SubSample = 0;
		var_SubSample = 0;
		mean_Capture_Count = 0;	
		// Average between Sample0/sample1/.../sampleN from 32 captures
		// signal_Captured is ADC_Value
		
		for (uint16_t n = 0; n < CAPTURE_COUNT; n++)
		{
			if((signal_Captured[n][sampleIndexF] & 0x000000FF) == 0) //if it's not marked
			{
				mean_SubSample = mean_SubSample + signal_Captured[n][sampleIndexF];
				mean_Capture_Count ++;
			}
		}
		mean_SubSample = mean_SubSample / (float)mean_Capture_Count;
	
		// Calculate the Variance of the Sample0/sample1/.../sampleN from 32 captures
		for (uint16_t n = 0; n < CAPTURE_COUNT; n++)
		{
			var_SubSample = var_SubSample + (signal_Captured[n][sampleIndexF] - mean_SubSample)*(signal_Captured[n][sampleIndexF] - mean_SubSample);
		}
		var_SubSample = sqrt(var_SubSample / CAPTURE_COUNT);

		
		// Replace far samples with average by comparing  it to the whole captured signals at this same sampleIndexF
		for (uint16_t n = 0; n < CAPTURE_COUNT ; n++)
		{
			// if the difference between a sample and mean_SubSampleN value is more than 2*var_SubSampleN, it should be replaced with average value
			if (fabs(abs(signal_Captured[n][sampleIndexF]) - fabs(mean_SubSample)) > (1.0f * var_SubSample)) // A.N. it was 2 * Var and could not omit some spikes
			{
				// A.N. 		
				//signal has spike at this point 
				// it should be replaced with the calculated average across several aptures at this same sample      
				signal_SubSampled_SpikeLess[n][sampleIndexF] = mean_SubSample;					
				
				//To detect which sample is spike and ignore it for calculating mean_Subsampled
				signal_Captured[n][sampleIndexF] = signal_Captured[n][sampleIndexF] + 1; // we mark signal which has spike using 1
			}
			else
			{
				//signal value is correct so put it into output array
				signal_SubSampled_SpikeLess[n][sampleIndexF] = signal_Captured[n][sampleIndexF];
			}			
		}	
	}
}

// Returns the median of three floats (robust & branch-light).
static inline float median3f(float a, float b, float c)
{
    if (a > b) { float t = a; a = b; b = t; }
    if (b > c) { float t = b; b = c; c = t; }
    if (a > b) { float t = a; a = b; b = t; }
    return b;
}

void OmitSpikes(void)
{
    // ---- Pass 1: median filter along samples (wrap 256) for each capture ----
    for (uint16_t n = 0; n < CAPTURE_COUNT; n++)
    {
        for (uint16_t i = 0; i < 256; i++)
        {
            // Wrap indices for a 256-sample row
            uint16_t ip = (uint16_t)((i + 255u) & 0xFFu); // i-1
            uint16_t in = (uint16_t)((i + 1u)   & 0xFFu); // i+1

            float aa = signal_Captured[n][ip]; // prev
            float bb = signal_Captured[n][i];  // curr
            float cc = signal_Captured[n][in]; // next

            signal_SubSampled_SpikeLess[n][i] = median3f(aa, bb, cc);
        }
    }

    // ---- Pass 2: median filter across captures (wrap CAPTURE_COUNT) per sample ----
    for (uint16_t i = 0; i < 256; i++)
    {
        for (uint16_t n = 0; n < CAPTURE_COUNT; n++)
        {
            uint16_t np = (n == 0) ? (CAPTURE_COUNT - 1) : (uint16_t)(n - 1);
            uint16_t nn = (uint16_t)((n + 1u) % CAPTURE_COUNT);

            float aa = signal_SubSampled_SpikeLess[np][i]; // prev capture
            float bb = signal_SubSampled_SpikeLess[n][i];  // this capture
            float cc = signal_SubSampled_SpikeLess[nn][i]; // next capture

            // NOTE: This writes in-place like your original.
            // If you want "pure" two-pass behavior (no in-place dependency),
            // write to a secondary buffer here, then copy back after the loop.
            signal_SubSampled_SpikeLess[n][i] = median3f(aa, bb, cc);
        }
    }
}


void Apply_LowPass_Filter_On_Signal_SubSampled(void)
{
	for(uint16_t sampleIndexF = 0; sampleIndexF < 256; sampleIndexF++)
	{
		// LPF on signal_SubSampled_SpikeLess
		signal_SubSampled_LPF[0][sampleIndexF] = signal_SubSampled_SpikeLess[0][sampleIndexF];
		
		for (uint16_t captureIndexF = 1; captureIndexF < CAPTURE_COUNT; captureIndexF++)
		{
			signal_SubSampled_LPF[captureIndexF][sampleIndexF] = 0.9f * signal_SubSampled_LPF[captureIndexF - 1][sampleIndexF] + 0.1f * signal_SubSampled_SpikeLess[captureIndexF][sampleIndexF];			
		}
		// keep the stable value for the next measurement cycle to avoid start from 0 (it will achieve the final value faster)
		finalPoint[sampleIndexF] = signal_SubSampled_LPF[CAPTURE_COUNT - 1][sampleIndexF];
	}
}
// Apply Median_3 Filter on the samples of all CAPTURE_COUNT period because it is stable enough
void Apply_Median_Filter(void)
{
	float aa;
	float bb;
	float cc;
	
	for (uint16_t i = 0; i < 254; i++)
	{
		// Used last capture since it contains the result of LPF on sub_Sampled captures
		aa = signal_SubSampled_LPF[CAPTURE_COUNT - 1][i];
		bb = signal_SubSampled_LPF[CAPTURE_COUNT - 1][i + 1];
		cc = signal_SubSampled_LPF[CAPTURE_COUNT - 1][i + 2];

		if (aa <= bb && bb <= cc)
		{
			signal_Median[i] = bb;
		}
		else if (aa <= cc && cc <= bb)
		{
			signal_Median[i] = cc;
		}
		else if (bb <= aa && aa <= cc)
		{
			signal_Median[i] = aa;
		}
		else if (bb <= cc && cc <= aa)
		{
			signal_Median[i] = cc;
		}
		else if (cc <= aa && aa <= bb)
		{
			signal_Median[i] = aa;
		}
		else if (cc <= bb && bb <= aa)
		{
			signal_Median[i] = bb;
		}
	}
	signal_Median[254] = signal_Median[253];
	signal_Median[255] = signal_Median[253];
	
}

//Apply LPF On the samples of 32th period
void Apply_LowPass_Filter_And_Cut(void)
{	
	float meanN = 0;
	float meanP = 0;
	// Calculate initial value for LPF 
	//by averaging end part of positive or negative pulses (omit transients)
	for (uint16_t n = TIP_CUT_SAMPLE; n < TAIL_CUT_SAMPLE; n++)
	{
		meanN = meanN + signal_Median[n];
		meanP = meanP + signal_Median[n + (SAMPLES_PER_CAPTURE / 2)];
	}
	meanN = meanN / (TAIL_CUT_SAMPLE - TIP_CUT_SAMPLE);
	meanP = meanP / (TAIL_CUT_SAMPLE - TIP_CUT_SAMPLE);


	//Cut start of waveform
	for (uint16_t n = 0; n < (SAMPLES_PER_CAPTURE / 2) + 1; n++)
	{
		if(n < TIP_CUT_SAMPLE)
		{
			yN[n] = 0;
			yP[n] = 0;
		}
		
		// Apply LPF
		yN[TIP_CUT_SAMPLE - 1] = meanN;
	  yP[TIP_CUT_SAMPLE - 1] = meanP;
		
		if(TIP_CUT_SAMPLE <= n && n < TAIL_CUT_SAMPLE )
		{
			yN[n] = 0.8f * yN[n - 1] + 0.2f * signal_Median[n - 1];
			yP[n] = 0.8f * yP[n - 1] + 0.2f * signal_Median[n + (SAMPLES_PER_CAPTURE/2) - 1];
		}
		
		//Cut End of waveform
		if(TAIL_CUT_SAMPLE <= n)
		{
			yN[n] = 0;
			yP[n] = 0;
		}
	}

	for (uint16_t n = 0; n < (SAMPLES_PER_CAPTURE / 2) + 1; n++)
	{
		signal_LPF_And_Cut[n] = yN[n];
		signal_LPF_And_Cut[n + (SAMPLES_PER_CAPTURE / 2) + 1] = yP[n];
	}
	
	Calculate_RMS_Plus_Minus();
}	

void Calculate_RMS_Plus_Minus(void)
{
	double meanN = 0;
	double meanP = 0;

	change_sign_counter = 0;

	for (uint16_t n = TIP_CUT_SAMPLE; n < TAIL_CUT_SAMPLE; n++)
	{
		meanN = meanN + signal_LPF_And_Cut[n];
		meanP = meanP + signal_LPF_And_Cut[n + (SAMPLES_PER_CAPTURE / 2)];
	}
	meanN = meanN / (TAIL_CUT_SAMPLE - TIP_CUT_SAMPLE);
	meanP = meanP / (TAIL_CUT_SAMPLE - TIP_CUT_SAMPLE);
	

	for(uint16_t n = TIP_CUT_SAMPLE; n < TAIL_CUT_SAMPLE; n++)
	{
		if((signal_LPF_And_Cut[n + SAMPLES_PER_CAPTURE / 2] - meanP/2.0) * (signal_LPF_And_Cut[n+1 + SAMPLES_PER_CAPTURE / 2] - meanP/2.0) < 0)
			change_sign_counter++;
		if((signal_LPF_And_Cut[n] - meanN/2.0) * (signal_LPF_And_Cut[n+1] - meanN/2.0) < 0)
			change_sign_counter++;
	}
	
}
void Calculate_RMS_Amplitude(void)
{	
	float SumP2_Pluse = 0;
	float SumP2_Minus = 0;
	float DC = 256.0f * absolute_DC_Level;	

	for (uint16_t n = AVE_START_SAMPLE; n < AVE_END_SAMPLE; n++)
	{
		SumP2_Pluse += ((signal_LPF_And_Cut[n + SAMPLES_PER_CAPTURE / 2] - DC) * (signal_LPF_And_Cut[n + SAMPLES_PER_CAPTURE / 2] - DC));
	}
	
	for (uint16_t n = AVE_START_SAMPLE; n < AVE_END_SAMPLE; n++)
	{
		SumP2_Minus += ((signal_LPF_And_Cut[n] - DC) * (signal_LPF_And_Cut[n] - DC));
	}
	
	#ifdef SOIL // for measurment of upper than 500k, beacuse of exponential result we should calc resistivity in this way
		float y1M = signal_LPF_And_Cut[AVE_START_SAMPLE + 10] - DC;
		float y2M = signal_LPF_And_Cut[AVE_START_SAMPLE + 20] - DC;
		float y3M = signal_LPF_And_Cut[AVE_START_SAMPLE + 40] - DC;
		float y1P = signal_LPF_And_Cut[AVE_START_SAMPLE + 10 + SAMPLES_PER_CAPTURE / 2] - DC;
		float y2P = signal_LPF_And_Cut[AVE_START_SAMPLE + 20 + SAMPLES_PER_CAPTURE / 2] - DC;
		float y3P = signal_LPF_And_Cut[AVE_START_SAMPLE + 40 + SAMPLES_PER_CAPTURE / 2] - DC;
		if((y3M / y1M) > 1.5f || (y3P / y1P) > 1.5f)
		{
			SumP2_Pluse = ((y2M*y2M - y1M*y3M)/(2*y2M - (y1M + y3M))) * ((y2M*y2M - y1M*y3M)/(2*y2M - (y1M + y3M)));
			SumP2_Minus = ((y2P*y2P - y1P*y3P)/(2*y2P - (y1P + y3P))) * ((y2P*y2P - y1P*y3P)/(2*y2P - (y1P + y3P)));
			SumP2_Pluse = SumP2_Pluse * (AVE_END_SAMPLE - AVE_START_SAMPLE);
			SumP2_Minus = SumP2_Minus * (AVE_END_SAMPLE - AVE_START_SAMPLE);
		}	
	#endif
	
	amplitude_RMS = sqrt((SumP2_Pluse + SumP2_Minus) /(2.0f * (AVE_END_SAMPLE - AVE_START_SAMPLE)));
	amplitude_RMS = amplitude_RMS / 256.0f;
}

void GetSamplePlusAverage(void)
{
	double Ave = 0;	
	float signal_LPF1;
	float signal_LPF2;
	float signal_LPF3;
	
	// Calculate average of a chosen portion after cut
	for (uint16_t n = AVE_START_SAMPLE; n < AVE_END_SAMPLE; n++)
	{
			Ave += signal_LPF_And_Cut[n + SAMPLES_PER_CAPTURE / 2];
	}
	Ave /= (AVE_END_SAMPLE - AVE_START_SAMPLE);

	
	if(probeTypeIndex == 2) // Use Exponential Approximation
	{
	 signal_LPF1 = signal_LPF_And_Cut[TAIL_CUT_SAMPLE + (SAMPLES_PER_CAPTURE / 2) - 5];
	 signal_LPF2 = signal_LPF_And_Cut[TAIL_CUT_SAMPLE + (SAMPLES_PER_CAPTURE / 2) - 20];
	 signal_LPF3 = signal_LPF_And_Cut[TAIL_CUT_SAMPLE + (SAMPLES_PER_CAPTURE / 2) - 40];
	 
	 if((signal_LPF1 + signal_LPF3 - 2.0f * signal_LPF2) > 1000)		 
	 {
			Ave = ((signal_LPF1 * signal_LPF3) - (signal_LPF2 * signal_LPF2)) / (signal_LPF1 + signal_LPF3 - 2.0f * signal_LPF2);
	 }
	 else
	 {
			for (uint16_t n = AVE_START_SAMPLE; n < AVE_END_SAMPLE; n++)
			{
					Ave += signal_LPF_And_Cut[n + SAMPLES_PER_CAPTURE / 2];
			}
			Ave /= (AVE_END_SAMPLE - AVE_START_SAMPLE);
	 }
	}


	// Find variance to detect unconnected and floating probe position	
	for (int n = AVE_START_SAMPLE; n < AVE_END_SAMPLE; n++)
	{
		varPlus += (signal_LPF_And_Cut[n + (SAMPLES_PER_CAPTURE / 2)] - Ave)*(signal_LPF_And_Cut[n + (SAMPLES_PER_CAPTURE / 2)] - Ave);
	}	
	varPlus = sqrt(varPlus / (AVE_END_SAMPLE - AVE_START_SAMPLE));
//	varPlus = varPlus / 256.0f;
	
	varPlus = (int)varPlus / 256.0f;
	samplePlus = (int)(Ave / 256.0f);
		
}

void GetSampleMinusAverage(void)
{
	double Ave = 0;
	float signal_LPF1;
	float signal_LPF2;
	float signal_LPF3;
	// Calculate average of a chosen portion after cut
	for (uint16_t n = AVE_START_SAMPLE; n < AVE_END_SAMPLE; n++)
	{
		Ave += signal_LPF_And_Cut[n];
	}
	
	Ave /= (AVE_END_SAMPLE - AVE_START_SAMPLE);
	
	
	 if(probeTypeIndex == 2)// Use Exponential Approximation
	 {
		 signal_LPF1 = signal_LPF_And_Cut[TAIL_CUT_SAMPLE - 5];
		 signal_LPF2 = signal_LPF_And_Cut[TAIL_CUT_SAMPLE - 20];
		 signal_LPF3 = signal_LPF_And_Cut[TAIL_CUT_SAMPLE - 40];
		 
		 
		 if((signal_LPF1 + signal_LPF3 - 2.0f * signal_LPF2) > 1000)		 
		 {
				Ave_Minus = ((signal_LPF1 * signal_LPF3) - (signal_LPF2 * signal_LPF2)) / (signal_LPF1 + signal_LPF3 - 2.0f * signal_LPF2);
		 }
		 else
		 {
				for (uint16_t n = AVE_START_SAMPLE; n < AVE_END_SAMPLE; n++)
				{
					Ave += signal_LPF_And_Cut[n];
				}
				
				Ave /= (AVE_END_SAMPLE - AVE_START_SAMPLE);
		 }
	 }

	
	// Find variance to detect unconnected and floating probe position		
	for (uint16_t n = AVE_START_SAMPLE; n < AVE_END_SAMPLE; n++)
	{
		varMinus += (signal_LPF_And_Cut[n] - Ave) * (signal_LPF_And_Cut[n] - Ave);
	}	
	varMinus =  sqrt(varMinus / (AVE_END_SAMPLE - AVE_START_SAMPLE));
//	varMinus = varMinus / 256.0f;
	
	varMinus = (int)varMinus / 256.0f;
	sampleMinus = (int)(Ave / 256.0f);

}

void Apply_Piecewise_Linear_for_Concrete_LowRange(void)
{
	// Use piecewise linear approxilation for the following ranges of ADC level:
	// 0 ... 17500
	// 17500 ... 75000
	// 75000 ... 375000
	// 375000 ... more
	
	if(( 0 <= average_Amplitude_Temp_Corrected) && (average_Amplitude_Temp_Corrected < 35000))
	{
		resistance_Fast = 0.0001f * (CON_LOW_L1_G * average_Amplitude_Temp_Corrected  + CON_LOW_L1_O);
		range_number = 1;
	}
	if((35000 <= average_Amplitude_Temp_Corrected) && (average_Amplitude_Temp_Corrected < 150000))
	{
		resistance_Fast = 0.0001f * (CON_LOW_L2_G * average_Amplitude_Temp_Corrected  + CON_LOW_L2_O);
		range_number = 2;
	}
	if((150000 <= average_Amplitude_Temp_Corrected) && (average_Amplitude_Temp_Corrected < 750000))
	{
		resistance_Fast = 0.0001f * (CON_LOW_L3_G * average_Amplitude_Temp_Corrected  + CON_LOW_L3_O);
		range_number = 3;
	}
	if(750000 <= average_Amplitude_Temp_Corrected)
	{
		resistance_Fast = 0.0001f * (CON_LOW_L4_G * average_Amplitude_Temp_Corrected  + CON_LOW_L4_O);
		range_number = 4;
	}
}
void Apply_Piecewise_Linear_for_Concrete_HighRange(void)
{
	// Use piecewise linear approxilation for the following ranges of ADC level:
	// 0 ... 37500
	// 37500 ... 175000
	// 175000 ... 750000
	// 750000 ... more
	
	if((0 <= average_Amplitude_Temp_Corrected) && (average_Amplitude_Temp_Corrected < 35000))
	{
		resistance_Fast = 0.01f * (CON_HIGH_L1_G * average_Amplitude_Temp_Corrected  + CON_HIGH_L1_O);
		range_number = 5;
	}
	if((35000 <= average_Amplitude_Temp_Corrected) && (average_Amplitude_Temp_Corrected < 150000))
	{
		resistance_Fast = 0.01f * (CON_HIGH_L2_G * average_Amplitude_Temp_Corrected  + CON_HIGH_L2_O);
		range_number = 6;
	}
	if((150000 <= average_Amplitude_Temp_Corrected) && (average_Amplitude_Temp_Corrected < 750000))
	{
		resistance_Fast = 0.01f * (CON_HIGH_L3_G * average_Amplitude_Temp_Corrected  + CON_HIGH_L3_O);
		range_number = 7;
	}
	if(750000 <= average_Amplitude_Temp_Corrected)
	{
		resistance_Fast = 0.01f * (CON_HIGH_L4_G * average_Amplitude_Temp_Corrected  + CON_HIGH_L4_O);
		range_number = 8;
	}
}

void Apply_Piecewise_Linear_for_Soil_LowRange(void)
{
	// Use a linear approxilation for all ranges of ADC level	
//	if(average_Amplitude_Temp_Corrected > 50000)
//	{
//		resistance_Fast =  (WEN_LOW_L1_G * average_Amplitude_Temp_Corrected  + WEN_LOW_L1_O);	
//	}
//	if(average_Amplitude_Temp_Corrected < 50000)
//	{
//		resistance_Fast = WEN_LOW_L1_G *( average_Amplitude_Temp_Corrected_Prev1 + average_Amplitude_Temp_Corrected_Prev2 + average_Amplitude_Temp_Corrected_Prev3)/3.0f + WEN_LOW_L1_O;
//	}
}
void Apply_Piecewise_Linear_for_Soil_MiddleRange(void)
{
	// Use a linear approxilation for all ranges of ADC level	
//	if(average_Amplitude_Temp_Corrected > 50000)
//	{
//		resistance_Fast =  (WEN_LOW_L1_G * average_Amplitude_Temp_Corrected  + WEN_LOW_L1_O);	
//	}
//	if(average_Amplitude_Temp_Corrected < 50000)
//	{
//		resistance_Fast = WEN_LOW_L1_G *( average_Amplitude_Temp_Corrected_Prev1 + average_Amplitude_Temp_Corrected_Prev2 + average_Amplitude_Temp_Corrected_Prev3)/3.0f + WEN_LOW_L1_O;
//	}
}

void Apply_Piecewise_Linear_for_Soil_HighRange(void)
{
	// Use a linear approxilation for all ranges of ADC level	
//	resistance_Fast =  (WEN_HIGH_L1_G * average_Amplitude_Temp_Corrected  + WEN_HIGH_L1_O);
}

void Calibrate_Resistance(void)
{		
	// Temp correction: Displayed Resistivity is compensated regarding sample temperature and the Calibrated value is calculated for 25 degrees centigrade.
	if(temp_Correction_flag == 1)
	{
		//amplitude_RMS ? your measured value at temp_Deg_Display °C.

		//(temp_Deg_Display - 23.0f) ? difference from the reference temperature.

		//0.02f ? ASTM coefficient (2 % per °C).

		//Divide by (1 + a?T) to normalize back to 23 °C.
		average_Amplitude_Temp_Corrected = amplitude_RMS / (1.0f + 0.02f * (temp_Deg_Display - 23.0f));
	}
	else
	{
		average_Amplitude_Temp_Corrected =  amplitude_RMS;
	}
	
	// Calculate resistivity and bulk resistance according to probe size(38mm/50mm)
	//SOIL : Ladder resistance_Fast function is used to fix final resistance_Fast value after 5 seconds
	if(measurementRange == LowRange)
	{
		Apply_Piecewise_Linear_for_Concrete_LowRange();	
	}	
	if(measurementRange == MiddleRange)
	{
//		#ifdef CONCRETE			
//			Apply_Piecewise_Linear_for_Concrete_LowRange();	
//			
//		#endif

//		#ifdef SOIL
//			
//			Apply_Piecewise_Linear_for_Soil_MiddleRange();	

//		#endif
	}	
	if(measurementRange == HighRange)
	{
		#ifdef CONCRETE
		
			Apply_Piecewise_Linear_for_Concrete_HighRange();
			
		#endif

		#ifdef SOIL				
		
			Apply_Piecewise_Linear_for_Soil_HighRange();
		
		#endif
	}		
}

void Select_Range(void)
{	
	Measurement_Range_TypeDef measurementRange_Prev = measurementRange;
	
	// Select Range
	switch (range_ForcingMode)
	{		
		case AutoRange:								
		
			//Select Operating Range based on signal level
//			if(average_Amplitude_Temp_Corrected < LOW_RANGE_THERESHOLD && measurementRange == MiddleRange)
//			{
//				Go_to_Low_Range();
//			}
//			if(average_Amplitude_Temp_Corrected < LOW_RANGE_THERESHOLD && measurementRange == LowRange)
//			{
//				Go_to_Middle_Range();
//			}
			#ifdef SOIL
			switch(measurementRange)
			{
				case HighRange:
					if(average_Amplitude_Temp_Corrected < SOIL_HIGH_RANGE_LOW_THERESHOLD)
					{
						Go_to_Middle_Range();
					}
					break;
					
				case MiddleRange:
					if(average_Amplitude_Temp_Corrected > SOIL_MIDDLE_RANGE_HIGH_THERESHOLD ||absolute_DC_Level > 2E6f) // This happens when the system is in low range and a high resistance is connected
					{
						Go_to_High_Range();
					}
					if(average_Amplitude_Temp_Corrected < SOIL_MIDDLE_RANGE_LOW_THERESHOLD ) 
					{
						Go_to_Low_Range();
					}
					
					break;
					
				case LowRange:
						
					break;
			}
			#endif
			
			#ifdef CONCRETE
			switch(measurementRange)
			{
				case HighRange:
					if(average_Amplitude_Temp_Corrected < CONCRETE_LOW_RANGE_THERESHOLD)
					{
						Go_to_Low_Range();
					}
			case LowRange:
					if(average_Amplitude_Temp_Corrected > CONCRETE_HIGH_RANGE_THERESHOLD ||absolute_DC_Level > 2E6f) // This happens when the system is in low range and a high resistance is connected
					{
						Go_to_High_Range();
					}
					break;
		  }
			
			#endif
			break;
			
		case Force_MiddleRange:						
				Go_to_Middle_Range();
			break;
		
		case Force_HighRange:			
				Go_to_High_Range();
			break;
		
		case Force_LowRange:
				Go_to_Low_Range();
			break;
	}
	
	// If Measurement Range has changed restart output filter
	if(measurementRange != measurementRange_Prev)
	{		
		Restart_Output_Filter(); // Go to fast tracking for 8 measurements and then apply strong filter
	}
}
void Calculate_Resistivity(void)
{	
	// Calculate resistivity and bulk resistance according to probe size(38mm/50mm)
	// in SOIL  Ladder resistance_Fast function is used to fix final resistance_Fast value after 5 seconds
	#ifdef CONCRETE

		if(probeTypeIndex == 0)
		{
			resistivity_Final = 2.0f * PI_NUMBER * 3.8f * resistance_Ohm_Final;		
		}
		if(probeTypeIndex == 1)
		{
			resistivity_Final = 2.0f * PI_NUMBER * 5.0f * resistance_Ohm_Final;		
		}
		
		//BULK resistance		
		bulk_Or_cond_Final = resistance_Ohm_Final; // in CONCRETE Bulk Resistance is shown in second display
		
	#endif

	#ifdef SOIL
		// resistivity = resistance x A/L  ,  A/L = 1
		resistivity_Final = resistance_Ohm_Final; // Since A/L = 1cm, resistivity equals resistance (Ohm.cm)
		
		//Conductivity
		bulk_Or_cond_Final = 100.0/resistance_Ohm_Final; // in SOIL Conductivity is shown in S/m on second display

	#endif
		
}

void Go_to_Middle_Range(void)
{
	measurementRange = MiddleRange;
	HAL_GPIO_WritePin(RANGE_RELAY_GPIO_Port , RANGE_RELAY_Pin, GPIO_PIN_RESET);		
	
	HAL_GPIO_WritePin(HIGH_CURRENT_EN_GPIO_Port, HIGH_CURRENT_EN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(HIGH_CURRENT_NOTEN_GPIO_Port, HIGH_CURRENT_NOTEN_Pin, GPIO_PIN_SET);
}

void Go_to_High_Range(void)
{
	measurementRange = HighRange;	
	HAL_GPIO_WritePin(RANGE_RELAY_GPIO_Port , RANGE_RELAY_Pin, GPIO_PIN_SET);	
	
	HAL_GPIO_WritePin(HIGH_CURRENT_EN_GPIO_Port, HIGH_CURRENT_EN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(HIGH_CURRENT_NOTEN_GPIO_Port, HIGH_CURRENT_NOTEN_Pin, GPIO_PIN_SET);	
}
void Go_to_Low_Range(void)
{
	measurementRange = LowRange;	
	HAL_GPIO_WritePin(RANGE_RELAY_GPIO_Port , RANGE_RELAY_Pin, GPIO_PIN_RESET);		
	HAL_GPIO_WritePin(HIGH_CURRENT_EN_GPIO_Port, HIGH_CURRENT_EN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(HIGH_CURRENT_NOTEN_GPIO_Port, HIGH_CURRENT_NOTEN_Pin, GPIO_PIN_RESET);
}
void Apply_Filter_After_N_Measurements(void)
{ 
	// For N measurements just output the result and do not filter it in time
	// After N measurements apply a strg filter
	
	switch(OutputFilterOperation)
	{
		case FastOperation:
			// the probe is connected and the system remains in agile operation for 16 Measurements 
			// unless disconnecting the probe keeps the system in FastOperation
			 hold_flag = 0;
			 resistance_Ohm_Final = resistance_Fast;	
//			resistance_Ohm_Final = 0.5f * resistance_Fast + 0.5f * resistance_Ohm_Final;		
			
			// Count to keep the system agile for 16 Measurements
			fast_Output_Counter++;
			if(fast_Output_Counter == 16)
			{
				fast_Output_Counter = 0;				
				OutputFilterOperation = StableOperation;
				hold_flag = 1;
			}
			break;
			
		case StableOperation:
			// Apply strong filter 8 Measurements after probe connection			
				resistance_Ohm_Final = 0.02f * resistance_Fast + 0.98f * resistance_Ohm_Final;	
//			resistance_Ohm_Final = 0.01f * resistance_Fast + 0.99f * resistance_Ohm_Final;	
			//resistance_Ohm_Final = 0.1f * resistance_Fast + 0.9f * resistance_Ohm_Final;		
			break;			
	}			
}

void Find_Peak(void)
{
	int32_t plus_peak = -1e7;
	
	int32_t minus_peak = 1e7;
	
	for (int i =0; i < 255; i++)
	{
		if(signal_Median[i] > plus_peak)
		{
			plus_peak = signal_Median[i];
		}
		
		if(signal_Median[i] < minus_peak)
		{
			minus_peak = signal_Median[i];
		}
	}
	
	peak_of_median_signal = fabs((plus_peak - minus_peak) / 2.0f) / 256.0f;
	
}

void Check_Electrical_Connection(void)
{	
	// Save Previous values bofore updating
	average_Amplitude_Temp_Corrected_Prev3 = average_Amplitude_Temp_Corrected_Prev2;
	average_Amplitude_Temp_Corrected_Prev2 = average_Amplitude_Temp_Corrected_Prev1;
	average_Amplitude_Temp_Corrected_Prev1 = average_Amplitude_Temp_Corrected;
	
	Soil_Fast_disconnected_Criteria_Prev2 = Soil_Fast_disconnected_Criteria_Prev1;
	Soil_Fast_disconnected_Criteria_Prev1 = Soil_Fast_disconnected_Criteria;
	Soil_Fast_disconnected_Criteria = (varPlus + varMinus) * 100.0f / (2.0f * average_Amplitude_Temp_Corrected);
	
	Soil_Stable_disconnected_Criteria_Prev2 = Soil_Stable_disconnected_Criteria_Prev1;
	Soil_Stable_disconnected_Criteria_Prev1 = Soil_Stable_disconnected_Criteria;
	Soil_Stable_disconnected_Criteria = fabs(resistance_Ohm_Final - resistance_Fast);
	
	Concrete_disconnect_Criteria_Prev2 = Concrete_disconnect_Criteria_Prev1;
	Concrete_disconnect_Criteria_Prev1 = Concrete_disconnect_Criteria;
	
	Find_Peak();
	if (average_Amplitude_Temp_Corrected > 3e6)
	{
		Concrete_disconnect_Criteria = 2;//peak_of_median_signal / average_Amplitude_Temp_Corrected;//(varPlus + varMinus)*100/(2*amplitude_RMS);
	}
	else
	{
		Concrete_disconnect_Criteria = 1;
	}
	
	
	//Concrete_disconnect_Criteria = (varPlus + varMinus) * 100.0f / (2.0f * amplitude_RMS);	

	electrical_Connection_Status = Connected;			

	 // Assume electrical connection is OK, then check for conditions which show it is not
	
	#ifdef SOIL
		// in Soil disconnection may occur as a result of disconnection of just one cable
		// in which case the resistance_Fast and the final filtered value make large differences
		// this condition can be checked to see if a disconnection happens after a full setup
	
	if(OutputFilterOperation == StableOperation)
	{
		if(( Soil_Stable_disconnected_Criteria > 0.02 * resistance_Fast)//0.02 
			&&(Soil_Stable_disconnected_Criteria_Prev1 > 0.02 *  resistance_Fast)
			&&(Soil_Stable_disconnected_Criteria_Prev2 > 0.02 * resistance_Fast))
			{			
				electrical_Connection_Status = Disconnected;						
			}
	}
//	else
//	{	
		//Disconnect after 3 Check
//		if(((Soil_Fast_disconnected_Criteria) > 10 && (Soil_Fast_disconnected_Criteria_Prev1 > 10) && (Soil_Fast_disconnected_Criteria_Prev2 > 10))
		if(((Soil_Fast_disconnected_Criteria) > 15 && (Soil_Fast_disconnected_Criteria_Prev1 > 15) && (Soil_Fast_disconnected_Criteria_Prev2 > 15))
			|| ((average_Amplitude_Temp_Corrected_Prev1 < 100) && (average_Amplitude_Temp_Corrected_Prev2 < 100) && (average_Amplitude_Temp_Corrected_Prev3 < 100))
			)
		{
			
			electrical_Connection_Status = Disconnected;
		}
//		else if(((average_Amplitude_Temp_Corrected_Prev1 < 100) && (average_Amplitude_Temp_Corrected_Prev2 < 100) && (average_Amplitude_Temp_Corrected_Prev3 < 100)))
//		{
//			
//			electrical_Connection_Status = Disconnected;
//		}
//	}
		
	#endif
	
	#ifdef CONCRETE
		// in Concrete Resistibvity Meter, disconnection may occur as a result of  just one pin disconnection 
		// in which case the resistance_Fast and the final filtered value make large differences
		// this condition can be checked to see if a disconnection happens after a full setup
	
//	if(OutputFilterOperation == StableOperation) 
//	{
		// In Stable Operation the strong LPF is active and if the disconnect criteria happens to be under DISCONNECTED_THERESHOLD 
		// for three consecutive measurements, assume that the probe is disconnected, this id don to filter out instant diconection 
		if((Concrete_disconnect_Criteria > DISCONNECTED_THERESHOLD)
			&&
			(Concrete_disconnect_Criteria_Prev1 > DISCONNECTED_THERESHOLD)
			&&
			(Concrete_disconnect_Criteria_Prev2 > DISCONNECTED_THERESHOLD))
			{
				electrical_Connection_Status = Disconnected;
				first_time_disconnection_flag = 0;		
			}
		if(first_time_disconnection_flag == 1)
			{				
				if((Concrete_disconnect_Criteria < LOWER_DISCONNECTED_THERESHOLD)
				&&
				(Concrete_disconnect_Criteria_Prev1 < LOWER_DISCONNECTED_THERESHOLD)
				&&
				(Concrete_disconnect_Criteria_Prev2 < LOWER_DISCONNECTED_THERESHOLD))
				{
					electrical_Connection_Status = Disconnected;					
				}
				
			}
		if(change_sign_counter > 2)
		{
			//electrical_Connection_Status = Disconnected;
		}
//	}
//	else
//	{
//		// again (for the sake of compliance with Soil)
//		// In Fast Operation the weak LPF is active and if the disconnect criteria happens to be under DISCONNECTED_THERESHOLD 
//		// for three consecutive measurements, assume that the probe is disconnected, this id don to filter out instant diconection 
//		if((Concrete_disconnect_Criteria > DISCONNECTED_THERESHOLD)
//			&&
//			(Concrete_disconnect_Criteria_Prev1 > DISCONNECTED_THERESHOLD)
//			&&
//			(Concrete_disconnect_Criteria_Prev2 > DISCONNECTED_THERESHOLD))
//			{
//				electrical_Connection_Status = Disconnected;
//			}		
//	}
		
	#endif
		
	if(electrical_Connection_Status == Disconnected)
	{
		
		Restart_Output_Filter(); //Go to Fast Operation and Don't apply Output Filter
	}


	
}

void Restart_Output_Filter(void)
{
	//Restart_Output_Filter
	fast_Output_Counter = 0;	
	OutputFilterOperation = FastOperation;	
}
void Restart_Output_Filter_in_Save_And_CreatingProj(void)
{
	//Restart_Output_Filter
	if(measurementRange == MiddleRange)
	{
		fast_Output_Counter = 8;	
	}
	else if(measurementRange == HighRange)
	{
		fast_Output_Counter = 4;	
	}
	OutputFilterOperation = FastOperation;
}

