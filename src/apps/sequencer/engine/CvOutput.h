#pragma once

#include "Config.h"

#include "drivers/DAC.h"

#include <array>

class CvOutput {
public:
    static constexpr int Channels = CONFIG_CV_OUTPUT_CHANNELS;

    CvOutput(DAC &dac);

    void init();

    void update();

    float channel(int index) const {
        return _channels[index];
    }

    void setChannel(int index, float value) {
        _channels[index] = value;
    }

private:
    DAC &_dac;

    std::array<float, Channels> _channels;
};
