#pragma once
#include "filter.hpp"
#include "value/value.hpp"
#include <memory>

namespace CUI::Filter {
    class Opacity : public Filter {
        static unsigned int program;
        static const char* fragmentSource;

    public:
        Val<float> opacity;
        Opacity(Val<float> opacity);

        std::vector<Step> getSteps() override;
    };
}

namespace CUI::Factories {
    // Opacity filter. Reduces the opacity of the input.
    inline std::shared_ptr<Filter::Opacity> Opacity(Val<float> opacity) {
        return std::make_shared<Filter::Opacity>(opacity);
    }
}