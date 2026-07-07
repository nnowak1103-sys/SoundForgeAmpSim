#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class ForgeLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ForgeLookAndFeel();
    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height, float sliderPos,
                           float rotaryStartAngle, float rotaryEndAngle, juce::Slider&) override;
    void drawComboBox (juce::Graphics&, int width, int height, bool, int, int, int, int, juce::ComboBox&) override;
};

class SoundForgeAmpSimAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit SoundForgeAmpSimAudioProcessorEditor (SoundForgeAmpSimAudioProcessor&);
    ~SoundForgeAmpSimAudioProcessorEditor() override;
    void paint (juce::Graphics&) override;
    void resized() override;
private:
    struct Control
    {
        juce::String id, name;
        std::unique_ptr<juce::Slider> slider;
        std::unique_ptr<juce::Label> label;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
    };

    SoundForgeAmpSimAudioProcessor& processor;
    ForgeLookAndFeel look;
    juce::ComboBox modelBox, cabBox;
    juce::Label title, subtitle, ampLabel, cabLabel, preLabel, stackLabel, fxLabel;
    std::vector<std::unique_ptr<Control>> controls;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modelAttachment, cabAttachment;

    void addKnob (const juce::String& paramId, const juce::String& labelText);
    void layoutSection (juce::Rectangle<int> area, int start, int count);
    void styleCombo (juce::ComboBox& box);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundForgeAmpSimAudioProcessorEditor)
};
