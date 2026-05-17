#include "ui.hpp"
#include "background.hpp"
#include "button.hpp"
#include "frame.hpp"
#include "image.hpp"
#include "res.hpp"
#include "vertex.hpp"
#include <SDL3/SDL.h>
#include <algorithm>

static Frame* flow;
static Image* detail;
TTF_Font* def_font;

static void set_selected_button(Container* btn, bool enabled = true) {
    // Hacky way to allow toggle
    if (reinterpret_cast<Button*>(btn)->bg_color.g != 0.f)
        vertex_mode = 0;
    for (const char* id :
         {"Button_Add", "Button_Join", "Button_Trash", "Button_Start", "Button_Stop"}) {
        Button* b = reinterpret_cast<Button*>(root->child_by_id(id));
        b->bg_color.g = (btn != b || vertex_mode == 0) ? 0.f : 0.2f;
        if (b->id != "Button_Start" && b->id != "Button_Stop") {
            b->enabled = enabled;
            b->border_color = enabled ? Color(0.f, 1.f, 0.f) : Color(1.f, 0.f, 0.f);
        }
        b->refresh_color();
    }
}

void construct_ui() {
    def_font = res->load_font("VCR_OSD_MONO.ttf", 24.f);
    last_edge = nullptr;
    last_vertex = nullptr;
    vertex_mode = 0;

    // Workflow
    flow = new Frame("Frame_Flow");
    root->add_child(flow)->set_resize_handler(
        [&](Container* self, Container* parent) { self->rect = parent->rect; });

    // Add button
    root->add_child(new Button("Button_Add"))
        ->set_child((new Image("Image_Add"))->set_texture(res->load_texture("add.png")))
        ->set_click_handler([&](Button* self, Container*) {
            vertex_mode = 1;
            set_selected_button(self);
        })
        ->set_rect({10.f, 10.f, 64.f, 64.f});
    // Join button
    root->add_child(new Button("Button_Join"))
        ->set_child((new Image("Image_Join"))->set_texture(res->load_texture("join.png")))
        ->set_click_handler([&](Button* self, Container*) {
            vertex_mode = 2;
            set_selected_button(self);
        })
        ->set_rect({84.f, 10.f, 64.f, 64.f});
    // Trash button
    root->add_child(new Button("Button_Trash"))
        ->set_child((new Image("Image_Trash"))->set_texture(res->load_texture("trash.png")))
        ->set_click_handler([&](Button* self, Container*) {
            vertex_mode = 3;
            set_selected_button(self);
        })
        ->set_rect({158.f, 10.f, 64.f, 64.f});
    // Start button
    root->add_child(new Button("Button_Start"))
        ->set_child((new Image("Image_Start"))->set_texture(res->load_texture("start.png")))
        ->set_click_handler([&](Button* self, Container*) {
            last_edge = nullptr;
            vertex_mode = 0;
            set_selected_button(self, false);
            detail->visible = true;
            self->visible = false;
            root->child_by_id("Button_Stop")->visible = true;
        })
        ->set_rect({232.f, 10.f, 64.f, 64.f});
    // Stop button
    root->add_child(new Button("Button_Stop"))
        ->set_child((new Image("Image_Stop"))->set_texture(res->load_texture("stop.png")))
        ->set_click_handler([&](Button* self, Container*) {
            last_edge = nullptr;
            vertex_mode = 0;
            set_selected_button(self, true);
            detail->visible = false;
            self->visible = false;
            root->child_by_id("Button_Start")->visible = true;
        })
        ->set_rect({232.f, 10.f, 64.f, 64.f});
    root->child_by_id("Button_Stop")->visible = false;

    // Background
    flow->add_child(new Background("Background"))
        ->set_down_handler([&](Background*, Container*, Point pos) {
            if (vertex_mode == 1) {
                // push_back might reallocate memory
                last_vertex = nullptr;
                last_edge = nullptr;
                flow->add_child(new Vertex("Vertex_NAME_TODO"))
                    ->set_rect(Rect({pos.x - 20.f, pos.y - 20.f, 40.f, 40.f}));
            }
        })
        ->set_resize_handler(
            [&](Container* self, Container* parent) { self->rect = parent->rect; });
    // Detail for demo
    detail = new Image("Detail", true);
    detail->visible = false;
    flow->add_child(detail)
        ->set_texture(res->load_texture("cogwheel.png"))
        ->set_rect({0.f, 0.f, 32.f, 32.f});
    // S, T vertexes
    flow->add_child(new Vertex("Vertex_S"))->set_rect({100.f, 100.f, 40.f, 40.f});
    flow->add_child(new Vertex("Vertex_T"))->set_rect({100.f, 150.f, 40.f, 40.f});
}

void kbd_ui(char key) {
    if (key == '+' && last_edge) {
        last_edge->weight++;
        last_edge->update_text();
    } else if (key == '-' && last_edge) {
        if (last_edge->weight > 1) {
            last_edge->weight--;
            last_edge->update_text();
        } else {
            auto index = last_edge - last_vertex->edges.data();
            last_vertex->edges.erase(last_vertex->edges.begin() + index);
            last_edge = nullptr;
        }
    }
}

void destroy_ui() { TTF_CloseFont(def_font); }
