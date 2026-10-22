#pragma once

#include "../../cui.hpp"
#include <memory>
#include <vector>

namespace CUI::Layout {
    // Positions children in the same rect, on top of each other.
    // Natural size is that of one of the children, which must be specified.
    class ZStack : public Element {
      public:
        std::vector<ElementPtr> children;
        // The index of the child that determines natural size
        int layoutChild;

        ZStack(std::vector<ElementPtr> children, int layoutChild) 
            : children(children), layoutChild(layoutChild) {}

        Vec2 naturalSize(Vec2 availableSpace) override {
            int layoutChildNow = layoutChild < 0 ? children.size() + layoutChild : layoutChild;
            return children[layoutChildNow]->naturalSize(availableSpace);
        }

        void draw(Rect rect) override {
            Element::draw(rect);

            for (auto &&child : children)
                child->draw(rect);
        }
    };
}

namespace CUI::Factories {
    /// Positions children in the same rect, on top of each other.
    /// Natural size is that of one of the children, which must be specified.
    /// @param layoutChild The index of the element which determines natural size. 0 means first, 1 means second, etc. -1 means last, -2 means second last, etc.
    inline std::shared_ptr<CUI::Layout::ZStack> ZStack(std::vector<ElementPtr> children, int layoutChild) {
        return std::make_shared<CUI::Layout::ZStack>(children, layoutChild);
    }
}