#pragma once
#include <JuceHeader.h>

class AmpDSP
{
public:
    void prepare (double sampleRate, int maximumBlockSize, int channels);
    void reset();
    void process (juce::AudioBuffer<float>& buffer,
                  float inputGain, float drive, float bass, float mid, float treble,
                  float tone, float presence, float master, float gateDb, int model);
private:
    double sr = 44100.0;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowCut;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> bassFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> midFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> trebleFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> cabLowPass;
    juce::SmoothedValue<float> masterSmoothed { 0.0f };
    float softClip (float x, float driveAmount, int model) const;
};
