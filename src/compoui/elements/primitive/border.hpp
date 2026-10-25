#pragma once

#include "../../cui.hpp"
#include "../../color.hpp"
#include "shape.hpp"
#include "value/value.hpp"
#include <memory>

namespace CUI::Primitive {
    // Fills the rect with a solid color.
    // Natural size is zero (can be overriden)
    class Border : public ShapePrimitive {
        static const char* fragmentSource;

    public:
        Border(
            Val<Color> background, 
            Val<Color> border, 
            Val<float> width,
            std::shared_ptr<Shape::Shape> shape, 
            Val<Vec2> size
        );

        Vec2 naturalSize(Vec2 availableSpace) override {
            return size.get();
        }

        void setDerivedUniforms(CUI::Rect) override;
    
        // Fill color
        Val<Color> background;

        // Border color
        Val<Color> border;

        // Border thickness
        Val<float> width;

        // Natural size, defaults to 0
        Val<Vec2> size;
    };
}

namespace CUI::Factories {
    // Fills the rect with a solid color.
    // Natural size is zero (can be overriden)
    inline std::shared_ptr<CUI::Primitive::Border> Border(
        std::shared_ptr<Shape::Shape> shape, 
        Val<float> width,
        Val<Color> border, 
        Val<Color> background = Color::transparent, 
        Val<Vec2> size = Vec2{0,0}
    ) {
        return std::make_shared<CUI::Primitive::Border>(background, border, width, shape, size);
    }
}