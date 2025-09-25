

#include "PowerManagement.h"
#define v_Battery_filter_count  50


uint8_t battery_Level_is_OK = 1;
uint16_t v_Input_Power_Levels;

uint16_t v_Bat_Levels = 0;
float	v_Bat_Volts = 0;
float v_Bat_Volts_Filtered = 0;
float v_Bat_Percentage = 0;

float latest_v_Bat_Array[v_Battery_filter_count];
uint8_t vBat_Get_Percentage(float vBat);

uint16_t battery_Voltage_Filter_Counter;
Voltage_Filter_Mode_TypeDef 	v_Bat_Voltage_Filter_Mode = Fast_Operation;

float	v_Input_Power_Volts = 0;
float v_Input_Power_Volts_Filtered = 0;
uint16_t input_Power_Voltage_Filter_Counter;
Voltage_Filter_Mode_TypeDef 	input_Power_Voltage_Filter_Mode = Fast_Operation;

uint16_t v_Input_Jack_Charger_Levels =0;
float	v_Input_Jack_Charger_Volts = 0;
float v_Input_Jack_Charger_Volts_Filtered = 0;
uint16_t input_Jack_Charger_Voltage_Filter_Counter;
Voltage_Filter_Mode_TypeDef 	input_Jack_Charger_Voltage_Filter_Mode = Fast_Operation;


static const float vBatLUT_Volts[] = {
    3.00f, 3.20f, 3.40f, 3.50f, 3.60f,
    3.70f, 3.75f, 3.80f, 3.85f, 3.90f,
    3.95f, 4.00f, 4.05f, 4.10f, 4.20f
};

static const uint8_t vBatLUT_Percent[] = {
      0,   5,  10,  20,  30,
     40,  50,  60,  70,  75,
     80,  85,  90,  95, 100
};




float I_Charge_mA;

// Power LED 
uint8_t power_LED_Blink_Mode = 0;
uint16_t power_LED_Color = Farand_GRN;
uint8_t  power_LED_ON_OFF;

Charging_Mode_TypeDef Charging_Mode = UNPLUGGED;

void Check_Power_And_Charging(void);
void Apply_LPF_On_V_Battery(void);
void Apply_LPF_On_Input_Power(void);
void Apply_LPF_On_Input_Jack_Charger(void);
void VBat_Push(float *arr, size_t n, float new_sample);
float Apply_Median_Average_On_V_Battery(const float *arr, size_t n,
                                          size_t trim_low, size_t trim_high);

uint8_t charge_pin_val;
uint8_t full_charge_pin_val;
uint8_t charge_pin_val_prev;
uint8_t full_charge_pin_val_Prev;

