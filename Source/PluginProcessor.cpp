#include "PluginProcessor.h"
#include "PluginEditor.h"

SonicForgeAudioProcessor::SonicForgeAudioProcessor()
    : AudioProcessor (BusesProperties().withInput ("Input", juce::AudioChannelSet::stereo(), true)
                                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMETERS", createParameterLayout()),
      presetManager (apvts)
{}

juce::AudioProcessorValueTreeState::ParameterLayout SonicForgeAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> p;
    auto f = [](float min, float max, float start) { return juce::NormalisableRange<float>(min, max, 0.001f), start; };
    p.push_back (std::make_unique<juce::AudioParameterChoice>("amp", "Amp", juce::StringArray{"US Rectifier", "British JCM", "Bogner Modern", "SLO Lead", "5150 Block", "Blackface Clean", "AC Chime", "Orange Doom"}, 0));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("input", "Input", juce::NormalisableRange<float>(-24.f, 24.f, .1f), 0.f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("gain", "Gain", juce::NormalisableRange<float>(0.f, 10.f, .01f), 5.5f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("bass", "Bass", juce::NormalisableRange<float>(0.f, 10.f, .01f), 5.f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("mid", "Mid", juce::NormalisableRange<float>(0.f, 10.f, .01f), 5.f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("treble", "Treble", juce::NormalisableRange<float>(0.f, 10.f, .01f), 5.f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("tone", "Tone", juce::NormalisableRange<float>(0.f, 10.f, .01f), 5.f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("presence", "Presence", juce::NormalisableRange<float>(0.f, 10.f, .01f), 5.f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("resonance", "Resonance", juce::NormalisableRange<float>(0.f, 10.f, .01f), 4.f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("master", "Master", juce::NormalisableRange<float>(0.f, 10.f, .01f), 6.f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("volume", "Volume", juce::NormalisableRange<float>(-60.f, 6.f, .1f), -8.f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("gate", "Gate", juce::NormalisableRange<float>(-90.f, -20.f, .1f), -60.f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("cab", "Cab Mix", juce::NormalisableRange<float>(0.f, 1.f, .001f), 1.f));
    p.push_back (std::make_unique<juce::AudioParameterFloat>("room", "Room", juce::NormalisableRange<float>(0.f, 1.f, .001f), .08f));
    p.push_back (std::make_unique<juce::AudioParameterBool>("bypass", "Bypass", false));
    return { p.begin(), p.end() };
}

void SonicForgeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    engine.prepare (sampleRate, samplesPerBlock, getTotalNumOutputChannels());
}

bool SonicForgeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono()
        || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void SonicForgeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ignoreUnused (midi);
    juce::ScopedNoDenormals noDenormals;
    for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch) buffer.clear (ch, 0, buffer.getNumSamples());
    if (*apvts.getRawParameterValue("bypass") > 0.5f) return;
    SonicForge::AmpParams params;
    params.amp = (int)*apvts.getRawParameterValue("amp");
    params.inputDb = *apvts.getRawParameterValue("input"); params.gain = *apvts.getRawParameterValue("gain");
    params.bass = *apvts.getRawParameterValue("bass"); params.mid = *apvts.getRawParameterValue("mid"); params.treble = *apvts.getRawParameterValue("treble");
    params.tone = *apvts.getRawParameterValue("tone"); params.presence = *apvts.getRawParameterValue("presence"); params.resonance = *apvts.getRawParameterValue("resonance");
    params.master = *apvts.getRawParameterValue("master"); params.volumeDb = *apvts.getRawParameterValue("volume");
    params.gateDb = *apvts.getRawParameterValue("gate"); params.cabMix = *apvts.getRawParameterValue("cab"); params.room = *apvts.getRawParameterValue("room");
    engine.process (buffer, params);
}

void SonicForgeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml()) copyXmlToBinary (*xml, destData);
}

void SonicForgeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType())) apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessorEditor* SonicForgeAudioProcessor::createEditor() { return new SonicForgeAudioProcessorEditor (*this); }
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new SonicForgeAudioProcessor(); }
