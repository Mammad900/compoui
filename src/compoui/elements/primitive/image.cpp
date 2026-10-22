#include "image.hpp"
#include "cui.hpp"
#include "util/shader.hpp"
#include <algorithm>
#include <memory>

unsigned int CUI::Primitive::Image::program = 0;
const char* CUI::Primitive::Image::fragmentSource = R"(
    #version 460 core

    layout(location = 2) uniform sampler2D tImageTex;
    layout(location = 3) uniform vec4 uColor;
    layout(location = 4) uniform bool uIsAlpha;

    in vec2 vLocalUV;
    out vec4 outColor;

    void main() {
        vec4 tex = texture(tImageTex, vLocalUV);
        if(uIsAlpha)
            outColor = vec4(1.0, 1.0, 1.0, tex.r) * uColor;
        else
            outColor = tex * uColor;
    }
)";

CUI::Primitive::Image::Image(std::shared_ptr<Texture> texture, Val<Vec2> size, Val<Color> color)
    : Primitive(program, fragmentSource), texture(texture), size(size), color(color) { }

void CUI::Primitive::Image::setDerivedUniforms(CUI::Rect rect) {
    auto texture = this->texture.get();
    Color color = this->color.get();

    // bind the texture unit 0 and set the sampler to that unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform1i(2, 0);

    glUniform4fv(3, 1, &color.r);
    glUniform1i(4, texture->monochrome);
}

CUI::Vec2 CUI::Primitive::Image::naturalSize(Vec2 availableSpace) {
    Vec2 size = this->size.get();

    if(availableSpace.x >= size.x && availableSpace.y >= size.y)
        return size;
    float ratioX = availableSpace.x / size.x;
    float ratioY = availableSpace.y / size.y;
    return size * std::min(ratioX, ratioY);
}