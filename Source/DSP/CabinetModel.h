#pragma once
#include <JuceHeader.h>
namespace soundforge {
class CabinetModel {
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void setBlend(float value);
    void process(juce::AudioBuffer<float>& buffer);
private:
    float blend = 0.8f;
};
}
