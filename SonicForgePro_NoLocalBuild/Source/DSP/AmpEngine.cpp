#include "AmpEngine.h"
#include <cmath>
namespace SonicForge {
void AmpEngine::prepare(double sampleRate, int blockSize, int channels) {
    sr = sampleRate; low.resize(channels); mid.resize(channels); high.resize(channels); presence.resize(channels); resonance.resize(channels);
    juce::dsp::ProcessSpec spec{ sampleRate, (juce::uint32) blockSize, (juce::uint32) channels };
    for (auto* vec : {&low,&mid,&high,&presence,&resonance}) for (auto& f:*vec) f.prepare(spec);
    gate.prepare(sampleRate, channels); cab.prepare(sampleRate, blockSize, channels); reverb.prepare(spec);
}
float AmpEngine::waveshape(float x, const AmpParams& p) noexcept {
    const float modelGain[] = { 4.8f, 3.2f, 4.1f, 4.4f, 4.7f, 1.2f, 1.6f, 3.8f };
    const float asym[]      = { .18f, .08f, .14f, .10f, .16f, .02f, .04f, .22f };
    const int m = juce::jlimit(0,7,p.amp);
    float drive = std::pow(10.f, p.inputDb/20.f) * (0.35f + p.gain * 0.18f) * modelGain[m];
    float y = std::tanh(x * drive + asym[m] * x*x);
    y = 0.72f * y + 0.28f * std::tanh(y * (1.0f + p.master * .16f));
    return juce::jlimit(-1.0f, 1.0f, y);
}
void AmpEngine::updateFilters(const AmpParams& p) {
    auto db = [](float knob, float range){ return (knob - 5.f) / 5.f * range; };
    for(size_t ch=0; ch<low.size(); ++ch) {
        *low[ch].coefficients = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(sr, 105.f + p.resonance*14.f, .72f, juce::Decibels::decibelsToGain(db(p.bass, 10.f)));
        *mid[ch].coefficients = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sr, 650.f + (5.f-p.mid)*35.f, .75f, juce::Decibels::decibelsToGain(db(p.mid, 12.f)));
        *high[ch].coefficients = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sr, 2500.f, .70f, juce::Decibels::decibelsToGain(db(p.treble, 11.f) + db(p.tone, 5.f)));
        *presence[ch].coefficients = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sr, 4200.f, .9f, juce::Decibels::decibelsToGain(db(p.presence, 8.f)));
        *resonance[ch].coefficients = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sr, 120.f, 1.1f, juce::Decibels::decibelsToGain(db(p.resonance, 7.f)));
    }
    juce::dsp::Reverb::Parameters rp; rp.roomSize = .18f + p.room*.45f; rp.damping=.58f; rp.wetLevel=p.room*.18f; rp.dryLevel=1.f; rp.width=1.f; reverb.setParameters(rp);
}
void AmpEngine::process(juce::AudioBuffer<float>& buffer, const AmpParams& p) {
    updateFilters(p); gate.process(buffer, p.gateDb);
    for(int ch=0; ch<buffer.getNumChannels(); ++ch) {
        auto* data=buffer.getWritePointer(ch);
        for(int i=0;i<buffer.getNumSamples();++i) data[i]=waveshape(data[i],p);
        juce::dsp::AudioBlock<float> block(buffer); auto single=block.getSingleChannelBlock((size_t)ch); juce::dsp::ProcessContextReplacing<float> ctx(single);
        resonance[ch].process(ctx); low[ch].process(ctx); mid[ch].process(ctx); high[ch].process(ctx); presence[ch].process(ctx);
    }
    cab.process(buffer, p.cabMix, p.amp); juce::dsp::AudioBlock<float> block(buffer); juce::dsp::ProcessContextReplacing<float> ctx(block); reverb.process(ctx);
    buffer.applyGain(juce::Decibels::decibelsToGain(p.volumeDb));
}
}
