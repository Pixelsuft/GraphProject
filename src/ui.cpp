#include "ui.hpp"
#include "button.hpp"
#include "frame.hpp"
#include <SDL3/SDL.h>

void construct_ui() {
    root->add_child(new Button("Button_Test"))
        ->set_click_handler([&](Button* self) {})
        ->set_resize_handler([&](Container* self) { SDL_Log("resize"); })
        ->set_rect({100.f, 100.f, 50.f, 50.f});
}
