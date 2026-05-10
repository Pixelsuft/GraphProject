#pragma once
#include "container.hpp"

class Background final : public Container {
public:
    Background(std::string id);
    void on_draw(Container* parent) override;
};
