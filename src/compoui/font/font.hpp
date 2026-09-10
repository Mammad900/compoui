// ---------- font.hpp ----------
#pragma once

#include "../rect.hpp"
#include <filesystem>
#include <glm/glm.hpp>
#include <unordered_map>

namespace CUI {
    class Font {
    public:
        unsigned int size;
        float lineHeight, ascender, descender;
        struct CharInfo {
            Rect uv; // texture coordinates
            glm::ivec2 size;    // bitmap width, height
            glm::ivec2 bearing; // offset from baseline to left/top
            float advance; // horizontal advance
        };

        explicit Font(std::filesystem::path fontPath, unsigned int size);
        ~Font();

        const CharInfo& get(char c) const { return chars_.at(c); }
        unsigned int atlas() const { return tex_; }

    private:

        unsigned int tex_;
        std::unordered_map<char, CharInfo> chars_;
    };
}