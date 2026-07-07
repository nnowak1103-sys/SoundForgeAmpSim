#pragma once
#include <JuceHeader.h>
#include "NoiseGate.h"
#include "CabinetIR.h"

namespace SonicForge {
struct AmpParams { int amp = 0; float inputDb=0, gain=5, bass=5, mid=5, treble=5, tone=5, presence=5, resonance=4, master=6, volumeDb=-8, gateDb=-60, cabMix=1, room=.08f; };
class AmpEngine {
public:
    void prepare(double sampleRate, int blockSize, int channels);
    void process(juce::AudioBuffer<float>& buffer, const AmpParams& p);
private:
    float waveshape(float x, const AmpParams& p) noexcept;
    void updateFilters(const AmpParams& p);
    double sr=44100.0;
    std::vector<juce::dsp::IIR::Filter<float>> low, mid, high, presence, resonance;
    NoiseGate gate; CabinetIR cab;
    juce::dsp::Reverb reverb;
};
}
