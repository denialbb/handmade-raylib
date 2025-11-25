#include "managers/audio.hpp"
#include "managers/config.hpp"
#include <cstdio>
#include <raylib.h>

Sound _ost;
bool _is_muted = false;
float _volume = 0.2f;

bool initializeAudio() {
    InitAudioDevice();
    SetMasterVolume(_volume);
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

void muteMaster() {
    if (!_is_muted) {
        _is_muted = true;
        _volume = GetMasterVolume();
        SetMasterVolume(0);
    }
}

void unmuteMaster() {
    if (_is_muted) {
        _is_muted = false;
        SetMasterVolume(_volume);
    }
}

void closeAudio() {
    UnloadSound(_ost);
    CloseAudioDevice();
}

void toggleMasterMute() {
    if (!_is_muted) {
        muteMaster();
    } else {
        unmuteMaster();
    }
}
