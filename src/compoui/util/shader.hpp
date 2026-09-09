#pragma once

#include "../glad/glad.h"

namespace CUI::Util {
    extern const char* genericVertexShader;
    GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);
}
