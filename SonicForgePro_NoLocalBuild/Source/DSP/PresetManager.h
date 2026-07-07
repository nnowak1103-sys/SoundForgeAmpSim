#pragma once
#include <JuceHeader.h>
namespace SonicForge { class PresetManager { public: explicit PresetManager(juce::AudioProcessorValueTreeState& s):state(s){} void saveToFile(const juce::File& f){ if(auto xml=state.copyState().createXml()) xml->writeTo(f); } void loadFromFile(const juce::File& f){ juce::XmlDocument doc(f); if(auto xml=doc.getDocumentElement()) state.replaceState(juce::ValueTree::fromXml(*xml)); } private: juce::AudioProcessorValueTreeState& state; }; }
