#include "TrackSetupPage.h"

#include "Pages.h"

#include "ui/LedPainter.h"
#include "ui/painters/WindowPainter.h"

#include "core/utils/StringBuilder.h"

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

TrackSetupPage::TrackSetupPage(PageManager &manager, PageContext &context) :
    ListPage(manager, context, _noteTrackListModel),
    _contextMenu(
        manager.pages().contextMenu,
        contextMenuItems,
        int(ContextAction::Last),
        [&] (int index) { contextAction(index); },
        [&] (int index) { return contextActionEnabled(index); }
    )
{}

void TrackSetupPage::enter() {
    setTrack(_project.selectedTrack());
}

void TrackSetupPage::exit() {
}

void TrackSetupPage::draw(Canvas &canvas) {
    WindowPainter::clear(canvas);
    WindowPainter::drawHeader(canvas, _model, _engine, "TRACK");

    // const char *functionNames[] = { nullptr, nullptr, nullptr, nullptr, nullptr };
    // WindowPainter::drawFunctionKeys(canvas, functionNames, _keyState);

    ListPage::draw(canvas);
}

void TrackSetupPage::updateLeds(Leds &leds) {
    ListPage::updateLeds(leds);
}

void TrackSetupPage::keyPress(KeyPressEvent &event) {
    const auto &key = event.key();

    if (key.isContextMenu()) {
        _contextMenu.show();
        event.consume();
        return;
    }

    if (key.pageModifier()) {
        return;
    }

    if (key.isTrackSelect()) {
        _project.setSelectedTrackIndex(key.trackSelect());
        setTrack(_project.selectedTrack());
    }

    ListPage::keyPress(event);
}

void TrackSetupPage::setTrack(Track &track) {
    switch (track.trackMode()) {
    case Track::TrackMode::Note:
        _noteTrackListModel.setTrack(track.noteTrack());
        setListModel(_noteTrackListModel);
        break;
    case Track::TrackMode::Curve:
        _curveTrackListModel.setTrack(track.curveTrack());
        setListModel(_curveTrackListModel);
        break;
    case Track::TrackMode::MidiCv:
        _midiCvTrackListModel.setTrack(track.midiCvTrack());
        setListModel(_midiCvTrackListModel);
        break;
    case Track::TrackMode::Last:
        break;
    }
}

void TrackSetupPage::contextAction(int index) {
    switch (ContextAction(index)) {
    case ContextAction::Init:
        initTrackSetup();
        break;
    case ContextAction::Copy:
        copyTrackSetup();
        break;
    case ContextAction::Paste:
        pasteTrackSetup();
        break;
    case ContextAction::Last:
        break;
    }
}

bool TrackSetupPage::contextActionEnabled(int index) const {
    switch (ContextAction(index)) {
    case ContextAction::Paste:
        return _model.clipBoard().canPasteTrack();
    default:
        return true;
    }
}

void TrackSetupPage::initTrackSetup() {
    _project.selectedTrack().clear();
    setTrack(_project.selectedTrack());
}

void TrackSetupPage::copyTrackSetup() {
    _model.clipBoard().copyTrack(_project.selectedTrack());
}

void TrackSetupPage::pasteTrackSetup() {
    _model.clipBoard().pasteTrack(_project.selectedTrack());
    setTrack(_project.selectedTrack());
}

