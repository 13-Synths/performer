#include "TopPage.h"

#include "Pages.h"

#include "ui/LedPainter.h"

TopPage::TopPage(PageManager &manager, PageContext &context) :
    BasePage(manager, context)
{}

void TopPage::updateLeds(Leds &leds) {
    leds.set(
        Key::Start,
        _engine.running() && _engine.tick() % CONFIG_PPQN < (CONFIG_PPQN / 8),
        false // _engine.running()
    );

    LedPainter::drawTracksGateAndSelected(leds, _engine, _project.selectedTrackIndex());
}

void TopPage::keyDown(KeyEvent &event) {
    event.consume();
}

void TopPage::keyUp(KeyEvent &event) {
    event.consume();
}

void TopPage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();

    if (key.isTrackSelect()) {
        _project.setSelectedTrackIndex(key.trackSelect());
        if (isSequencePage()) {
            setSequencePage();
        } else if (isSequenceSetupPage()) {
            setSequenceSetupPage();
        }
    }

    if (key.isModeSelect()) {
        switch (key.modeSelect()) {
        case 0: setMainPage(_manager.pages().project); break;
        case 1: setMainPage(_manager.pages().pattern); break;
        case 2: setMainPage(_manager.pages().trackSetup); break;
        case 3: setSequencePage(); break;
        case 4: setSequenceSetupPage(); break;
        case 7: setMainPage(_manager.pages().performer); break;
        }
    }

    if (key.is(Key::Start)) {
        if (key.shiftModifier()) {
            // restart
            _engine.start();
        } else {
            // start/stop
            if (!_engine.running()) {
                _engine.resume();
            } else {
                _engine.stop();
            }
        }
        event.consume();
    }

    if (key.is(Key::BPM)) {
        if (key.shiftModifier()) {
            // clock setup page
            _manager.pages().clockSetup.show();
        } else {
            // bpm page
            _manager.pages().bpm.show();
        }
    }

    // if (key.is(Key::Mute)) {
    //     _manager.pages().mute.show();
    // }

    if (key.is(Key::Global)) {
        _manager.pages().monitor.show();
    }

    event.consume();
}

void TopPage::encoder(EncoderEvent &event) {
    event.consume();
}

void TopPage::setMainPage(Page &page) {
    _manager.reset(&_manager.pages().top);
    _manager.push(&page);
}

bool TopPage::isSequencePage() {
    return
        _manager.top() == &_manager.pages().noteSequence ||
        _manager.top() == &_manager.pages().curveSequence;
}

void TopPage::setSequencePage() {
    switch (_project.selectedTrackSetup().trackMode()) {
    case Types::TrackMode::Note:
        setMainPage(_manager.pages().noteSequence);
        break;
    case Types::TrackMode::Curve:
        setMainPage(_manager.pages().curveSequence);
        break;
    case Types::TrackMode::Last:
        break;
    }
}

bool TopPage::isSequenceSetupPage() {
    return
        _manager.top() == &_manager.pages().noteSequenceSetup ||
        _manager.top() == &_manager.pages().curveSequenceSetup;
}

void TopPage::setSequenceSetupPage() {
    switch (_project.selectedTrackSetup().trackMode()) {
    case Types::TrackMode::Note:
        setMainPage(_manager.pages().noteSequenceSetup);
        break;
    case Types::TrackMode::Curve:
        setMainPage(_manager.pages().curveSequenceSetup);
        break;
    case Types::TrackMode::Last:
        break;
    }
}
