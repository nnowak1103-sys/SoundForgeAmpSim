#include "AmpModel.h"
namespace soundforge {
void AmpModel::prepare(const juce::dsp::ProcessSpec&) {}
void AmpModel::setDrive(float value) { drive = juce::jlimit(0.0f, 10.0f, value); }
void AmpModel::process(juce::AudioBuffer<float>& buffer)
{
    const float pregain = juce::Decibels::decibelsToGain(drive * 3.0f);
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            const float x = data[i] * pregain;
            data[i] = std::tanh(x) * 0.72f + 0.08f * std::tanh(x * 0.35f);
        }
    }
}
}
