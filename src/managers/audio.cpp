#include "managers/audio.hpp"
#include "managers/config.hpp"
#include <cstdio>
#include <raylib.h>

Sound _ost;

bool initializeAudio() {
    InitAudioDevice();
    SetMasterVolume(1);
    return IsAudioDeviceReady();
}

void loadOST() {
    printf("Loading OST from: %s\n", config.ost_path);
    _ost = LoadSound(config.ost_path);

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
