#pragma once
#include <SDL3_ttf/SDL_textengine.h>

class TextEngine {
    TTF_TextEngine* handle;

public:
    TextEngine(void* ren_handle);
    ~TextEngine();
    bool is_inited();
    void* create_text(TTF_Font* font);
    void destroy_text(void* text_obj);
};

extern TextEngine* text;
