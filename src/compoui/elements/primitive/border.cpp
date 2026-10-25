#include "border.hpp"
#include "util/shader.hpp"
#include <memory>

const char* CUI::Primitive::Border::fragmentSource = R"(
    #version 460 core

    // layout(location = 0) uniform vec4 uRect;
    layout(location = 2) uniform vec4 uBackground;
    layout(location = 3) uniform vec4 uBorder;
    layout(location = 4) uniform float uWidth;

    in vec2 vLocalUV;
    in vec2 vLocalPixel;

    out vec4 outColor;

    float sdf();

    void main() {
        float d = sdf();
        float t = clamp(0.5 - d - uWidth, 0.0, 1.0);
        outColor = mix(uBorder, uBackground, t);
        t = clamp(0.5 - d, 0.0, 1.0);
        outColor = mix(vec4(0.0, 0.0, 0.0, 0.0), outColor, t);
        // outColor = vec4(d/-10.0, 0.0, 0.0, 1.0);
    }
)";

CUI::Primitive::Border::Border(
    Val<Color> background, 
    Val<Color> border, 
    Val<float> width,
    std::shared_ptr<Shape::Shape> shape,
    Val<Vec2> size
) 
    : ShapePrimitive(shape, fragmentSource), background(background), border(border), width(width), size(size) { }

void CUI::Primitive::Border::setDerivedUniforms(CUI::Rect rect) {
    Color back = background.get();
    Color bord = border.get();
    glUniform4fv(2, 1, &back.r);
    glUniform4fv(3, 1, &bord.r);
    glUniform1f(4, width.get());

    ShapePrimitive::setDerivedUniforms(rect);
}