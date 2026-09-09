#pragma once

#include "../../cui.hpp"
#include <memory>
#include <vector>

namespace CUI::Layout {
    class ZStack : public Element {
      public:
        std::vector<std::shared_ptr<Element>> children;
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

            // if(isDebugTarget())
            //     ctx->drawRect(rect, DebugColors::rect);
        }

        // DebugInfo getDebugInfo() override {
        //     return {
        //         .name= "Layout.ZStack",
        //         .summary= std::format("{} children", children.size()),
        //         .children= mapVector<std::shared_ptr<Element>, std::pair<std::shared_ptr<Element>, std::string>>(children, [this](std::shared_ptr<Element> &child, size_t i) {
        //             return std::make_pair(
        //                 child,
        //                 i == layoutChild ? "Layout child" : ""
        //             );
        //         })
        //     };
        // }
    };
}

namespace CUI::Factories {
    inline std::shared_ptr<CUI::Layout::ZStack> ZStack(std::vector<std::shared_ptr<Element>> children, uint layoutChild) {
        return std::make_shared<CUI::Layout::ZStack>(children, layoutChild);
    }
}