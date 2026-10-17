#pragma once

#include "../../cui.hpp"
#include "../../color.hpp"
#include "primitive.hpp"
#include <memory>

namespace CUI::Primitive {
    // Fills the rect with a solid color.
    // Natural is zero (can be overriden)
    class Fill : public Primitive {
        static unsigned int program;
        static const char* fragmentSource;

    public:
        Fill(Color background, Vec2 size = {0,0});

        Vec2 naturalSize(Vec2 availableSpace) override {
            return size;
        }

        void setDerivedUniforms(ContextPtr, CUI::Rect) override;
    
        // Color to fill
        Color background;
        // Natural size, defaults to 0
        Vec2 size;
    };
}

namespace CUI::Factories {
    inline std::shared_ptr<CUI::Primitive::Fill> Fill(Color background, Vec2 size={0,0}) {
        return std::make_shared<CUI::Primitive::Fill>(background, size);
    }
}