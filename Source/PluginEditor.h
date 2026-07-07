#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/KnobLookAndFeel.h"
class SonicForgeAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit SonicForgeAudioProcessorEditor (SonicForgeAudioProcessor&);
    ~SonicForgeAudioProcessorEditor() override;
    void paint (juce::Graphics&) override;
    void resized() override;
private:
    SonicForgeAudioProcessor& processor;
    SonicForge::KnobLookAndFeel look;
    juce::ComboBox ampBox;
    juce::Slider input, gain, bass, mid, treble, tone, presence, resonance, master, volume, gate, cab, room;
    juce::ToggleButton bypass;
    std::vector<juce::Component*> controls;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    std::unique_ptr<ComboAttachment> ampAttach; std::vector<std::unique_ptr<SliderAttachment>> sliderAttachments; std::unique_ptr<ButtonAttachment> bypassAttach;
    void setupSlider(juce::Slider&, const juce::String& name);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SonicForgeAudioProcessorEditor)
};
