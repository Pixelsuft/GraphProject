#pragma once
#include "container.hpp"
#include <vector>

class Frame final : public Container {
public:
    std::vector<Container*> child;
    Container* c_hovered;
    Point inner_offset;
    float scale;
    bool left_down;

    Frame(std::string id);
    ~Frame();
    void add_child(Container* cont);
    void on_resize(Container* parent) override;
    void on_update(Container* parent) override;
    void on_draw(Container* parent) override;
    void on_mouse_move(Container* parent, Point pos, Point dp, bool holding) override;
    void on_mouse_enter(Container* parent, Point pos, bool entered) override;
    void on_mouse_down(Container* parent, Point pos, uint8_t index, bool down) override;
};

extern Frame* ui;
