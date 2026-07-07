cmake_minimum_required(VERSION 3.22)

project(SoundForgeAmpSim VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.8
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(JUCE)

juce_add_plugin(SoundForgeAmpSim
    COMPANY_NAME "SoundForge Audio"
    BUNDLE_ID "com.soundforge.ampsim"
    PLUGIN_MANUFACTURER_CODE Sfge
    PLUGIN_CODE Sfas
    FORMATS Standalone VST3
    PRODUCT_NAME "SoundForge Amp Sim"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
)

juce_generate_juce_header(SoundForgeAmpSim)

target_sources(SoundForgeAmpSim PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/AmpDSP.cpp
    Source/PresetManager.cpp
)

target_compile_definitions(SoundForgeAmpSim PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(SoundForgeAmpSim PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_dsp
    juce::juce_gui_extra
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
