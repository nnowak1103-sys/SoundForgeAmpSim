#pragma once
#include <JuceHeader.h>

struct FactoryPreset
{
    juce::String name;
    std::map<juce::String, float> values;
};

class PresetManager
{
public:
    static std::vector<FactoryPreset> factoryPresets();
};
