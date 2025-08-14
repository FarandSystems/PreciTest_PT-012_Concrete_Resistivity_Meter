#include <gui/containers/Digital_Clock.hpp>

Digital_Clock::Digital_Clock()
{

}

void Digital_Clock::initialize()
{
    Digital_ClockBase::initialize();
}
void Digital_Clock::setTime24Hour(uint8_t hour, uint8_t minute, uint8_t second)
{
	digitalClock.setTime24Hour(hour,minute,second);
	digitalClock.invalidate();
}
