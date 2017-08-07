#pragma once

#include "sim/Audio.h"
#include "sim/Instrument.h"

#include "soloud.h"

namespace sim {

class Synth;
class Voice;

class SynthInstance : public SoLoud::AudioSourceInstance {
public:
    SynthInstance(Synth &synth);
    virtual ~SynthInstance();

    virtual void getAudio(float *aBuffer, unsigned int aSamples) override;
    virtual bool hasEnded() override;

private:
    Synth &_synth;
    std::unique_ptr<Voice> _voice;
};

class Synth : public Instrument, SoLoud::AudioSource {
public:
    Synth(Audio &audio);

    virtual SoLoud::AudioSourceInstance *createInstance() override;

    void setGate(bool gate);
    void setCV(float cv);

private:
    Audio &_audio;
    bool _gate;
    float _cv;

    friend class SynthInstance;
};

} // namespace sim
