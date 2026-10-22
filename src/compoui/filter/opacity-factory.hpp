#pragma once

#include "cui.hpp"
#include "elements/foreground-filter.hpp"
#include "filter/opacity.hpp"
#include <memory>
namespace CUI::Factories {
    inline std::shared_ptr<Misc::ForegroundFilter> Opacity(Val<float> opacity, ElementPtr child) {
        return ForegroundFilter(Opacity(opacity), child);
    }
}