#pragma once
#include "texture.hpp"
#include <SDL3/SDL_surface.h>
#include <string>

class Res {
public:
    Res();
    ~Res();
    SDL_Surface* load_surface(std::string fn);
    Texture load_texture(std::string fn);
};

extern Res* res;
