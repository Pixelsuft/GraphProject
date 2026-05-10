#pragma once
#include "container.hpp"
#include <functional>

class Background final : public Container {
    std::function<void(Background*, Container*, Point)> onDown;

public:
    Background(std::string id);
    void on_draw(Container* parent) override;
    void on_mouse_move(Container* parent, Point pos, Point dp, bool holding) override;
    bool has_mouse_collision(Container* parent, Point pos) override;
    void on_mouse_down(Container* parent, Point pos, uint8_t index, bool down) override;
    Background* set_down_handler(std::function<void(Background*, Container*, Point)> handler);
};
