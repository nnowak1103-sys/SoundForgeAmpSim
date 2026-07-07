#include "PluginEditor.h"

SoundForgeAmpSimAudioProcessorEditor::SoundForgeAmpSimAudioProcessorEditor (SoundForgeAmpSimAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    modelBox.addItemList ({"Clean", "British JCM", "Recto Modern", "Boutique Lead"}, 1);
    addAndMakeVisible (modelBox);
    modelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (processor.apvts, "model", modelBox);

    addKnob ("input", "Input"); addKnob ("drive", "Drive"); addKnob ("bass", "Bass"); addKnob ("mid", "Mid");
    addKnob ("treble", "Treble"); addKnob ("tone", "Tone"); addKnob ("presence", "Presence"); addKnob ("master", "Master"); addKnob ("gate", "Gate");
    setSize (920, 440);
}

void SoundForgeAmpSimAudioProcessorEditor::addKnob (const juce::String& paramId, const juce::String& labelText)
{
    auto slider = std::make_unique<juce::Slider>();
    slider->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 76, 20);
    addAndMakeVisible (*slider);

    auto label = std::make_unique<juce::Label>();
    label->setText (labelText, juce::dontSendNotification);
    label->setJustificationType (juce::Justification::centred);
    label->setColour (juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible (*label);

    attachments.push_back (std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processor.apvts, paramId, *slider));
    knobs.push_back (std::move (slider)); labels.push_back (std::move (label));
}

void SoundForgeAmpSimAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff111215));
    auto r = getLocalBounds().reduced (18);
    g.setColour (juce::Colour (0xff20242b));
    g.fillRoundedRectangle (r.toFloat(), 18.0f);
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (30.0f, juce::Font::bold));
    g.drawText ("SoundForge Amp Sim", 34, 24, 420, 48, juce::Justification::left);
    g.setFont (juce::FontOptions (14.0f));
    g.setColour (juce::Colour (0xffbfc7d5));
    g.drawText ("Milestone 1: native JUCE standalone + VST3 amp simulator", 38, 70, 620, 24, juce::Justification::left);
}

void SoundForgeAmpSimAudioProcessorEditor::resized()
{
    modelBox.setBounds (650, 36, 220, 30);
    auto area = getLocalBounds().reduced (34).withTrimmedTop (95);
    const int w = 92, h = 116, gap = 8;
    int x = area.getX(); int y = area.getY();
    for (size_t i = 0; i < knobs.size(); ++i)
    {
        knobs[i]->setBounds (x, y + 22, w, 88);
        labels[i]->setBounds (x, y, w, 22);
        x += w + gap;
        if (x + w > area.getRight()) { x = area.getX(); y += h + 10; }
    }
}
