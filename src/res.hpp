#pragma once
#include <SDL3/SDL_surface.h>
#include <string>

void res_init();
SDL_Surface* res_load_surface(std::string fn);
