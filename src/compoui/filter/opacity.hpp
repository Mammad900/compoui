#pragma once
#include "filter.hpp"
#include <memory>

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

namespace CUI::Factories {
    inline std::shared_ptr<Filter::Opacity> Opacity(float opacity) {
        return std::make_shared<Filter::Opacity>(opacity);
    }
}