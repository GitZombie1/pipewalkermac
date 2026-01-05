// SPDX-License-Identifier: MIT
// Fireworks used in level completion animation.
// Copyright (C) 2024 Artem Senichev <artemsen@gmail.com>

#pragma once

#include <SDL3/SDL.h>

struct Firework {
    /**
     * Constructor.
     * @param init initial position and size of the cell
     */
    Firework(const SDL_FRect& init);

    /** Update firework state. */
    void update();

    SDL_FRect current; ///< Current position and size
    size_t variant;    ///< Texture variant [0-4)
    double angle;      ///< Current angle
    double alpha;      ///< Current transparency

private:
    SDL_FRect initial; ///< Initial position and size
    size_t birth_time; ///< Creation timestamp
    size_t age_limit;  ///< Age limit in ms
    float delta_x;     ///< Direction and diff of the final x coordinate
};
