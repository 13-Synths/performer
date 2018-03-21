#pragma once

#include "Config.h"
#include "Types.h"
#include "ClockSetup.h"
#include "Track.h"
#include "PlayState.h"
#include "Routing.h"
#include "Serialize.h"

#include "core/math/Math.h"
#include "core/utils/StringBuilder.h"
#include "core/utils/StringUtils.h"

class Project {
public:
    //----------------------------------------
    // Types
    //----------------------------------------

    static constexpr size_t NameLength = 16;

    typedef std::array<Track, CONFIG_TRACK_COUNT> TrackArray;

    Project();

    //----------------------------------------
    // Properties
    //----------------------------------------

    // slot

    int slot() const { return _slot; }
    void setSlot(int slot) {
        _slot = slot;
    }
    bool slotAssigned() const {
        return _slot != uint8_t(-1);
    }

    // name

    const char *name() const { return _name; }
    void setName(const char *name) {
        StringUtils::copy(_name, name, sizeof(_name));
    }

    // bpm

    float bpm() const { return _bpm; }
    void setBpm(float bpm) {
        _bpm = clamp(bpm, 1.f, 1000.f);
    }

    void editBpm(int value, bool shift) {
        setBpm(bpm() + value * (shift ? 0.1f : 1.f));
    }

    void printBpm(StringBuilder &str) const {
        str("%.1f", bpm());
    }

    // swing

    int swing() const { return _swing; }
    void setSwing(int swing) {
        _swing = clamp(swing, 50, 75);
    }

    void editSwing(int value, bool shift) {
        setSwing(ModelUtils::adjustedByStep(swing(), value, 5, shift));
    }

    void printSwing(StringBuilder &str) const {
        str("%d%%", swing());
    }

    // syncMeasure

    int syncMeasure() const { return _syncMeasure; }
    void setSyncMeasure(int syncMeasure) {
        _syncMeasure = clamp(syncMeasure, 1, 128);
    }

    void editSyncMeasure(int value, bool shift) {
        setSyncMeasure(ModelUtils::adjustedByPowerOfTwo(syncMeasure(), value, shift));
    }

    void printSyncMeasure(StringBuilder &str) const {
        str("%d", syncMeasure());
    }

    // clockSetup

    const ClockSetup &clockSetup() const { return _clockSetup; }
          ClockSetup &clockSetup()       { return _clockSetup; }

    // tracks

    const TrackArray &tracks() const { return _tracks; }
          TrackArray &tracks()       { return _tracks; }

    const Track &track(int index) const { return _tracks[index]; }
          Track &track(int index)       { return _tracks[index]; }

    // playState

    const PlayState &playState() const { return _playState; }
          PlayState &playState()       { return _playState; }

    // routing

    const Routing &routing() const { return _routing; }
          Routing &routing()       { return _routing; }

    // selectedTrackIndex

    int selectedTrackIndex() const { return _selectedTrackIndex; }
    void setSelectedTrackIndex(int index) {
        _selectedTrackIndex = clamp(index, 0, CONFIG_TRACK_COUNT - 1);
    }

    bool isSelectedTrack(int index) const { return _selectedTrackIndex == index; }

    // selectedPatternIndex

    int selectedPatternIndex() const { return _selectedPatternIndex; }
    void setSelectedPatternIndex(int index) {
        _selectedPatternIndex = clamp(index, 0, CONFIG_PATTERN_COUNT - 1);
    }

    bool isSelectedPattern(int index) const { return _selectedPatternIndex == index; }

    void editSelectedPatternIndex(int value, bool shift) {
        setSelectedPatternIndex(selectedPatternIndex() + value);
    }

    //----------------------------------------
    // Methods
    //----------------------------------------

    void setTrackMode(int trackIndex, Types::TrackMode trackMode);

    const Track &selectedTrack() const { return _tracks[_selectedTrackIndex]; }
          Track &selectedTrack()       { return _tracks[_selectedTrackIndex]; }

    const NoteSequence &noteSequence(int trackIndex, int patternIndex) const { return _tracks[trackIndex].noteTrack().sequence(patternIndex); }
          NoteSequence &noteSequence(int trackIndex, int patternIndex)       { return _tracks[trackIndex].noteTrack().sequence(patternIndex); }

    const NoteSequence &selectedNoteSequence() const { return noteSequence(_selectedTrackIndex, _selectedPatternIndex); }
          NoteSequence &selectedNoteSequence()       { return noteSequence(_selectedTrackIndex, _selectedPatternIndex); }

    const CurveSequence &curveSequence(int trackIndex, int patternIndex) const { return _tracks[trackIndex].curveTrack().sequence(patternIndex); }
          CurveSequence &curveSequence(int trackIndex, int patternIndex)       { return _tracks[trackIndex].curveTrack().sequence(patternIndex); }

    const CurveSequence &selectedCurveSequence() const { return curveSequence(_selectedTrackIndex, _selectedPatternIndex); }
          CurveSequence &selectedCurveSequence()       { return curveSequence(_selectedTrackIndex, _selectedPatternIndex); }


    void clear();
    void clearPattern(int patternIndex);

    void demoProject();

    void write(WriteContext &context) const;
    void read(ReadContext &context);

    fs::Error write(const char *path) const;
    fs::Error read(const char *path);

private:
    uint8_t _slot = uint8_t(-1);
    char _name[NameLength + 1] = { 'I', 'N', 'I', 'T', '\0' };
    float _bpm = 120.f;
    uint8_t _swing = 50;
    uint8_t _syncMeasure = 1;

    ClockSetup _clockSetup;
    TrackArray _tracks;
    PlayState _playState;
    Routing _routing;

    int _selectedTrackIndex = 0;
    int _selectedPatternIndex = 0;
};
