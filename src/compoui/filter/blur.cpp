#include "blur.hpp"
#include "../util/shader.hpp"
#include "filter.hpp"
#include <algorithm>
#include <cmath>

unsigned int CUI::Filter::Blur::program = 0;
const char* CUI::Filter::Blur::fragmentSource = R"(
    #version 460 core

    #define MAX_RADIUS 16

    layout(location = 2) uniform sampler2D tImageTex;
    layout(location = 3) uniform sampler1D tWeightsTex;
    layout(location = 4) uniform int uRadius;
    layout(location = 5) uniform vec2 stepUV; // (1, 0) or (0, 1)

    // in vec2 vGlobalPixel;
    // in vec2 vLocalPixel;
    // in vec2 vLocalUV;
    in vec2 vGlobalUV;

    out vec4 outColor;

    void main()
    {
        vec2 uv = vec2(vGlobalUV.x, 1.0 - vGlobalUV.y);
        // Weight texture: texel 0 = center, texel i = offset i.
        float w0 = texelFetch(tWeightsTex, 0, 0).r;
        vec4 result = texture(tImageTex, uv) * w0;

        for (int i = 1; i <= uRadius; ++i) {

            float weight = texelFetch(tWeightsTex, i, 0).r;
            vec2 offset = stepUV * float(i);

            result += (
                texture(tImageTex, uv + offset) +
                texture(tImageTex, uv - offset)
            ) * weight;
        }

        outColor = result;
    }
)";

namespace {
GLuint createGaussianWeightsTexture(float sigma, int radius) {
    if (sigma <= 0.0f || radius < 0)
        return 0;

    std::vector<float> weights(radius + 1);
    const float denominator = 2.0f * sigma * sigma;

    // Store the unnormalized weights.
    weights[0] = 1.0f;
    float total = weights[0];

    for (int i = 1; i <= radius; ++i)
    {
        const float x = static_cast<float>(i);
        weights[i] = std::exp(-(x * x) / denominator);
        total += 2.0f * weights[i];
    }

    // Normalize for the center and both sides of every offset.
    for (float& weight : weights)
        weight /= total;

    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_1D, texture);

    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    glTexImage1D(
        GL_TEXTURE_1D,
        0,
        GL_R32F,
        static_cast<GLsizei>(weights.size()),
        0,
        GL_RED,
        GL_FLOAT,
        weights.data()
    );

    glBindTexture(GL_TEXTURE_1D, 0);
    return texture;
}
}

std::vector<CUI::Filter::Step> CUI::Filter::Blur::getSteps() {
    int radius = std::max(1, (int)std::ceil(sigma * 3.0f));

    if(sigma != lastSigma) {
        if(weightsTex != 0)
            glDeleteTextures(1, &weightsTex);

        weightsTex = createGaussianWeightsTexture(sigma, radius);
    }

    return {
        Step{ // Horizontal pass
            program,
            [this, radius](std::shared_ptr<FBO> &fbo, ContextPtr ctx) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_1D, weightsTex);
                glUniform1i(3, 1);

                glUniform1i(4, radius);
                glUniform2f(5, 1.f / ctx->windowSize.x, 0.f );
            },
            (float)radius,
        },
        Step{ // Horizontal pass
            program,
            [this, radius](std::shared_ptr<FBO> &fbo, ContextPtr ctx) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_1D, weightsTex);
                glUniform1i(3, 1);

                glUniform1i(4, radius);
                glUniform2f(5, 0.f, 1.f / ctx->windowSize.y);
            },
            clip ? 0 : (float)radius,
            true
        }
    };
}