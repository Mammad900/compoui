#pragma once

#include "cui.hpp"
#include "value/value.hpp"
#include <memory>

namespace CUI::Layout {
    // Offsets the position of its child, without affecting the size.
    class Offset : public Element {
    public:
        Val<Vec2> offset;
        ElementPtr child;

        Offset(Val<Vec2> offset, ElementPtr child)
            : offset(offset), child(child) {}

        Vec2 naturalSize(Vec2 availableSpace) override {
            return child->naturalSize(availableSpace);
        }

        void draw(CUI::Rect rect) override {
            Element::draw(rect);

            child->draw({rect.pos + offset.get(), rect.size});
        }
    };
}

namespace CUI::Factories {
    /// Offsets the position of its child, without affecting the size.
    /// @param offset The position of the child relative to the parent.
    inline std::shared_ptr<Layout::Offset> Offset(Val<Vec2> offset, ElementPtr child) {
        return std::make_shared<Layout::Offset>(offset, child);
    }
}