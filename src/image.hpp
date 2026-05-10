#pragma once
#include "container.hpp"
#include "texture.hpp"

class Image final : public Container {
public:
    Texture tex;
    bool stretch;

    Image(std::string id);
    ~Image();

    void on_draw(Container* parent) override;
    Image* set_texture(Texture texture);
};
