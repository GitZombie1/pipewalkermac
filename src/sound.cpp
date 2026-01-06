// SPDX-License-Identifier: MIT
// Sound subsystem.
// Copyright (C) 2024 Artem Senichev <artemsen@gmail.com>

#include "sound.hpp"

#include "buildcfg.h"

#include <SDL3/SDL.h>

#include <cstring>

// The only supported audio format
constexpr const SDL_AudioSpec sound_spec = { SDL_AUDIO_S16, 2, 44100 };

Sound::~Sound()
{
    for (size_t i = 0; i < sizeof(waves) / sizeof(waves[0]); ++i) {
        SDL_free(waves[i].data);
    }
}

bool Sound::initialize()
{
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO,
                     "Sound subsystem initialization error: %s",
                     SDL_GetError());
        return false;
    }

    // initialize audio stream
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
                                       &sound_spec, nullptr, nullptr);
    if (!stream) {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO,
                     "Error initializing audio stream: %s", SDL_GetError());
        return false;
    }

    // load wave files
    if (!load(APP_DATADIR)) {
        // try portable variant
        std::filesystem::path path(SDL_GetBasePath());
        path /= "data";
        if (!load(path.c_str())) {
            return false;
        }
    }

    return true;
}

void Sound::play(Sound::Type type)
{
    if (enable) {
        SDL_PutAudioStreamData(stream, waves[type].data, waves[type].size);
        SDL_ResumeAudioStreamDevice(stream);
    }
}

bool Sound::load(const std::filesystem::path& dir)
{
    for (size_t i = 0; i < sizeof(waves) / sizeof(waves[0]); ++i) {
        SDL_AudioSpec spec;
        std::filesystem::path file = dir;
        switch (i) {
            case Clatz:
                file /= "clatz.wav";
                break;
            case Complete:
                file /= "complete.wav";
                break;
        }
        const std::string wav_path = file.string();
        if (!SDL_LoadWAV(wav_path.c_str(), &spec, &waves[i].data,
                         &waves[i].size)) {
            SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Error loading wav %s: %s",
                         wav_path.c_str(), SDL_GetError());
            break;
        }
        if (spec.format != sound_spec.format ||
            spec.channels != sound_spec.channels ||
            spec.freq != sound_spec.freq) {
            SDL_LogError(SDL_LOG_CATEGORY_AUDIO,
                         "Error loading wav %s: unsupported format",
                         wav_path.c_str());
            break;
        }
    }

    if (!waves[Clatz].data || !waves[Complete].data) {
        SDL_free(waves[Clatz].data);
        SDL_free(waves[Complete].data);
        memset(waves, 0, sizeof(waves));
        return false;
    }

    return true;
}
