#include "AmpDSP.h"

void AmpDSP::prepare (double sampleRate, int maximumBlockSize, int channels)
{
    sr = sampleRate;
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (maximumBlockSize), static_cast<juce::uint32> (channels) };
    lowCut.prepare (spec); bassFilter.prepare (spec); midFilter.prepare (spec); trebleFilter.prepare (spec); cabLowPass.prepare (spec);
    masterSmoothed.reset (sampleRate, 0.02);
    reset();
}

void AmpDSP::reset()
{
    lowCut.reset(); bassFilter.reset(); midFilter.reset(); trebleFilter.reset(); cabLowPass.reset();
}

float AmpDSP::softClip (float x, float driveAmount, int model) const
{
    const float g = juce::jlimit (1.0f, 45.0f, driveAmount);
    switch (model)
    {
        case 1: return std::tanh (x * g) * 0.72f; // British crunch style
        case 2: return std::atan (x * g * 1.35f) * 0.62f; // Rectifier style
        case 3: return (2.0f / juce::MathConstants<float>::pi) * std::atan (x * g * 2.1f); // modern high gain
        default: return std::tanh (x * g * 0.55f) * 0.9f; // clean edge
    }
}

void AmpDSP::process (juce::AudioBuffer<float>& buffer,
                      float inputGain, float drive, float bass, float mid, float treble,
                      float tone, float presence, float master, float gateDb, int model)
{
    const auto channels = buffer.getNumChannels();
    const auto samples = buffer.getNumSamples();
    const float inputLinear = juce::Decibels::decibelsToGain (inputGain);
    const float gateLinear = juce::Decibels::decibelsToGain (gateDb);

    *lowCut.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass (sr, 70.0f);
    *bassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf (sr, 145.0f, 0.707f, juce::Decibels::decibelsToGain ((bass - 0.5f) * 18.0f));
    *midFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sr, 720.0f, 0.85f, juce::Decibels::decibelsToGain ((mid - 0.5f) * 20.0f));
    *trebleFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf (sr, 2800.0f, 0.707f, juce::Decibels::decibelsToGain ((treble - 0.5f) * 20.0f + presence * 5.0f));
    *cabLowPass.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sr, 4200.0f + tone * 5200.0f);

    for (int ch = 0; ch < channels; ++ch)
    {
        auto* data = buffer.getWritePointer (ch);
        for (int i = 0; i < samples; ++i)
        {
            float x = data[i] * inputLinear;
            if (std::abs (x) < gateLinear) x = 0.0f;
            x = softClip (x, 1.0f + drive * 34.0f, model);
            data[i] = x;
        }
    }

    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> ctx (block);
    lowCut.process (ctx); bassFilter.process (ctx); midFilter.process (ctx); trebleFilter.process (ctx); cabLowPass.process (ctx);

    masterSmoothed.setTargetValue (juce::Decibels::decibelsToGain (master));
    for (int ch = 0; ch < channels; ++ch)
    {
        auto* data = buffer.getWritePointer (ch);
        for (int i = 0; i < samples; ++i)
            data[i] *= masterSmoothed.getNextValue();
    }
}
