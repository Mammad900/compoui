#pragma once

#include "../../cui.hpp"
#include <memory>

namespace CUI::Layout {
    // Positions the child's edges slightly offset inwards relative to its own.
    // Natural size the child's natural size plus twice the padding.
    class Padding : public Element {
      public:
        std::shared_ptr<Element> child;
        // The amount of padding at the start and end on each axis
        Vec2 padding;

        Padding(std::shared_ptr<Element> child, Vec2 padding)
            : child(child), padding(padding) {}

        Vec2 naturalSize(Vec2 max) override {
            return child->naturalSize({
                max.x == 0 ? 0 : max.x - padding.x*2.f,
                max.y == 0 ? 0 : max.y - padding.y*2.f
            }) + padding*2.f;
        }

        void draw(ContextPtr ctx, Rect rect) override {
            Element::draw(ctx, rect);

            Rect childRect {rect.pos + padding, rect.size - padding*2.f};

            child->draw(ctx, childRect);
        }
    };
}

namespace CUI::Factories {
    inline std::shared_ptr<Layout::Padding> Padding(Vec2 padding, std::shared_ptr<Element> child) {
        return std::make_shared<Layout::Padding>(child, padding);
    }
    inline std::shared_ptr<Layout::Padding> Padding(float padding, std::shared_ptr<Element> child) {
        return std::make_shared<Layout::Padding>(child, Vec2{padding, padding});
    }
}