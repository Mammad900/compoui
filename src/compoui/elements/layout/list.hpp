#pragma once

#include "../../cui.hpp"
#include <memory>
#include <vector>

namespace CUI::Layout {
    class List : public Element {
      public:
        std::vector<std::shared_ptr<Element>> children;
        float gap;

        List(std::vector<std::shared_ptr<Element>> children, float gap = 0) 
            : children(children), gap(gap) {}

        Vec2 naturalSize(Vec2 max) override {
            float h = 0, w = 0;
            for (auto &child : children) {
                Vec2 size = child->naturalSize({max.x, 0});
                h += size.y + gap;
                w = std::max(w, size.x);
            }
            
            return {max.x == 0 ? w : max.x, h - gap}; // because there's n-1 gaps
        }

        void draw(ContextPtr ctx, Rect rect) override {
            Element::draw(ctx, rect);

            // if(isDebugTarget())
            //     ctx->drawRect(rect, DebugColors::padding);

            float y = 0;
            for (auto &child : children) {
                float height = child->naturalSize({0, rect.size.x}).y;
                Rect childRect{
                    Vec2{0, y} + rect.pos, 
                    {rect.size.x, height}
                };
                child->draw(ctx, childRect);
                y += height + gap;

                // if(isDebugTarget())
                //     ctx->drawRect(childRect, DebugColors::rect);
            }
        }
        
        // DebugInfo getDebugInfo() override {
        //     return {
        //         .name= "Layout.List",
        //         .summary= std::format("{} children", children.size()),
        //         .children= mapVector<std::shared_ptr<Element>, std::pair<std::shared_ptr<Element>, std::string>>(children, [](std::shared_ptr<Element> &child, size_t) {
        //             return std::make_pair(child, std::string(""));
        //         })
        //     };
        // }
    };
}

namespace CUI::Factories {
    inline std::shared_ptr<Layout::List> List(float gap, std::vector<std::shared_ptr<Element>> children) {
        return std::make_shared<Layout::List>(children, gap);
    }
    inline std::shared_ptr<Layout::List> List(std::vector<std::shared_ptr<Element>> children) {
        return std::make_shared<Layout::List>(children, 0);
    }
}