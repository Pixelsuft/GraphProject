#include "render.hpp"
#include "window.hpp"
#include <SDL3/SDL.h>

Render::Render(void* win_handle) {
    handle = nullptr;
    scale = 1.f;
    SDL_PropertiesID props = SDL_CreateProperties();
    if (!props)
        return;
    SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, win_handle);
    SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);
#ifdef _WIN32
    SDL_SetStringProperty(props, SDL_PROP_RENDERER_CREATE_NAME_STRING, "direct3d");
#endif
    handle = SDL_CreateRendererWithProperties(props);
    if (!handle) {
        win->show_error(std::string("Failed to create render: ") + SDL_GetError());
        SDL_DestroyProperties(props);
        return;
    }
    SDL_DestroyProperties(props);
}

SDL_Renderer* Render::get_handle() { return handle; }

Render::~Render() {
    if (handle) {
        SDL_DestroyRenderer(handle);
        handle = nullptr;
    }
}

bool Render::is_inited() { return handle != nullptr; }

void Render::on_resize() {
    int w_buf, h_buf;
    if (SDL_GetRenderOutputSize(handle, &w_buf, &h_buf) ||
        SDL_GetWindowSize(win->get_handle(), &w_buf, &h_buf)) {
        size.x = static_cast<float>(w_buf);
        size.y = static_cast<float>(h_buf);
    } else {
        size.x = 800.f;
        size.y = 600.f;
    }
}

void Render::set_scale_enabled(bool enabled) {
    SDL_SetRenderScale(handle, enabled ? scale : 1.f, enabled ? scale : 1.f);
}

void Render::present() { SDL_RenderPresent(handle); }

void Render::clear(Color col) {
    if (col.a <= 0.f)
        return;
    SDL_SetRenderDrawColorFloat(handle, col.r, col.g, col.b, col.a);
    SDL_RenderClear(handle);
}

void Render::fill_rect(Rect rect, Color col) {
    if (col.a <= 0.f)
        return;
    Rect real_rect = rect + offset;
    SDL_SetRenderDrawColorFloat(handle, col.r, col.g, col.b, col.a);
    SDL_SetRenderDrawBlendMode(handle, col.a >= 1.f ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(handle, real_rect.as_frect());
}

void Render::draw_rect(Rect rect, Color col) {
    if (col.a <= 0.f)
        return;
    Rect real_rect = rect + offset;
    SDL_SetRenderDrawColorFloat(handle, col.r, col.g, col.b, col.a);
    SDL_SetRenderDrawBlendMode(handle, col.a >= 1.f ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
    SDL_RenderRect(handle, real_rect.as_frect());
}

void Render::fill_circle(Point center, float radius, Color col) {
    if (col.a <= 0.f || radius <= 0.f)
        return;
    set_scale_enabled(false);
    Point real_center = center + offset * scale;
    float r = radius * scale;
    // TODO: switch to rendering geometry???
    SDL_SetRenderDrawColorFloat(handle, col.r, col.g, col.b, col.a);
    SDL_SetRenderDrawBlendMode(handle, col.a >= 1.f ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
    for (float dy = -r; dy <= r; dy += 1.0f) {
        float dx = SDL_sqrtf(r * r - dy * dy);
        if (dx < 0.1f)
            continue;
        float x1 = real_center.x - dx;
        float x2 = real_center.x + dx;
        float y = real_center.y + dy;

        SDL_RenderLine(handle, x1, y, x2, y);
    }
    set_scale_enabled(true);
}

void Render::draw_texture(Point pos, Texture texture) {
    SDL_FRect dst = {pos.x + offset.x, pos.y + offset.y, texture.size.x, texture.size.y};
    SDL_RenderTexture(handle, texture.handle, nullptr, &dst);
}

void Render::draw_texture_sized(Rect rect, Texture texture) {
    SDL_FRect dst = {rect.x + offset.x, rect.y + offset.y, rect.w, rect.h};
    SDL_RenderTexture(handle, texture.handle, nullptr, &dst);
}
