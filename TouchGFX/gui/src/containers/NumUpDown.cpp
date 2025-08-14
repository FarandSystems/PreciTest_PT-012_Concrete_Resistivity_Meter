#include <gui/containers/NumUpDown.hpp>

NumUpDown::NumUpDown()
{

}

void NumUpDown::initialize()
{
    NumUpDownBase::initialize();
		//set_center();
}
void NumUpDown::setNumber(uint8_t Number)	
{
	Unicode::snprintf(TextBoxBuffer,TEXTBOX_SIZE,"%02d",Number);
	TextBox.resizeToCurrentTextWithAlignment();
	//set_center();
	TextBox.invalidate();	
}


void NumUpDown::set_center()
{
	int parent_width = getWidth();
	int parent_height = getHeight();
	int text_width = TextBox.getTextHeight();
	int text_height = TextBox.getTextWidth();
//	TextBox.setXY(parent_width/2 - 0*text_width/2,parent_height/2 - 0*text_height/2); 
//	TextBox.setXY(25/2,25/2);  
}
