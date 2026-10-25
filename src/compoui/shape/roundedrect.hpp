#pragma once

#include "shape.hpp"
#include "../value/value.hpp"
#include <memory>

namespace CUI::Shape {
    class RoundedRect : public Shape {
        static const char* source;
    public:
        Val<float> radius;
        const char* getSdfSource() override {return source;}
        void setUniforms() override;
        RoundedRect(Val<float> radius) : radius(radius) {}
    };
}

namespace CUI::Factories {
    inline std::shared_ptr<Shape::RoundedRect> RoundedRect(Val<float> radius) {
        return std::make_shared<Shape::RoundedRect>(radius);
    }
}