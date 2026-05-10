#include "background.hpp"
#include "render.hpp"

Background::Background(std::string id) : Container(id) {}

void Background::on_draw(Container* parent) {
    for (float x = rect.x + 20.f; x < rect.x + rect.w + 40.f; x += 40.f) {
        for (float y = rect.y + 20.f; y < rect.y + rect.h + 40.f; y += 40.f) {
            ren->fill_rect({ x, y, 1.f, 1.f }, Color(1.f, 1.f, 1.f));
        }
    }
}