void Check_Charging_Mode(void)
{
	static uint8_t stable_counter = 0;
	
	charge_pin_val_prev = charge_pin_val;
	//full_charge_pin_val_Prev = full_charge_pin_val;
	charge_pin_val = HAL_GPIO_ReadPin(Charging_GPIO_Port,Charging_Pin);
	full_charge_pin_val = HAL_GPIO_ReadPin(Full_Charge_GPIO_Port,Full_Charge_Pin);
	
	if(charge_pin_val == charge_pin_val_prev)
	{
		stable_counter++;
	}
	else
	{
		stable_counter = 0;
	}
	
	if(stable_counter == 2)
	{
		stable_counter = 0;
		if(charge_pin_val == GPIO_PIN_SET && full_charge_pin_val == GPIO_PIN_SET)
		{
			Charging_Mode = CHARGING;
			v_Bat_Voltage_Filter_Mode = Fast_Operation;
		}
		else if(full_charge_pin_val == GPIO_PIN_RESET)
		{
			Charging_Mode = FULL_CHARGE;
			v_Bat_Voltage_Filter_Mode = Fast_Operation;
		}
		else if(charge_pin_val == GPIO_PIN_RESET)
		{
			Charging_Mode = UNPLUGGED;
		}	
	}
}
void Set_LED_Color(void)
{
	if( v_Bat_Volts_Filtered >= 3.0f)
	{
		if(Charging_Mode == FULL_CHARGE)
		{
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);//ON
			HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
		}
		else if(Charging_Mode == CHARGING)
		{
			//HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);//ON
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
		}
		else if(Charging_Mode == UNPLUGGED)
		{
			HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);//ON
			//HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);

		}
	}
	else
	{
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
		if(Charging_Mode == CHARGING)
		{
			//HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		}
		else if(Charging_Mode == UNPLUGGED)
		{
			//HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		}
	}	
}

// Linear interpolation between LUT points
uint8_t vBat_Get_Percentage(float vBat)
{
    if (vBat <= vBatLUT_Volts[0])
        return 0;
    if (vBat >= vBatLUT_Volts[14])
        return 100;

    for (int i = 0; i < 14; i++)
    {
        if (vBat >= vBatLUT_Volts[i] && vBat <= vBatLUT_Volts[i+1])
        {
            float x0 = vBatLUT_Volts[i];
            float x1 = vBatLUT_Volts[i+1];
            float y0 = vBatLUT_Percent[i];
            float y1 = vBatLUT_Percent[i+1];

            float t = (vBat - x0) / (x1 - x0);
            return (uint8_t)(y0 + t * (y1 - y0));
        }
    }
    return 0; // fallback
}

void Check_Power_And_Charging(void)
{
	static uint8_t waiting_counter = 0;
	
	/* The system has 2 operating modes
	1 - Normal Operation
	2 - Charging Operation

	in charging mode same colors are used but the LED blinks

	*/	
	
	// ADC_ConvertedValue[0] -> Internal Battery Sense , internal Li-Polymer (3.7V) sense

	Check_Charging_Mode();
	
	waiting_counter++;
	if(waiting_counter > 50)
	{		
		Set_LED_Color();
	}
	
	#ifdef POWER_IN_5V_WITH_Li_ION_INTERNAL_BATTERY
		
		v_Bat_Levels = ADC_ConvertedValue[0]; // Read Li-Polymer battery voltage			
		//v_Input_Power_Levels = ADC_ConvertedValue[1]; // Read Input Power Voltage
		//v_Input_Jack_Charger_Levels = ADC_ConvertedValue[2]; // Read Input Jack Charger
		
		
		// Read and Filter Battery Voltage (+12V Battery input on P4 Connector)
		// Voltage Calibration 
		// v = level * (Vref / 4095) * (Vin/Vo), Vin/Vo = divider_Ratio
		// Vin: Volage to be measured
		// Vo: Voltage output of the resistor divider
//		v_Bat_Volts = 1.190f *((v_Bat_Levels * V_REF)/(4095.0f * DIVIDER_INTERNAL_BATTERY_RATIO)); // modify coef = 1.190f 
		v_Bat_Volts = 3 + (4.2 - 3) * (v_Bat_Levels - 2270) / (3333 - 2270); // Measured Values
		//v_Input_Power_Volts = ((v_Input_Power_Levels * V_REF)/(4095.0f * DIVIDER_INTERNAL_BATTERY_RATIO)); 
		//v_Input_Jack_Charger_Volts = 1.190f*((v_Input_Jack_Charger_Levels * V_REF)/(4095.0f * DIVIDER_INPUT_JACK_CHARGER_RATIO)); 
	
		VBat_Push(latest_v_Bat_Array, v_Battery_filter_count, v_Bat_Volts);
		v_Bat_Volts_Filtered = Apply_Median_Average_On_V_Battery(latest_v_Bat_Array, v_Battery_filter_count, 10, 10); //trim 10
		
//		Apply_LPF_On_V_Battery();
//		Apply_LPF_On_Input_Power();
//		Apply_LPF_On_Input_Jack_Charger();
	
//		I_Charge_mA = 1000.0f * (v_Input_Power_Levels - v_Bat_Levels)/CURRENT_SENSE_RESISTOR;
		
		// Calculate and bound Battery Percentage
//		v_Bat_Percentage = (v_Bat_Volts_Filtered - 3.0f) * 100.0f / (4.2f - 3.0f);
			v_Bat_Percentage = vBat_Get_Percentage(v_Bat_Volts_Filtered);
		
		if(v_Bat_Percentage > 100)
		{
			v_Bat_Percentage = 100;
		}
		
		if(v_Bat_Percentage < 0)
		{
			v_Bat_Percentage = 0;
		}
		
		v_Bat_Percentage = (int)((v_Bat_Percentage + 5) / 5.0f) * 5; // 5% increment in bettery percent
		
		if(v_Bat_Percentage < 0 )
		{
			v_Bat_Percentage = 0;
		}
		if(v_Bat_Percentage > 100)
		{
			v_Bat_Percentage = 100;
		}

		
/*
//		// if charging is in progress, Blink otherwise don't
//		if( I_Charge_mA > 10)
//		{
//			power_LED_Blink_Mode = 1; // Blink
//		}
//		else
//		{
//			power_LED_Blink_Mode = 0; // Don't Blink
//		}
	
		
		// Check Battery Level ( or charge current), and determine LED color, by applying 0.1V ( or 20mA ) hysteresis
		// to avoid color instability in voltage thresholds
		
		// if charging is in progress ( v_Input_Jack_Charger_Volts > 0), LED colors are determined by charge current
		// if the device is not connectod to charger, LED colors are determined by battery voltage
		if( v_Input_Jack_Charger_Volts > 3)
		{
			// the device is getting charged
			switch(power_LED_Color)
			{
				case Farand_RED:
					if( I_Charge_mA < 100)
					{						
						power_LED_Color = Farand_ORG;	
					}
					break;
				case Farand_ORG:
					if( v_Bat_Volts_Filtered > 120)
					{											
						power_LED_Color = Farand_RED;		
					}
					else if(v_Bat_Volts_Filtered < 80)
					{						
						power_LED_Color = Farand_GRN;	
					}					
					break;
				case Farand_GRN:
					if( v_Bat_Volts_Filtered > 100)
					{						
						power_LED_Color = Farand_ORG;		
					}
					break;
			}	

			battery_Level_is_OK = 1; // Let the device turn on anyway, reguardless of charging level
			
		}
		else
		{
			// The device is not connected to charger and is in normal operation
			
			switch(power_LED_Color)
			{
				case Farand_RED:
					if( v_Bat_Volts_Filtered > 3.5f)
					{
						power_LED_Color = Farand_ORG;	
					}
					break;
				case Farand_ORG:
					if( v_Bat_Volts_Filtered < 3.4f)
					{										
						power_LED_Color = Farand_RED;		
					}
					else if(v_Bat_Volts_Filtered > 3.8f)
					{
						power_LED_Color = Farand_GRN;	
					}					
					break;
				case Farand_GRN:
					if( v_Bat_Volts_Filtered <3.7f)
					{
						power_LED_Color = Farand_ORG;		
					}
					break;
			}		
			
			
			if( v_Bat_Volts_Filtered >= 3.2f)
			{
				battery_Level_is_OK = 1;
			}
			else
			{
				battery_Level_is_OK = 0; // if the voltage is under 11V don't let the device turn ON
			}	
			
		}
*/			

	#endif	
	
	#ifdef POWER_IN_5V_WITH_NO_INTERNAL_BATTERY
		
//		v_Input_Jack_Charger_Levels = ADC_ConvertedValue[2]; // Read input Jack charger		

		// Read and Filter Input Jack Charger Voltage (+12V Battery input on P3 Connector)
		// Voltage Calibration 
			// v = level * (Vref / 4095) * (Vin/Vo), Vin/Vo = divider_Ratio
			// Vin: Volage to be measured
			// Vo: Voltage output of the resistor divider
		v_Input_Jack_Charger_Volts = ((v_Input_Jack_Charger_Levels * V_REF)/(4095.0 * DIVIDER_INPUT_JACK_CHARGER_RATIO)); //
		Apply_LPF_On_Input_Jack_Charger();
	
		// Calculate and bound Battery Percentage
		if(v_Input_Jack_Charger_Volts > 4.5)
		{
			v_Bat_Percentage = 101; // Code to signal DISCO board not to show battery icon and battery percentage
		}
		else
		{
			v_Bat_Percentage = 0;
		}

		power_LED_Blink_Mode = 0; // Don't Blink
	
		
		// determine LED color
		power_LED_Color = Farand_GRN;	
		
		battery_Level_is_OK = 1;
			
		power_LED_ON_OFF = 1; // The power LED is always ON when the input jack is connected to supply the device with external power	
		
	#endif	
		
	//Set_LED_Parameters(power_LED_ON_OFF, power_LED_Blink_Mode, power_LED_Color);
}

void Apply_LPF_On_V_Battery(void)
{	
	// Apply Fast or Stable Filtering on measured voltages -----------------
	// This function is called 500 times/sec
	
	// LPF coefficients, larger values correspond to stronger filtering 0 < value < 1
	float alpha = 0.8; // Fast Filter Operation Coefficient 
	
	if(v_Bat_Voltage_Filter_Mode == Fast_Operation)
	{		
		alpha = 0.95;//0.3;
		battery_Voltage_Filter_Counter ++;
		if (battery_Voltage_Filter_Counter >=  100)
		{ 
			battery_Voltage_Filter_Counter = 0;			
			v_Bat_Voltage_Filter_Mode = Stable_Operation;
		}
	}
	// ---------------------------------------------------------------------------
	
		
	if(v_Bat_Voltage_Filter_Mode == Stable_Operation)
	{
		alpha = 0.95; // Stable Filter Operation Coefficient		
		
		// If the input voltage deviates from the stable filter output more than 0.1V switch to Fast Filter Operation
//		if(fabs(v_Bat_Volts_Filtered - v_Bat_Volts) > 0.1) //Percantage
//		{
//			v_Bat_Voltage_Filter_Mode = Fast_Operation;
//		}
	}
	
	// Vout(t) = alpha * Vout(t-dt) + (1-alpha)* Vin(t)
	// alpha = 0.80 for fast filtering
	// alpha = 0.99 for fast filtering
	v_Bat_Volts_Filtered = alpha * v_Bat_Volts_Filtered + (1 - alpha) * v_Bat_Volts;	
}

/* Shift arr[0..n-1] left by 1 and append new_sample at arr[n-1].
   If n == 0, it does nothing. */
void VBat_Push(float *arr, size_t n, float new_sample)
{
    if (n == 0) return;
    /* move elements 1..n-1 to 0..n-2 */
    for (size_t i = 0; i + 1 < n; i++) arr[i] = arr[i + 1];
    /* put newest at the tail */
    arr[n - 1] = new_sample;
}

/* Insertion sort: OK for small n (O(n^2)), stable, no malloc */
static void insertion_sort_float(float *a, size_t n)
{
    for (size_t i = 1; i < n; i++) {
        float key = a[i];
        size_t j = i;
        while (j > 0 && a[j-1] > key) {
            a[j] = a[j-1];
            j--;
        }
        a[j] = key;
    }
}

float Apply_Median_Average_On_V_Battery(const float *arr, size_t n,
                                          size_t trim_low, size_t trim_high)
{
    if (!arr || n == 0) return 0.0f;
    if (n > 50) n = 50;

    float work[50];
    for (size_t i = 0; i < n; i++) work[i] = arr[i];
    insertion_sort_float(work, n);

    if (trim_low + trim_high >= n) return 0.0f;

    size_t start = trim_low;
    size_t end   = n - trim_high;  /* exclusive */
    double acc = 0.0;
    size_t count = end - start;
    for (size_t i = start; i < end; i++) acc += (double)work[i];

    return (float)(acc / (double)count);
}

void Apply_LPF_On_Input_Power(void)
{	
	// Apply Fast or Stable Filtering on measured voltages -----------------
	// This function is called 500 times/sec
	
	// LPF coefficients, larger values correspond to stronger filtering 0 < value < 1
	float alpha = 0.8; // Fast Filter Operation Coefficient 
	
	if(input_Power_Voltage_Filter_Mode == Fast_Operation)
	{		
		alpha = 0.8;	
		input_Power_Voltage_Filter_Counter ++;
		if (input_Power_Voltage_Filter_Counter >=  500)
		{ 
			// If fast filtering is applied 500 succesive times (1 sec), then switch to stable filtering
			input_Power_Voltage_Filter_Counter = 0;			
			input_Power_Voltage_Filter_Mode = Stable_Operation;
		}
	}
	// ---------------------------------------------------------------------------
		
	if(input_Power_Voltage_Filter_Mode == Stable_Operation)
	{
		alpha = 0.99; // Stable Filter Operation Coefficient		
		
		// If the input voltage deviates from the stable filter output more than 0.1V switch to Fast Filter Operation
		if(fabs(v_Input_Power_Volts_Filtered - v_Input_Power_Volts) > 0.1) //Percantage
		{
			input_Power_Voltage_Filter_Mode = Fast_Operation;
		}
	}
	
	// Vout(t) = alpha * Vout(t-dt) + (1-alpha)* Vin(t)
	// alpha = 0.80 for fast filtering
	// alpha = 0.99 for fast filtering
	v_Input_Power_Volts_Filtered = alpha * v_Input_Power_Volts_Filtered + (1 - alpha) * v_Input_Power_Volts;		
	
}

void Apply_LPF_On_Input_Jack_Charger(void)
{	
	// Apply Fast or Stable Filtering on measured voltages -----------------
	// This function is called 500 times/sec
	
	// LPF coefficients, larger values correspond to stronger filtering 0 < value < 1
	float alpha = 0.8; // Fast Filter Operation Coefficient 
	
	if(input_Jack_Charger_Voltage_Filter_Mode == Fast_Operation)
	{		
		alpha = 0.8;	
		input_Jack_Charger_Voltage_Filter_Counter ++;
		if (input_Jack_Charger_Voltage_Filter_Counter >=  500)
		{ 
			// If fast filtering is applied 500 succesive times (1 sec), then switch to stable filtering
			input_Jack_Charger_Voltage_Filter_Counter = 0;			
			input_Jack_Charger_Voltage_Filter_Mode = Stable_Operation;
		}
	}
	// ---------------------------------------------------------------------------
	
		
	if(input_Jack_Charger_Voltage_Filter_Mode == Stable_Operation)
	{
		alpha = 0.99; // Stable Filter Operation Coefficient		
		
		// If the input voltage deviates from the stable filter output more than 0.1V switch to Fast Filter Operation
		if(fabs(v_Input_Jack_Charger_Volts_Filtered - v_Input_Jack_Charger_Volts) > 0.1) //Percantage
		{
			input_Jack_Charger_Voltage_Filter_Mode = Fast_Operation;
		}
	}
	
	// Vout(t) = alpha * Vout(t-dt) + (1-alpha)* Vin(t)
	// alpha = 0.80 for fast filtering
	// alpha = 0.99 for fast filtering
	v_Input_Jack_Charger_Volts_Filtered = alpha * v_Input_Jack_Charger_Volts_Filtered + (1 - alpha) * v_Input_Jack_Charger_Volts;		
	
}
