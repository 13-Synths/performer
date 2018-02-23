#pragma once

#include "BasePage.h"

class TopPage : public BasePage {
public:
    TopPage(PageManager &manager, PageContext &context);

    virtual void updateLeds(Leds &leds) override;

    virtual void keyDown(KeyEvent &event) override;
    virtual void keyUp(KeyEvent &event) override;
    virtual void keyPress(KeyPressEvent &event) override;
    virtual void encoder(EncoderEvent &event) override;

private:
    void setMainPage(Page &page);

    bool isSequencePage();
    void setSequencePage();

    bool isSequenceSetupPage();
    void setSequenceSetupPage();
};
