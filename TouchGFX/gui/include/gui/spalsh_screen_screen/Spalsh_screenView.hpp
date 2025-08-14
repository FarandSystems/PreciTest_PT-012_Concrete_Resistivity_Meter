#ifndef SPALSH_SCREENVIEW_HPP
#define SPALSH_SCREENVIEW_HPP

#include <gui_generated/spalsh_screen_screen/Spalsh_screenViewBase.hpp>
#include <gui/spalsh_screen_screen/Spalsh_screenPresenter.hpp>
#include <Mirror.hpp>

class Spalsh_screenView : public Spalsh_screenViewBase
{
public:
    Spalsh_screenView();
    virtual ~Spalsh_screenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
		virtual	void Set_Splash_Title(void);
		Mirror screen_rotation;
protected:
};

#endif // SPALSH_SCREENVIEW_HPP
