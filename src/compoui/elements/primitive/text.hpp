#pragma once

#include "../../cui.hpp"
#include "../../font/font.hpp"
#include "../../color.hpp"
#include <memory>
#include <string>

namespace CUI::Primitive {
    class Text : public Element {
    public:
        std::shared_ptr<Font> font;
        std::string text;
        Color color;
        static const char *vertSource, *fragSource;
        static int program;

        Text(std::shared_ptr<Font> font, std::string text, Color color);
        ~Text();
        
        void draw(ContextPtr ctx, Rect rect) override;

        Vec2 naturalSize(Vec2 availableSpace) override {
            return totalSize;
        }

    private:
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