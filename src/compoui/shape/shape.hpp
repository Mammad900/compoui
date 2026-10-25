#pragma once

namespace CUI::Shape {
    class Shape {
    public:
        virtual const char* getSdfSource() = 0;
        virtual void setUniforms() = 0;
    };
}