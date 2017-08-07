#pragma once

#include "Common.h"
#include "Window.h"
#include "Audio.h"
#include "Instrument.h"

namespace sim {

class Simulator {
public:
    Simulator();

    Window &window() { return _window; }
    Audio &audio() { return _audio; }

    bool terminate() const;

    void update();
    void render();

    double ticks();

    typedef std::function<void()> UpdateCallback;

    void addUpdateCallback(UpdateCallback callback);

    // Hardware IO emulation
    void writeGate(int channel, bool value);
    void writeDAC(int channel, uint16_t value);
    uint16_t readADC(int channel);

    // MIDI emulation
    enum MIDIPort {
        MIDIHardwarePort,
        MIDIUSBHostPort,
    };

    typedef std::function<void(uint8_t)> MIDIRecvCallback;

    void sendMIDI(int port, uint8_t data);
    void recvMIDI(int port, MIDIRecvCallback callback);

private:
    void setupInstruments();

    Window _window;
    Audio _audio;
    std::vector<Instrument::Ptr> _instruments;

    double _timerFrequency;
    double _timerStart;

    std::array<bool, 8> _gate;
    std::array<uint16_t, 8> _dac;

    std::vector<UpdateCallback> _updateCallbacks;
};

} // namespace sim
