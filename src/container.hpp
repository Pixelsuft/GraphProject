#pragma once
#include "rect.hpp"
#include <string>
#include <functional>

class Container {
    std::function<void(Container*, Container*)> onResize;

public:
    std::string id;
    Rect rect;
    bool visible;
    bool enabled;

    Container(std::string id);
    virtual ~Container();
    virtual void on_resize(Container* parent);
    virtual void on_update(Container* parent);
    virtual void on_draw(Container* parent);
    virtual void on_mouse_move(Container* parent, Point pos, Point dp, bool holding);
    virtual void on_mouse_down(Container* parent, Point pos, uint8_t index, bool down);
    virtual void on_mouse_enter(Container* parent, Point pos, bool entered);
    virtual bool has_mouse_collision(Container* parent, Point pos);
    virtual Container* set_rect(Rect new_rect);
    Container* set_resize_handler(std::function<void(Container*, Container*)> handler);
};
