#include "shadow.hpp"
#include "elements/primitive/shape.hpp"
#include "util/shader.hpp"
#include <memory>

const char* CUI::Primitive::Shadow::fragmentSource = R"(
    #version 460 core

    // layout(location = 0) uniform vec4 uRect;
    layout(location = 2) uniform vec4 uColor;
    layout(location = 3) uniform float uShrink;
    layout(location = 4) uniform float uBlur;
    layout(location = 5) uniform float uGrow;

    in vec2 vLocalUV;
    in vec2 vLocalPixel;

    out vec4 outColor;

    float sdf();

    void main() {
        float d = sdf() - uGrow;
        outColor = vec4(uColor.rgb, uColor.a * (1.0 - smoothstep(-uBlur, uBlur, d)));
    }
)";

CUI::Primitive::Shadow::Shadow(
    std::shared_ptr<Shape::Shape> shape,
    Val<Color> color,
    Val<Vec2> shift,
    Val<float> blur,
    Val<float> grow
) 
    : ShapePrimitive(shape, fragmentSource), color(color), shift(shift), blur(blur), grow(grow) { }

void CUI::Primitive::Shadow::draw(Rect rect) {
    rect.pos += shift.get();
    ShapePrimitive::draw(rect.grow(blur.get()+grow.get()));
}

void CUI::Primitive::Shadow::setDerivedUniforms(CUI::Rect rect) {
    Color c = color.get().premultiply();
    float b = blur.get();
    float g = grow.get();

    glUniform4fv(2, 1, &c.r);
    glUniform1f(3, b + g);
    glUniform1f(4, b);
    glUniform1f(5, g);

    ShapePrimitive::setDerivedUniforms(rect);
}