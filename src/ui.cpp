#include "ui.hpp"
#include "button.hpp"
#include "frame.hpp"
#include <SDL3/SDL.h>

void construct_ui() {
    root->add_child(new Button("Button_Test"))
        ->set_click_handler([&](Button* self, Container*) { SDL_Log("Click"); })
        ->set_resize_handler([&](Container* self, Container*) { SDL_Log("Resize"); })
        ->set_rect({100.f, 100.f, 50.f, 50.f});
}
