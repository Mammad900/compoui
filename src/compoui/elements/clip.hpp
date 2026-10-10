#include "../cui.hpp"
#include <memory>

namespace CUI::Misc {
    class Clip : public Element {
    public:
        std::shared_ptr<Element> child;

        Clip(std::shared_ptr<Element> child) : child(child) {}

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
    inline std::shared_ptr<Misc::Clip> Clip(std::shared_ptr<Element> child) {
        return std::make_shared<Misc::Clip>(child);
    }
}