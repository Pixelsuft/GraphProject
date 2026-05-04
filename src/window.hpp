#pragma once
#include "rect.hpp"
#include <SDL3/SDL_video.h>
#include <string>

class Window {
    SDL_Window* handle;

public:
    Point size;

    Window();
    ~Window();
    SDL_Window* get_handle();
    void show_error(const std::string& text);
    bool is_inited();
    void on_resize();
    void set_shown(bool show);
    void set_title(const std::string& title);
};

extern Window* win;
