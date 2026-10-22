#pragma once

#include "../../cui.hpp"
#include "../../color.hpp"
#include "primitive.hpp"
#include "value/value.hpp"
#include <memory>

namespace CUI::Primitive {
    // Fills the rect with a solid color.
    // Natural size is zero (can be overriden)
    class Fill : public Primitive {
        static unsigned int program;
        static const char* fragmentSource;

    public:
        Fill(Val<Color> background, Val<Vec2> size = Vec2{0,0});

        Vec2 naturalSize(Vec2 availableSpace) override {
            return size.get();
        }

        void setDerivedUniforms(CUI::Rect) override;
    
        // Color to fill
        Val<Color> background;
        
        // Natural size, defaults to 0
        Val<Vec2> size;
    };
}

namespace CUI::Factories {
    // Fills the rect with a solid color.
    // Natural size is zero (can be overriden)
    inline std::shared_ptr<CUI::Primitive::Fill> Fill(Val<Color> background, Val<Vec2> size = Vec2{0,0}) {
        return std::make_shared<CUI::Primitive::Fill>(background, size);
    }
}