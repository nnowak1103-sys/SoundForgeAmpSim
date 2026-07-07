#include "PluginEditor.h"
SonicForgeAudioProcessorEditor::SonicForgeAudioProcessorEditor (SonicForgeAudioProcessor& p) : AudioProcessorEditor (&p), processor (p)
{
    setSize (1040, 580);
    ampBox.addItemList(juce::StringArray{"US Rectifier", "British JCM", "Bogner Modern", "SLO Lead", "5150 Block", "Blackface Clean", "AC Chime", "Orange Doom"}, 1); addAndMakeVisible(ampBox); controls.push_back(&ampBox);
    ampAttach = std::make_unique<ComboAttachment>(processor.apvts,"amp",ampBox);
    std::pair<juce::Slider*, const char*> ss[]={{&input,"input"},{&gain,"gain"},{&bass,"bass"},{&mid,"mid"},{&treble,"treble"},{&tone,"tone"},{&presence,"presence"},{&resonance,"resonance"},{&master,"master"},{&volume,"volume"},{&gate,"gate"},{&cab,"cab"},{&room,"room"}};
    for(auto& it:ss){ setupSlider(*it.first, it.second); sliderAttachments.push_back(std::make_unique<SliderAttachment>(processor.apvts,it.second,*it.first)); controls.push_back(it.first); }
    bypass.setButtonText("Bypass"); addAndMakeVisible(bypass); bypassAttach=std::make_unique<ButtonAttachment>(processor.apvts,"bypass",bypass); controls.push_back(&bypass);
}
SonicForgeAudioProcessorEditor::~SonicForgeAudioProcessorEditor(){ for(auto* c: controls) c->setLookAndFeel(nullptr); }
void SonicForgeAudioProcessorEditor::setupSlider(juce::Slider& s, const juce::String& name){ s.setName(name); s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag); s.setTextBoxStyle(juce::Slider::TextBoxBelow,false,74,20); s.setLookAndFeel(&look); addAndMakeVisible(s); }
void SonicForgeAudioProcessorEditor::paint (juce::Graphics& g){ auto r=getLocalBounds(); g.fillAll(juce::Colour(0xff101216)); g.setColour(juce::Colour(0xffe7c873)); g.setFont(juce::FontOptions(30, juce::Font::bold)); g.drawText("SonicForge Pro Amp Sim", r.removeFromTop(64).reduced(28,0), juce::Justification::centredLeft); g.setColour(juce::Colour(0xff2a2d34)); g.fillRoundedRectangle(getLocalBounds().reduced(24,86).toFloat(),18.f); g.setColour(juce::Colour(0xffb8bdc7)); g.setFont(14.f); g.drawText("Native C++ standalone + VST3 amp platform", 34,48,430,24, juce::Justification::left); }
void SonicForgeAudioProcessorEditor::resized(){ auto area=getLocalBounds().reduced(34); area.removeFromTop(68); auto top=area.removeFromTop(50); ampBox.setBounds(top.removeFromLeft(260).reduced(0,8)); bypass.setBounds(top.removeFromLeft(120).reduced(12,8)); area.removeFromTop(24); juce::Grid grid; grid.templateColumns={}; for(int i=0;i<7;++i) grid.templateColumns.add(juce::Grid::TrackInfo(1_fr)); grid.autoRows=juce::Grid::TrackInfo(150_px); grid.gap=juce::Grid::Px(14); for(auto* s:{&input,&gain,&bass,&mid,&treble,&tone,&presence,&resonance,&master,&volume,&gate,&cab,&room}) grid.items.add(juce::GridItem(*s)); grid.performLayout(area); }
