#include "PluginProcessor.h"
#include "PluginEditor.h"

SoundForgeAmpSimAudioProcessor::SoundForgeAmpSimAudioProcessor()
    : AudioProcessor (BusesProperties().withInput ("Input", juce::AudioChannelSet::stereo(), true)
                                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout SoundForgeAmpSimAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back (std::make_unique<juce::AudioParameterChoice> ("model", "Amp Model", juce::StringArray {"Clean", "British JCM", "Recto Modern", "Boutique Lead"}, 2));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("input", "Input", juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f), 0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("drive", "Drive", 0.0f, 1.0f, 0.55f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("bass", "Bass", 0.0f, 1.0f, 0.55f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("mid", "Mid", 0.0f, 1.0f, 0.50f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("treble", "Treble", 0.0f, 1.0f, 0.60f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("tone", "Cab Tone", 0.0f, 1.0f, 0.52f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("presence", "Presence", 0.0f, 1.0f, 0.55f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("master", "Master", juce::NormalisableRange<float> (-60.0f, 6.0f, 0.1f), -12.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("gate", "Gate", juce::NormalisableRange<float> (-90.0f, -20.0f, 0.1f), -65.0f));
    return { params.begin(), params.end() };
}

void SoundForgeAmpSimAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    amp.prepare (sampleRate, samplesPerBlock, getTotalNumOutputChannels());
}

bool SoundForgeAmpSimAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono()
        || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void SoundForgeAmpSimAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear (ch, 0, buffer.getNumSamples());

    amp.process (buffer,
                 apvts.getRawParameterValue ("input")->load(),
                 apvts.getRawParameterValue ("drive")->load(),
                 apvts.getRawParameterValue ("bass")->load(),
                 apvts.getRawParameterValue ("mid")->load(),
                 apvts.getRawParameterValue ("treble")->load(),
                 apvts.getRawParameterValue ("tone")->load(),
                 apvts.getRawParameterValue ("presence")->load(),
                 apvts.getRawParameterValue ("master")->load(),
                 apvts.getRawParameterValue ("gate")->load(),
                 static_cast<int> (apvts.getRawParameterValue ("model")->load()));
}

juce::AudioProcessorEditor* SoundForgeAmpSimAudioProcessor::createEditor()
{
    return new SoundForgeAmpSimAudioProcessorEditor (*this);
}

void SoundForgeAmpSimAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void SoundForgeAmpSimAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoundForgeAmpSimAudioProcessor();
}
