#ifndef READY_HPP
#define READY_HPP

#include <gui_generated/containers/ReadyBase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
class Ready : public ReadyBase
{
public:
    Ready();
    virtual ~Ready() {}

    virtual void initialize();
		virtual void set_range(TEXTS Range_Type);	
		virtual void set_color(uint8_t Red, uint8_t Green, uint8_t Blue);
protected:
};

#endif // READY_HPP
