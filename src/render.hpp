#pragma once
#include "color.hpp"
#include "rect.hpp"
#include "texture.hpp"
#include <SDL3/SDL_render.h>

class Render {
    SDL_Renderer* handle;

public:
    Point size;
    Point offset;
    float scale;

    Render(void* win_handle);
    ~Render();
    SDL_Renderer* get_handle();
    bool is_inited();
    void present();
    void on_resize();
    void set_scale_enabled(bool enabled);
    void clear(Color col);
    void draw_line(Point start, Point end, Color col);
    void fill_rect(Rect rect, Color col);
    void draw_rect(Rect rect, Color col);
    void fill_circle(Point center, float radius, Color col);
    void draw_texture(Point pos, Texture texture);
    void draw_texture_sized(Rect rect, Texture texture);
};

extern Render* ren;
