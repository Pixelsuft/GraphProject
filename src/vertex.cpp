#include "vertex.hpp"
#include "render.hpp"
#include <SDL3/SDL.h>

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
    Point center = rect.as_point() + rect.as_size() / 2.f;
    ren->fill_circle(center, r, color);
}

void Vertex::on_mouse_move(Container* parent, Point pos, Point dp, bool holding) {
    if (holding)
        rect += dp;
}

bool Vertex::has_mouse_collision(Container* parent, Point pos) {
    if (Container::has_mouse_collision(parent, pos)) {
        float r = rect.w / 2.f;
        Point center = rect.as_point() + rect.as_size() / 2.f - pos;
        return center.x * center.x + center.y * center.y <= r * r;
    }
    return false;
}
