#include <gui/main_screen_screen/Main_ScreenView.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include "main.h"

#include "bootloader_entry.h"

uint8_t display_Prescalar = 0;
uint8_t panel_READY_PreviousCondition = 0;
uint8_t panel_USB_PreviousCondition = 0;

int holdTicks = 0;  // 60 ticks ~ 1 s if your GUI is 60 Hz

Main_ScreenView::Main_ScreenView():
BATTERY_PERCENTClickedCallback(this, &Main_ScreenView::BATTERY_PERCENTClickHandler)
{

}
void Main_ScreenView::setupScreen()
{
  Main_ScreenViewBase::setupScreen();
	
	BATTERY_PERCENT.setClickAction(BATTERY_PERCENTClickedCallback);
	
	screen_rotation.setPosition(0, 0, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT);
	add(screen_rotation);
	
	Set_Fixed_Panels_First_Time();	
}
void Main_ScreenView::tearDownScreen()
{
  Main_ScreenViewBase::tearDownScreen();
}

void Main_ScreenView::handleTickEvent()
{
	display_Prescalar++;
	if(display_Prescalar == 30)// approximately each 0.5 second
	{
		display_Prescalar = 0;	
		UpdateDisplay();
	}
	
	if(save_and_Create_delay_flag == 1)
	{		
		Save_And_Create();
	}
	
	    if (MEASURE.getPressedState()) {
        if (++holdTicks > 180) {        // ~3 seconds
            Bootloader_RequestAndReset();
            holdTicks = -100000;        // ensure we don't re-enter
        }
    } else {
        holdTicks = 0;
    }
}
void Main_ScreenView::Set_Fixed_Panels_First_Time(void)
{	
	
	projectRecordNumber = ReadParameter(LATEST_PROJECT_INFO_FLASH_INDEX);
	
	projectNumber = (projectRecordNumber & 0xFF000000) >> 24;
	
	recordNumber = (projectRecordNumber & 0x00FF0000) >> 16;

	Unicode::snprintf(PROJECT_NUMBuffer, PROJECT_NUM_SIZE,"%2d",projectNumber);	
	PROJECT_NUM.resizeToCurrentTextWithAlignment();
	PROJECT_NUM.invalidate();

	if(recordNumber == 0)
	{
		NEW_PROJECT.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_FILLED_ID), touchgfx::Bitmap(BITMAP_BUTTON_FILLED_ID));
		NEW_PROJECT.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
		NEW_PROJECT.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
		NEW_PROJECT.setLabelText(touchgfx::TypedText(T_NEW_PROJECT));
		NEW_PROJECT.invalidate();
	}
	#ifdef CONCRETE		
		if(probeTypeIndex == 0)
		{
			MODE.setLabelText(touchgfx::TypedText(T_MODE_38MM));	
		}	
		if(probeTypeIndex == 1)
		{
			MODE.setLabelText(touchgfx::TypedText(T_MODE_50MM));	
		}		
		if(probeTypeIndex == 2)
		{
			MODE.setLabelText(touchgfx::TypedText(T_MODE_BULK));		
		}	
		TITLE.setLabelText(touchgfx::TypedText(T_FW412));
	#endif
	#ifdef SOIL	
	
		MODE.setLabelText(touchgfx::TypedText(T_MODE_SOIL));
		
		TITLE.setLabelText(touchgfx::TypedText(T_FW403));
		
	#endif
	MODE.invalidate();		

	
}
void Main_ScreenView::ShowMessageBox(char* Message_Text)
{
	if(CLOCK_AND_DATE_SETTING.isVisible()== false)
	{
		message_Box.Set_Text(Message_Text);
		message_Box.setVisible(true);
		message_Box.invalidate();
	}
}
void Main_ScreenView::Hold_btn_CallBack()
{
	turn_off_counter = 0;	
	if(OutputFilterOperation == StableOperation)
	{
		if(hold_flag == 0)
		{
			HOLD.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_FILLED_ID), touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_ID));
			HOLD.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
			HOLD.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(253, 255, 123));
			hold_flag = 1;
		}
		else if(hold_flag == 1)
		{
			HOLD.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_ID), touchgfx::Bitmap(BITMAP_BUTTON_FILLED_ID));
			HOLD.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(253, 255, 123));
			HOLD.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
			hold_flag = 0;
		}
		HOLD.invalidate();
		Alarm(SHORT_BEEP_X1,1,8, BEEP_ON);
	}
}
void Main_ScreenView::Measure_btn_CallBack()
{
	turn_off_counter = 0;
	OutputFilterOperation = FastOperation;
	Alarm(SHORT_BEEP_X1,1,8, BEEP_ON);
}
void Main_ScreenView::Mode_btn_CallBack()
{	
	turn_off_counter = 0;
	
	#ifdef CONCRETE	
		probeTypeIndex ++;
		if(probeTypeIndex > 2)// 50mm,38mm,Bulk
		{
			probeTypeIndex = 0;
		}	
		if(probeTypeIndex == 0)
		{
			MODE.setLabelText(touchgfx::TypedText(T_MODE_38MM));	
		}	
		if(probeTypeIndex == 1)
		{
			MODE.setLabelText(touchgfx::TypedText(T_MODE_50MM));	
		}		
		if(probeTypeIndex == 2)
		{
			MODE.setLabelText(touchgfx::TypedText(T_MODE_BULK));		
		}		
		Alarm(SHORT_BEEP_X1,1,8, BEEP_ON);
	#endif
	#ifdef SOIL	
		MODE.setLabelText(touchgfx::TypedText(T_MODE_SOIL));
	#endif
	MODE.invalidate();
	
}
void Main_ScreenView::New_Proj_btn_CallBack()
{	
	turn_off_counter = 0;
	if(OutputFilterOperation == StableOperation)
	{		 
		if(recordNumber != 0)
		{
			createPrj_flag = 1;
			NEW_PROJECT.setLabelText(touchgfx::TypedText(T_CREATING));
			NEW_PROJECT.invalidate();
			HAL_TIM_Base_Start_IT(&htim3);
			Alarm(SHORT_BEEP_X1,1,8, BEEP_ON);
		}
		else if(recordNumber == 0)
		{
			Alarm(SHORT_BEEP_X1,1,8, BEEP_ON);
			ShowMessageBox((char*)"Project is Free!");
		}
	}

}
void Main_ScreenView::Save_btn_CallBack()
{
	turn_off_counter = 0;
	if(OutputFilterOperation == StableOperation)
	{
		save_flag = 1 ;
		SAVE.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_FILLED_ID), touchgfx::Bitmap(BITMAP_BUTTON_FILLED_ID));
		SAVE.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
		SAVE.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
		SAVE.setLabelText(touchgfx::TypedText(T_SAVING));
		SAVE.invalidate();
		HAL_TIM_Base_Start_IT(&htim3); // Start 1 second delay
		Alarm(SHORT_BEEP_X1,1,8, BEEP_ON);		
	}	
}
void Main_ScreenView::BATTERY_PERCENTClickHandler(const TextProgress& text_progress, const ClickEvent& e)
{
	turn_off_counter = 0;
	
	if (&text_progress == &BATTERY_PERCENT)
	{
		if (e.getType() == e.RELEASED)
		{	
			Alarm(SHORT_BEEP_X1,1,8, BEEP_ON);	
			auto_turn_off_mode = 1 - auto_turn_off_mode;
			if (auto_turn_off_mode == 1)
			{
				ShowMessageBox((char*)"Auto Turn Off Enabled!");					
			}
			else
			{
				ShowMessageBox((char*)"Auto Turn Off Disabled!");
			}
		}
	}
}

