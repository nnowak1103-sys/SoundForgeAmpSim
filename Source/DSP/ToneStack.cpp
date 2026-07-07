#include "ToneStack.h"
namespace soundforge {
void ToneStack::prepare(const juce::dsp::ProcessSpec& spec) { sampleRate = spec.sampleRate; }
void ToneStack::setControls(float bass, float mid, float treble, float tone, float presence) { b=bass; m=mid; t=treble; tn=tone; p=presence; }
void ToneStack::process(juce::AudioBuffer<float>& buffer)
{
    const float low = juce::Decibels::decibelsToGain((b - 5.0f) * 1.2f);
    const float midGain = juce::Decibels::decibelsToGain((m - 5.0f) * 1.0f);
    const float high = juce::Decibels::decibelsToGain((t - 5.0f) * 1.2f + (p - 5.0f) * 0.7f);
    const float overall = (low + midGain + high) / 3.0f * juce::jmap(tn, 0.0f, 10.0f, 0.82f, 1.08f);
    buffer.applyGain(overall);
}
}
