#include "vertex.hpp"
#include "frame.hpp"
#include "render.hpp"
#include <SDL3/SDL.h>

int vertex_mode;

Vertex::Vertex(std::string id) : Container(id) {
    color = Color(0.f, 1.f, 0.f, 0.5f);
    if (id.back() == 'S')
        color = Color(0.f, 1.f, 1.f, 0.5f);
    else if (id.back() == 'T')
        color = Color(1.f, 0.f, 0.f, 0.5f);
}

Vertex::~Vertex() {}

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
    }
}

void Vertex::on_mouse_move(Container* parent, Point pos, Point dp, bool holding) {
    if (holding && vertex_mode != 1 && vertex_mode != 2)
        rect += dp;
    last_mouse = pos;
}

void Vertex::on_mouse_down(Container* parent, Point pos, uint8_t index, bool down) {
    Frame* f = reinterpret_cast<Frame*>(parent);
    if (vertex_mode == 3 && down && color.r == 0.f && color.b == 0.f) {
        f->remove_child(this);
    }
    if (vertex_mode == 2 && !down) {
        Container* focused = f->find_focused(pos);
        if (focused != nullptr && focused != f->child[0] && focused != f->child[1] &&
            focused != this) {
            Vertex* v = reinterpret_cast<Vertex*>(focused);
            Edge edge;
            edge.end = v;
            edge.weight = 1;
            edges.push_back(edge);
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