void Main_ScreenView::Save_And_Create(void)
{
    save_and_Create_delay_flag = 0;

    if (save_flag == 1)
    {
        save_flag = 0;
        HAL_TIM_Base_Stop_IT(&htim3);

        /* Update next record/project indexes (handles rollover) */
        Update_Project_Info();

        /* Save the actual record */
        Save_Record_Data_On_Flash();

        /* UI refresh */
        SAVE.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_ID), touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_ID));
        SAVE.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(253, 255, 123));
        SAVE.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(253, 255, 123));
        SAVE.setLabelText(touchgfx::TypedText(T_SAVE));
        SAVE.invalidate();

        NEW_PROJECT.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_ID), touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_ID));
        NEW_PROJECT.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(253, 255, 123));
        NEW_PROJECT.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(253, 255, 123));
        NEW_PROJECT.setLabelText(touchgfx::TypedText(T_NEW_PROJECT));
        NEW_PROJECT.invalidate();

        Unicode::snprintf(HELPBuffer, HELP_SIZE, "Press Save to Record as Number %d", recordNumber + 1);
        HELP.setWildcard(HELPBuffer);
        HELP.resizeToCurrentTextWithAlignment();
        HELP.invalidate();

        Restart_Output_Filter_in_Save_And_CreatingProj();
    }

    if (createPrj_flag == 1)
    {
        createPrj_flag = 0;
        HAL_TIM_Base_Stop_IT(&htim3);

        /* Read last saved project/record (word packed) */
        projectRecordNumber = ReadParameter(LATEST_PROJECT_INFO_FLASH_INDEX);
        projectNumber = (uint8_t)((projectRecordNumber >> 24) & 0xFF);
        recordNumber  = (uint8_t)((projectRecordNumber >> 16) & 0xFF);

        /* Move to next project (1..24), wrap to 1 */
        if (projectNumber < (PROJECTS_COUNT - 1u)) {   /* 1..23 -> +1 */
            projectNumber++;
        } else {
            projectNumber = 1;                          /* wrap to 1 */
        }
        recordNumber = 0;

        /* Erase the chosen project's space (100 words) */
        if (!Erase_one_Project_On_Flash(projectNumber)) {
            /* Optional: show error */
            Alarm(EMERGENCY_BEEP, 1, 32, BEEP_ON);
        } else {
//            Alarm(SHORT_BEEP_X1, 1, 8, BEEP_ON);
        }

        /* Update UI: NEW_PROJECT button, project number, help text */
        NEW_PROJECT.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_FILLED_ID), touchgfx::Bitmap(BITMAP_BUTTON_FILLED_ID));
        NEW_PROJECT.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
        NEW_PROJECT.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
        NEW_PROJECT.setLabelText(touchgfx::TypedText(T_NEW_PROJECT));
        NEW_PROJECT.invalidate();

        Unicode::snprintf(PROJECT_NUMBuffer, PROJECT_NUM_SIZE, "%2d", projectNumber);
        PROJECT_NUM.resizeToCurrentTextWithAlignment();
        PROJECT_NUM.invalidate();

        Unicode::snprintf(HELPBuffer, HELP_SIZE, "Press Save to Record as Number %d", recordNumber + 1);
        HELP.setWildcard(HELPBuffer);
        HELP.resizeToCurrentTextWithAlignment();
        HELP.invalidate();

        /* Persist latest project info to flash */
        latestProjectInfo[0] = projectNumber; /* project */
        latestProjectInfo[1] = recordNumber;  /* record */
        /* latestProjectInfo[2], [3] can remain as-is (reserved) */

        Save_In_Four_Words(LATEST_PROJECT_INFO_FLASH_INDEX, latestProjectInfo);
    }
}

