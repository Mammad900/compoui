#pragma once
#include "filter.hpp"
#include <cmath>
#include <memory>

namespace CUI::Filter {
    class Blur : public Filter {
        static unsigned int program;
        static const char* fragmentSource;
        float lastSigma = NAN;
        unsigned int weightsTex = 0;
    public:
        float sigma;
        bool clip;
        Blur(float sigma, bool clip)
         : Filter(program, fragmentSource), sigma(sigma), clip(clip) { }

        std::vector<Step> getSteps() override;
    };
}

namespace CUI::Factories {
    inline std::shared_ptr<Filter::Blur> Blur(float sigma, bool clip = false) {
        return std::make_shared<Filter::Blur>(sigma, clip);
    }
}