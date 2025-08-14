#ifndef CUSTOMCONTAINER1_HPP
#define CUSTOMCONTAINER1_HPP

#include <gui_generated/containers/CustomContainer1Base.hpp>
#include "math.h"

class CustomContainer1 : public CustomContainer1Base
{
public:
    CustomContainer1();
    virtual ~CustomContainer1() {}

    virtual void initialize();
		void setNumber(int no)
		{
			Unicode::snprintf(textArea1Buffer,TEXTAREA1_SIZE,"%d",no);
		}		
		void offset(int16_t x)
    {        
        textArea1.moveTo(80 + x, textArea1.getY());
    }

    //The new declaration and implementation of the setY() function
    virtual void setY(int16_t y)
    {
        //set Y as normal
        CustomContainer1Base::setY(y);

        const int circleRadius = 250;

        //center around middle of background
        y = y + getHeight() / 2 - 390 /2;

        //calculate x
        float x_f = circleRadius - sqrtf((float)((circleRadius * circleRadius) - (y * y)));
        int16_t x = (int16_t)(x_f + 0.5f);

        offset(x);
    }
protected:
};

#endif // CUSTOMCONTAINER1_HPP
