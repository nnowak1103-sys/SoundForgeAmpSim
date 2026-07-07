#pragma once
#include <JuceHeader.h>
namespace soundforge {
class ToneStack {
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void setControls(float bass, float mid, float treble, float tone, float presence);
    void process(juce::AudioBuffer<float>& buffer);
private:
    double sampleRate = 44100.0;
    float b = 5, m = 5, t = 5, tn = 5, p = 5;
};
}
