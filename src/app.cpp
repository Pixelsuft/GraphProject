#include "app.hpp"
#include "button.hpp"
#include "clock.hpp"
#include "frame.hpp"
#include "render.hpp"
#include "window.hpp"
#include <SDL3/SDL.h>

Window* win;
Render* ren;
Frame* ui;

App::App() {
    win = new Window();
    ren = new Render(win->get_handle());
    if (!is_inited())
        return;
    clock = new Clock();
    win->set_shown(true);
    ui = new Frame("Main_UI");
    mouse_entered = false;
}

App::~App() {
    if (is_inited()) {
        delete ui;
        delete clock;
    }
    delete ren;
    delete win;
}

bool App::is_inited() { return win->is_inited() && ren->is_inited(); }

void App::setup_ui() {
    auto btn = new Button("Button_test");
    btn->rect.x = btn->rect.y = 100.f;
    btn->rect.w = btn->rect.h = 50.f;
    ui->add_child(btn);
}

void App::on_resize() {
    win->on_resize();
    ren->on_resize();
    ui->rect.w = ren->size.x;
    ui->rect.h = ren->size.y;
    ui->scale = (ren->size.x / win->size.x + ren->size.y / win->size.y) / 2.f;
    ui->on_resize(nullptr);
}

bool App::on_event(SDL_Event& ev) {
    switch (ev.type) {
    case SDL_EVENT_MOUSE_MOTION:
        if (SDL_ConvertEventToRenderCoordinates(ren->get_handle(), &ev))
            ui->on_mouse_move(nullptr, Point(ev.motion.x, ev.motion.y),
                              Point(ev.motion.xrel, ev.motion.yrel));
        return true;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (SDL_ConvertEventToRenderCoordinates(ren->get_handle(), &ev))
            ui->on_mouse_down(nullptr, Point(ev.button.x, ev.button.y), ev.button.button,
                              ev.button.down);
        return true;
    case SDL_EVENT_WINDOW_MOUSE_ENTER:
        if (!mouse_entered) {
            mouse_entered = true;
            ui->on_mouse_enter(nullptr, Point(-1, -1), true);
        }
        return true;
    case SDL_EVENT_WINDOW_MOUSE_LEAVE:
        if (mouse_entered) {
            mouse_entered = false;
            ui->on_mouse_enter(nullptr, Point(-1, -1), false);
        }
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
    clock->on_update();
    win->set_title(std::string("FPS: ") + std::to_string(clock->fps));
    ren->clear(Color());
    ui->on_update(nullptr);
    ren->set_scale_enabled(true);
    ui->on_draw(nullptr);
    ren->set_scale_enabled(false);
    ren->present();
    return true;
}
