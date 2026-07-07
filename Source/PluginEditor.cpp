#include "PluginEditor.h"

ForgeLookAndFeel::ForgeLookAndFeel()
{
    setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffe9edf5));
    setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    setColour (juce::ComboBox::backgroundColourId, juce::Colour (0xff191e27));
    setColour (juce::ComboBox::textColourId, juce::Colour (0xfff5f0df));
    setColour (juce::ComboBox::outlineColourId, juce::Colour (0xff7c6a42));
    setColour (juce::PopupMenu::backgroundColourId, juce::Colour (0xff161a22));
    setColour (juce::PopupMenu::textColourId, juce::Colour (0xfff0e9d6));
}

void ForgeLookAndFeel::drawComboBox (juce::Graphics& g, int width, int height, bool, int, int, int, int, juce::ComboBox&)
{
    auto r = juce::Rectangle<float> (0, 0, (float) width, (float) height).reduced (1.0f);
    g.setGradientFill (juce::ColourGradient (juce::Colour (0xff242a35), 0, 0, juce::Colour (0xff12161e), 0, (float) height, false));
    g.fillRoundedRectangle (r, 8.0f);
    g.setColour (juce::Colour (0xffa9874f));
    g.drawRoundedRectangle (r, 8.0f, 1.0f);
    juce::Path arrow;
    arrow.addTriangle ((float) width - 22.0f, height * 0.42f, (float) width - 12.0f, height * 0.42f, (float) width - 17.0f, height * 0.62f);
    g.fillPath (arrow);
}

void ForgeLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                         float rotaryStartAngle, float rotaryEndAngle, juce::Slider&)
{
    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height).reduced (8.0f);
    const auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    const auto centre = bounds.getCentre();
    const auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    g.setColour (juce::Colour (0xaa000000));
    g.fillEllipse (bounds.translated (0, 4));
    g.setGradientFill (juce::ColourGradient (juce::Colour (0xff3b414c), centre.x - radius, centre.y - radius,
                                             juce::Colour (0xff0e1117), centre.x + radius, centre.y + radius, true));
    g.fillEllipse (bounds);
    g.setColour (juce::Colour (0xffb88a43));
    g.drawEllipse (bounds.reduced (1.0f), 1.5f);

    juce::Path arc;
    arc.addCentredArc (centre.x, centre.y, radius + 5.0f, radius + 5.0f, 0.0f, rotaryStartAngle, angle, true);
    g.setColour (juce::Colour (0xffffc66d));
    g.strokePath (arc, juce::PathStrokeType (3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Path pointer;
    pointer.addRoundedRectangle (-2.0f, -radius + 9.0f, 4.0f, radius * 0.44f, 2.0f);
    pointer.applyTransform (juce::AffineTransform::rotation (angle).translated (centre.x, centre.y));
    g.setColour (juce::Colour (0xfff2e5c2));
    g.fillPath (pointer);
}

SoundForgeAmpSimAudioProcessorEditor::SoundForgeAmpSimAudioProcessorEditor (SoundForgeAmpSimAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setLookAndFeel (&look);
    modelBox.addItemList ({"Studio Clean", "Marshall JCM", "Mesa Rectifier", "Bogner Ecstasy", "Soldano SLO", "5150/6505", "Orange Rocker", "Vox/Fender Glass"}, 1);
    cabBox.addItemList ({"Open 1x12", "Vintage 2x12", "Modern 4x12", "Dark 4x12", "Bright Mix"}, 1);
    styleCombo (modelBox); styleCombo (cabBox);
    addAndMakeVisible (modelBox); addAndMakeVisible (cabBox);
    modelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (processor.apvts, "model", modelBox);
    cabAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (processor.apvts, "cab", cabBox);

    for (auto* label : { &title, &subtitle, &ampLabel, &cabLabel, &preLabel, &stackLabel, &fxLabel })
    {
        addAndMakeVisible (*label);
        label->setColour (juce::Label::textColourId, juce::Colour (0xfff2e8d0));
    }
    title.setText ("SoundForge Amp Sim", juce::dontSendNotification);
    subtitle.setText ("Milestones 2/3: high-gain amps, cabinets, pedals, modulation, ambience", juce::dontSendNotification);
    ampLabel.setText ("AMPLIFIER", juce::dontSendNotification);
    cabLabel.setText ("CABINET", juce::dontSendNotification);
    preLabel.setText ("INPUT / PREAMP", juce::dontSendNotification);
    stackLabel.setText ("TONE STACK", juce::dontSendNotification);
    fxLabel.setText ("PEDALS / SPACE", juce::dontSendNotification);

    addKnob ("input", "Input"); addKnob ("gate", "Gate"); addKnob ("compressor", "Comp"); addKnob ("screamer", "Screamer");
    addKnob ("drive", "Gain"); addKnob ("bass", "Bass"); addKnob ("mid", "Mid"); addKnob ("treble", "Treble");
    addKnob ("presence", "Presence"); addKnob ("resonance", "Resonance"); addKnob ("tone", "Cab Tone"); addKnob ("chorus", "Chorus");
    addKnob ("delay", "Delay"); addKnob ("reverb", "Reverb"); addKnob ("mix", "Mix"); addKnob ("master", "Master");

    setResizable (true, true);
    setResizeLimits (1040, 640, 1800, 1000);
    setSize (1180, 720);
}

SoundForgeAmpSimAudioProcessorEditor::~SoundForgeAmpSimAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void SoundForgeAmpSimAudioProcessorEditor::styleCombo (juce::ComboBox& box)
{
    box.setJustificationType (juce::Justification::centredLeft);
    box.setColour (juce::ComboBox::textColourId, juce::Colour (0xfff3ead7));
}

void SoundForgeAmpSimAudioProcessorEditor::addKnob (const juce::String& paramId, const juce::String& labelText)
{
    auto c = std::make_unique<Control>();
    c->id = paramId; c->name = labelText;
    c->slider = std::make_unique<juce::Slider>();
    c->slider->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    c->slider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 72, 20);
    c->slider->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xff11151d));
    c->label = std::make_unique<juce::Label>();
    c->label->setText (labelText, juce::dontSendNotification);
    c->label->setJustificationType (juce::Justification::centred);
    c->label->setColour (juce::Label::textColourId, juce::Colour (0xffd8cfb9));
    addAndMakeVisible (*c->slider);
    addAndMakeVisible (*c->label);
    c->attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processor.apvts, paramId, *c->slider);
    controls.push_back (std::move (c));
}

void SoundForgeAmpSimAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto b = getLocalBounds().toFloat();
    g.setGradientFill (juce::ColourGradient (juce::Colour (0xff090b10), 0, 0, juce::Colour (0xff1e242f), b.getWidth(), b.getHeight(), false));
    g.fillAll();

    auto panel = getLocalBounds().reduced (22).toFloat();
    g.setColour (juce::Colour (0x66000000));
    g.fillRoundedRectangle (panel.translated (0, 5), 24.0f);
    g.setGradientFill (juce::ColourGradient (juce::Colour (0xff282d37), panel.getX(), panel.getY(), juce::Colour (0xff12161f), panel.getRight(), panel.getBottom(), false));
    g.fillRoundedRectangle (panel, 24.0f);
    g.setColour (juce::Colour (0xff9a7543));
    g.drawRoundedRectangle (panel.reduced (1.0f), 24.0f, 1.2f);

    auto header = panel.removeFromTop (96.0f).reduced (18.0f, 12.0f);
    g.setColour (juce::Colour (0xff0e1118));
    g.fillRoundedRectangle (header, 16.0f);
    g.setColour (juce::Colour (0x55d6a75d));
    g.drawRoundedRectangle (header, 16.0f, 1.0f);

    auto glow = juce::Rectangle<float> (panel.getX() + 48, panel.getY() + 130, panel.getWidth() - 96, 76);
    g.setGradientFill (juce::ColourGradient (juce::Colour (0x66ff9b34), glow.getX(), glow.getCentreY(), juce::Colour (0x00202020), glow.getRight(), glow.getCentreY(), false));
    g.fillRoundedRectangle (glow, 24.0f);
}

void SoundForgeAmpSimAudioProcessorEditor::layoutSection (juce::Rectangle<int> area, int start, int count)
{
    const int knobW = juce::jmax (76, area.getWidth() / juce::jmax (1, count));
    for (int i = 0; i < count && start + i < (int) controls.size(); ++i)
    {
        auto r = area.removeFromLeft (knobW).reduced (5, 0);
        controls[(size_t) start + i]->label->setBounds (r.removeFromTop (22));
        controls[(size_t) start + i]->slider->setBounds (r.reduced (0, 0));
    }
}

void SoundForgeAmpSimAudioProcessorEditor::resized()
{
    auto root = getLocalBounds().reduced (42);
    auto header = root.removeFromTop (96).reduced (18, 12);
    title.setBounds (header.removeFromLeft (420).withTrimmedTop (4));
    title.setFont (juce::FontOptions (32.0f, juce::Font::bold));
    subtitle.setBounds (60, 72, 680, 24);
    subtitle.setFont (juce::FontOptions (14.5f));

    auto selector = header.removeFromRight (510).reduced (0, 8);
    ampLabel.setBounds (selector.removeFromTop (18));
    modelBox.setBounds (selector.removeFromLeft (285).withHeight (36));
    selector.removeFromLeft (18);
    cabLabel.setBounds (selector.getX(), header.getY() + 8, 150, 18);
    cabBox.setBounds (selector.withHeight (36).translated (0, 18));

    root.removeFromTop (24);
    auto top = root.removeFromTop (170).reduced (8);
    auto middle = root.removeFromTop (170).reduced (8);
    auto bottom = root.removeFromTop (170).reduced (8);

    preLabel.setBounds (top.removeFromTop (22));
    layoutSection (top, 0, 5);
    stackLabel.setBounds (middle.removeFromTop (22));
    layoutSection (middle, 5, 6);
    fxLabel.setBounds (bottom.removeFromTop (22));
    layoutSection (bottom, 11, 5);
}
