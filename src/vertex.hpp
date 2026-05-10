#pragma once
#include "container.hpp"
#include "color.hpp"

// hardcoding is fun :)
extern int vertex_mode;

class Vertex final : public Container {
public:
    Color color;

    Vertex(std::string id);
    ~Vertex() override;
    void on_draw(Container* parent) override;
    void on_mouse_move(Container* parent, Point pos, Point dp, bool holding) override;
    void on_mouse_down(Container* parent, Point pos, uint8_t index, bool down) override;
    bool has_mouse_collision(Container* parent, Point pos) override;
};
