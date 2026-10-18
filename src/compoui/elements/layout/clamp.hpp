#pragma once

#include "../../cui.hpp"
#include "../../vec2.hpp"
#include <memory>

namespace CUI::Layout {
    // Clamps the child's natural size between two sizes.
    // Has no effect on the draw command passed down from the parent.
    // Use a parent that uses natural size, such as Align or List.
    // Cannot clamp to more than the available space. In other words, available space can override minimum.
    class Clamp : public Element {
    public:
        Vec2 min, max;
        std::shared_ptr<Element> child;

        Clamp(Vec2 min, Vec2 max, std::shared_ptr<Element> child) 
            : min(min), max(max), child(child) {}
        
        Vec2 naturalSize(Vec2 availableSpace) override {
            if(min == max) { // optimization: bypass naturalSize
                return Vec2::min(min, availableSpace);
            }

            availableSpace = Vec2::min(availableSpace, max);
            Vec2 natural = child->naturalSize(availableSpace);
            return Vec2::max(Vec2::min(natural, max), min);
        }

        void draw(ContextPtr ctx, CUI::Rect rect) override {
            child->draw(ctx, rect);
        }
    };
}

namespace CUI::Factories {
    // Clamps the child's natural size between the given sizes.
    inline std::shared_ptr<Layout::Clamp> Clamp(Vec2 min, Vec2 max, std::shared_ptr<Element> child) {
        return std::make_shared<Layout::Clamp>(min, max, child);
    }

    // Prevents the child's natural size from being bigger than the given size.
    // Shorthand for Clamp({0,0}, max, child)
    inline std::shared_ptr<Layout::Clamp> Clamp(Vec2 max, std::shared_ptr<Element> child) {
        return std::make_shared<Layout::Clamp>(Vec2{0,0}, max, child);
    }

    // Prevents the child's natural size from being smaller than the given size.
    // Shorthand for Clamp(min, {∞,∞}, child)
    inline std::shared_ptr<Layout::Clamp> ClampMin(Vec2 min, std::shared_ptr<Element> child) {
        return std::make_shared<Layout::Clamp>(min, Vec2{INFINITY, INFINITY}, child);
    }

    // Fixes the child's natural size to the given size
    // Shorthand for Clamp(size, size, child)
    inline std::shared_ptr<Layout::Clamp> ClampFixed(Vec2 size, std::shared_ptr<Element> child) {
        return std::make_shared<Layout::Clamp>(size, size, child);
    }
}