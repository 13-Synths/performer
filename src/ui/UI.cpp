#include "UI.h"
#include "Keys.h"

#include "core/Debug.h"
#include "core/profiler/Profiler.h"
#include "core/utils/StringBuilder.h"

#include "model/Model.h"

PROFILER_INTERVAL(UIDraw, "UI.draw")
PROFILER_INTERVAL(UIInterval, "UI.interval")

UI::UI(Model &model, Engine &engine, LCD &lcd, ButtonLedMatrix &blm) :
    _model(model),
    _engine(engine),
    _lcd(lcd),
    _blm(blm),
    _frameBuffer(LCD_WIDTH, LCD_HEIGHT, _frameBufferData),
    _canvas(_frameBuffer),
    _pageManager(_pages),
    _pages(_pageManager, _model, _engine)
{
    _pageManager.push(_pages.topPage);
    _pageManager.push(_pages.mainPage);
}

void UI::init() {
    _canvas.setColor(0xf);
    _canvas.drawText(10, 30, "Hello World!");
    _lcd.draw(_frameBuffer.data());
}

void UI::update() {
    handleKeys();

    _pageManager.updateLeds(_leds);
    _blm.setLeds(_leds.data());

    static int counter = 0;
    if (counter % 20 == 0) {
        PROFILER_INTERVAL_END(UIInterval);
        PROFILER_INTERVAL_BEGIN(UIInterval);
        PROFILER_INTERVAL_BEGIN(UIDraw);

        _pageManager.draw(_canvas);
        _lcd.draw(_frameBuffer.data());

        PROFILER_INTERVAL_END(UIDraw);
    }
    ++counter;
}

void UI::handleKeys() {
    ButtonLedMatrix::Event event;
    while (_blm.nextEvent(event)) {
        switch (event.action()) {
        case ButtonLedMatrix::KeyDown: {
            KeyEvent keyEvent(Event::KeyDown, Key(event.value(), _blm.buttonState(Key::Shift)));
            _pageManager.dispatchEvent(keyEvent);
            break;
        }
        case ButtonLedMatrix::KeyUp: {
            KeyEvent keyEvent(Event::KeyUp, Key(event.value(), _blm.buttonState(Key::Shift)));
            _pageManager.dispatchEvent(keyEvent);
            break;
        }
        case ButtonLedMatrix::Encoder: {
            EncoderEvent encoderEvent(Event::Encoder, event.value());
            _pageManager.dispatchEvent(encoderEvent);
            break;
        }
        }
    }
}
