#pragma once

#include "BasePage.h"

#include "ui/model/ListModel.h"

class ListPage : public BasePage {
public:
    ListPage(PageManager &manager, PageContext &context, ListModel &listModel);

    virtual void show() override;

    virtual void enter() override;
    virtual void exit() override;

    virtual void draw(Canvas &canvas) override;
    virtual void updateLeds(Leds &leds) override;

    virtual void keyPress(KeyPressEvent &event) override;
    virtual void encoder(EncoderEvent &event) override;

    int selectedRow() const { return _selectedRow; }
    void setSelectedRow(int selectedRow);

protected:
    void scrollTo(int row);

    ListModel &_listModel;
    int _selectedRow = 0;
    int _displayRow = 0;
    bool _edit = false;
};
