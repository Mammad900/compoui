#pragma once
#include "../cui.hpp"
#include "../filter/filter.hpp"
#include "fbo.hpp"
#include "filter/identity.hpp"
#include <cstddef>
#include <memory>
#include <vector>

namespace CUI::Misc {
    class BackgroundFilter : public Element {
    public:
        std::shared_ptr<Filter::Filter> filter; 
        bool useMain;

        BackgroundFilter(std::shared_ptr<Filter::Filter> filter, bool useMain) 
          : filter(filter), useMain(useMain) {}

        Vec2 naturalSize(Vec2 availableSpace) override {
            return {0,0};
        }

        void draw(CUI::Rect rect) override {
            auto parentFbo = ctx->currentFBO;

            std::vector<Filter::Step> steps = filter->getSteps();
            if(steps.size() == 0) return;
            if(steps.size() == 1) steps.push_back(Filter::Identity().getSteps()[0]);

            std::shared_ptr<FBOLease> tempFbo = nullptr;

            for (size_t i = 0; i < steps.size(); i++) {
                Filter::Step &step = steps[i];
                bool isFirst = i == 0;
                bool isLast = i == steps.size() - 1;
                std::shared_ptr<FBOLease> sourceFbo = isFirst ? nullptr : tempFbo;
                if(!isLast) {
                    tempFbo = nullptr;
                    tempFbo = ctx->getFBO();
                }
                std::shared_ptr<FBO> destFbo = isLast ? parentFbo : tempFbo->fbo;

                if(isLast) step.grow = 0;
                
                ctx->activateFBO(destFbo, !isLast);
                filter->draw(rect, step, sourceFbo ? sourceFbo->fbo : useMain ? ctx->mainFBO : parentFbo, true);
            }

            ctx->activateFBO(parentFbo);
        }
    };
}
namespace CUI::Factories {
    /// Applies a filter to what's behind it. Equivalent to CSS `backdrop-filter`.
    /// May not function correctly if the background is not fully opaque.
    /// @param filter The filter to apply.
    /// @param useMain If a descendant of a ForegroundFilter, this uses the contents behind that ForegroundFilter as the source. Otherwise, it uses anything behind this element but still inside the ForegroundFilter.
    inline std::shared_ptr<Misc::BackgroundFilter> BackgroundFilter(std::shared_ptr<Filter::Filter> filter, bool useMain = false) {
        return std::make_shared<Misc::BackgroundFilter>(filter, useMain);
    }
}