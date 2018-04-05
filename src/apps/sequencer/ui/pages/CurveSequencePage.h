#pragma once

#include "ListPage.h"
#include "ContextMenu.h"

#include "ui/model/CurveSequenceListModel.h"

class CurveSequencePage : public ListPage {
public:
    CurveSequencePage(PageManager &manager, PageContext &context);

    virtual void enter() override;
    virtual void exit() override;

    virtual void draw(Canvas &canvas) override;
    virtual void updateLeds(Leds &leds) override;

    virtual void keyPress(KeyPressEvent &event) override;

private:
    void contextAction(int index);
    bool contextActionEnabled(int index) const;
    void initSequence();
    void copySequence();
    void pasteSequence();
    void duplicateSequence();

    CurveSequenceListModel _listModel;

    ContextMenu _contextMenu;
};
