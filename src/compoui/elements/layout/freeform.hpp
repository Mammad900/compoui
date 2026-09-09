#pragma once
#include "../../cui.hpp"
#include "../../util/size-fallback.hpp"
#include "../../rect.hpp"
#include <vector>

namespace CUI::Layout {
    class Freeform : public Element {
      public:
        struct Child {
            Rect rect;
            std::shared_ptr<Element> element;
        };

        Freeform(std::vector<Child> children) : children(children) {}
        
        std::vector<Child> children;
        void draw(ContextPtr ctx, Rect rect) override {
            Element::draw(ctx, rect);

            for (Child &child : children) {
                Vec2 naturalSize = child.element->naturalSize({0,0});
                Rect childRect = sizeFallback(child.rect, naturalSize);
                if(childRect.size.x == 0 || childRect.size.y == 0) continue;

                if(childRect.pos.x < 0)
                    childRect.pos.x += rect.size.x - childRect.size.x + 1;
                if(childRect.pos.y < 0)
                    childRect.pos.y += rect.size.y - childRect.size.y + 1;

                childRect.pos += rect.pos;

                child.element->draw(ctx, childRect);
                // if(isDebugTarget())
                //     ctx->drawRect(childRect, DebugColors::rect);
            }
        }
    };
}

namespace CUI::Factories {
    inline std::shared_ptr<CUI::Layout::Freeform> Freeform(std::vector<CUI::Layout::Freeform::Child> children) {
        return std::make_shared<CUI::Layout::Freeform>(children);
    }
}