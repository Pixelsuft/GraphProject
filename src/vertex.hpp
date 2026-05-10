#pragma once
#include "container.hpp"
#include "color.hpp"

class Vertex final : public Container {
public:
    Color color;

    Vertex(std::string id);
    ~Vertex() override;
    void on_draw(Container* parent) override;
    void on_mouse_move(Container* parent, Point pos, Point dp, bool holding) override;
    bool has_mouse_collision(Container* parent, Point pos) override;
};
