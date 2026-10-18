#pragma once
#include "filter.hpp"
#include <memory>

namespace CUI::Filter {
    class Opacity : public Filter {
        static unsigned int program;
        static const char* fragmentSource;

    public:
        float opacity;
        Opacity(float opacity);

        std::vector<Step> getSteps() override;
    };
}

namespace CUI::Factories {
    // Opacity filter. Reduces the opacity of the input.
    inline std::shared_ptr<Filter::Opacity> Opacity(float opacity) {
        return std::make_shared<Filter::Opacity>(opacity);
    }
}