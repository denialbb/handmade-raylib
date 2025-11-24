#include "managers/audio.hpp"
#include <cstdio>
#include <raylib.h>

const char *OST_FILE = "assets/ost/ost1.ogg";
Sound _ost;

bool initializeAudio() {
    InitAudioDevice();
    SetMasterVolume(2);
    return IsAudioDeviceReady();
}

void loadOST() {
    _ost = LoadSound(OST_FILE);

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
