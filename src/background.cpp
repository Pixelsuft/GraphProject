#include "background.hpp"
#include "frame.hpp"
#include "render.hpp"
#include "vertex.hpp"

Background::Background(std::string id) : Container(id) {}

void Background::on_draw(Container* parent) {
    Point offset = reinterpret_cast<Frame*>(parent)->inner_offset;
    offset.x = SDL_fmodf(offset.x, 40.f);
    offset.y = SDL_fmodf(offset.y, 40.f);
    offset -= reinterpret_cast<Frame*>(parent)->inner_offset;
    offset += rect.as_point();
    for (float x = -20.f; x < rect.w + 40.f; x += 40.f) {
        for (float y = -20.f; y < rect.h + 40.f; y += 40.f) {
            ren->fill_rect({x + offset.x, y + offset.y, 1.f, 1.f}, Color(1.f, 1.f, 1.f));
        }
    }
}

void Background::on_mouse_move(Container* parent, Point pos, Point dp, bool holding) {
    // Let's hardcode this
    if (holding && vertex_mode != 1)
        reinterpret_cast<Frame*>(parent)->inner_offset += dp;
}

void Background::on_mouse_down(Container* parent, Point pos, uint8_t index, bool down) {
    if (onDown && down)
        onDown(this, parent, pos);
}

bool Background::has_mouse_collision(Container* parent, Point pos) { return true; }

Background*
Background::set_down_handler(std::function<void(Background*, Container*, Point)> handler) {
    onDown = std::move(handler);
    return this;
}
