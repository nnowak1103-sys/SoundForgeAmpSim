#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class SoundForgeAmpSimAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit SoundForgeAmpSimAudioProcessorEditor (SoundForgeAmpSimAudioProcessor&);
    ~SoundForgeAmpSimAudioProcessorEditor() override = default;
    void paint (juce::Graphics&) override;
    void resized() override;
private:
    SoundForgeAmpSimAudioProcessor& processor;
    juce::ComboBox modelBox;
    std::vector<std::unique_ptr<juce::Slider>> knobs;
    std::vector<std::unique_ptr<juce::Label>> labels;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modelAttachment;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> attachments;
    void addKnob (const juce::String& paramId, const juce::String& labelText);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundForgeAmpSimAudioProcessorEditor)
};
