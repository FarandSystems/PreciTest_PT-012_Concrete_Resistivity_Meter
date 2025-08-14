#ifndef DIGITAL_CLOCK_HPP
#define DIGITAL_CLOCK_HPP

#include <gui_generated/containers/Digital_ClockBase.hpp>

class Digital_Clock : public Digital_ClockBase 
{
public:
    Digital_Clock();
    virtual ~Digital_Clock() {}
		virtual void setTime24Hour(uint8_t hour, uint8_t minute, uint8_t second);	
    virtual void initialize();
protected:
};

#endif // DIGITAL_CLOCK_HPP
