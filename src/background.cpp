#include "background.hpp"
#include "frame.hpp"
#include "render.hpp"
#include <cmath>

Background::Background(std::string id) : Container(id) {}

void Background::on_draw(Container* parent) {
    Point offset = reinterpret_cast<Frame*>(parent)->inner_offset;
    offset.x = std::fmodf(offset.x, 40.f);
    offset.y = std::fmodf(offset.y, 40.f);
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
    if (holding)
        reinterpret_cast<Frame*>(parent)->inner_offset += dp;
}

bool Background::has_mouse_collision(Container* parent, Point pos) {
    return true;
}
