#include "fill.hpp"
#include "util/shader.hpp"

unsigned int CUI::Primitive::Fill::program = 0;
const char* CUI::Primitive::Fill::fragmentSource = R"(
    #version 460 core

    layout(location = 2) uniform vec4 uBackground;

    out vec4 outColor;

    void main() {
        outColor = uBackground;
    }
)";

CUI::Primitive::Fill::Fill(Color background, Vec2 size) 
    : Primitive(program, fragmentSource), background(background), size(size) { }

void CUI::Primitive::Fill::setDerivedUniforms(ContextPtr, CUI::Rect) {
    glUniform4fv(2, 1, &background.r);
}