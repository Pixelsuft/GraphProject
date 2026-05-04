#pragma once

struct Color {
    float r;
    float g;
    float b;
    float a;

    Color() : r(0.f), g(0.f), b(0.f), a(1.f) {}
    Color(float r, float g, float b) : r(r), g(g), b(b), a(1.f) {}
    Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
};
