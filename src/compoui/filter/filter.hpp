#pragma once
#include "../cui.hpp"
#include <functional>
#include <memory>
#include <vector>
#include "../fbo.hpp"

namespace CUI::Filter {
    struct Step {
        std::function<void(std::shared_ptr<FBO> &fbo)> setUniforms;
        float grow;
        unsigned int program;
    };

    class Filter {
        int vpSizeLoc, rectLoc;
    protected:
        Filter(unsigned int &program, const char* fragmentSource);
    public:
        virtual std::vector<Step> getSteps() = 0;
        void draw(ContextPtr ctx, Rect rect, Step& step, std::shared_ptr<FBO> &fbo);
    };
}