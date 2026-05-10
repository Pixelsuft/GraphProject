#pragma once
#include "color.hpp"
#include "container.hpp"
#include "fade.hpp"
#include <functional>

class Button final : public Container {
    std::function<void(Button*, Container*)> onClick;

public:
    ColorFade fade;
    Color bg_color;
    Color border_color;
    Color hover_color;
    Color down_color;
    Container* child;
    bool real_hovered;

    Button(std::string id);
    ~Button() override;
    void on_draw(Container* parent) override;
    void on_update(Container* parent) override;
    void on_resize(Container* parent) override;
    void on_mouse_enter(Container* parent, Point pos, bool entered) override;
    void on_mouse_down(Container* parent, Point pos, uint8_t index, bool down) override;
    void on_mouse_move(Container* parent, Point pos, Point dp, bool holding) override;
    Button* set_click_handler(std::function<void(Button*, Container*)> handler);
    Button* set_child(Container* cont);
    Container* set_rect(Rect new_rect) override;
    void refresh_color();
};
