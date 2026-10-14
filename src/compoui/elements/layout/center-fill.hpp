#pragma once

#include "../../cui.hpp"
#include <algorithm>
#include <memory>

namespace CUI::Layout {
    // Places two children at the start and end, and fills the rest with the center child.
    class CenterFill : public Element {
    public:
        std::shared_ptr<Element> start, center, end;
        Orientation orientation;

        CenterFill(std::shared_ptr<Element> start, std::shared_ptr<Element> center, std::shared_ptr<Element> end, Orientation orientation)
            : start(start), center(center), end(end), orientation(orientation) {}

        Vec2 naturalSize(Vec2 availableSpace) override {
            float h = 0, w = 0;
            for (auto &el : {start, center, end}) {
                if(el) {
                    Vec2 size = el->naturalSize(availableSpace);
                    if(orientation == vertical) {
                        h+= size.y;
                        w = std::max(w, size.x);
                    } else {
                        w+= size.x;
                        h = std::max(h, size.y);
                    }
                }
            }
            return {w, h};
        }

        void draw(ContextPtr ctx, CUI::Rect rect) override {
            if(orientation == vertical) {
                float startH = start ? start->naturalSize(rect.size).y : 0;
                float endH = end ? end->naturalSize(rect.size).y : 0;

                if(start) start->draw(ctx, {
                    rect.pos, 
                    {rect.size.x, startH}
                });
                if(center) center->draw(ctx, {
                    rect.pos + Vec2{0, startH}, 
                    rect.size - Vec2{0, startH + endH}
                });
                if(end) end->draw(ctx, {
                    rect.pos + Vec2{0, rect.size.y - endH}, 
                    {rect.size.x, endH}
                });
            }
            else {
                float startW = start ? start->naturalSize(rect.size).x : 0;
                float endW = end ? end->naturalSize(rect.size).x : 0;

                if(start) start->draw(ctx, {
                    rect.pos, 
                    {startW, rect.size.y}
                });
                if(center) center->draw(ctx, {
                    rect.pos + Vec2{startW, 0}, 
                    rect.size - Vec2{startW + endW, 0}
                });
                if(end) end->draw(ctx, {
                    rect.pos + Vec2{rect.size.x - endW, 0}, 
                    {endW, rect.size.y}
                });
            }
        }
    };
}

namespace CUI::Factories {
    inline std::shared_ptr<Layout::CenterFill> CenterFill(
        Orientation orientation,
        std::shared_ptr<Element> start,
        std::shared_ptr<Element> center,
        std::shared_ptr<Element> end
    ) {
        return std::make_shared<Layout::CenterFill>(start, center, end, orientation);
    }
}