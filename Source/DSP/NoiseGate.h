#pragma once
#include <JuceHeader.h>
namespace SonicForge { class NoiseGate { public: void prepare(double sr, int ch){ env.assign(ch,0.f); coeff=std::exp(-1.0/(0.01*sr)); } void process(juce::AudioBuffer<float>& b, float thresholdDb){ float th=juce::Decibels::decibelsToGain(thresholdDb); for(int c=0;c<b.getNumChannels();++c){ auto*d=b.getWritePointer(c); for(int i=0;i<b.getNumSamples();++i){ env[c]=std::max(std::abs(d[i]), env[c]*coeff); float g= env[c] < th ? env[c]/juce::jmax(th,1e-6f) : 1.f; d[i]*=juce::jlimit(0.f,1.f,g); }}} private: std::vector<float> env; float coeff=.99f; }; }
