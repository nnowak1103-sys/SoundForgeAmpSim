#pragma once
#include <JuceHeader.h>

class AmpDSP
{
public:
    void prepare (double sampleRate, int maximumBlockSize, int channels);
    void reset();

    void process (juce::AudioBuffer<float>& buffer,
                  float inputGainDb,
                  float drive,
                  float bass,
                  float mid,
                  float treble,
                  float tone,
                  float presence,
                  float resonance,
                  float masterDb,
                  float gateDb,
                  int model,
                  int cab,
                  float screamer,
                  float compressor,
                  float chorus,
                  float delay,
                  float reverb,
                  float mix);

private:
    double sr = 44100.0;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowCut, bassFilter, midFilter, trebleFilter, presenceFilter, resonanceFilter, cabLowPass, cabNotch;
    juce::SmoothedValue<float> masterSmoothed;
    juce::AudioBuffer<float> delayBuffer;
    int delayWrite = 0;
    float env = 0.0f;
    float compGain = 1.0f;
    float lfo = 0.0f;
    float revL = 0.0f, revR = 0.0f;
    float preamp (float x, float driveAmount, int model) const;
    float shapeTone (float x, int model) const;
};
