#include "NoiseGate.h"
namespace soundforge {
void NoiseGate::prepare(const juce::dsp::ProcessSpec&) {}
void NoiseGate::setThresholdDb(float db) { thresholdDb = db; }
void NoiseGate::process(juce::AudioBuffer<float>& buffer)
{
    const float threshold = juce::Decibels::decibelsToGain(thresholdDb);
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
            if (std::abs(data[i]) < threshold) data[i] *= 0.15f;
    }
}
}
