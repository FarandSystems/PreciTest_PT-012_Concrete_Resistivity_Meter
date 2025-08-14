#include <gui/containers/Ready.hpp>

Ready::Ready()
{

}

void Ready::initialize()
{
    ReadyBase::initialize();
}

void Ready::set_range(TEXTS Range_Type)
{	
	CircleText.setTypedText(touchgfx::TypedText(Range_Type));	
}

void Ready::set_color(uint8_t Red, uint8_t Green, uint8_t Blue)
{
	CirclePainter.setColor(touchgfx::Color::getColorFromRGB(Red, Green, Blue));
}