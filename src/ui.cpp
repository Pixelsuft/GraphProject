#include "ui.hpp"
#include "button.hpp"
#include "frame.hpp"
#include "image.hpp"
#include "res.hpp"
#include <SDL3/SDL.h>

static Frame* flow;

static void set_selected_button(Button* btn) {
    for (const char* id : { "Button_Add", "Button_Join" }) {
        Button* b = reinterpret_cast<Button*>(root->child_by_id(id));
        b->bg_color.g = btn != b ? 0.f : 0.2f;
        b->refresh_color();
    }
}

void construct_ui() {
    // Workflow
    root->add_child(new Frame("Frame_Flow"))
        ->set_resize_handler(
            [&](Container* self, Container* parent) { self->rect = parent->rect; });
    // Add button
    root->add_child(new Button("Button_Add"))
        ->set_child((new Image("Image_Add"))->set_texture(res->load_texture("add.png")))
        ->set_click_handler([&](Button* self, Container*) { set_selected_button(self); })
        ->set_rect({10.f, 10.f, 64.f, 64.f});
    // Join button
    root->add_child(new Button("Button_Join"))
        ->set_child((new Image("Image_Join"))->set_texture(res->load_texture("join.png")))
        ->set_click_handler([&](Button* self, Container*) { set_selected_button(self); })
        ->set_rect({84.f, 10.f, 64.f, 64.f});
}
