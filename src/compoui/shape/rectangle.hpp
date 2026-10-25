#pragma once

#include "shape.hpp"
#include <memory>

namespace CUI::Shape {
    class Rectangle : public Shape {
    public:
        static const char* source;
        const char * getSdfSource() override {return source;}
        void setUniforms() override;
    };
}

namespace CUI::Factories {
    inline std::shared_ptr<Shape::Rectangle> Rectangle() {
        static std::shared_ptr<Shape::Rectangle> r = std::make_shared<Shape::Rectangle>();
        return r;
    }
}