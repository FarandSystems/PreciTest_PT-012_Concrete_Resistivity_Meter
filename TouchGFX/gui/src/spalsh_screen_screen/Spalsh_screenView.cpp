#include <gui/spalsh_screen_screen/Spalsh_screenView.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include "main.h"


Spalsh_screenView::Spalsh_screenView()
{

}

void Spalsh_screenView::setupScreen()
{
    Spalsh_screenViewBase::setupScreen();
		screen_rotation.setPosition(0, 0, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT);
		add(screen_rotation);
		Set_Splash_Title();
}

void Spalsh_screenView::tearDownScreen()
{
    Spalsh_screenViewBase::tearDownScreen();
}
void Spalsh_screenView::Set_Splash_Title(void)
{
	#ifdef CONCRETE	
		SPLASH_TITLE.setTypedText(touchgfx::TypedText(T_SPLASH_FW412));
	#endif
	#ifdef SOIL			
		SPLASH_TITLE.setTypedText(touchgfx::TypedText(T_SPLASH_FW403));
	#endif
}
