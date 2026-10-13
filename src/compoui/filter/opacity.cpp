#include "opacity.hpp"
#include "../util/shader.hpp"
#include "filter.hpp"

unsigned int CUI::Filter::Opacity::program = 0;
const char* CUI::Filter::Opacity::fragmentSource = R"(
    #version 460 core

    uniform sampler2D tImageTex;
    uniform float uOpacity;

    in vec2 vLocalUV;
    out vec4 outColor;

    void main() {
        vec4 src = texture(tImageTex, vLocalUV);
        outColor = vec4(src.rgb, src.a * uOpacity);
    }
)";

CUI::Filter::Opacity::Opacity(float opacity) : Filter(program, fragmentSource), opacity(opacity) {
    imageLoc = glGetUniformLocation(program, "tImageTex");
    opacityLoc = glGetUniformLocation(program, "uOpacity");
}

std::vector<CUI::Filter::Step> CUI::Filter::Opacity::getSteps() {
    return {
        Step{
            [this](std::shared_ptr<FBO> &fbo) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, fbo->tex);
                glUniform1i(imageLoc, 0);

                glUniform1f(opacityLoc, opacity);
            },
            0,
            program
        }
    };
}