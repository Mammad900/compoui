#pragma once

#include "../../cui.hpp"
#include <cmath>
#include <memory>
#include <vector>

namespace CUI::Layout {
    // Positions elements in a straight vertical/horizontal line
    class List : public Element {
      public:
        std::vector<std::shared_ptr<Element>> children;
        float gap;
        Orientation orientation;

        List(std::vector<std::shared_ptr<Element>> children, float gap = 0, Orientation orientation = vertical) 
            : children(children), gap(gap), orientation(orientation) {}

        Vec2 naturalSize(Vec2 max) override {
            if(orientation == vertical) { 
                float h = 0, w = 0;
                for (auto &child : children) {
                    Vec2 size = child->naturalSize({max.x, INFINITY});
                    h += size.y + gap;
                    w = std::max(w, size.x);
                }
                return {w, h - gap}; // because there's n-1 gaps
            } else {
                float h = 0, w = 0;
                for (auto &child : children) {
                    Vec2 size = child->naturalSize({INFINITY, max.y});
                    w += size.x + gap;
                    h = std::max(h, size.y);
                }
                return {w - gap, h}; // n-1 gaps
            }
        }

        void draw(ContextPtr ctx, Rect rect) override {
            Element::draw(ctx, rect);

            if(orientation == vertical) {
                float y = 0;
                for (auto &child : children) {
                    float height = child->naturalSize({INFINITY, rect.size.x}).y;
                    Rect childRect{
                        Vec2{0, y} + rect.pos, 
                        {rect.size.x, height}
                    };
                    child->draw(ctx, childRect);
                    y += height + gap;
                }
            } else {
                float x = 0;
                for (auto &child : children) {
                    float width = child->naturalSize({rect.size.y, INFINITY}).x;
                    Rect childRect{
                        Vec2{ x, 0 } + rect.pos,
                        {width, rect.size.y}
                    };
                    child->draw(ctx, childRect);
                    x += width + gap;
                }
            }
        }
    };
}

namespace CUI::Factories {
    inline std::shared_ptr<Layout::List> List(float gap, Orientation orientation, std::vector<std::shared_ptr<Element>> children) {
        return std::make_shared<Layout::List>(children, gap, orientation);
    }
    inline std::shared_ptr<Layout::List> List(Orientation orientation, std::vector<std::shared_ptr<Element>> children) {
        return std::make_shared<Layout::List>(children, 0, orientation);
    }
    inline std::shared_ptr<Layout::List> List(float gap, std::vector<std::shared_ptr<Element>> children) {
        return std::make_shared<Layout::List>(children, gap);
    }
    inline std::shared_ptr<Layout::List> List(std::vector<std::shared_ptr<Element>> children) {
        return std::make_shared<Layout::List>(children, 0);
    }
}