#include "filter.hpp"
#include "../glad/glad.h"
#include "../util/shader.hpp"

void CUI::Filter::Filter::draw(ContextPtr ctx, CUI::Rect rect, Step &step, std::shared_ptr<FBO> &fbo) {
    glUseProgram(step.program);

    // Uniforms common to all primitives.
    rect.pos.y += rect.size.y;
    rect.size.y *= -1;
    glUniform4fv(0, 1, &rect.pos.x);

    glUniform2fv(1, 1, &ctx->windowSize.x);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo->tex);
    glUniform1i(2, 0);

    // Derived class may add its own uniforms here.
    step.setUniforms(fbo);

    glBindVertexArray(ctx->vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

CUI::Filter::Filter::Filter(unsigned int &program, const char* fragmentSource) {
    if(program == 0) {
        program = Util::createShaderProgram(Util::genericVertexShader, fragmentSource);
    }
}