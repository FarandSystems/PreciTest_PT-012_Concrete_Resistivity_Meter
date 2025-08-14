#ifndef MIRROR_HPP
#define MIRROR_HPP

#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/hal/HAL.hpp>

/**
 * Highly experimental widget.
 * Reverses pixels on both axis.
 * REQUIRES that 
 *   - The size of this widget is DISPLAY_WIDTH, DISPLAY_HEIGHT
 *   - This widget is the frontmost widget in Z-hierarchy
 *   - Entire screen is redrawn if something is invalidated.
 *
 * NOTE: The DISP_WIDTH variable at the bottom must be updated to match actual display width.
 */
class Mirror : public touchgfx::Widget
{
public:
    Mirror() {}
    virtual ~Mirror(){}
    virtual touchgfx::Rect getSolidRect() const
    {
        return Rect(0, 0, 0, 0);
    }
    virtual void draw(const touchgfx::Rect& invalidatedArea) const
    {
        uint16_t *framebuffer = touchgfx::HAL::getInstance()->lockFrameBuffer();

        for(int y = 0; y < HAL::DISPLAY_HEIGHT/2; y++)
        {
            // Read top line.
            touchgfx::HAL::getInstance()->blockCopy((void*)rowBufferTop, (const void*)(framebuffer+y*HAL::DISPLAY_WIDTH), HAL::DISPLAY_WIDTH*2);

            // Reverse pixels
            uint16_t* start = rowBufferTop;
            uint16_t* end = start+HAL::DISPLAY_WIDTH-1;
            
            for(int x = 0; x < HAL::DISPLAY_WIDTH/2; x++)
            {                
                uint16_t temp = *start;
                *start = *end;
                *end = temp;
                start++;
                end--;
            }
            // Read bottom line
            touchgfx::HAL::getInstance()->blockCopy((void*)rowBufferBottom, (const void*)(framebuffer+(HAL::DISPLAY_HEIGHT-1-y)*HAL::DISPLAY_WIDTH), HAL::DISPLAY_WIDTH*2);

            // Reverse pixels
            start = rowBufferBottom;
            end = start+HAL::DISPLAY_WIDTH-1;
            
            for(int x = 0; x < HAL::DISPLAY_WIDTH/2; x++)
            {                
                uint16_t temp = *start;
                *start = *end;
                *end = temp;
                start++;
                end--;
            }
            // Commit reversed rows (in reverse Y).
            touchgfx::HAL::getInstance()->blockCopy((void*)(framebuffer+y*HAL::DISPLAY_WIDTH), rowBufferBottom, HAL::DISPLAY_WIDTH*2);
            touchgfx::HAL::getInstance()->blockCopy((void*)(framebuffer+(HAL::DISPLAY_HEIGHT-1-y)*HAL::DISPLAY_WIDTH), rowBufferTop, HAL::DISPLAY_WIDTH*2);
        }

        touchgfx::HAL::getInstance()->unlockFrameBuffer();
    }
private:
    static const uint16_t DISP_WIDTH = 480; //TODO set this to correct screen width in pixels!!
    mutable uint16_t rowBufferTop[DISP_WIDTH];
    mutable uint16_t rowBufferBottom[DISP_WIDTH];
};


#endif /* MIRROR_HPP */
