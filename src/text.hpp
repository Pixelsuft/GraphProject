#pragma once
#include "color.hpp"
#include "rect.hpp"
#include <SDL3_ttf/SDL_textengine.h>

class TextEngine {
    TTF_TextEngine* handle;

public:
    TextEngine(void* ren_handle);
    ~TextEngine();
    bool is_inited();
    void* create_text(TTF_Font* font);
    void set_text(void* text_obj, const char* s);
    void set_color(void* text_obj, Color col);
    void destroy_text(void* text_obj);
    void draw(void* text_obj, Point pos, bool center = true);
};

extern TextEngine* text;
