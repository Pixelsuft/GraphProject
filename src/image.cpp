#include "image.hpp"
#include "render.hpp"

Image::Image(std::string id) : Container(id) { stretch = false; }

Image::~Image() { tex.destroy(); }

void Image::on_draw(Container* parent) {
    if (stretch) {
        ren->draw_texture_sized(rect, tex);
    } else {
        Point pos = rect.as_point() + (rect.as_size() - tex.size) / 2.f;
        ren->draw_texture(pos, tex);
    }
}

Image* Image::set_texture(Texture texture) {
    tex = texture;
    return this;
}
