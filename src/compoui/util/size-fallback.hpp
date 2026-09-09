#pragma once

#include "../rect.hpp"

namespace CUI {
    inline Rect sizeFallback(const Rect &a, const Rect &b) {
        return {a.pos, {
            a.size.x == 0 ? b.size.x : a.size.x,
            a.size.y == 0 ? b.size.y : a.size.y
        }};
    }

    inline Rect sizeFallback(const Rect &a, const Vec2 &b) {
        return {a.pos, {
            a.size.x == 0 ? b.x : a.size.x,
            a.size.y == 0 ? b.y : a.size.y
        }};
    }
}