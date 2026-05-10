#include "ui.hpp"
#include "button.hpp"
#include "frame.hpp"
#include "image.hpp"
#include "res.hpp"
#include <SDL3/SDL.h>

static Frame* flow;

void construct_ui() {
    // Workflow
    root->add_child(new Frame("Frame_Flow"))
        ->set_resize_handler(
            [&](Container* self, Container* parent) { self->rect = parent->rect; });
    // Add button
    root->add_child(new Button("Button_Add"))
        ->set_child((new Image("Image_Add"))->set_texture(res->load_texture("add.png")))
        ->set_click_handler([&](Button* self, Container*) { SDL_Log("Click"); })
        ->set_rect({10.f, 10.f, 64.f, 64.f});
}
