#pragma once
#include "color.hpp"
#include "container.hpp"
#include "fade.hpp"

class Button final : public Container {
public:
    ColorFade fade;
    Color bg_color;
    Color border_color;
    Color hover_color;
    Color down_color;
    bool real_hovered;

    Button(std::string id);
    ~Button();
    void on_draw(Container* parent) override;
    void on_update(Container* parent) override;
    void on_mouse_enter(Container* parent, Point pos, bool entered) override;
    void on_mouse_down(Container* parent, Point pos, uint8_t index, bool down) override;
    void on_mouse_move(Container* parent, Point pos, Point dp, bool holding) override;
    void refresh_color();
};
