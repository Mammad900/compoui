#pragma once
#include "../cui.hpp"
#include <memory>

namespace CUI::Misc {
    class Layer : public Element, public std::enable_shared_from_this<Layer> {
    public:
        ElementPtr child;

        Layer(ElementPtr child) : child(child) {}

        Vec2 naturalSize(Vec2 availableSpace) override {
            return child->naturalSize(availableSpace);
        }

        void draw(CUI::Rect rect) override {
            PendingLayer l{
                .rect = rect,
                .root = child,
            };
            if(ctx->root.get() == this)
                l.draw();
            else
                ctx->currentLayer->children.push_back(l);
        }
    };
}

namespace CUI::Factories {
    // Postpones the rendering of its child until after everything else has been rendered.
    inline std::shared_ptr<Misc::Layer> Layer(ElementPtr child) {
        return std::make_shared<Misc::Layer>(child);
    }
}