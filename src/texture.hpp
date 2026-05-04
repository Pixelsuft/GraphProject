#pragma once
#include "rect.hpp"
#include <SDL3/SDL_render.h>

struct Texture {
    SDL_Texture* handle;
    Point size;
};
