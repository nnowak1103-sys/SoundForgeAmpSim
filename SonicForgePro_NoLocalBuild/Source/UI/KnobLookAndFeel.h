#pragma once
#include <JuceHeader.h>
namespace SonicForge { class KnobLookAndFeel : public juce::LookAndFeel_V4 { public: void drawRotarySlider(juce::Graphics&, int x, int y, int w, int h, float pos, float start, float end, juce::Slider&) override; }; }
