#pragma once
#include "filter.hpp"
#include <memory>
#include <vector>

namespace CUI::Filter {
    class Combine : public Filter {
        std::vector<Step> steps;

    public:
        std::vector<std::shared_ptr<Filter>> filters;
        Combine(std::vector<std::shared_ptr<Filter>> filters) : filters(filters) {}

        std::vector<Step> getSteps() override {
            steps.clear();
            for (auto &&filter : filters) {
                for (auto &&step : filter->getSteps()) {
                    steps.push_back(std::move(step));
                }
            }
            return steps;
        }
    };
}
namespace CUI::Factories {
    inline std::shared_ptr<Filter::Combine> Filters(std::vector<std::shared_ptr<Filter::Filter>> filters) {
        return std::make_shared<Filter::Combine>(filters);
    }
}