void Main_ScreenView::Temp_Correct_btn_CallBack()
{
	if(OutputFilterOperation == StableOperation)
	{
		if(temp_Correction_flag == 0)
		{
			TEMP_CORRECT.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_FILLED_ID), touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_ID));
			TEMP_CORRECT.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
			TEMP_CORRECT.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(253, 255, 123));
			temp_Correction_flag = 1;
		}
		else if(temp_Correction_flag == 1)
		{
			TEMP_CORRECT.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_ID), touchgfx::Bitmap(BITMAP_BUTTON_FILLED_ID));
			TEMP_CORRECT.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(253, 255, 123));
			TEMP_CORRECT.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
			temp_Correction_flag = 0;
		}
		OutputFilterOperation = FastOperation;
		TEMP_CORRECT.invalidate();
		Alarm(SHORT_BEEP_X1,1,8, BEEP_ON);
	}
}
void Main_ScreenView::Display_Resistivity(float r)
{
	if(r < 1e-6f)
	{
		displayedResistivity = 0;
		bulk_cond_unit_PrefixNumber = 0;
	}	
	else if(0.000001f <= r && r < 0.001f)
	{
		displayedResistivity = r/1e-6f;
		bulk_cond_unit_PrefixNumber = -6;
	}	
	else if(0.001f <= r && r < 1.0f)
	{
		displayedResistivity = r/1e-3f;
		bulk_cond_unit_PrefixNumber = -3;		
	}	
	else if(1.0f <= r && r < 1000.0f)
	{
		displayedResistivity = r/1e0f;
		bulk_cond_unit_PrefixNumber = 0;		
	}	
	else if(1000.0f <= r && r < 1000000.0f)
	{
		displayedResistivity = r/1e3f;
		bulk_cond_unit_PrefixNumber = 3;		
	}		
	else if(1000000.0f <= r && r < 1000000000.0f) 
	{
		displayedResistivity = r/1e6f;
		bulk_cond_unit_PrefixNumber = 6;		
	}	
		
	if(electrical_Connection_Status == Connected)
	{					
		//Change Z Orders to change order of drawing panels (Higher z order is drawn on top of lower z order)
		WARNING.setVisible(false);

		RESISTIVITY.setVisible(true);
		OHM_CM_UNIT.setVisible(true);

//  in SOIL  probeTypeIndex != 2 always will is true	
		if(bulk_cond_unit_PrefixNumber == 6 && probeTypeIndex != 2)
		{
			OHM_CM_UNIT.setTypedText(touchgfx::TypedText(T_MEGA_OHM_CM));
		}
		else if(bulk_cond_unit_PrefixNumber == 3 && probeTypeIndex != 2)
		{
			OHM_CM_UNIT.setTypedText(touchgfx::TypedText(T_KILO_OHM_CM));
		}
		else if(bulk_cond_unit_PrefixNumber == 0 && probeTypeIndex != 2)
		{	
			OHM_CM_UNIT.setTypedText(touchgfx::TypedText(T_OHM_CM));
		}
		else if(bulk_cond_unit_PrefixNumber == -3 && probeTypeIndex != 2)
		{
			OHM_CM_UNIT.setTypedText(touchgfx::TypedText(T_MILI_OHM_CM));
		}
		else if(bulk_cond_unit_PrefixNumber == -6 && probeTypeIndex != 2)
		{	
			OHM_CM_UNIT.setTypedText(touchgfx::TypedText(T_MICRO_OHM_CM));
		}
//-------------------------------------------------		
		if(displayedResistivity == 0 && probeTypeIndex != 2)
		{
			Unicode::snprintfFloat(RESISTIVITYBuffer,RESISTIVITY_SIZE,"%1.3f",displayedResistivity);			
		}
		else if(1 <= displayedResistivity &&  displayedResistivity < 10 && probeTypeIndex != 2)
		{	
			Unicode::snprintfFloat(RESISTIVITYBuffer,RESISTIVITY_SIZE,"%1.3f",displayedResistivity);	
		}		
		else if(10 <= displayedResistivity && displayedResistivity < 100 && probeTypeIndex != 2)
		{	
			Unicode::snprintfFloat(RESISTIVITYBuffer,RESISTIVITY_SIZE,"%2.2f",displayedResistivity);				
		}		
		else if(100 <= displayedResistivity && displayedResistivity < 1000 && probeTypeIndex != 2)
		{		
			Unicode::snprintfFloat(RESISTIVITYBuffer,RESISTIVITY_SIZE,"%3.1f",displayedResistivity);	
		}		
		else if(probeTypeIndex == 2) // probe is connected and mode is Bulk
		{
			RESISTIVITY.setVisible(0);
			OHM_CM_UNIT.setVisible(0);			
		}
		RESISTIVITY.resizeToCurrentTextWithAlignment();
		OHM_CM_UNIT.resizeToCurrentTextWithAlignment();
		WARNING.resizeToCurrentTextWithAlignment();
		WARNING.invalidate();
		RESISTIVITY.invalidate();	
		OHM_CM_UNIT.invalidate();
	}
	else // Probe Not Connected
	{
		WARNING.setVisible(true);
		RESISTIVITY.setVisible(false);
		OHM_CM_UNIT.setVisible(false);
		RESISTIVITY.invalidate();	
		OHM_CM_UNIT.invalidate();
		WARNING.invalidate();
	}
	BACKGROUND.invalidate();
}
//Update Temperature and Range(High Range,Low Range)
void Main_ScreenView::Update_Temperature_Battery_Range_Panels(void)
{
	static uint8_t wating_Counter = 0;
	
	Update_Battery_Panel();
	
	wating_Counter++;
	if(wating_Counter > 30) //wait for some seconds(15s) 
	{
		// Update Temperature Panel
		Unicode::snprintfFloat(TEMPERATUREBuffer,TEMPERATURE_SIZE,"%3.1f",(double)(0.5f * (int)(2.0f * temp_Deg_Display + 0.5f)));
		TEMPERATURE.setVisible(true);
		BATTERY_PERCENT.setVisible(true);				
	}
	if(Charging_Mode == CHARGING && wating_Counter > 30)
	{
		wating_Counter = 0;
	}
		
	
	if(wating_Counter > 20) //wait for some seconds(10s)
	{
		Connect_Charger_Warning();
	}
	
	
	// Update Range Panel
	if(measurementRange == HighRange)
	{
		READY.set_range(T_HIGH_RANGE);
	}
	else if(measurementRange == LowRange)
	{
		READY.set_range(T_LOW_RANGE);
	}
	TEMPERATURE.resizeToCurrentTextWithAlignment();
	TEMPERATURE.invalidate();
	BACKGROUND.invalidate();		
}
void Main_ScreenView::Connect_Charger_Warning(void)
{
	if(v_Bat_Percentage < 30)
	{
		RESISTANCE.setVisible(false);
		OHM_UNIT.setVisible(false);
		RESISTANCE.invalidate();
		OHM_UNIT.invalidate();	
		RESISTIVITY.setVisible(false);
		OHM_CM_UNIT.setVisible(false);
		RESISTIVITY.invalidate();	
		OHM_CM_UNIT.invalidate();				
		
		WARNING.setVisible(true);		
		Unicode::snprintf(WARNINGBuffer,WARNING_SIZE,"Connect Charger!");
		WARNING.setWildcard(WARNINGBuffer);
		WARNING.resizeToCurrentTextWithAlignment();
		WARNING.invalidate();	
		OutputFilterOperation = FastOperation;
	}
}
void Main_ScreenView::Update_Battery_Panel(void)
{	
	if(Charging_Mode == CHARGING)
	{
		BATTERY_PERCENT.setBackground(touchgfx::Bitmap(BITMAP_BAT_SYMB_CHARGING_ID));
		BATTERY_PERCENT.setTypedText(touchgfx::TypedText(T_BATTERY_NO_VALUE));
	}
	else
	{
		BATTERY_PERCENT.setTypedText(touchgfx::TypedText(T_BATTERY_VALUE));
		BATTERY_PERCENT.setValue((int)v_Bat_Percentage);
		
		if(auto_turn_off_mode == 1)
		{			
			BATTERY_PERCENT.setBackground(touchgfx::Bitmap(BITMAP_BATTERY_SYMBOL_STROKED_ID));
		}
		else
		{
			BATTERY_PERCENT.setBackground(touchgfx::Bitmap(BITMAP_BATTERY_SYMBOL_ID));
		}
	}
}

