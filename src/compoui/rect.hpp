#pragma once

#include "vec2.hpp"

namespace CUI {
    // Represents a 2D axis aligned bounding box
    struct Rect {
        // Top left point
        Vec2 pos;
        // Size (difference of top left to bottom right)
        Vec2 size;

        // Returns area of the bounding box (product of size)
        float area() {
            return size.area();
        }

        // Returns bottom right point
        Vec2 max() {
            return pos + size;
        }

        // Returns center point (pos + size/2)
        Vec2 center() {
            return pos + size * 0.5f;
        }

        // Returns true if point is within this rect
        bool contains(Vec2 point) {
            Vec2 m = max();
            return  point.x >= pos.x &&
                    point.x < m.x &&
                    point.y >= pos.y &&
                    point.y < m.y;
        }

        Rect grow(Vec2 amount) {
            return {pos - amount, size + amount*2.f};
        }
        Rect grow(float amount) {
            return grow({amount, amount});
        }
    };
}
