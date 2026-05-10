#pragma once
#include "container.hpp"
#include <type_traits>
#include <vector>

class Frame final : public Container {
    void add_child_real(Container* cont);

public:
    std::vector<Container*> child;
    Container* c_hovered;
    Point inner_offset;
    float scale;
    bool left_down;

    Frame(std::string id);
    ~Frame() override;
    template <typename T> T* add_child(T* cont) {
        static_assert(std::is_base_of<Container, T>::value, "T must be a descendant of Container");
        add_child_real(cont);
        return cont;
    }
    Container* child_by_id(std::string id);
    void on_resize(Container* parent) override;
    void on_update(Container* parent) override;
    void on_draw(Container* parent) override;
    void on_mouse_move(Container* parent, Point pos, Point dp, bool holding) override;
    void on_mouse_enter(Container* parent, Point pos, bool entered) override;
    void on_mouse_down(Container* parent, Point pos, uint8_t index, bool down) override;
};

extern Frame* root;
