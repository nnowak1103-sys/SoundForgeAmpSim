#pragma once
#include <JuceHeader.h>
namespace soundforge {
class AmpModel {
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void setDrive(float value);
    void process(juce::AudioBuffer<float>& buffer);
private:
    float drive = 4.0f;
};
}
