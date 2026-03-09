// SPDX-License-Identifier: MIT
// Skin loader.
// Copyright (C) 2024 Artem Senichev <artemsen@gmail.com>

#include "skin.hpp"

#include "buildcfg.h"

/**
 * Load PNG image.
 * @param path to the image file
 * @return image surface handle, nullptr on error
 */
static SDL_Surface* load_png(const char* path)
{
    SDL_Surface* surface = SDL_LoadPNG(path);

    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Error loading skin from %s: %s",
                     path, SDL_GetError());
        return nullptr;
    }
    if (surface->w % 512 != 0 || surface->h % 448 != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Invalid skin file: %s", path);
        SDL_DestroySurface(surface);
        return nullptr;
    }

    return surface;
}

SDL_Surface* Skin::initialize(const std::string& name)
{
    SDL_Surface* image = nullptr;

    if (!search(APP_DATADIR)) {
        // try portable variant
        std::filesystem::path path(SDL_GetBasePath());
        path /= "data";
        search(path);
    }

    // search for specified skin
    for (size_t i = 0; i < skins.size(); ++i) {
        if (name == skins[i].stem().string()) {
            image = load(i);
            break;
        }
    }
    if (!image) {
        // fallback: load first available
        for (size_t i = 0; i < skins.size(); ++i) {
            image = load(i);
            if (image) {
                break;
            }
        }
    }

    return image;
}

SDL_Surface* Skin::prev()
{
    SDL_Surface* image = nullptr;

    for (ssize_t i = skin_index - 1; !image && i >= 0; --i) {
        image = load(i);
    }
    for (ssize_t i = skins.size() - 1;
         !image && i > static_cast<ssize_t>(skin_index); --i) {
        image = load(i);
    }

    return image;
}

SDL_Surface* Skin::next()
{
    SDL_Surface* image = nullptr;

    for (size_t i = skin_index + 1; !image && i < skins.size(); ++i) {
        image = load(i);
    }
    for (size_t i = 0; !image && i < skin_index; ++i) {
        image = load(i);
    }

    return image;
}

SDL_Surface* Skin::load(size_t index)
{
    const std::filesystem::path& path = skins[index];
    SDL_Surface* image = load_png(path.string().c_str());
    if (image) {
        skin_name = path.stem().string();
        skin_index = index;
    }
    return image;
}

bool Skin::search(const std::filesystem::path& dir)
{
    int count;
    char** files = SDL_GlobDirectory(dir.string().c_str(), "*.png", 0, &count);
    for (int i = 0; i < count; ++i) {
        std::filesystem::path path = dir / files[i];
        skins.push_back(path);
    }
    SDL_free(files);
    return count > 0;
}

const std::string& Skin::name() const
{
    return skin_name;
}
