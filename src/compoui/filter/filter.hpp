#pragma once
#include "../cui.hpp"
#include <functional>
#include <memory>
#include <vector>
#include "../fbo.hpp"

namespace CUI::Filter {
    struct Step {
        unsigned int program;
        std::function<void(std::shared_ptr<FBO> &fbo, ContextPtr ctx)> setUniforms;
        float grow;
        bool growCarry;
    };

    class Filter {
        protected:
        Filter(unsigned int &program, const char* fragmentSource);
        Filter() {}
    public:
        virtual std::vector<Step> getSteps() = 0;
        static void draw(ContextPtr ctx, Rect rect, Step& step, std::shared_ptr<FBO> &fbo);
    };
}