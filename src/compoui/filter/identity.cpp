#include "identity.hpp"
#include "filter.hpp"

unsigned int CUI::Filter::Identity::program = 0;
const char* CUI::Filter::Identity::fragmentSource = R"(
    #version 460 core

    layout(location = 2) uniform sampler2D tImageTex;

    in vec2 vLocalUV;
    out vec4 outColor;

    void main() {
        outColor = texture(tImageTex, vLocalUV);
    }
)";

CUI::Filter::Identity::Identity() : Filter(program, fragmentSource) { }

std::vector<CUI::Filter::Step> CUI::Filter::Identity::getSteps() {
    return {
        Step{
            [](std::shared_ptr<FBO> &fbo) {},
            0,
            program,
        }
    };
}