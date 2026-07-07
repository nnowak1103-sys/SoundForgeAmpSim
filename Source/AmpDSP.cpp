#include "AmpDSP.h"

void AmpDSP::prepare (double sampleRate, int maximumBlockSize, int channels)
{
    sr = sampleRate;
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (maximumBlockSize), static_cast<juce::uint32> (channels) };
    lowCut.prepare (spec); bassFilter.prepare (spec); midFilter.prepare (spec); trebleFilter.prepare (spec);
    presenceFilter.prepare (spec); resonanceFilter.prepare (spec); cabLowPass.prepare (spec); cabNotch.prepare (spec);
    delayBuffer.setSize (juce::jmax (2, channels), static_cast<int> (sampleRate * 2.0));
    masterSmoothed.reset (sampleRate, 0.02);
    reset();
}

void AmpDSP::reset()
{
    lowCut.reset(); bassFilter.reset(); midFilter.reset(); trebleFilter.reset(); presenceFilter.reset(); resonanceFilter.reset(); cabLowPass.reset(); cabNotch.reset();
    delayBuffer.clear(); delayWrite = 0; env = 0.0f; compGain = 1.0f; lfo = 0.0f; revL = revR = 0.0f;
}

float AmpDSP::preamp (float x, float driveAmount, int model) const
{
    const float g = juce::jlimit (1.0f, 80.0f, driveAmount);
    switch (model)
    {
        case 1:  return std::tanh (x * g * 0.92f) * 0.80f;                // Marshall JCM crunch
        case 2:  return std::atan (x * g * 1.45f) * 0.58f;                // Mesa Rectifier
        case 3:  return std::tanh (x * g * 1.35f + 0.12f * x * x) * 0.68f; // Bogner boutique
        case 4:  return std::atan (x * g * 1.60f) * 0.56f;                // Soldano SLO
        case 5:  return std::tanh (x * g * 1.85f) * 0.62f;                // 5150/6505
        case 6:  return std::tanh (x * g * 1.10f) * 0.76f;                // Orange thick
        case 7:  return std::tanh (x * g * 0.48f) * 0.95f;                // Vox/Fender clean
        default: return std::tanh (x * g * 0.40f) * 0.98f;
    }
}

float AmpDSP::shapeTone (float x, int model) const
{
    switch (model)
    {
        case 2:  return x * 1.10f - 0.06f * x * x * x; // aggressive recto edge
        case 3:  return x * 0.96f + 0.04f * std::sin (x * 2.4f);
        case 5:  return x * 1.14f;
        case 6:  return x * 0.88f;
        default: return x;
    }
}

