// SPDX-License-Identifier: MIT
// Skin loader.
// Copyright (C) 2024 Artem Senichev <artemsen@gmail.com>

#pragma once

#include <SDL3/SDL.h>

#include <filesystem>
#include <string>
#include <vector>

/** Skin loader. */
class Skin {
public:
    /**
     * Initialization.
     * @param name name of the skin loaded by default
     * @return image handle, nullptr on error
     */
    SDL_Surface* initialize(const std::string& name);

    /**
     * Load previous available skin.
     * @return image handle, nullptr on error
     */
    SDL_Surface* prev();

    /**
     * Load next available skin.
     * @return image handle, nullptr on error
     */
    SDL_Surface* next();

    /**
     * Get current skin name.
     * @return skin name
     */
    const std::string& name() const;

private:
    /**
     * Search skin files in the specified directory.
     * @param dir path to directory with skin files (png)
     * @return true if at least one skin was found
     */
    bool search(const std::filesystem::path& dir);

    /**
     * Load skin.
     * @param index index of the skin record
     * @return image handle, nullptr on error
     */
    SDL_Surface* load(size_t index);

    std::vector<std::filesystem::path> skins; ///< List of available skins
    size_t skin_index;                        ///< Index of the current skin
    std::string skin_name;                    ///< Name of the current skin
};
