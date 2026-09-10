// ---------- font.cpp ----------
#include "font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../glad/glad.h"

#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace {
    struct Glyph {
        char character;

        glm::ivec2 size;
        glm::ivec2 bearing;

        // FreeType stores advance values in 1/64 pixel units.
        long advance;

        std::vector<unsigned char> bitmap;

        int width() const {
            return size.x;
        }

        int height() const {
            return size.y;
        }
    };

    struct PlacedGlyph {
        Glyph* glyph;
        int x;
        int y;
    };

    static void buildAtlas(
        FT_Face face,
        unsigned int& tex_,
        std::unordered_map<char, CUI::Font::CharInfo>& chars_)
    {
        constexpr int first = 32;
        constexpr int last = 127;       // exclusive
        constexpr int maxAtlasWidth = 512;
        constexpr int padding = 1;

        std::vector<Glyph> glyphs;
        glyphs.reserve(last - first);

        // ------------------------------------------------------------------
        // 1. Load all glyphs and copy their bitmap + metrics.
        // ------------------------------------------------------------------

        for (int c = first; c < last; ++c) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                continue;
            }

            const FT_GlyphSlot slot = face->glyph;

            Glyph glyph{};
            glyph.character = static_cast<char>(c);

            glyph.size = glm::ivec2(
                static_cast<int>(slot->bitmap.width),
                static_cast<int>(slot->bitmap.rows));

            glyph.bearing = glm::ivec2(
                slot->bitmap_left,
                slot->bitmap_top);

            glyph.advance = slot->advance.x;

            // FreeType bitmaps can have pitch != width.
            if (slot->bitmap.width > 0 && slot->bitmap.rows > 0) {
                glyph.bitmap.resize(
                    static_cast<size_t>(slot->bitmap.width) *
                    static_cast<size_t>(slot->bitmap.rows));

                for (int y = 0; y < glyph.height(); ++y) {
                    const unsigned char* src =
                        slot->bitmap.buffer +
                        static_cast<size_t>(y) * slot->bitmap.pitch;

                    unsigned char* dst =
                        glyph.bitmap.data() +
                        static_cast<size_t>(y) * glyph.width();

                    std::copy(
                        src,
                        src + glyph.width(),
                        dst);
                }
            }

            glyphs.push_back(std::move(glyph));
        }

        if (glyphs.empty()) {
            throw std::runtime_error("Font contains no usable ASCII glyphs");
        }

        // ------------------------------------------------------------------
        // 2. Calculate glyph positions.
        //
        // We calculate the positions once and use exactly those positions
        // when uploading the atlas. This prevents the two packing passes
        // from disagreeing.
        // ------------------------------------------------------------------

        std::vector<PlacedGlyph> placed;
        placed.reserve(glyphs.size());

        int atlasWidth = 0;
        int atlasHeight = 0;

        int rowX = 0;
        int rowY = 0;
        int rowHeight = 0;

        for (Glyph& glyph : glyphs) {
            const int glyphWidth = glyph.width();
            const int glyphHeight = glyph.height();

            // A glyph wider than the atlas cannot be packed.
            if (glyphWidth + padding > maxAtlasWidth) {
                throw std::runtime_error(
                    "Font glyph is wider than the maximum atlas width");
            }

            // Start a new row if this glyph doesn't fit.
            if (rowX > 0 &&
                rowX + glyphWidth + padding > maxAtlasWidth) {

                rowY += rowHeight + padding;
                rowX = 0;
                rowHeight = 0;
            }

            placed.push_back({
                &glyph,
                rowX,
                rowY
            });

            rowX += glyphWidth + padding;
            rowHeight = std::max(rowHeight, glyphHeight);

            atlasWidth = std::max(atlasWidth, rowX);
        }

        atlasHeight = rowY + rowHeight + padding;

        // Don't create a zero-sized texture.
        atlasWidth = std::max(atlasWidth, 1);
        atlasHeight = std::max(atlasHeight, 1);

        // ------------------------------------------------------------------
        // 3. Create the OpenGL texture.
        // ------------------------------------------------------------------

        glGenTextures(1, &tex_);
        glBindTexture(GL_TEXTURE_2D, tex_);

        // FreeType glyph rows are byte-sized. Without this, widths that
        // aren't multiples of 4 can cause GL_UNPACK_ALIGNMENT corruption.
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            atlasWidth,
            atlasHeight,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            nullptr);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR);

        // ------------------------------------------------------------------
        // 4. Upload glyphs and construct CharInfo.
        // ------------------------------------------------------------------

        chars_.clear();

        for (const PlacedGlyph& placedGlyph : placed) {
            const Glyph& glyph = *placedGlyph.glyph;

            const int x = placedGlyph.x;
            const int y = placedGlyph.y;

            if (glyph.width() > 0 && glyph.height() > 0) {
                glTexSubImage2D(
                    GL_TEXTURE_2D,
                    0,
                    x,
                    y,
                    glyph.width(),
                    glyph.height(),
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    glyph.bitmap.data());
            }

            CUI::Font::CharInfo info{};

            info.size = glyph.size;
            info.bearing = glyph.bearing;

            // FreeType uses 26.6 fixed-point values.
            //
            // If CharInfo::advance is currently a long/int and your renderer
            // expects FreeType's raw value, remove the /64.0 conversion.
            //
            // This assumes CharInfo::advance is a floating-point type.
            info.advance = static_cast<float>(glyph.advance) / 64.0f;

            // Texture coordinates.
            //
            // OpenGL texture coordinate V=0 corresponds to the bottom of
            // the texture. FreeType's bitmap data is ordered top-to-bottom.
            //
            // These UVs describe the bitmap's location in the atlas. If your
            // renderer uses top-left UVs instead, swap the V coordinates.
            info.uv.pos = {
                (float)(x) / (float)(atlasWidth),
                (float)(y) / (float)(atlasHeight)
            };

            info.uv.size = {
                (float)(glyph.width()) / (float)(atlasWidth),
                (float)(glyph.height()) / (float)(atlasHeight)
            };

            chars_.emplace(glyph.character, info);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }
} // namespace

CUI::Font::Font(std::filesystem::path path, unsigned int size) : size(size) {
    FT_Library ft = nullptr;

    if (FT_Init_FreeType(&ft)) {
        throw std::runtime_error("FreeType init failed");
    }

    FT_Face face = nullptr;

    // Keep the string alive for the duration of FT_New_Face().
    const std::string pathString = path.string();

    if (FT_New_Face(ft, pathString.c_str(), 0, &face)) {
        FT_Done_FreeType(ft);
        throw std::runtime_error(
            "Failed to load font: " + pathString);
    }
    
    if (FT_Set_Pixel_Sizes(face, 0, size)) {
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        throw std::runtime_error(
            "Failed to set font pixel size");
    }

    lineHeight = static_cast<float>(face->size->metrics.height)   / 64.0f;
    ascender   = static_cast<float>(face->size->metrics.ascender)/ 64.0f;
    descender  = static_cast<float>(face->size->metrics.descender)/ 64.0f;

    try {
        buildAtlas(face, tex_, chars_);
    }
    catch (...) {
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        throw;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

CUI::Font::~Font()
{
    if (tex_ != 0) {
        glDeleteTextures(1, &tex_);
        tex_ = 0;
    }
}