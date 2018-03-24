#pragma once

#include "Config.h"

#include "ListModel.h"

#include "model/MidiCvTrack.h"

class MidiCvTrackListModel : public ListModel {
public:
    void setTrack(MidiCvTrack &track) {
        _track = &track;
    }

    virtual int rows() const override {
        return Last;
    }

    virtual int columns() const override {
        return 2;
    }

    virtual void cell(int row, int column, StringBuilder &str) const override {
        if (column == 0) {
            formatName(Item(row), str);
        } else if (column == 1) {
            formatValue(Item(row), str);
        }
    }

    virtual void edit(int row, int column, int value, bool shift) override {
        if (column == 1) {
            editValue(Item(row), value, shift);
        }
    }

private:
    enum Item {
        Port,
        Channel,
        Voices,
        Last
    };

    static const char *itemName(Item item) {
        switch (item) {
        case Port:      return "Port";
        case Channel:   return "Channel";
        case Voices:    return "Voices";
        case Last:      break;
        }
        return nullptr;
    }

    void formatName(Item item, StringBuilder &str) const {
        str(itemName(item));
    }

    void formatValue(Item item, StringBuilder &str) const {
        switch (item) {
        case Port:
            _track->printPort(str);
            break;
        case Channel:
            _track->printChannel(str);
            break;
        case Voices:
            _track->printVoices(str);
            break;
        case Last:
            break;
        }
    }

    void editValue(Item item, int value, bool shift) {
        switch (item) {
        case Port:
            _track->editPort(value, shift);
            break;
        case Channel:
            _track->editChannel(value, shift);
            break;
        case Voices:
            _track->editVoices(value, shift);
            break;
        case Last:
            break;
        }
    }

    MidiCvTrack *_track;
};
