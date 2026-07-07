#include "PresetManager.h"

std::vector<FactoryPreset> PresetManager::factoryPresets()
{
    return {
        { "Clean American", {{"model",0},{"input",0},{"drive",0.14f},{"bass",0.55f},{"mid",0.45f},{"treble",0.62f},{"tone",0.62f},{"presence",0.35f},{"master",-8},{"gate",-70}} },
        { "British JCM Crunch", {{"model",1},{"input",2},{"drive",0.48f},{"bass",0.48f},{"mid",0.72f},{"treble",0.64f},{"tone",0.55f},{"presence",0.55f},{"master",-10},{"gate",-62}} },
        { "Recto Modern", {{"model",2},{"input",3},{"drive",0.74f},{"bass",0.68f},{"mid",0.36f},{"treble",0.66f},{"tone",0.50f},{"presence",0.72f},{"master",-12},{"gate",-56}} },
        { "Bogner Lead", {{"model",3},{"input",1},{"drive",0.66f},{"bass",0.58f},{"mid",0.61f},{"treble",0.58f},{"tone",0.48f},{"presence",0.63f},{"master",-11},{"gate",-58}} }
    };
}
