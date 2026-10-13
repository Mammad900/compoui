#pragma once

#include "vec2.hpp"

namespace CUI {

// Represents a texture for use in the Image element
class Texture {
public:
    // OpenGL texture handle
    unsigned int id;

    // True if the texture contains only a single channel (alpha)
    bool monochrome;

    // Size of the texture in pixels
    Vec2 size;

    /**
     * @brief Construct a texture from raw data.
     *
     * @param sz   The width and height of the texture.
     * @param buf  Pointer to the pixel buffer.  
     *             For monochrome textures, the buffer should contain one byte (A) per pixel;  
     *             for color textures it should be four bytes (RGBA) per pixel.
     * @param mono True if `buf` is single‑channel data, otherwise false.
     */
    Texture(const Vec2& sz, const uint8_t* buf, bool mono = false);

    Texture(const Vec2& size, unsigned int id) : id(id), monochrome(false), size(size) {}

    ~Texture();
};

} // namespace CUI