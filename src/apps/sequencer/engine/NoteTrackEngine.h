#pragma once

#include "TrackEngine.h"
#include "SequenceState.h"
#include "SortedQueue.h"
#include "Groove.h"

#include "model/Track.h"

class NoteTrackEngine : public TrackEngine {
public:
    NoteTrackEngine(const Track &track, const TrackEngine *linkedTrackEngine) :
        TrackEngine(track, linkedTrackEngine),
        _noteTrack(track.noteTrack())
    {}

    virtual void reset() override;
    virtual void tick(uint32_t tick) override;
    virtual void changePattern() override;

    virtual const TrackLinkData *linkData() const override { return &_linkData; }

    virtual bool activity() const override { return _gate; }
    virtual bool gateOutput(int index) const override { return _gateOutput; }
    virtual float cvOutput(int index) const override { return _cvOutput; }

    const NoteSequence &sequence() const { return *_sequence; }
    bool isActiveSequence(const NoteSequence &sequence) const { return &sequence == _sequence; }

    int currentStep() const { return _currentStep; }

    static constexpr Track::TrackMode trackMode = Track::TrackMode::Note;

private:
    void triggerStep(uint32_t tick, uint32_t divisor);
    uint32_t applySwing(uint32_t tick);

    const NoteTrack &_noteTrack;

    TrackLinkData _linkData;

    const NoteSequence *_sequence;
    SequenceState _sequenceState;
    int _currentStep;

    bool _gate;
    bool _gateOutput;
    float _cvOutput;

    struct Gate {
        uint32_t tick;
        bool gate;
    };

    struct GateCompare {
        bool operator()(const Gate &a, const Gate &b) {
            return a.tick < b.tick;;
        }
    };

    SortedQueue<Gate, 16, GateCompare> _gateQueue;

    struct Cv {
        uint32_t tick;
        float cv;
    };

    struct CvCompare {
        bool operator()(const Cv &a, const Cv &b) {
            return a.tick < b.tick;
        }
    };

    SortedQueue<Cv, 16, CvCompare> _cvQueue;
};
