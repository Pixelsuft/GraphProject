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
    Point center = rect.as_point() + rect.as_size() / 2.f;
    ren->fill_circle(center, r, color);
}

void Vertex::on_mouse_move(Container* parent, Point pos, Point dp, bool holding) {
    if (holding && vertex_mode != 1 && vertex_mode != 2)
        rect += dp;
}

void Vertex::on_mouse_down(Container* parent, Point pos, uint8_t index, bool down) {
    if (vertex_mode == 3 && down && color.r == 0.f && color.b == 0.f) {
        reinterpret_cast<Frame*>(parent)->remove_child(this);
    }
}

bool Vertex::has_mouse_collision(Container* parent, Point pos) {
    if (Container::has_mouse_collision(parent, pos)) {
        float r = rect.w / 2.f;
        Point center = rect.as_point() + rect.as_size() / 2.f - pos;
        return center.x * center.x + center.y * center.y <= r * r;
    }
    return false;
}
