#pragma once

#include "ListPage.h"

#include "ui/model/ProjectListModel.h"

class ProjectPage : public ListPage {
public:
    ProjectPage(PageManager &manager, PageContext &context);

    virtual void enter() override;
    virtual void exit() override;

    virtual void draw(Canvas &canvas) override;
    virtual void updateLeds(Leds &leds) override;

    virtual void keyDown(KeyEvent &event) override;
    virtual void keyUp(KeyEvent &event) override;
    virtual void encoder(EncoderEvent &event) override;

private:
    void loadProject();
    void saveProject();
    void formatSDCard();

    ProjectListModel _listModel;
};
