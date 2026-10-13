#pragma once
#include "filter.hpp"

namespace CUI::Filter {
    class Opacity : public Filter {
        static unsigned int program;
        static const char* fragmentSource;

        int imageLoc, opacityLoc;

    public:
        float opacity;
        Opacity(float opacity);

        std::vector<Step> getSteps() override;
    };
}