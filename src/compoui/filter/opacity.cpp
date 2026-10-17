#include "opacity.hpp"
#include "../util/shader.hpp"
#include "filter.hpp"

unsigned int CUI::Filter::Opacity::program = 0;
const char* CUI::Filter::Opacity::fragmentSource = R"(
    #version 460 core

    layout(location = 2) uniform sampler2D tImageTex;
    layout(location = 3) uniform float uOpacity;

    in vec2 vLocalUV;
    out vec4 outColor;

    void main() {
        vec4 src = texture(tImageTex, vLocalUV);
        outColor = vec4(src.rgb, src.a * uOpacity);
    }
)";

CUI::Filter::Opacity::Opacity(float opacity) : Filter(program, fragmentSource), opacity(opacity) { }

std::vector<CUI::Filter::Step> CUI::Filter::Opacity::getSteps() {
    return {
        Step{
            [this](std::shared_ptr<FBO> &fbo) {
                glUniform1f(3, opacity);
            },
            0,
            program,
        }
    };
}