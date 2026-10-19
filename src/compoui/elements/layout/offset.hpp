#pragma once

#include "cui.hpp"
#include <memory>

namespace CUI::Layout {
    // Offsets the position of its child, without affecting the size.
    class Offset : public Element {
    public:
        Vec2 offset;
        ElementPtr child;

        Offset(Vec2 offset, ElementPtr child)
            : offset(offset), child(child) {}

        Vec2 naturalSize(Vec2 availableSpace) override {
            return child->naturalSize(availableSpace);
        }

        void draw(ContextPtr ctx, CUI::Rect rect) override {
            child->draw(ctx, {rect.pos + offset, rect.size});
        }
    };
}

namespace CUI::Factories {
    /// Offsets the position of its child, without affecting the size.
    /// @param offset The position of the child relative to the parent.
    inline std::shared_ptr<Layout::Offset> Offset(Vec2 offset, ElementPtr child) {
        return std::make_shared<Layout::Offset>(offset, child);
    }
}