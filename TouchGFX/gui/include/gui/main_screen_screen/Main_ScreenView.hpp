#ifndef MAIN_SCREENVIEW_HPP
#define MAIN_SCREENVIEW_HPP

#include <gui_generated/main_screen_screen/Main_ScreenViewBase.hpp>
#include <gui/main_screen_screen/Main_ScreenPresenter.hpp>
#include <touchgfx/Color.hpp>
#include <Mirror.hpp>

class Main_ScreenView : public Main_ScreenViewBase
{
public:
    Main_ScreenView();
    virtual ~Main_ScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
		virtual void handleTickEvent();	
		virtual void Save_btn_CallBack();
		virtual void History_btn_CallBack();
		virtual void Mode_btn_CallBack();
		virtual void Measure_btn_CallBack();	
		virtual void New_Proj_btn_CallBack();	
		virtual void Temp_Correct_btn_CallBack();				
		virtual void Display_Resistivity(float r);
		virtual void Update_Temperature_Battery_Range_Panels();
		virtual void Display_Resistance(float r);	
		virtual void ShowMessageBox(char* Message_Text);
		virtual void Save_And_Create(void);	
		virtual	void Set_Fixed_Panels_First_Time(void);
		virtual void UpdateDisplay(void);
		virtual void Show_Time_and_Date(void);
		virtual void Clock_Touch_CallBack(void);	
		virtual void Update_Battery_Panel(void);
		virtual void Show_Very_Low(void);
		virtual void Connect_Charger_Warning(void);
		
		virtual void BATTERY_PERCENTClickHandler(const TextProgress& text_progress, const ClickEvent& e);
		
		Mirror screen_rotation;		
protected:
	
	Callback<Main_ScreenView, const TextProgress&, const ClickEvent&> BATTERY_PERCENTClickedCallback;
};

#endif // MAIN_SCREENVIEW_HPP
