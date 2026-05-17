#pragma once
#include "color.hpp"
#include "container.hpp"
#include <vector>

// hardcoding is fun :)
extern int vertex_mode;

class Vertex;

struct Edge {
    Vertex* end;
    int weight;
    int used;

    Edge() : end(nullptr), weight(0), used(0) {}
};

class Vertex final : public Container {
public:
    std::vector<Edge> edges;
    Color color;
    Point last_mouse;
    bool holding;

    Vertex(std::string id);
    ~Vertex() override;
    inline Point get_center() { return rect.as_point() + rect.as_size() / 2.f; }
    void on_draw(Container* parent) override;
    void on_mouse_move(Container* parent, Point pos, Point dp, bool holding) override;
    void on_mouse_down(Container* parent, Point pos, uint8_t index, bool down) override;
    bool has_mouse_collision(Container* parent, Point pos) override;
};
