#pragma once

#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>

namespace CUI {
    struct Vec2 {
        float x, y;

        Vec2(float x, float y) : x(x), y(y) {}

        Vec2(glm::ivec2 vec) : x(vec.x), y(vec.y) {}

        float area() {
            return x*y;
        }
        
        // Default constructor (zero vector)
        Vec2() : x(0), y(0) {}

        // Element‑wise addition
        Vec2 operator+(const Vec2& b) const {
            return Vec2(x + b.x, y + b.y);
        }

        // Element‑wise subtraction
        Vec2 operator-(const Vec2& b) const {
            return Vec2(x - b.x, y - b.y);
        }

        // Scalar multiplication (vector * scalar)
        Vec2 operator*(float s) const {
            return Vec2(x * s, y * s);
        }

        Vec2 operator*(const Vec2 &b) const {
            return Vec2(x * b.x, y * b.y);
        }

        Vec2 operator/(const Vec2 &b) const {
            return Vec2(x / b.x, y / b.y);
        }

        // // Dot product
        // float dot(const Vec2& b) const {
        //     return x * b.x + y * b.y;
        // }

        // // Length (magnitude) of the vector
        // float length() const {
        //     return std::sqrt(dot(*this));
        // }

        // // Normalized unit vector (returns zero vector if length == 0)
        // Vec2 normalized() const {
        //     float len = length();
        //     return len > 0.0f ? (*this) * (1.0f / len) : Vec2(0, 0);
        // }

        // // Cross product (z‑component for 2D vectors)
        // float cross(const Vec2& b) const {
        //     return x * b.y - y * b.x;
        // }

        // In-place addition
        Vec2& operator+=(const Vec2& b) {
            x += b.x; y += b.y;
            return *this;
        }

        // In-place subtraction
        Vec2& operator-=(const Vec2& b) {
            x -= b.x; y -= b.y;
            return *this;
        }

        // In-place scalar multiplication
        Vec2& operator*=(float s) {
            x *= s; y *= s;
            return *this;
        }

        bool operator==(const Vec2& b) const {
            return x == b.x && y == b.y;
        }

        Vec2 operator-() const {
            return {-x, -y};
        }

        static Vec2 min(const Vec2& a, const Vec2& b) {
            return {std::min(a.x, b.x), std::min(a.y, b.y)};
        }
        static Vec2 max(const Vec2& a, const Vec2& b) {
            return {std::max(a.x, b.x), std::max(a.y, b.y)};
        }
    };
}
