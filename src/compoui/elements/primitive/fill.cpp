#include "fill.hpp"
#include "util/shader.hpp"

unsigned int CUI::Primitive::Fill::program = 0;
const char* CUI::Primitive::Fill::fragmentSource = R"(
    #version 460 core

    uniform vec4 background;

    out vec4 color;

    void main() {
        color = background;
    }
)";


CUI::Primitive::Fill::Fill(Color background, Vec2 size) : Primitive(program, fragmentSource), background(background), size(size) {
    backgroundLoc = glGetUniformLocation(program, "background");
}

void CUI::Primitive::Fill::setDerivedUniforms(ContextPtr, CUI::Rect) {
    glUniform4fv(backgroundLoc, 1, &background.r);
}