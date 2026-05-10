#pragma once
#include "container.hpp"

class Background final : public Container {
public:
    Background(std::string id);
    void on_draw(Container* parent) override;
    void on_mouse_move(Container* parent, Point pos, Point dp, bool holding) override;
    bool has_mouse_collision(Container* parent, Point pos) override;
};
