#ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <gui_generated/common/FrontendApplicationBase.hpp>
#include <touchgfx/hal/HAL.hpp>

class FrontendHeap;

using namespace touchgfx;

class FrontendApplication : public FrontendApplicationBase
{
public:
    FrontendApplication(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplication() { }

    virtual void handleTickEvent()
    {
        model.tick();
        FrontendApplicationBase::handleTickEvent();
    }
		 // Force redrawing entire screen
    virtual void draw(Rect& rect)
    {
        if (drawCacheEnabled)
        {
            // Invalidate entire screen instead of requested rect.
            Rect r(0, 0, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT);
            Application::draw(r);
        }
        else
        {
            // Use original rect if we are *actually* drawing and not just invalidating.
            Application::draw(rect);
        }
    }
private:
};

#endif // FRONTENDAPPLICATION_HPP
