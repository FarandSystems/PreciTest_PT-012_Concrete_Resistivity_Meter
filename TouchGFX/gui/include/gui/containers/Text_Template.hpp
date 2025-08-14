#ifndef TEXT_TEMPLATE_HPP
#define TEXT_TEMPLATE_HPP

#include <gui_generated/containers/Text_TemplateBase.hpp>

class Text_Template : public Text_TemplateBase
{
public:
    Text_Template();
    virtual ~Text_Template() {}
		virtual void setNumber(uint8_t Number);	
    virtual void initialize();
protected:
};

#endif // TEXT_TEMPLATE_HPP
