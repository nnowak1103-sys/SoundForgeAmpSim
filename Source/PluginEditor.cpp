#include "PluginEditor.h"

SoundForgeAmpSimAudioProcessorEditor::SoundForgeAmpSimAudioProcessorEditor(SoundForgeAmpSimAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(980, 520);
    addKnob("input", "Input");
    addKnob("gain", "Gain");
    addKnob("bass", "Bass");
    addKnob("mid", "Mid");
    addKnob("treble", "Treble");
    addKnob("tone", "Tone");
    addKnob("presence", "Presence");
    addKnob("gate", "Gate");
    addKnob("cab", "Cab");
    addKnob("master", "Master");
}

void SoundForgeAmpSimAudioProcessorEditor::addKnob(const juce::String& parameterId, const juce::String& labelText)
{
    auto slider = std::make_unique<juce::Slider>();
    slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 74, 20);
    auto label = std::make_unique<juce::Label>();
    label->setText(labelText, juce::dontSendNotification);
    label->setJustificationType(juce::Justification::centred);
    addAndMakeVisible(*slider);
    addAndMakeVisible(*label);
    attachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, parameterId, *slider));
    sliders.push_back(std::move(slider));
    labels.push_back(std::move(label));
}

void SoundForgeAmpSimAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff141414));
    g.setColour(juce::Colour(0xfff2d08a));
    g.setFont(32.0f);
    g.drawText("SoundForge Amp Sim", 24, 18, getWidth() - 48, 44, juce::Justification::centredLeft);
    g.setColour(juce::Colour(0xff272727));
    g.fillRoundedRectangle(24.0f, 86.0f, static_cast<float>(getWidth() - 48), 340.0f, 18.0f);
}

void SoundForgeAmpSimAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(34);
    area.removeFromTop(92);
    const int knobW = 88;
    const int gap = 8;
    int x = area.getX() + 20;
    int y = area.getY() + 60;

    for (size_t i = 0; i < sliders.size(); ++i)
    {
        labels[i]->setBounds(x, y - 26, knobW, 22);
        sliders[i]->setBounds(x, y, knobW, 112);
        x += knobW + gap;
    }
}
