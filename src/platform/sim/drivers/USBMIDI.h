#pragma once

#include "core/midi/MIDIMessage.h"
#include "core/midi/MIDIParser.h"

#include "sim/Simulator.h"

#include <functional>
#include <deque>

#include <cstdint>

class USBMIDI {
public:
    USBMIDI(sim::Simulator &simulator) :
        _simulator(simulator)
    {
        _simulator.recvMIDI(sim::Simulator::MIDIUSBHostPort, [this] (uint8_t data) {
            if (!_filter || !_filter(data)) {
                std::lock_guard<std::mutex> lock(_recvMutex);
                _recvQueue.emplace_back(data);
            }
        });
    }

    void send(const MIDIMessage &message) {
        _simulator.sendMIDI(sim::Simulator::MIDIUSBHostPort, message.raw(), message.length());
    }

    bool recv(MIDIMessage *message) {
        std::lock_guard<std::mutex> lock(_recvMutex);
        while (!_recvQueue.empty()) {
            uint8_t data = _recvQueue.front();
            _recvQueue.pop_front();
            if (_midiParser.feed(data)) {
                *message = _midiParser.message();
                return true;
            }
        }
        return false;
    }

    void setRecvFilter(std::function<bool(uint8_t)> filter) {
        _filter = filter;
    }

private:
    sim::Simulator &_simulator;
    std::deque<uint8_t> _recvQueue;
    std::mutex _recvMutex;
    std::function<bool(uint8_t)> _filter;
    MIDIParser _midiParser;
};
