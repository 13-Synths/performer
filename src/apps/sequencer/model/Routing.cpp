#include "Routing.h"

#include "Project.h"

struct ParamInfo {
    uint16_t min;
    uint16_t max;
};

const ParamInfo paramInfos[int(Routing::Param::Last)] = {
    [int(Routing::Param::BPM)]              = { 20,     500 },
    [int(Routing::Param::Swing)]            = { 50,     75  },
    [int(Routing::Param::SequenceParams)]   = { 0,      0   },
    [int(Routing::Param::FirstStep)]        = { 0,      63  },
    [int(Routing::Param::LastStep)]         = { 0,      63  },
};

static float normalizeParamValue(Routing::Param param, float value) {
    const auto &info = paramInfos[int(param)];
    return clamp((value - info.min) / (info.max - info.min), 0.f, 1.f);
}

static float denormalizeParamValue(Routing::Param param, float normalized) {
    const auto &info = paramInfos[int(param)];
    return normalized * (info.max - info.min) + info.min;
}

//----------------------------------------
// Routing::CvSource
//----------------------------------------

void Routing::CvSource::write(WriteContext &context) const {
    auto &writer = context.writer;
    writer.write(_index);
}

void Routing::CvSource::read(ReadContext &context) {
    auto &reader = context.reader;
    reader.read(_index);
}

//----------------------------------------
// Routing::TrackSource
//----------------------------------------

void Routing::TrackSource::write(WriteContext &context) const {
    auto &writer = context.writer;
    writer.write(_index);
}

void Routing::TrackSource::read(ReadContext &context) {
    auto &reader = context.reader;
    reader.read(_index);
}

//----------------------------------------
// Routing::MidiSource
//----------------------------------------

void Routing::MidiSource::write(WriteContext &context) const {
    auto &writer = context.writer;
    writer.write(_port);
    writer.write(_channel);
    writer.write(_kind);
    writer.write(_data);
}

void Routing::MidiSource::read(ReadContext &context) {
    auto &reader = context.reader;
    reader.read(_port);
    reader.read(_channel);
    reader.read(_kind);
    reader.read(_data);
}

//----------------------------------------
// Routing::Source
//----------------------------------------

void Routing::Source::clear() {
    _kind = Kind::None;
}

void Routing::Source::initCv(int index) {
    _kind = Kind::Cv;
    _source.cv.setIndex(index);
}

void Routing::Source::initTrack(int index) {
    _kind = Kind::Track;
    _source.track.setIndex(index);
}

void Routing::Source::initMidi() {
    _kind = Kind::Midi;
    // ...
}

void Routing::Source::write(WriteContext &context) const {
    auto &writer = context.writer;
    writer.write(_kind);
    switch (_kind) {
    case Kind::None:
        break;
    case Kind::Cv:
        _source.cv.write(context);
        break;
    case Kind::Track:
        _source.track.write(context);
        break;
    case Kind::Midi:
        _source.midi.write(context);
        break;
    case Kind::Last:
        break;
    }
}

void Routing::Source::read(ReadContext &context) {
    auto &reader = context.reader;
    reader.read(_kind);
    switch (_kind) {
    case Kind::None:
        break;
    case Kind::Cv:
        _source.cv.read(context);
        break;
    case Kind::Track:
        _source.track.read(context);
        break;
    case Kind::Midi:
        _source.midi.read(context);
        break;
    case Kind::Last:
        break;
    }
}

//----------------------------------------
// Routing::Route
//----------------------------------------

void Routing::Route::clear() {
    _active = false;
}

void Routing::Route::init(Param param, int track) {
    _param = param;
    _track = -1;
    _source.clear();
    _active = true;
}

void Routing::Route::write(WriteContext &context) const {
    auto &writer = context.writer;
    writer.write(_active);
    writer.write(_param);
    writer.write(_track);
    _source.write(context);
}

void Routing::Route::read(ReadContext &context) {
    auto &reader = context.reader;
    reader.read(_active);
    reader.read(_param);
    reader.read(_track);
    _source.read(context);
}

