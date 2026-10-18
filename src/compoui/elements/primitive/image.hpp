#pragma once

#include "../../cui.hpp"
#include "../../color.hpp"
#include "../../texture.hpp"
#include "primitive.hpp"
#include <memory>

namespace CUI::Primitive {
    // Renders a Texture.
    // Natural size is its pixel resolution (can be overriden).
    // Fills the quad and stretches.
    class Image : public Primitive {
        static unsigned int program;
        static const char* fragmentSource;

    public:
        Image(std::shared_ptr<Texture> texture, Vec2 size = {0,0}, Color color = Color::white);

        Vec2 naturalSize(Vec2 availableSpace) override;

        void setDerivedUniforms(ContextPtr, CUI::Rect) override;

        // The texture to draw
        std::shared_ptr<Texture> texture;
        // Natural size
        Vec2 size;
        // Multiply image colors by this color
        Color color;
    };
}

namespace CUI::Factories {
    /// Renders a Texture.
    /// Fills the rect and stretches.
    /// @param size natural size
    /// @param color multiply the image colors by this color
    inline std::shared_ptr<CUI::Primitive::Image> Image(std::shared_ptr<Texture> texture, Vec2 size, Color color = Color::white) {
        return std::make_shared<CUI::Primitive::Image>(texture, size, color);
    }

    /// Renders a Texture.
    /// Natural size is its pixel resolution.
    /// Fills the rect and stretches.
    /// @param color multiply the image colors by this color
    inline std::shared_ptr<CUI::Primitive::Image> Image(std::shared_ptr<Texture> texture, Color color) {
        return std::make_shared<CUI::Primitive::Image>(texture, texture->size, color);
    }

    /// Renders a Texture.
    /// Natural size is its pixel resolution.
    /// Fills the rect and stretches.
    inline std::shared_ptr<CUI::Primitive::Image> Image(std::shared_ptr<Texture> texture) {
        return std::make_shared<CUI::Primitive::Image>(texture, texture->size, Color::white);
    }
}