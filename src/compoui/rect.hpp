#pragma once

#include "vec2.hpp"

namespace CUI {
    struct Rect {
        Vec2 pos;
        Vec2 size;

        float area() {
            return size.area();
        }

        Vec2 max() {
            return pos + size;
        }

        Vec2 center() {
            return pos + size * 0.5f;
        }
    };
}
