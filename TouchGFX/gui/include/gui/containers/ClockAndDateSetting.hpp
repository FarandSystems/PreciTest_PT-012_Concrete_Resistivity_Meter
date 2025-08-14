#ifndef CLOCKANDDATESETTING_HPP
#define CLOCKANDDATESETTING_HPP

#include <gui_generated/containers/ClockAndDateSettingBase.hpp>

class ClockAndDateSetting : public ClockAndDateSettingBase
{
public:
    ClockAndDateSetting();
    virtual ~ClockAndDateSetting() {}
		virtual void Hour_setUpdateItem(NumUpDown& item, int16_t itemIndex);
		virtual void Minute_setUpdateItem(NumUpDown& item, int16_t itemIndex);
		virtual void Second_setUpdateItem(NumUpDown& item, int16_t itemIndex);
		virtual void Button_CallBack();
		virtual	void Show_Present_Time(uint8_t Hour,uint8_t Minute,uint8_t Second);
    virtual void initialize();
protected:
};

#endif // CLOCKANDDATESETTING_HPP
