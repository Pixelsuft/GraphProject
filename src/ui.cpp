#include "ui.hpp"
#include "background.hpp"
#include "button.hpp"
#include "frame.hpp"
#include "image.hpp"
#include "res.hpp"
#include <SDL3/SDL.h>

static Frame* flow;

static void set_selected_button(Button* btn) {
    for (const char* id :
         {"Button_Add", "Button_Join", "Button_Trash", "Button_Start", "Button_Stop"}) {
        Button* b = reinterpret_cast<Button*>(root->child_by_id(id));
        b->bg_color.g = btn != b ? 0.f : 0.2f;
        b->refresh_color();
    }
}

void construct_ui() {
    // Workflow
    flow = new Frame("Frame_Flow");
    root->add_child(flow)->set_resize_handler(
        [&](Container* self, Container* parent) { self->rect = parent->rect; });
    // Background
    flow->add_child(new Background("Background"))
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
    // Trash button
    root->add_child(new Button("Button_Trash"))
        ->set_child((new Image("Image_Trash"))->set_texture(res->load_texture("trash.png")))
        ->set_click_handler([&](Button* self, Container*) { set_selected_button(self); })
        ->set_rect({158.f, 10.f, 64.f, 64.f});
    // Start button
    root->add_child(new Button("Button_Start"))
        ->set_child((new Image("Image_Start"))->set_texture(res->load_texture("start.png")))
        ->set_click_handler([&](Button* self, Container*) { set_selected_button(self); })
        ->set_rect({232.f, 10.f, 64.f, 64.f});
    // Stop button
    root->add_child(new Button("Button_Stop"))
        ->set_child((new Image("Image_Stop"))->set_texture(res->load_texture("stop.png")))
        ->set_click_handler([&](Button* self, Container*) { set_selected_button(self); })
        ->set_rect({306.f, 10.f, 64.f, 64.f});
}
