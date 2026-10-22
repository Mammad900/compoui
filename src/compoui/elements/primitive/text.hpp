#pragma once

#include "../../cui.hpp"
#include "../../font/font.hpp"
#include "../../color.hpp"
#include "value/value.hpp"
#include <memory>
#include <string>

namespace CUI::Primitive {
    // Renders single line text.
    // Natural size is the text bounds.
    // Does not fill its rect and aligns to top left unless at natural size.
    class Text : public Element {
    public:
        // The font face
        Val<std::shared_ptr<Font>> font;
        // Text to render
        Val<std::string> text;
        // Text color
        Val<Color> color;

        Text(Val<std::shared_ptr<Font>> font, Val<std::string> text, Val<Color> color);
        ~Text();
        
        void draw(Rect rect) override;

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
        Vec2 totalSize;
        void updateGeometry();
    };
}

namespace CUI::Factories {
    /// Renders single line text.
    /// Natural size is the text bounds.
    /// Does not fill its rect and aligns to top left unless at natural size.
    inline std::shared_ptr<Primitive::Text> Text(Val<std::string> text, Val<std::shared_ptr<Font>> font, Val<Color> color) {
        return std::make_shared<Primitive::Text>(font, text, color);
    }
    inline std::shared_ptr<Primitive::Text> Text(std::string text, Val<std::shared_ptr<Font>> font, Val<Color> color) {
        return std::make_shared<Primitive::Text>(font, text, color);
    }
}