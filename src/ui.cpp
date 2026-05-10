#include "ui.hpp"
#include "button.hpp"
#include "frame.hpp"
#include "image.hpp"
#include "res.hpp"
#include <SDL3/SDL.h>

void construct_ui() {
    root->add_child(new Button("Button_Test"))
        ->set_child((new Image("Image_add"))->set_texture(res->load_texture("add.png")))
        ->set_click_handler([&](Button* self, Container*) { SDL_Log("Click"); })
        ->set_resize_handler([&](Container* self, Container*) { SDL_Log("Resize"); })
        ->set_rect({100.f, 100.f, 64.f, 64.f});
}
