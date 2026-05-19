#pragma once
#include "rect.hpp"

void construct_ui();
void destroy_ui();
void kbd_ui(char key);
void draw_ui();
void scale_ui(Point pos, float mul);
void update_ui_font_scale();
