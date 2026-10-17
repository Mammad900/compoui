#include "primitive.hpp"
#include "../../util/shader.hpp"

CUI::Primitive::Primitive::Primitive(unsigned int &program, const char *fragmentSource) : program(program) {
    if (program == 0) {
        // The vertex shader is shared across all primitives.
        program = Util::createShaderProgram(Util::genericVertexShader,
                                            fragmentSource);
    }
    glUseProgram(program);
}

void CUI::Primitive::Primitive::draw(ContextPtr ctx, CUI::Rect rect) {
    glUseProgram(program);

    // Uniforms common to all primitives.
    glUniform4fv(0, 1, &rect.pos.x);

    glUniform2fv(1, 1, &ctx->windowSize.x);

    // Derived class may add its own uniforms here.
    setDerivedUniforms(ctx, rect);

    glBindVertexArray(ctx->vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}