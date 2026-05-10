#include "frame.hpp"
#include "render.hpp"
#include <algorithm>
#include <cstdlib>

Frame::Frame(std::string id) : Container(id) {
    c_hovered = nullptr;
    scale = 1.f;
    left_down = false;
}

Frame::~Frame() {
    for (Container* c : child)
        delete c;
    child.clear();
}

void Frame::add_child_real(Container* cont) { child.push_back(cont); }

void Frame::on_resize(Container* parent) {
    Container::on_resize(parent);
    for (Container* c : child)
        c->on_resize(this);
}

void Frame::on_update(Container* parent) {
    for (Container* c : child)
        c->on_update(this);
}

void Frame::on_draw(Container* parent) {
    Point prev_offset = ren->offset;
    float prev_scale = ren->scale;
    // TODO: fix scaling when needed
    ren->offset = (prev_offset + rect.as_point()) * prev_scale / scale + inner_offset;
    ren->scale = prev_scale * scale;
    ren->set_scale_enabled(true);
    for (Container* c : child)
        c->on_draw(this);
    ren->offset = prev_offset;
    ren->scale = prev_scale;
    ren->set_scale_enabled(true);
}

Container* Frame::find_focused(Point pos) {
    Container* hovered = nullptr;
    for (auto it = child.rbegin(); it != child.rend(); ++it) {
        Container* c = *it;
        if (c->visible && c->enabled && c->has_mouse_collision(this, pos)) {
            hovered = c;
            break;
        }
    }
    return hovered;
}

void Frame::on_mouse_move(Container* parent, Point pos, Point dp, bool holding) {
    // TODO: fix offset/scale
    pos = (pos - rect.as_point() - inner_offset) / scale;
    dp /= scale;
    if (!left_down) {
        Container* new_hover = find_focused(pos);
        if (new_hover != c_hovered) {
            if (c_hovered)
                c_hovered->on_mouse_enter(this, pos, false);
            c_hovered = new_hover;
            if (c_hovered)
                c_hovered->on_mouse_enter(this, pos, true);
        }
    }
    if (c_hovered)
        c_hovered->on_mouse_move(this, pos, dp, left_down);
}

void Frame::on_mouse_enter(Container* parent, Point pos, bool entered) {
    if (parent == nullptr)
        on_mouse_move(nullptr, pos, Point(-1, -1), false); // Is this right?
}

void Frame::on_mouse_down(Container* parent, Point pos, uint8_t index, bool down) {
    // Only supporting left mouse button for now
    if (index == SDL_BUTTON_LEFT) {
        left_down = down;
        if (c_hovered != nullptr) {
            // TODO: fix offset/scale
            Point new_pos = (pos - rect.as_point() - inner_offset) / scale;
            c_hovered->on_mouse_down(this, new_pos, index, down);
        }
        if (!down)
            on_mouse_move(parent, pos, Point(), down);
    }
}

Container* Frame::child_by_id(std::string id) {
    auto it =
        std::find_if(child.begin(), child.end(), [&id](const Container* c) { return c->id == id; });
    if (it == child.end())
        std::abort();
    return (it != child.end()) ? *it : nullptr;
}

void Frame::remove_child(Container* cont) {
    if (c_hovered == cont)
        c_hovered = nullptr;
    delete cont;
    auto it = std::find(child.begin(), child.end(), cont);
    if (it == child.end())
        std::abort();
    child.erase(it);
}
