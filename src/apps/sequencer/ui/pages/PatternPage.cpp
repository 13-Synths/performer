#include "PatternPage.h"

#include "Pages.h"

#include "ui/LedPainter.h"
#include "ui/painters/WindowPainter.h"

#include "model/PlayState.h"

#include "core/utils/StringBuilder.h"

enum class Function {
    Edit    = 0,
};

static const char *functionNames[] = { "EDIT", nullptr, nullptr, nullptr, nullptr };

enum class ContextAction {
    Init,
    Copy,
    Paste,
    Last
};

const ContextMenuModel::Item contextMenuItems[] = {
    { "INIT" },
    { "COPY" },
    { "PASTE" },
};


PatternPage::PatternPage(PageManager &manager, PageContext &context) :
    BasePage(manager, context),
    _contextMenu(manager.pages().contextMenu, contextMenuItems, int(ContextAction::Last), [&] (int index) { contextAction(index); })
{}

void PatternPage::enter() {
}

void PatternPage::exit() {
}

void PatternPage::draw(Canvas &canvas) {

    WindowPainter::clear(canvas);
    WindowPainter::drawHeader(canvas, _model, _engine, "PATTERN");
    WindowPainter::drawFunctionKeys(canvas, functionNames, _keyState);

    const auto &playState = _project.playState();

    float globalMeasureFraction = _engine.globalMeasureFraction();
    bool hasRequested = false;

    canvas.setFont(Font::Tiny);
    canvas.setBlendMode(BlendMode::Set);

    for (int trackIndex = 0; trackIndex < CONFIG_TRACK_COUNT; ++trackIndex) {
        const auto &trackState = playState.trackState(trackIndex);

        int x = trackIndex * 32;
        int y = 16;

        int w = 16;

        x += 8;

        canvas.setColor(trackState.fill() ? 0xf : 0x7);
        canvas.drawTextCentered(x, y - 2, w, 8, FixedStringBuilder<8>("T%d", trackIndex + 1));

        y += 8;

        canvas.setColor(trackState.fill() ? 0xf : 0x7);
        canvas.drawTextCentered(x, y + 10, w, 8, FixedStringBuilder<8>("P%d", trackState.pattern() + 1));

        if (trackState.pattern() != trackState.requestedPattern()) {
            hasRequested = true;
        }
    }

    if (hasRequested) {
        canvas.setColor(0xf);
        canvas.hline(0, 10, globalMeasureFraction * Width);
    }
}

void PatternPage::updateLeds(Leds &leds) {
}

void PatternPage::keyDown(KeyEvent &event) {
}

void PatternPage::keyUp(KeyEvent &event) {
}

void PatternPage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();
    auto &playState = _project.playState();

    if (key.isContextMenu()) {
        _contextMenu.show();
        event.consume();
        return;
    }

    if (key.pageModifier()) {
        return;
    }

    if (key.isTrackSelect()) {
        event.consume();
    }

    if (key.isFunction()) {
        event.consume();
    }

    if (key.isStep()) {
        int pattern = key.step();

        if (_keyState[MatrixMap::fromFunction(int(Function::Edit))]) {
            // select edit pattern
            _project.setSelectedPatternIndex(pattern);
        } else {
            // select playing pattern
            bool globalChange = true;
            for (int trackIndex = 0; trackIndex < CONFIG_TRACK_COUNT; ++trackIndex) {
                if (_keyState[MatrixMap::fromTrack(trackIndex)]) {
                    playState.selectTrackPattern(trackIndex, pattern, key.shiftModifier() ? PlayState::Scheduled : PlayState::Immediate);
                    globalChange = false;
                }
            }
            if (globalChange) {
                playState.selectPattern(pattern, key.shiftModifier() ? PlayState::Scheduled : PlayState::Immediate);
            }
        }

        event.consume();
    }

    if (key.is(Key::Left)) {
        _project.editSelectedPatternIndex(-1, false);
        event.consume();
    }
    if (key.is(Key::Right)) {
        _project.editSelectedPatternIndex(1, false);
        event.consume();
    }
}

void PatternPage::encoder(EncoderEvent &event) {
    _project.editSelectedPatternIndex(event.value(), event.pressed());
}

void PatternPage::contextAction(int index) {
    switch (ContextAction(index)) {
    case ContextAction::Init:
        initPattern();
        break;
    case ContextAction::Copy:
        copyPattern();
        break;
    case ContextAction::Paste:
        pastePattern();
        break;
    case ContextAction::Last:
        break;
    }
}

void PatternPage::initPattern() {
    _project.clearPattern(_project.selectedPatternIndex());
}

void PatternPage::copyPattern() {
    _model.clipBoard().patternBuffer().copyFrom(_project, _project.selectedPatternIndex());
}

void PatternPage::pastePattern() {
    _model.clipBoard().patternBuffer().pasteTo(_project, _project.selectedPatternIndex());
}
