#ifndef NUMUPDOWN_HPP
#define NUMUPDOWN_HPP

#include <gui_generated/containers/NumUpDownBase.hpp>

class NumUpDown : public NumUpDownBase
{
public:
    NumUpDown();
    virtual ~NumUpDown() {}

    virtual void initialize();
		virtual	void setNumber(uint8_t Number);
		virtual	void set_center();
protected:
};

#endif // NUMUPDOWN_HPP
