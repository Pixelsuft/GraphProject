#include "label.hpp"
#include "text.hpp"
#include <SDL3_ttf/SDL_textengine.h>

Label::Label(std::string id, void* font) : Container(id), center(false) {
    t = text->create_text(reinterpret_cast<TTF_Font*>(font));
}

Label::~Label() { text->destroy_text(t); }

void Label::on_draw(Container* parent) { text->draw(t, rect.as_point(), center); }

Label* Label::set_color(Color color) {
    text->set_color(t, color);
    return this;
}

Label* Label::set_text(const char* data) {
    text->set_text(t, data);
    return this;
}
