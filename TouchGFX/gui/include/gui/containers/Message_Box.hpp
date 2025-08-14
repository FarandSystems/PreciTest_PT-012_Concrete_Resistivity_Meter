#ifndef MESSAGE_BOX_HPP
#define MESSAGE_BOX_HPP

#include <gui_generated/containers/Message_BoxBase.hpp>

class Message_Box : public Message_BoxBase 
{
public:
    Message_Box();
    virtual ~Message_Box() {}

    virtual void initialize();
		virtual void Button_CallBack()
		{
			Message_BoxBase::setVisible(false);
			Message_BoxBase::invalidate();
		}		
		virtual void Set_Text(char* value)
    {
			Unicode::strncpy(Message_TextBuffer, value, MESSAGE_TEXT_SIZE);
			Message_Text.setWildcard(Message_TextBuffer);
			Message_Text.resizeToCurrentTextWithAlignment();
			Message_Text.invalidate();
    }	

protected:
};

#endif // MESSAGE_BOX_HPP
