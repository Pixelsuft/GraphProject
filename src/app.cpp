#include "app.hpp"
#include "clock.hpp"
#include "frame.hpp"
#include "render.hpp"
#include "res.hpp"
#include "text.hpp"
#include "ui.hpp"
#include "window.hpp"
#include <SDL3/SDL.h>

Window* win;
Render* ren;
TextEngine* text;
Frame* root;
Clock* gclock;
Res* res;

App::App() {
    res = new Res();
    win = new Window();
    ren = new Render(win->get_handle());
    text = new TextEngine(ren->get_handle());
    if (!is_inited())
        return;
    gclock = new Clock();
    win->set_shown(true);
    root = new Frame("Frame_Root_UI");
    mouse_entered = false;
}

App::~App() {
    if (is_inited()) {
        delete root;
        delete gclock;
    }
    delete text;
    delete ren;
    delete win;
    delete res;
}

bool App::is_inited() { return win->is_inited() && ren->is_inited() && text->is_inited(); }

void App::on_resize() {
    win->on_resize();
    ren->on_resize();
    root->scale = (ren->size.x / win->size.x + ren->size.y / win->size.y) / 2.f;
    root->rect.w = ren->size.x / root->scale;
    root->rect.h = ren->size.y / root->scale;
    root->on_resize(nullptr);
}

bool App::on_event(SDL_Event& ev) {
    switch (ev.type) {
    case SDL_EVENT_MOUSE_MOTION:
        if (SDL_ConvertEventToRenderCoordinates(ren->get_handle(), &ev))
            root->on_mouse_move(nullptr, Point(ev.motion.x, ev.motion.y),
                                Point(ev.motion.xrel, ev.motion.yrel), false);
        return true;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (SDL_ConvertEventToRenderCoordinates(ren->get_handle(), &ev))
            root->on_mouse_down(nullptr, Point(ev.button.x, ev.button.y), ev.button.button,
                                ev.button.down);
        return true;
    case SDL_EVENT_WINDOW_MOUSE_ENTER:
        if (!mouse_entered) {
            mouse_entered = true;
            root->on_mouse_enter(nullptr, Point(-1, -1), true);
        }
        return true;
    case SDL_EVENT_WINDOW_MOUSE_LEAVE:
        if (mouse_entered) {
            mouse_entered = false;
            root->on_mouse_enter(nullptr, Point(-1, -1), false);
        }
        return true;
    case SDL_EVENT_KEY_DOWN:
        if (ev.key.scancode == SDL_SCANCODE_EQUALS || ev.key.scancode == SDL_SCANCODE_KP_PLUS)
            kbd_ui('+');
        else if (ev.key.scancode == SDL_SCANCODE_MINUS || ev.key.scancode == SDL_SCANCODE_KP_MINUS)
            kbd_ui('-');
        return true;
    case SDL_EVENT_WINDOW_RESIZED:
        on_resize();
        return true;
    case SDL_EVENT_QUIT:
        return false;
    default:
        return true;
    }
}

bool App::on_update() {
    gclock->on_update();
#ifdef _DEBUG
    win->set_title(std::string("FPS: ") + std::to_string(gclock->fps));
#endif
    ren->clear(Color());
    root->on_update(nullptr);
    ren->set_scale_enabled(true);
    root->on_draw(nullptr);
    draw_ui();
    ren->set_scale_enabled(false);
    ren->present();
    return true;
}
