#pragma once

#include "../../cui.hpp"
#include "../../font/font.hpp"
#include "../../color.hpp"
#include <memory>
#include <string>

namespace CUI::Primitive {
    // Renders single line text.
    // Natural size is the text bounds.
    // Does not fill its rect unless at natural size. Anchors to top left.
    class Text : public Element {
    public:
        // The font face
        std::shared_ptr<Font> font;
        // Text to render
        std::string text;
        // Text color
        Color color;

        Text(std::shared_ptr<Font> font, std::string text, Color color);
        ~Text();
        
        void draw(ContextPtr ctx, Rect rect) override;

        void setText(const std::string &text) {
            this->text = text;
            updateGeometry();
        }

        Vec2 naturalSize(Vec2 availableSpace) override {
            return totalSize;
        }

    private:
        static int program;
        static const char *fragSource;
        unsigned int vao, vbo, ebo, indexCount = 0;
        int rectLoc, vpSizeLoc, textColorLoc;
        Vec2 totalSize;
        void updateGeometry();
    };
}

namespace CUI::Factories {
    inline std::shared_ptr<Primitive::Text> Text(std::string text, std::shared_ptr<Font> font, Color color) {
        return std::make_shared<Primitive::Text>(font, text, color);
    }
}