#include "window.hpp"
#include "res.hpp"
#include <SDL3/SDL.h>

Window::Window() {
    handle =
        SDL_CreateWindow("SuperPuperMegaGraphProject", 800, 600,
                         SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_HIDDEN);
    if (!handle) {
        show_error(std::string("Failed to create window: ") + SDL_GetError());
        return;
    }
    SDL_SetWindowMinimumSize(handle, 320, 200);
    SDL_Surface* icon = res_load_surface("favicon.png");
    if (icon) {
        SDL_SetWindowIcon(handle, icon);
        SDL_free(icon);
    }
}

Window::~Window() {
    if (handle) {
        SDL_DestroyWindow(handle);
        handle = nullptr;
    }
}

SDL_Window* Window::get_handle() { return handle; }

bool Window::is_inited() { return handle != nullptr; }

void Window::show_error(const std::string& text) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", text.c_str(), handle);
#ifdef _DEBUG
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", text.c_str());
#endif
}

void Window::on_resize() {
    int w_buf, h_buf;
    if (SDL_GetWindowSize(handle, &w_buf, &h_buf)) {
        size.x = static_cast<float>(w_buf);
        size.y = static_cast<float>(h_buf);
    } else {
        size.x = 800.f;
        size.y = 600.f;
    }
}

void Window::set_shown(bool show) {
    bool ret = show ? SDL_ShowWindow(handle) : SDL_HideWindow(handle);
    if (!ret)
        show_error(std::string("Failed to set window shown: ") + SDL_GetError());
}

void Window::set_title(const std::string& title) { SDL_SetWindowTitle(handle, title.c_str()); }
