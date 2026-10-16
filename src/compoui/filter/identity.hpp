#pragma once
#include "filter.hpp"

namespace CUI::Filter {
    class Identity : public Filter {
        static unsigned int program;
        static const char* fragmentSource;

        int imageLoc;

    public:
        Identity();

        std::vector<Step> getSteps() override;
    };
}
namespace CUI::Factories {
    inline std::shared_ptr<Filter::Identity> IdentityFilter() {
        return std::make_shared<Filter::Identity>();
    }
}