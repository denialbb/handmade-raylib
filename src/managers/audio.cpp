#include "managers/audio.hpp"
#include "managers/config.hpp"
#include <cstdio>
#include <raylib.h>

Sound _ost;

bool initializeAudio() {
    InitAudioDevice();
    SetMasterVolume(0.2f);
    return IsAudioDeviceReady();
}

void loadOST() {
    const char *ost_path =
        asset_config.find("audio.ost.1.path")->second.c_str();
    printf("Loading OST from: %s\n", ost_path);
    _ost = LoadSound(ost_path);

    if (IsSoundValid(_ost)) {
        printf("Music stream is valid\n");
    } else {
        printf("Music stream is not valid\n");
        return;
    }
}

void playOST() { PlaySound(_ost); }

void closeAudio() {
    UnloadSound(_ost);
    CloseAudioDevice();
}
