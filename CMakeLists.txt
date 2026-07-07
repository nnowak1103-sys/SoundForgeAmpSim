cmake_minimum_required(VERSION 3.24)
project(SonicForgeProAmpSim VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum OS X deployment version")

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.8
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(SonicForgePro
    COMPANY_NAME "SonicForge Audio"
    BUNDLE_ID "com.sonicforge.proampsim"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Sfge
    PLUGIN_CODE Sfpr
    FORMATS Standalone VST3
    PRODUCT_NAME "SonicForge Pro Amp Sim"
    VST3_CATEGORIES "Fx;Distortion;EQ"
)

target_sources(SonicForgePro PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/DSP/AmpEngine.cpp
    Source/DSP/AmpModels.cpp
    Source/DSP/CabinetIR.cpp
    Source/DSP/Effects.cpp
    Source/DSP/NoiseGate.cpp
    Source/DSP/PresetManager.cpp
    Source/UI/KnobLookAndFeel.cpp
)

target_compile_definitions(SonicForgePro PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(SonicForgePro PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_dsp
    juce::juce_gui_extra
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
