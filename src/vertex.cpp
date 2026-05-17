#include "vertex.hpp"
#include "frame.hpp"
#include "render.hpp"
#include "text.hpp"
#include <SDL3/SDL.h>
#include <algorithm>

extern TTF_Font* def_font;
Edge* last_edge;
Vertex* last_vertex;
int vertex_mode;

Edge::Edge() : end(nullptr), info(nullptr), weight(1), used(0), flow(0) {}

void Edge::init(Vertex* e) {
    info = text->create_text(def_font);
    text->set_color(info, Color(1.f, 1.f, 1.f));
    end = e;
    update_text();
}

void Edge::destroy() { text->destroy_text(info); }

void Edge::update_text() {
    static char buf[32];
    SDL_itoa(used, buf, 10);
    size_t len = SDL_strlen(buf);
    buf[len] = '/';
    SDL_itoa(weight, buf + len + 1, 10);
    text->set_text(info, buf);
}

Vertex::Vertex(std::string id) : Container(id) {
    color = Color(0.f, 1.f, 0.f, 0.5f);
    if (id.back() == 'S')
        color = Color(0.f, 1.f, 1.f, 0.5f);
    else if (id.back() == 'T')
        color = Color(1.f, 0.f, 0.f, 0.5f);
}

Vertex::~Vertex() {
    if (last_vertex == this) {
        // For sure
        last_vertex = nullptr;
        last_edge = nullptr;
    }
}

Edge* Vertex::find_reverse(Edge* target) {
    auto vit = std::find_if(target->end->edges.begin(), target->end->edges.end(),
                            [this](const Edge& e) { return e.end == this; });
    return (vit == target->end->edges.end()) ? nullptr : &*vit;
}

void Vertex::clean_other_edges(Container* parent) {
    Frame* f = reinterpret_cast<Frame*>(parent);
    for (auto it = f->child.begin() + 2; it != f->child.end(); it++) {
        if (*it == this)
            continue;
        Vertex* v = reinterpret_cast<Vertex*>(*it);
        auto vit = std::find_if(v->edges.begin(), v->edges.end(),
                                [this](const Edge& e) { return e.end == this; });
        if (vit != v->edges.end()) {
            vit->destroy();
            v->edges.erase(vit);
        }
    }
}

void Vertex::on_draw(Container* parent) {
    float r = rect.w / 2.f;
    Point center = get_center();
    ren->fill_circle(center, r, color);
    if (holding && vertex_mode == 2) {
        ren->draw_arrow(center, last_mouse, Color(0.f, 1.f, 1.f));
    }
    for (auto& edge : edges) {
        Point route = edge.end->rect.as_point() - rect.as_point();
        if (route.empty())
            continue;
        Point norm = route / route.get_length();
        ren->draw_arrow(center + norm * r, edge.end->get_center() - norm * r, Color(1.f, 0.f, 0.f));
        text->draw(edge.info, center + route / 4.f);
    }
}

void Vertex::on_mouse_move(Container* parent, Point pos, Point dp, bool holding) {
    if (holding && vertex_mode != 1 && vertex_mode != 2)
        rect += dp;
    last_mouse = pos;
}

void Vertex::on_mouse_down(Container* parent, Point pos, uint8_t index, bool down) {
    last_edge = nullptr;
    Frame* f = reinterpret_cast<Frame*>(parent);
    if (vertex_mode == 3 && down && color.r == 0.f && color.b == 0.f) {
        clean_other_edges(parent);
        f->remove_child(this);
        return;
    }
    // not T
    if (this == f->child[3])
        return;
    if (vertex_mode == 2 && !down) {
        Container* focused = f->find_focused(pos);
        // not null, not bg, not cogwheel, not S, not this
        if (focused != nullptr && focused != f->child[0] && focused != f->child[1] &&
            focused != f->child[2] && focused != this) {
            Vertex* v = reinterpret_cast<Vertex*>(focused);
            auto it = std::find_if(edges.begin(), edges.end(),
                                   [&v](const Edge& e) { return e.end == v; });
            if (it == edges.end()) {
                edges.push_back(Edge());
                edges.back().init(v);
                last_edge = &edges.back();
            } else {
                it->weight++;
                it->update_text();
                last_edge = &*it;
            }
            last_vertex = this;
        }
    }
    holding = down;
}

bool Vertex::has_mouse_collision(Container* parent, Point pos) {
    if (Container::has_mouse_collision(parent, pos)) {
        float r = rect.w / 2.f;
        Point center = get_center() - pos;
        return center.x * center.x + center.y * center.y <= r * r;
    }
    return false;
}
