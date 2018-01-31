#pragma once

#include "SystemConfig.h"

#include <cstdint>
#include <cstdlib>

class DAC {
public:
    static constexpr size_t Channels = CONFIG_DAC_CHANNELS;

    typedef uint16_t Value;

    void init();

    void setValue(int channel, Value value) {
        _values[channel] = value;
    }

    void write(int channel);
    void write();

private:
    void writeDAC(uint8_t command, uint8_t address, uint16_t data, uint8_t function);

    void reset();
    void setInternalRef(bool enabled);

    enum ClearCode {
        ClearZeroScale  = 0,
        ClearMidScale   = 1,
        ClearFullScale  = 2,
        ClearIgnore     = 3,
    };

    void setClearCode(ClearCode code);

    Value _values[Channels];
};
