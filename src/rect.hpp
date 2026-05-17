#pragma once
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_stdinc.h>

struct Point {
    float x;
    float y;

    Point() noexcept : x(0.f), y(0.f) {}
    Point(float x, float y) noexcept : x(x), y(y) {}

    bool operator==(const Point& other) const noexcept { return x == other.x && y == other.y; }
    bool operator!=(const Point& other) const noexcept { return !(*this == other); }

    inline bool empty() const noexcept { return x == 0.f && y == 0.f; }
    inline float get_angle(Point base) noexcept { return SDL_atan2f(y - base.y, x - base.x); }
    inline float get_length() noexcept { return SDL_sqrtf(x * x + y * y); }
    inline Point rotate_point(Point pivot, float angle) {
        float s = SDL_sinf(angle);
        float c = SDL_cosf(angle);
        Point p = *this - pivot;
        return {p.x * c - p.y * s + pivot.x, p.x * s + p.y * c + pivot.y};
    }

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

    Point operator*(float scalar) const { return {x * scalar, y * scalar}; }

    Point operator/(float scalar) const { return {x / scalar, y / scalar}; }

    Point& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Point& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }
};

struct Rect {
    float x;
    float y;
    float w;
    float h;

    Rect() noexcept : x(0.f), y(0.f), w(0.f), h(0.f) {}
    Rect(float x, float y, float w, float h) noexcept : x(x), y(y), w(w), h(h) {}

    bool operator==(const Rect& other) const noexcept {
        return x == other.x && y == other.y && w == other.w && h == other.h;
    }
    bool operator!=(const Rect& other) const noexcept { return !(*this == other); }

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

    inline Point as_point() { return Point(x, y); }
    inline Point as_size() { return Point(w, h); }

    inline SDL_FRect* as_frect() { return reinterpret_cast<SDL_FRect*>(this); }
};