void AmpDSP::process (juce::AudioBuffer<float>& buffer,
                      float inputGainDb, float drive, float bass, float mid, float treble,
                      float tone, float presence, float resonance, float masterDb, float gateDb,
                      int model, int cab, float screamer, float compressor, float chorus, float delay, float reverb, float mix)
{
    const auto channels = buffer.getNumChannels();
    const auto samples = buffer.getNumSamples();
    const float inputLinear = juce::Decibels::decibelsToGain (inputGainDb);
    const float gateLinear = juce::Decibels::decibelsToGain (gateDb);
    const float cabBase = cab == 0 ? 7800.0f : cab == 1 ? 6500.0f : cab == 2 ? 5600.0f : cab == 3 ? 4800.0f : 7200.0f;
    const float cabCutoff = juce::jlimit (2400.0f, 11000.0f, cabBase + tone * 3400.0f);

    *lowCut.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass (sr, screamer > 0.5f ? 105.0f : 70.0f);
    *bassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf (sr, 135.0f, 0.707f, juce::Decibels::decibelsToGain ((bass - 0.5f) * 22.0f));
    *midFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sr, model == 2 ? 650.0f : 780.0f, 0.75f, juce::Decibels::decibelsToGain ((mid - 0.5f) * 24.0f));
    *trebleFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf (sr, 2800.0f, 0.707f, juce::Decibels::decibelsToGain ((treble - 0.5f) * 22.0f));
    *presenceFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf (sr, 5200.0f, 0.65f, juce::Decibels::decibelsToGain ((presence - 0.5f) * 14.0f));
    *resonanceFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf (sr, 95.0f, 0.8f, juce::Decibels::decibelsToGain ((resonance - 0.5f) * 12.0f));
    *cabLowPass.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sr, cabCutoff);
    *cabNotch.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sr, 3900.0f, 1.2f, juce::Decibels::decibelsToGain (-2.0f - cab * 0.8f));

    juce::AudioBuffer<float> dry;
    dry.makeCopyOf (buffer, true);

    const float screamerDrive = 1.0f + screamer * 10.0f;
    const float compAmount = compressor;
    for (int ch = 0; ch < channels; ++ch)
    {
        auto* data = buffer.getWritePointer (ch);
        for (int i = 0; i < samples; ++i)
        {
            float x = data[i] * inputLinear;
            const float a = std::abs (x);
            env = env * 0.995f + a * 0.005f;
            if (env < gateLinear) x *= 0.05f;
            const float targetComp = 1.0f / (1.0f + compAmount * 6.0f * juce::jmax (0.0f, env - 0.12f));
            compGain = compGain * 0.99f + targetComp * 0.01f;
            x *= compGain;
            if (screamer > 0.001f)
                x = std::tanh (x * screamerDrive) * (0.72f + 0.18f * (1.0f - screamer));
            x = preamp (x, 1.0f + drive * 58.0f, model);
            data[i] = shapeTone (x, model);
        }
    }

    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> ctx (block);
    lowCut.process (ctx); bassFilter.process (ctx); midFilter.process (ctx); trebleFilter.process (ctx); presenceFilter.process (ctx); resonanceFilter.process (ctx); cabLowPass.process (ctx); cabNotch.process (ctx);

    const int delaySamples = static_cast<int> (0.28 * sr);
    const int chorusMax = static_cast<int> (0.025 * sr);
    for (int i = 0; i < samples; ++i)
    {
        lfo += 0.23f / static_cast<float> (sr);
        if (lfo >= 1.0f) lfo -= 1.0f;
        const float c = chorus * 0.35f;
        const int mod = static_cast<int> ((0.010f + 0.008f * std::sin (juce::MathConstants<float>::twoPi * lfo)) * sr);
        for (int ch = 0; ch < channels; ++ch)
        {
            auto* data = buffer.getWritePointer (ch);
            float x = data[i];
            int chorusRead = (delayWrite + delayBuffer.getNumSamples() - juce::jmin (chorusMax, mod)) % delayBuffer.getNumSamples();
            int echoRead = (delayWrite + delayBuffer.getNumSamples() - juce::jmin (delayBuffer.getNumSamples() - 1, delaySamples)) % delayBuffer.getNumSamples();
            const float chorused = delayBuffer.getSample (ch, chorusRead);
            const float echo = delayBuffer.getSample (ch, echoRead);
            x = x * (1.0f - c) + chorused * c;
            x += echo * delay * 0.38f;
            if (ch == 0) { revL = 0.78f * revL + 0.22f * (x + revR * 0.18f); x += revL * reverb * 0.30f; }
            else { revR = 0.78f * revR + 0.22f * (x + revL * 0.18f); x += revR * reverb * 0.30f; }
            delayBuffer.setSample (ch, delayWrite, data[i] + echo * (0.30f + delay * 0.22f));
            data[i] = x;
        }
        delayWrite = (delayWrite + 1) % delayBuffer.getNumSamples();
    }

    masterSmoothed.setTargetValue (juce::Decibels::decibelsToGain (masterDb));
    const float wet = mix;
    for (int ch = 0; ch < channels; ++ch)
    {
        auto* data = buffer.getWritePointer (ch);
        auto* dryData = dry.getReadPointer (ch);
        for (int i = 0; i < samples; ++i)
            data[i] = (dryData[i] * (1.0f - wet) + data[i] * wet) * masterSmoothed.getNextValue();
    }
}
