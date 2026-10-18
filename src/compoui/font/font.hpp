// ---------- font.hpp ----------
#pragma once

#include "../rect.hpp"
#include <filesystem>
#include <glm/ext/vector_int2.hpp>
#include <unordered_map>

namespace CUI {
    // A font face with a given size. Used for the Text element.
    class Font {
    public:
        // The font size, in pixels.
        unsigned int size;
        // Not sure what these mean, google them.
        float lineHeight, ascender, descender;
        struct CharInfo {
            Rect uv; // texture coordinates in the texture atlas
            glm::ivec2 size;    // bitmap width, height
            glm::ivec2 bearing; // offset from baseline to left/top
            float advance; // horizontal advance (aka width)
        };

        // Load a font file supported by Freetype2.
        explicit Font(std::filesystem::path fontPath, unsigned int size);
        ~Font();

        // Get information about a glyph.
        const CharInfo& get(char c) const { return chars_.at(c); }
        // Get the OpenGL texture handle containing the glyph texture atlas.
        unsigned int atlas() const { return tex_; }

    private:

        unsigned int tex_;
        std::unordered_map<char, CharInfo> chars_;
    };
}