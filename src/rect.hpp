#pragma once
#include <SDL3/SDL_rect.h>

struct Point {
    float x;
    float y;

    Point() : x(0.f), y(0.f) {}
    Point(float x, float y) : x(x), y(y) {}

    Point operator+(const Point& other) const { return {x + other.x, y + other.y}; }
    Point operator-(const Point& other) const { return {x - other.x, y - other.y}; }

    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Point& operator-=(const Point& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};

struct Rect {
    float x;
    float y;
    float w;
    float h;

    Rect() : x(0.f), y(0.f), w(0.f), h(0.f) {}
    Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

    Rect operator+(const Point& p) const { return {x + p.x, y + p.y, w, h}; }
    Rect operator-(const Point& p) const { return {x - p.x, y - p.y, w, h}; }

    Rect& operator+=(const Point& p) {
        x += p.x;
        y += p.y;
        return *this;
    }
    Rect& operator-=(const Point& p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    Rect operator+(const Rect& other) const {
        return {x + other.x, y + other.y, w + other.w, h + other.h};
    }

    Rect operator-(const Rect& other) const {
        return {x - other.x, y - other.y, w - other.w, h - other.h};
    }

    Rect& operator+=(const Rect& other) {
        x += other.x;
        y += other.y;
        w += other.w;
        h += other.h;
        return *this;
    }

    Rect& operator-=(const Rect& other) {
        x -= other.x;
        y -= other.y;
        w -= other.w;
        h -= other.h;
        return *this;
    }

    inline SDL_FRect* as_frect() { return reinterpret_cast<SDL_FRect*>(this); }
};
