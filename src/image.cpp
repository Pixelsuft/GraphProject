#include "image.hpp"
#include "render.hpp"

Image::Image(std::string id, bool stretch) : Container(id), stretch(stretch), rot(0.0) {}

Image::~Image() { tex.destroy(); }

void Image::on_draw(Container* parent) {
    if (!visible)
        return;
    if (stretch) {
        if (rot == 0.0)
            ren->draw_texture_sized(rect, tex);
        else
            ren->draw_texture_sized_rotated(rect, rot, tex);
    } else {
        Point pos = rect.as_point() + (rect.as_size() - tex.size) / 2.f;
        // if (rot == 0.0)
        ren->draw_texture(pos, tex);
        // else
        // ren->draw_texture_rotated(pos, rot, tex);
    }
}

Image* Image::set_texture(Texture texture) {
    tex = texture;
    return this;
}
