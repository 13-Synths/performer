#include "MidiCvTrack.h"

void MidiCvTrack::clear() {
    _port = Types::MidiPort::Midi;
    _channel = -1;
    _voices = 1;
    _voiceConfig = VoiceConfig::Pitch;
}

void MidiCvTrack::gateOutputName(int index, StringBuilder &str) const {
    str("Gate %d", (index % _voices) + 1);
}

void MidiCvTrack::cvOutputName(int index, StringBuilder &str) const {
    int signals = int(_voiceConfig) + 1;
    int totalOutputs = _voices * signals;
    index %= totalOutputs;
    int voiceIndex = index % _voices;
    int signalIndex = index / _voices;
    switch (signalIndex) {
    case 0: str("V/Oct %d", voiceIndex + 1); break;
    case 1: str("Vel %d", voiceIndex + 1); break;
    case 2: str("Press %d", voiceIndex + 1); break;
    }
}

void MidiCvTrack::write(WriteContext &context) const {
    auto &writer = context.writer;
    writer.write(_port);
    writer.write(_channel);
    writer.write(_voices);
    writer.write(_voiceConfig);
}

void MidiCvTrack::read(ReadContext &context) {
    auto &reader = context.reader;
    reader.read(_port);
    reader.read(_channel);
    reader.read(_voices);
    reader.read(_voiceConfig);
}
