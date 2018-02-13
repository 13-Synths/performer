#include "ClockSetup.h"

void ClockSetup::write(WriteContext &context) const {
    auto &writer = context.writer;
    writer.write(_mode);
    writer.write(_clockInputPPQN);
    writer.write(_clockInputMode);
    writer.write(_clockOutputPPQN);
    writer.write(_clockOutputPulse);
    writer.write(_clockOutputMode);
    writer.write(_midiRx);
    writer.write(_midiTx);
    writer.write(_usbRx);
    writer.write(_usbTx);
}

void ClockSetup::read(ReadContext &context) {
    auto &reader = context.reader;
    reader.read(_mode);
    reader.read(_clockInputPPQN);
    reader.read(_clockInputMode);
    reader.read(_clockOutputPPQN);
    reader.read(_clockOutputPulse);
    reader.read(_clockOutputMode);
    reader.read(_midiRx);
    reader.read(_midiTx);
    reader.read(_usbRx);
    reader.read(_usbTx);
}