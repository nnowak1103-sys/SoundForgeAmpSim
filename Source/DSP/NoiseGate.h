#pragma once
#include <JuceHeader.h>
namespace soundforge {
class NoiseGate {
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void setThresholdDb(float db);
    void process(juce::AudioBuffer<float>& buffer);
private:
    float thresholdDb = -65.0f;
};
}
