#pragma once

#include "../../cui.hpp"
#include "../../color.hpp"
#include "shape.hpp"
#include "value/value.hpp"
#include <memory>

namespace CUI::Primitive {
    // Fills the rect with a solid color.
    // Natural size is zero (can be overriden)
    class Shadow : public ShapePrimitive {
        static const char* fragmentSource;

    public:
        Shadow(
            std::shared_ptr<Shape::Shape> shape, 
            Val<Color> color,
            Val<Vec2> shift,
            Val<float> blur,
            Val<float> grow
        );

        Vec2 naturalSize(Vec2 availableSpace) override {
            return {0,0};
        }

        void setDerivedUniforms(CUI::Rect) override;
        void draw(Rect rect) override;
    
        Val<Color> color;
        Val<Vec2> shift;
        Val<float> blur;
        Val<float> grow;
    };
}

namespace CUI::Factories {
    // Fills the rect with a solid color.
    // Natural size is zero (can be overriden)
    inline std::shared_ptr<CUI::Primitive::Shadow> Shadow(
        std::shared_ptr<Shape::Shape> shape, 
        Val<Color> color,
        Val<Vec2> shift,
        Val<float> blur,
        Val<float> grow = 0
    ) {
        return std::make_shared<CUI::Primitive::Shadow>(shape, color, shift, blur, grow);
    }
}