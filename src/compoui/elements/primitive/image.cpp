#include "image.hpp"
#include "util/shader.hpp"
#include <algorithm>
#include <memory>

unsigned int CUI::Primitive::Image::program = 0;
const char* CUI::Primitive::Image::fragmentSource = R"(
    #version 460 core

    uniform sampler2D imageTex;
    uniform vec4 color;
    uniform bool isAlpha;

    in vec2 localUV;
    out vec4 outColor;

    void main() {
        vec4 tex = texture(imageTex, localUV);
        if(isAlpha)
            outColor = vec4(1.0, 1.0, 1.0, tex.r) * color;
        else
            outColor = tex * color;
    }
)";

CUI::Primitive::Image::Image(std::shared_ptr<Texture> texture, Vec2 size, Color color)
    : Primitive(program, fragmentSource), texture(texture), size(size), color(color)
{
    // uniform locations
    imageLoc = glGetUniformLocation(program, "imageTex");
    colorLoc = glGetUniformLocation(program, "color");
    isAlphaLoc = glGetUniformLocation(program, "isAlpha");
}

void CUI::Primitive::Image::setDerivedUniforms(ContextPtr, CUI::Rect rect) {
    // bind the texture unit 0 and set the sampler to that unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform1i(imageLoc, 0);

    glUniform4fv(colorLoc, 1, &color.r);
    glUniform1i(isAlphaLoc, texture->monochrome);
}

CUI::Vec2 CUI::Primitive::Image::naturalSize(Vec2 availableSpace) {
    if(availableSpace.x >= size.x && availableSpace.y >= size.y)
        return size;
    float ratioX = availableSpace.x / size.x;
    float ratioY = availableSpace.y / size.y;
    return size * std::min(ratioX, ratioY);
}