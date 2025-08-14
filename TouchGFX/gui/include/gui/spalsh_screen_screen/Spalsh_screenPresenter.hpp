#ifndef SPALSH_SCREENPRESENTER_HPP
#define SPALSH_SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Spalsh_screenView;

class Spalsh_screenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Spalsh_screenPresenter(Spalsh_screenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~Spalsh_screenPresenter() {};

private:
    Spalsh_screenPresenter();

    Spalsh_screenView& view;
};

#endif // SPALSH_SCREENPRESENTER_HPP
