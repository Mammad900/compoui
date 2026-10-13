#pragma once
#include "../cui.hpp"
#include <memory>

namespace CUI::Misc {
    class Layer : public Element, public std::enable_shared_from_this<Layer> {
    public:
        std::shared_ptr<Element> child;

        Layer(std::shared_ptr<Element> child) : child(child) {}

        Vec2 naturalSize(Vec2 availableSpace) override {
            return child->naturalSize(availableSpace);
        }

        void draw(ContextPtr ctx, CUI::Rect rect) override {
            PendingLayer l{
                .rect = rect,
                .root = child,
            };
            if(ctx->root.get() == this)
                l.draw(ctx);
            else
                ctx->currentLayer->children.push_back(l);
        }
    };
}

namespace CUI::Factories {
    inline std::shared_ptr<Misc::Layer> Layer(std::shared_ptr<Element> child) {
        return std::make_shared<Misc::Layer>(child);
    }
}