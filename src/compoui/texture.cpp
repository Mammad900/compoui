#include "texture.hpp"
#include <glad/glad.h>          // or your GL loader header

CUI::Texture::Texture(const Vec2& sz, const uint8_t* buf, bool mono)
        : id(0), monochrome(mono), size(sz) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    GLenum format = mono ? GL_RED   : GL_RGBA;
    GLenum internalFormat = mono ? GL_R8  : GL_RGBA8;

    glTexImage2D(GL_TEXTURE_2D,
                    0,                 // level
                    internalFormat,    // internal format
                    static_cast<GLsizei>(size.x),
                    static_cast<GLsizei>(size.y),
                    0,                 // border
                    format,            // format of the pixel data
                    GL_UNSIGNED_BYTE,  // type of pixel data
                    buf);

    // Basic texture parameters – adjust as needed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

CUI::Texture::~Texture() {
    if (id) glDeleteTextures(1, &id);
}