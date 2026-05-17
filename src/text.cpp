#include "text.hpp"
#include "render.hpp"
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

void TextEngine::destroy_text(void* text_obj) {
    TTF_DestroyText(reinterpret_cast<TTF_Text*>(text_obj));
}

void TextEngine::set_text(void* text_obj, const char* s) {
    TTF_SetTextString(reinterpret_cast<TTF_Text*>(text_obj), s, 0);
}

void TextEngine::set_color(void* text_obj, Color col) {
    TTF_SetTextColorFloat(reinterpret_cast<TTF_Text*>(text_obj), col.r, col.g, col.b, col.a);
}

void TextEngine::draw(void* text_obj, Point pos, bool center) {
    pos += ren->offset;
    int w, h;
    if (center && TTF_GetTextSize(reinterpret_cast<TTF_Text*>(text_obj), &w, &h)) {
        pos -= Point(static_cast<float>(w) / 2.f, static_cast<float>(h) / 2.f);
    }
    TTF_DrawRendererText(reinterpret_cast<TTF_Text*>(text_obj), pos.x, pos.y);
}
