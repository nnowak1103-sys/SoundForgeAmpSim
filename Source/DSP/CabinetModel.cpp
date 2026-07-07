#include "CabinetModel.h"
namespace soundforge {
void CabinetModel::prepare(const juce::dsp::ProcessSpec&) {}
void CabinetModel::setBlend(float value) { blend = juce::jlimit(0.0f, 1.0f, value); }
void CabinetModel::process(juce::AudioBuffer<float>& buffer)
{
    // Starter cab voicing placeholder. Replace with partitioned convolution IR loader for production tone.
    const float cabGain = juce::jmap(blend, 0.0f, 1.0f, 1.0f, 0.82f);
    buffer.applyGain(cabGain);
}
}
