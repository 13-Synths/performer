#pragma once

#include "ListPage.h"

#include "ui/model/TrackSetupListModel.h"

class TrackSetupPage : public ListPage {
public:
    TrackSetupPage(PageManager &manager, PageContext &context);

    virtual void enter() override;
    virtual void exit() override;

    virtual void draw(Canvas &canvas) override;
    virtual void updateLeds(Leds &leds) override;

    virtual void keyPress(KeyPressEvent &event) override;

private:
    TrackSetupListModel _listModel;
};
