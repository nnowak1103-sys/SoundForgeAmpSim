#include "PluginProcessor.h"
#include "PluginEditor.h"

SoundForgeAmpSimAudioProcessor::SoundForgeAmpSimAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout SoundForgeAmpSimAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    auto addFloat = [&](const juce::String& id, const juce::String& name, float min, float max, float def)
    {
        params.push_back(std::make_unique<juce::AudioParameterFloat>(id, name, juce::NormalisableRange<float>(min, max), def));
    };

    addFloat("input", "Input", 0.0f, 2.0f, 1.0f);
    addFloat("gain", "Gain", 0.0f, 10.0f, 4.0f);
    addFloat("bass", "Bass", 0.0f, 10.0f, 5.0f);
    addFloat("mid", "Mid", 0.0f, 10.0f, 5.0f);
    addFloat("treble", "Treble", 0.0f, 10.0f, 5.0f);
    addFloat("tone", "Tone", 0.0f, 10.0f, 5.0f);
    addFloat("presence", "Presence", 0.0f, 10.0f, 5.0f);
    addFloat("master", "Master Volume", 0.0f, 1.5f, 0.75f);
    addFloat("gate", "Noise Gate", -90.0f, -20.0f, -65.0f);
    addFloat("cab", "Cab Blend", 0.0f, 1.0f, 0.8f);

    return { params.begin(), params.end() };
}

bool SoundForgeAmpSimAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto& mainIn = layouts.getMainInputChannelSet();
    const auto& mainOut = layouts.getMainOutputChannelSet();
    return mainIn == mainOut && (mainOut == juce::AudioChannelSet::mono() || mainOut == juce::AudioChannelSet::stereo());
}

void SoundForgeAmpSimAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32>(samplesPerBlock), static_cast<juce::uint32>(getTotalNumOutputChannels()) };
    amp.prepare(spec);
    toneStack.prepare(spec);
    cabinet.prepare(spec);
    gate.prepare(spec);
}

void SoundForgeAmpSimAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear(getTotalNumInputChannels(), buffer.getNumSamples(), getTotalNumOutputChannels() - getTotalNumInputChannels());

    const float input = parameters.getRawParameterValue("input")->load();
    const float gain = parameters.getRawParameterValue("gain")->load();
    const float bass = parameters.getRawParameterValue("bass")->load();
    const float mid = parameters.getRawParameterValue("mid")->load();
    const float treble = parameters.getRawParameterValue("treble")->load();
    const float tone = parameters.getRawParameterValue("tone")->load();
    const float presence = parameters.getRawParameterValue("presence")->load();
    const float master = parameters.getRawParameterValue("master")->load();
    const float gateDb = parameters.getRawParameterValue("gate")->load();
    const float cabBlend = parameters.getRawParameterValue("cab")->load();

    buffer.applyGain(input);
    gate.setThresholdDb(gateDb);
    gate.process(buffer);
    amp.setDrive(gain);
    amp.process(buffer);
    toneStack.setControls(bass, mid, treble, tone, presence);
    toneStack.process(buffer);
    cabinet.setBlend(cabBlend);
    cabinet.process(buffer);
    buffer.applyGain(master);
}

void SoundForgeAmpSimAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto xml = parameters.copyState().createXml())
        copyXmlToBinary(*xml, destData);
}

void SoundForgeAmpSimAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessorEditor* SoundForgeAmpSimAudioProcessor::createEditor()
{
    return new SoundForgeAmpSimAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoundForgeAmpSimAudioProcessor();
}
