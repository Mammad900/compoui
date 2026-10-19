#include "../cui.hpp"
#include <memory>

namespace CUI::Misc {
    // Prevents children from overflowing out of its bounds
    class Clip : public Element {
    public:
        ElementPtr child;

        Clip(ElementPtr child) : child(child) {}

        Vec2 naturalSize(Vec2 availableSpace) override {
            return child->naturalSize(availableSpace);
        }

        void draw(ContextPtr ctx, CUI::Rect rect) override {
            std::optional<Rect> previousClip = ctx->clipRect;
            ctx->setClip(rect);
            child->draw(ctx, rect);
            ctx->setClip(previousClip);
        }
    };
}

namespace CUI::Factories {
    // Prevents children from overflowing out of its bounds
    inline std::shared_ptr<Misc::Clip> Clip(ElementPtr child) {
        return std::make_shared<Misc::Clip>(child);
    }
}