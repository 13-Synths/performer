#pragma once

#include "Clock.h"
#include "TapTempo.h"
#include "NudgeTempo.h"
#include "Track.h"
#include "Controller.h"
#include "CVInput.h"
#include "CVOutput.h"

#include "model/Model.h"

#include "drivers/ClockTimer.h"
#include "drivers/ADC.h"
#include "drivers/DAC.h"
#include "drivers/DIO.h"
#include "drivers/GateOutput.h"
#include "drivers/MIDI.h"
#include "drivers/USBMIDI.h"

#include <array>

#include <cstdint>

class Engine {
public:
    typedef std::array<TrackEngine, CONFIG_TRACK_COUNT> TrackEngineArray;

    typedef std::function<void(const char *text, uint32_t duration)> MessageHandler;

    enum ClockSource {
        ClockSourceExternal,
        ClockSourceMIDI,
        ClockSourceUSBMIDI,
    };

    Engine(Model &model, ClockTimer &clockTimer, ADC &adc, DAC &dac, DIO &dio, GateOutput &gateOutput, MIDI &midi, USBMIDI &usbMidi);

    void init();
    void update();

    // transport control
    void start();
    void stop();
    void resume();
    bool running() const { return _running; }

    // tempo
    float bpm() { return _clock.bpm(); }

    // tap tempo
    void tapTempoReset();
    void tapTempoTap();

    // nudge tempo
    void nudgeTempoSetDirection(int direction);
    float nudgeTempoStrength() const;

    // time base
    uint32_t tick() const { return _tick; }

    const CVInput &cvInput() const { return _cvInput; }
    const CVOutput &cvOutput() const { return _cvOutput; }

    // gate overrides
    bool gateOutputOverride() const { return _gateOutputOverride; }
    void setGateOutputOverride(bool enabled) { _gateOutputOverride = enabled; }
    void setGateOutput(uint8_t gates) { _gateOutputOverrideValue = gates; }

    bool cvOutputOverride() const { return _cvOutputOverride; }
    void setCvOutputOverride(bool enabled) { _cvOutputOverride = enabled; }
    void setCvOutput(int channel, float value) { _cvOutputOverrideValues[channel] = value; }

    const Clock &clock() const { return _clock; }
          Clock &clock()       { return _clock; }

    const TrackEngineArray &tracks() const { return _tracks; }
          TrackEngineArray &tracks()       { return _tracks; }

    const TrackEngine &track(int index) const { return _tracks[index]; }
          TrackEngine &track(int index)       { return _tracks[index]; }

    // message handling
    void showMessage(const char *text, uint32_t duration = 1000);
    void setMessageHandler(MessageHandler handler);

private:
    void setupClockSources();
    void setupClockOutputs();

    Model &_model;
    ADC &_adc;
    DAC &_dac;
    DIO &_dio;
    GateOutput &_gateOutput;
    MIDI &_midi;
    USBMIDI &_usbMidi;

    CVInput _cvInput;
    CVOutput _cvOutput;

    Clock _clock;
    TapTempo _tapTempo;
    NudgeTempo _nudgeTempo;

    TrackEngineArray _tracks;

    ControllerManager _controllerManager;

    bool _running = false;
    uint32_t _tick = 0;

    uint32_t _lastSystemTicks = 0;

    // gate output overrides
    bool _gateOutputOverride = false;
    uint8_t _gateOutputOverrideValue = 0;

    // cv output overrides
    bool _cvOutputOverride = false;
    std::array<float, CVOutput::Channels> _cvOutputOverrideValues;

    MessageHandler _messageHandler;
};
