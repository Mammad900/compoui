#include "identity.hpp"
#include "../util/shader.hpp"
#include "filter.hpp"

unsigned int CUI::Filter::Identity::program = 0;
const char* CUI::Filter::Identity::fragmentSource = R"(
    #version 460 core

    uniform sampler2D tImageTex;

    in vec2 vLocalUV;
    out vec4 outColor;

    void main() {
        outColor = texture(tImageTex, vLocalUV);
    }
)";

CUI::Filter::Identity::Identity() : Filter(program, fragmentSource) {
    imageLoc = glGetUniformLocation(program, "tImageTex");
}

std::vector<CUI::Filter::Step> CUI::Filter::Identity::getSteps() {
    return {
        Step{
            [this](std::shared_ptr<FBO> &fbo) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, fbo->tex);
                glUniform1i(imageLoc, 0);
            },
            0,
            program,
            vpSizeLoc,
            rectLoc
        }
    };
}