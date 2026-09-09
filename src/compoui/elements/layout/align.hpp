#pragma once

#include "../../cui.hpp"
#include <memory>

namespace CUI::Layout {
    class Align : public Element {
      public:
        enum class Mode { start, center, end, fill };
        // static std::string modeToString(Mode mode) {
        //     switch (mode) {
        //         case start: return "start";
        //         case center: return "center";
        //         case end: return "end";
        //         case fill: return "fill";
        //         default: return "unknown";
        //     }
        // }

        std::shared_ptr<Element> child;
        Mode horizontal, vertical;

        Align(std::shared_ptr<Element> child, Mode horizontal, Mode vertical)
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

            // if(isDebugTarget()) {
            //     ctx->drawRect(rect, DebugColors::padding);
            //     ctx->drawRect(childRect, DebugColors::rect);
            // }
        }

        // DebugInfo getDebugInfo() override {
        //     return {
        //         .name= "Layout.Align",
        //         .summary= std::format("Horizontal={}, Vertical={}", modeToString(horizontal), modeToString(vertical)),
        //         .children= {{child, ""}}
        //     };
        // }
    };
}

namespace CUI::Factories {
    inline std::shared_ptr<Layout::Align> Align(Layout::Align::Mode horizontal, Layout::Align::Mode vertical, std::shared_ptr<Element> child) {
        return std::make_shared<Layout::Align>(child, horizontal, vertical);
    }
    inline std::shared_ptr<Layout::Align> Align(Layout::Align::Mode both, std::shared_ptr<Element> child) {
        return std::make_shared<Layout::Align>(child, both, both);
    }
    using AlignMode = Layout::Align::Mode;
}