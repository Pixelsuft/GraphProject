#pragma once
#include "container.hpp"
#include "color.hpp"

class Label final : public Container {
    void* t;
public:
    bool center;

    Label(std::string id, void* font);
    ~Label() override;
    void on_draw(Container* parent) override;
    Label* set_color(Color color);
    Label* set_text(const char* data);
};
