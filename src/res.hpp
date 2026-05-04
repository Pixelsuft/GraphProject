#pragma once
#include <SDL3/SDL_surface.h>
#include <string>

class Res {
public:
    Res();
    ~Res();
    SDL_Surface* load_surface(std::string fn);
};

extern Res* res;
