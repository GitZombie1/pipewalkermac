// SPDX-License-Identifier: MIT
// Sound subsystem.
// Copyright (C) 2024 Artem Senichev <artemsen@gmail.com>

#pragma once

#include <SDL3/SDL.h>

#include <filesystem>

/** Sound subsystem. */
class Sound {
public:
    ~Sound();

    /** Available sounds. */
    enum Type {
        Clatz,   ///< Rotation complete sound
        Complete ///< Level complete sound
    };

    /**
     * Initialization.
     * @return false if something went wrong
     */
    bool initialize();

    /**
     * Play specified sound.
     * @param type sound to play
     */
    void play(Type type);

    bool enable = true; ///< Sound enable/disable

private:
    /**
     * Load sound files from the specified directory.
     * @param dir path to directory with sound files (wav)
     * @return false if load failed
     */
    bool load(const std::filesystem::path& dir);

    struct Wave {
        Uint8* data; ///< Plain wave data
        Uint32 size; ///< Size of wave data
    };
    Wave waves[2] = {}; ///< Wav data instances

    SDL_AudioStream* stream; ///< Output audio stream
};
