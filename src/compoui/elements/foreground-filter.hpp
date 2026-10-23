#pragma once
#include "../cui.hpp"
#include "../filter/filter.hpp"
#include "fbo.hpp"
#include <cstddef>
#include <memory>
#include <vector>

namespace CUI::Misc {
    // Applies a filter to its child.
    class ForegroundFilter : public Element {
    public:
        ElementPtr child;
        std::shared_ptr<Filter::Filter> filter; 

        ForegroundFilter(ElementPtr child, std::shared_ptr<Filter::Filter> filter) : child(child), filter(filter) {}

        Vec2 naturalSize(Vec2 availableSpace) override {
            return child->naturalSize(availableSpace);
        }

        void draw(CUI::Rect rect) override {
            auto parentFbo = ctx->currentFBO;
            auto childrenFbo = ctx->getFBO();
            ctx->activateFBO(childrenFbo->fbo, true);
            child->draw(rect);

            std::vector<Filter::Step> steps = filter->getSteps();

            std::shared_ptr<FBOLease> tempFbo = nullptr;
            float growCarry = 0;

            for (size_t i = 0; i < steps.size(); i++) {
                Filter::Step &step = steps[i];
                bool isFirst = i == 0;
                bool isLast = i == steps.size() - 1;
                std::shared_ptr<FBOLease> sourceFbo = isFirst ? childrenFbo : tempFbo;
                if(!isLast) {
                    tempFbo = nullptr;
                    tempFbo = ctx->getFBO();
                }
                std::shared_ptr<FBO> destFbo = isLast ? parentFbo : tempFbo->fbo;

                float grow = step.grow;
                step.grow += growCarry;
                if(step.growCarry) growCarry += grow;
                
                ctx->activateFBO(destFbo, !isLast);
                filter->draw(rect, step, sourceFbo->fbo, false);
            }

            ctx->activateFBO(parentFbo);
        }
    };
}
namespace CUI::Factories {
    /// Applies a filter to its child.
    /// The child is drawn in isolation and then pasted into the main UI.
    /// Side effect: clips everything inside to its bounds.
    /// Side effect: Any BackgroundFilter must choose between using only inside this element, or only behind it.
    /// @param filter The filter to apply
    /// @param child the element to draw and filter.
    inline std::shared_ptr<Misc::ForegroundFilter> ForegroundFilter(std::shared_ptr<Filter::Filter> filter, ElementPtr child) {
        return std::make_shared<Misc::ForegroundFilter>(child, filter);
    }
}