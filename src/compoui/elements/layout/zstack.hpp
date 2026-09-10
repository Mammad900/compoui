#pragma once

#include "../../cui.hpp"
#include <memory>
#include <vector>

namespace CUI::Layout {
    // Positions children in the same rect, on top of each other.
    // Natural size is that of one of the children, which must be specified.
    class ZStack : public Element {
      public:
        std::vector<std::shared_ptr<Element>> children;
        // The index of the child that determines natural size
        uint layoutChild;

        ZStack(std::vector<std::shared_ptr<Element>> children, uint layoutChild) 
            : children(children), layoutChild(layoutChild) {}

        Vec2 naturalSize(Vec2 max) override {
            return children[layoutChild]->naturalSize(max);
        }

        void draw(ContextPtr ctx, Rect rect) override {
            Element::draw(ctx, rect);

            for (auto &&child : children)
                child->draw(ctx, rect);
        }
    };
}

namespace CUI::Factories {
    inline std::shared_ptr<CUI::Layout::ZStack> ZStack(std::vector<std::shared_ptr<Element>> children, uint layoutChild) {
        return std::make_shared<CUI::Layout::ZStack>(children, layoutChild);
    }
}