void Main_ScreenView::Show_Very_Low(void)
{
//	if(probeTypeIndex == 2)
//	{
		first_time_disconnection_flag = 0;
		RESISTANCE.setVisible(false);
		OHM_UNIT.setVisible(false);
		RESISTANCE.invalidate();
		OHM_UNIT.invalidate();	
		RESISTIVITY.setVisible(false);
		OHM_CM_UNIT.setVisible(false);
		RESISTIVITY.invalidate();	
		OHM_CM_UNIT.invalidate();				
		
		WARNING.setVisible(true);		
		Unicode::snprintf(WARNINGBuffer,WARNING_SIZE,"Very Low!");
		WARNING.setWildcard(WARNINGBuffer);
		WARNING.resizeToCurrentTextWithAlignment();
		WARNING.invalidate();	
		OutputFilterOperation = FastOperation;	
//	}
}
void Main_ScreenView::Display_Resistance(float r)
{	
	if(r < 1e-6f)
	{
		displayed_Resistance_Or_Conductivity = 0.0f;
		resistance_conductivity_Range = 0;
	}	
	else if(0.000001f <= r && r < 0.001f)
	{
		displayed_Resistance_Or_Conductivity = r/1e-6f;
		resistance_conductivity_Range = -6;
	}	
	else if(0.001f <= r && r < 1.0f)
	{
		displayed_Resistance_Or_Conductivity = r/1e-3f;
		resistance_conductivity_Range = -3;		
	}	
	else if(1.0f <= r && r < 1000.0f)
	{
		displayed_Resistance_Or_Conductivity = r/1e0f;
		resistance_conductivity_Range = 0;		
	}	
	else if(1000.0f <= r && r < 1000000.0f)
	{
		displayed_Resistance_Or_Conductivity = r/1e3f;
		resistance_conductivity_Range = 3;		
	}	
	else if(1000000.0f <= r && r < 1000000000.0f) 
	{
		displayed_Resistance_Or_Conductivity = r/1e6f;
		resistance_conductivity_Range = 6;		
	}	
	
	resistance_Or_conductivityX10 = (uint16_t)(displayed_Resistance_Or_Conductivity * 10);
	
	if(electrical_Connection_Status == Connected)
	{
		//Change Z Orders to change order of drawing panels (Higher z order is drawn on top of lower z order)
			WARNING.setVisible(false);
			RESISTANCE.setVisible(true);
			OHM_UNIT.setVisible(true);	
		
		#ifdef  CONCRETE
			if(resistance_conductivity_Range == 6)
			{	
				OHM_UNIT.setTypedText(touchgfx::TypedText(T_MEGA_OHM));
			}
			else if(resistance_conductivity_Range == 3)
			{	
				OHM_UNIT.setTypedText(touchgfx::TypedText(T_KILO_OHM));
			}
			else if(resistance_conductivity_Range == 0)
			{	
				OHM_UNIT.setTypedText(touchgfx::TypedText(T_OHM));
				
				if(displayed_Resistance_Or_Conductivity < 2.0f)
				{
					Show_Very_Low();
				}
			}
			else if(resistance_conductivity_Range == -3)
			{	
				//OHM_UNIT.setTypedText(touchgfx::TypedText(T_MILI_OHM));
				Show_Very_Low();
			}
			else if(resistance_conductivity_Range == -6)
			{	
//				OHM_UNIT.setTypedText(touchgfx::TypedText(T_MICRO_OHM));
				Show_Very_Low();
			}	
			

		#endif
			
		#ifdef  SOIL
			if(resistance_conductivity_Range == 6)
			{	
				OHM_UNIT.setTypedText(touchgfx::TypedText(T_MEGA_S_M));
			}
			else if(resistance_conductivity_Range == 3)
			{	
				OHM_UNIT.setTypedText(touchgfx::TypedText(T_KILO_S_M));
			}
			else if(resistance_conductivity_Range == 0)
			{	
				OHM_UNIT.setTypedText(touchgfx::TypedText(T_S_M));
			}
			else if(resistance_conductivity_Range == -3)
			{	
				OHM_UNIT.setTypedText(touchgfx::TypedText(T_MILI_S_M));
			}
			else if(resistance_conductivity_Range == -6)
			{	
				OHM_UNIT.setTypedText(touchgfx::TypedText(T_MICRO_S_M));
			}	
		#endif
			
			
		if(displayed_Resistance_Or_Conductivity == 0)
		{	
			Unicode::snprintfFloat(RESISTANCEBuffer,RESISTANCE_SIZE,"%1.3f",displayed_Resistance_Or_Conductivity);
		}
		else if(1 <= displayed_Resistance_Or_Conductivity &&  displayed_Resistance_Or_Conductivity < 10)
		{
			Unicode::snprintfFloat(RESISTANCEBuffer,RESISTANCE_SIZE,"%1.3f",displayed_Resistance_Or_Conductivity);
		}		
		else if(10 <= displayed_Resistance_Or_Conductivity && displayed_Resistance_Or_Conductivity < 100)
		{
			Unicode::snprintfFloat(RESISTANCEBuffer,RESISTANCE_SIZE,"%2.2f",displayed_Resistance_Or_Conductivity);			
		}		
		else if(100 <= displayed_Resistance_Or_Conductivity && displayed_Resistance_Or_Conductivity < 1000)
		{
			Unicode::snprintfFloat(RESISTANCEBuffer,RESISTANCE_SIZE,"%3.1f",displayed_Resistance_Or_Conductivity);			
		}
		RESISTANCE.resizeToCurrentTextWithAlignment();
		OHM_UNIT.resizeToCurrentTextWithAlignment();
		RESISTANCE.invalidate();		
		OHM_UNIT.invalidate();
		WARNING.invalidate();
	}
	else
	{		
		// If the mesured vaule is not valid, the warning is displayed
		RESISTANCE.setVisible(false);
		OHM_UNIT.setVisible(false);
		RESISTANCE.invalidate();
		OHM_UNIT.invalidate();		
		
		WARNING.setVisible(true);	
		#ifdef  CONCRETE		
			Unicode::snprintf(WARNINGBuffer,WARNING_SIZE,"Press the probe !");	
		#endif
		
		#ifdef  SOIL
			Unicode::snprintf(WARNINGBuffer,WARNING_SIZE,"Connect the soil box!");			
		#endif
		WARNING.setWildcard(WARNINGBuffer);
    WARNING.resizeToCurrentTextWithAlignment();
		WARNING.invalidate();
	}
	BACKGROUND.invalidate();
}
void Main_ScreenView::UpdateDisplay(void)
{
	if(hold_flag == 0)
	{
		Display_Resistivity(resistivity_Final);		
		Display_Resistance(bulk_Or_cond_Final);
		Update_Temperature_Battery_Range_Panels();	
	}	
	if(hold_flag == 0)
	{
		if(electrical_Connection_Status == Disconnected && OutputFilterOperation == FastOperation)
		{
			READY.setVisible(false);				
			READY.invalidate();		
		}
		else if(OutputFilterOperation == FastOperation)
		{
			READY.set_color(0xc4,0x72,0x38);// orange
			if(READY.isVisible() == true)
			{
				READY.setVisible(false);
			}
			else
			{
				READY.setVisible(true);
			}
			READY.invalidate();		
		}
		if(OutputFilterOperation == FastOperation && panel_READY_PreviousCondition == 0)
		{
			panel_READY_PreviousCondition = 1;			
			HOLD.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_GRAY_ID), touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_GRAY_ID));
			HOLD.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(206, 202, 206));
			HOLD.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(206, 202, 206));
			TEMP_CORRECT.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_GRAY_ID), touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_GRAY_ID));
			TEMP_CORRECT.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(206, 202, 206));
			TEMP_CORRECT.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(206, 202, 206));
			SAVE.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_GRAY_ID), touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_GRAY_ID));
			SAVE.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(206, 202, 206));
			SAVE.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(206, 202, 206));	
			HOLD.invalidate();
			TEMP_CORRECT.invalidate();
			SAVE.invalidate();
		}
		else if(OutputFilterOperation == StableOperation && panel_READY_PreviousCondition == 1)
		{
			panel_READY_PreviousCondition = 0;
			READY.set_color(0x8C,0xC6,0x3A); // green
			READY.setVisible(true);
			READY.invalidate();	
			HOLD.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_ID), touchgfx::Bitmap(BITMAP_BUTTON_FILLED_ID));
			HOLD.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(253, 255, 123));
			HOLD.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));

			if(temp_Correction_flag == 1)
			{
				TEMP_CORRECT.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_FILLED_ID), touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_ID));
				TEMP_CORRECT.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
				TEMP_CORRECT.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(253, 255, 123));
			}
			else if(temp_Correction_flag == 0)
			{
				TEMP_CORRECT.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_ID), touchgfx::Bitmap(BITMAP_BUTTON_FILLED_ID));
				TEMP_CORRECT.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(253, 255, 123));
				TEMP_CORRECT.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));				
			}
			SAVE.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTON_HOLLOW_ID), touchgfx::Bitmap(BITMAP_BUTTON_FILLED_ID));
			SAVE.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(253, 255, 123));
			SAVE.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
			HOLD.invalidate();
			TEMP_CORRECT.invalidate();
			SAVE.invalidate();			
		}
	}
	
	
	if(usb_connection_state == 1 && panel_USB_PreviousCondition == 0)
	{
		panel_USB_PreviousCondition = 1;			
		USB_SYMBOL.setVisible(true);
		USB_SYMBOL.invalidate();	
	}
	if(usb_connection_state == 0 && panel_USB_PreviousCondition == 1)
	{
		panel_USB_PreviousCondition = 0;	
		USB_SYMBOL.setVisible(false);
		USB_SYMBOL.invalidate();
		ShowMessageBox((char*)"USB Disconnected!");
	}
	
	
	Show_Time_and_Date();
}
void Main_ScreenView::Show_Time_and_Date(void)
{
	if(TIME.isVisible() == true)
	{
		TIME.setTime24Hour(Hour, Minute, Second);	
		TIME.invalidate();
	}
}
void Main_ScreenView::Clock_Touch_CallBack()	
{
	turn_off_counter = 0;
	if(message_Box.isVisible() == false)
	{		
		CLOCK_AND_DATE_SETTING.Show_Present_Time(Hour,Minute,Second);
		CLOCK_AND_DATE_SETTING.setVisible(true);
		CLOCK_AND_DATE_SETTING.invalidate();
		Alarm(SHORT_BEEP_X1,1,8, BEEP_ON);
	}
}
