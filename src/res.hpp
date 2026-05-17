#pragma once
#include "texture.hpp"
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

class Res {
public:
    Res();
    ~Res();
    SDL_Surface* load_surface(std::string fn);
    TTF_Font* load_font(std::string fn, float size);
    Texture load_texture(std::string fn);
};

extern Res* res;
