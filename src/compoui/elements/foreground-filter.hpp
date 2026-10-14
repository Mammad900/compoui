#pragma once
#include "../cui.hpp"
#include "../filter/filter.hpp"
#include <memory>

namespace CUI::Misc {
    class ForegroundFilter : public Element {
    public:
        std::shared_ptr<Element> child;
        std::shared_ptr<Filter::Filter> filter; 

        ForegroundFilter(std::shared_ptr<Element> child, std::shared_ptr<Filter::Filter> filter) : child(child), filter(filter) {}

        Vec2 naturalSize(Vec2 availableSpace) override {
            return child->naturalSize(availableSpace);
        }

        void draw(ContextPtr ctx, CUI::Rect rect) override {
            auto fbo = ctx->getFBO();
            ctx->activateFBO(fbo->fbo, true);
            child->draw(ctx, rect);
            ctx->activateFBO(ctx->mainFBO);

            auto steps = filter->getSteps();
            auto step = steps[0];
            
            filter->draw(ctx, rect, step, fbo->fbo);
        }
    };
}
namespace CUI::Factories {
    inline std::shared_ptr<Misc::ForegroundFilter> ForegroundFilter(std::shared_ptr<Filter::Filter> filter, std::shared_ptr<Element> child) {
        return std::make_shared<Misc::ForegroundFilter>(child, filter);
    }
}