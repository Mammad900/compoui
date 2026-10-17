#include "identity.hpp"
#include "filter.hpp"

unsigned int CUI::Filter::Identity::program = 0;
const char* CUI::Filter::Identity::fragmentSource = R"(
    #version 460 core

    layout(location = 2) uniform sampler2D tImageTex;

    in vec2 vGlobalUV;
    out vec4 outColor;

    void main() {
        vec2 uv = vec2(vGlobalUV.x, 1.0 - vGlobalUV.y);
        outColor = texture(tImageTex, uv);
    }
)";

CUI::Filter::Identity::Identity() : Filter(program, fragmentSource) { }

std::vector<CUI::Filter::Step> CUI::Filter::Identity::getSteps() {
    return {
        Step{
            program,
            [](std::shared_ptr<FBO> &fbo, ContextPtr ctx) {},
            0,
        }
    };
}