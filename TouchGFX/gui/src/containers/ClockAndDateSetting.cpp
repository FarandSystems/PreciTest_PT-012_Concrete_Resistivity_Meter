#include <gui/containers/ClockAndDateSetting.hpp>
#include "main.h"
ClockAndDateSetting::ClockAndDateSetting()
{

}

void ClockAndDateSetting::initialize()
{
    ClockAndDateSettingBase::initialize();
}
void ClockAndDateSetting::Hour_setUpdateItem(NumUpDown& item, int16_t itemIndex)
{
	 item.setNumber(itemIndex);
}
void ClockAndDateSetting::Minute_setUpdateItem(NumUpDown& item, int16_t itemIndex)
{
	 item.setNumber(itemIndex);
}
void ClockAndDateSetting::Second_setUpdateItem(NumUpDown& item, int16_t itemIndex)
{
	 item.setNumber(itemIndex);
}
void ClockAndDateSetting::Button_CallBack()
{	
	Set_Colck_and_Date(Decimal_To_BCD(Hour_set.getSelectedItem()), Decimal_To_BCD(Minute_set.getSelectedItem()), Decimal_To_BCD(Second_set.getSelectedItem()), 0,0,0);
	ClockAndDateSetting::setVisible(false);
	ClockAndDateSetting::invalidate();
}
void ClockAndDateSetting::Show_Present_Time(uint8_t Hour,uint8_t Minute,uint8_t Second)
{
  Hour_set.animateToItem(Hour, 0);
	Minute_set.animateToItem(Minute, 0);
	Second_set.animateToItem(Second, 0);
}
