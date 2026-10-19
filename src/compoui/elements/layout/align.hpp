#pragma once

#include "../../cui.hpp"
#include <memory>

namespace CUI::Layout {
    // Positions an element at the start/center/end of itself, at its natural size.
    // Its natural size is the same as its child's.
    class Align : public Element {
      public:
        enum class Mode {
            // Child is anchored to left/top, and natural size.
            start, 
            // Child is centered, with equal empty space on the sides, and natural size.
            center, 
            // Child is anchored to right/bottom, and natural size.
            end,
            // Child fills the parent on this axis, so not natural size.
            fill
        };
        ElementPtr child;
        Mode horizontal, vertical;

        Align(ElementPtr child, Mode horizontal, Mode vertical)
            : child(child), horizontal(horizontal), vertical(vertical) {}

        Vec2 naturalSize(Vec2 max) override {
            return child->naturalSize(max);
        }

        void draw(ContextPtr ctx, Rect rect) override {
            Element::draw(ctx, rect);

            Rect childRect = rect;
            childRect.size = child->naturalSize(rect.size);

            switch (horizontal) {
                case Mode::start:
                    childRect.pos.x = 0;
                break;
                case Mode::center:
                    childRect.pos.x = (rect.size.x - childRect.size.x) / 2.f;
                break;
                case Mode::end:
                    childRect.pos.x = rect.size.x - childRect.size.x;
                break;
                case Mode::fill:
                    childRect.pos.x = 0;
                    childRect.size.x = rect.size.x;
                break;
            }

            switch (vertical) {
                case Mode::start:
                    childRect.pos.y = 0;
                break;
                case Mode::center:
                    childRect.pos.y = (rect.size.y - childRect.size.y) / 2.f;
                break;
                case Mode::end:
                    childRect.pos.y = rect.size.y - childRect.size.y;
                break;
                case Mode::fill:
                    childRect.pos.y = 0;
                    childRect.size.y = rect.size.y;
                break;
            }

            childRect.pos += rect.pos;

            child->draw(ctx, childRect);
        }
    };
}

namespace CUI::Factories {
    /// Positions an element at the start/center/end of itself, at its natural size.
    /// Its natural size is the same as its child's.
    /// @param horizontal How the child should be aligned horizontally
    /// @param vertical How the child should be aligned vertically
    /// @param child The element to align
    inline std::shared_ptr<Layout::Align> Align(Layout::Align::Mode horizontal, Layout::Align::Mode vertical, ElementPtr child) {
        return std::make_shared<Layout::Align>(child, horizontal, vertical);
    }

    /// Positions an element at the start/center/end of itself, at its natural size.
    /// Its natural size is the same as its child's.
    /// @param both How the child should be aligned, in both axes
    /// @param child The element to align
    inline std::shared_ptr<Layout::Align> Align(Layout::Align::Mode both, ElementPtr child) {
        return std::make_shared<Layout::Align>(child, both, both);
    }
    using AlignMode = Layout::Align::Mode;
}