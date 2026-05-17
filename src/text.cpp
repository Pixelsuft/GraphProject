#include "text.hpp"
#include "window.hpp"

TextEngine::TextEngine(void* ren_handle) {
    handle = TTF_CreateRendererTextEngine(reinterpret_cast<SDL_Renderer*>(ren_handle));
    if (!handle) {
        win->show_error(std::string("Failed to create text engine: ") + SDL_GetError());
        return;
    }
}

TextEngine::~TextEngine() {
    if (handle) {
        TTF_DestroyRendererTextEngine(handle);
        handle = nullptr;
    }
}

bool TextEngine::is_inited() { return handle != nullptr; }

void* TextEngine::create_text(TTF_Font* font) { return TTF_CreateText(handle, font, "", 0); }

void TextEngine::destroy_text(void* text_obj) { TTF_DestroyText(reinterpret_cast<TTF_Text*>(text_obj)); }
