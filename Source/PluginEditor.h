#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class SoundForgeAmpSimAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit SoundForgeAmpSimAudioProcessorEditor(SoundForgeAmpSimAudioProcessor&);
    ~SoundForgeAmpSimAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    SoundForgeAmpSimAudioProcessor& processor;
    std::vector<std::unique_ptr<juce::Slider>> sliders;
    std::vector<std::unique_ptr<juce::Label>> labels;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> attachments;

    void addKnob(const juce::String& parameterId, const juce::String& labelText);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SoundForgeAmpSimAudioProcessorEditor)
};