//----------------------------------------
// Routing
//----------------------------------------

Routing::Routing(Project &project) :
    _project(project)
{}

void Routing::clear() {
    for (auto &route : _routes) {
        route.clear();
    }

    // {
    //     auto route = addRoute(Param::BPM);
    //     route->source().initMIDI();
    //     // route->source().midi().setKind(MIDISource::Kind::ControllerAbs);
    //     // route->source().midi().setPort(MIDISource::Port::MIDI);
    //     // route->source().midi().setChannel(1);
    //     // route->source().midi().setController(16);

    //     // route->source().midi().setKind(MIDISource::Kind::PitchBend);
    //     // route->source().midi().setPort(MIDISource::Port::MIDI);
    //     // route->source().midi().setChannel(1);

    //     route->source().midi().setKind(MIDISource::Kind::NoteVelocity);
    //     route->source().midi().setPort(MIDISource::Port::MIDI);
    //     route->source().midi().setChannel(1);
    //     route->source().midi().setNote(60);

    // }

    // {
    //     auto route = addRoute(Param::Swing);
    //     route->source().initMIDI();
    //     route->source().midi().setKind(MIDISource::Kind::ControllerRel);
    //     route->source().midi().setPort(MIDISource::Port::MIDI);
    //     route->source().midi().setChannel(1);
    //     route->source().midi().setController(63);
    // }
}

Routing::Route *Routing::nextFreeRoute() {
    for (auto &route : _routes) {
        if (!route.active()) {
            return &route;
        }
    }
    return nullptr;
}

const Routing::Route *Routing::findRoute(Param param, int trackIndex) const {
    for (auto &route : _routes) {
        if (route.active() && route.param() == param && route.track() == trackIndex) {
            return &route;
        }
    }
    return nullptr;
}

Routing::Route *Routing::findRoute(Param param, int trackIndex) {
    for (auto &route : _routes) {
        if (route.active() && route.param() == param && route.track() == trackIndex) {
            return &route;
        }
    }
    return nullptr;
}

Routing::Route *Routing::addRoute(Param param, int trackIndex) {
    Route *route = findRoute(param, trackIndex);
    if (route) {
        return route;
    }

    route = nextFreeRoute();
    if (!route) {
        return nullptr;
    }

    route->init(param, trackIndex);

    return route;
}

void Routing::removeRoute(Route *route) {
    if (route) {
        route->clear();
    }
}

void Routing::writeParam(Param param, int trackIndex, int patternIndex, float value) {
    value = denormalizeParamValue(param, value);
    switch (param) {
    case Param::BPM:
        _project.setBpm(value);
        break;
    case Param::Swing:
        _project.setSwing(value);
        break;
    default:
        writeTrackParam(param, trackIndex, patternIndex, value);
        break;
    }
}

void Routing::writeTrackParam(Param param, int trackIndex, int patternIndex, float value) {
    auto &track = _project.track(trackIndex);
    switch (track.trackMode()) {
    case Track::TrackMode::Note:
        writeNoteSequenceParam(track.noteTrack().sequence(patternIndex), param, value);
        break;
    case Track::TrackMode::Curve:
        writeCurveSequenceParam(track.curveTrack().sequence(patternIndex), param, value);
        break;
    case Track::TrackMode::Last:
        break;
    }
}

void Routing::writeNoteSequenceParam(NoteSequence &sequence, Param param, float value) {
    switch (param) {
    case Param::FirstStep:
        sequence.setFirstStep(value);
        break;
    case Param::LastStep:
        sequence.setLastStep(value);
        break;
    default:
        break;
    }
}

void Routing::writeCurveSequenceParam(CurveSequence &sequence, Param param, float value) {

}

float Routing::readParam(Param param, int patternIndex, int trackIndex) const {
    switch (param) {
    case Param::BPM:
        return _project.bpm();
    case Param::Swing:
        return _project.swing();
    default:
        return 0.f;
    }
}

void Routing::write(WriteContext &context) const {
    writeArray(context, _routes);
}

void Routing::read(ReadContext &context) {
    readArray(context, _routes);
}
