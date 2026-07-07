#pragma once
#include <JuceHeader.h>
namespace SonicForge { class CabinetIR { public: void prepare(double sampleRate, int blockSize, int channels); void process(juce::AudioBuffer<float>& b, float mix, int amp); private: double sr=44100; std::vector<juce::dsp::IIR::Filter<float>> hp, lp, notch; }; }
