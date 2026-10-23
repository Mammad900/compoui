#include "filter.hpp"
#include "../glad/glad.h"
#include "../util/shader.hpp"

void CUI::Filter::Filter::draw(CUI::Rect rect, Step &step, std::shared_ptr<FBO> &fbo, bool isBackground) {
    glUseProgram(step.program);

    // Uniforms common to all primitives.
    Rect rectGrown = rect.grow(step.grow);
    glUniform4fv(0, 1, &rectGrown.pos.x);

    glUniform2fv(1, 1, &ctx->windowSize.x);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo->tex);
    glUniform1i(2, 0);

    // Derived class may add its own uniforms here.
    step.setUniforms(fbo, isBackground);

    glBindVertexArray(ctx->vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

CUI::Filter::Filter::Filter(unsigned int &program, const char* fragmentSource) {
    if(program == 0) {
        program = Util::createShaderProgram(Util::genericVertexShader, fragmentSource);
    }
}