#include "shader.hpp"

const char* CUI::Util::genericVertexShader = R"(
    #version 460 core

    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec2 aTexCoord;

    layout(location = 0) uniform vec4 uRect;
    layout(location = 1) uniform vec2 uViewportSize;

    out vec2 vGlobalPixel;
    out vec2 vLocalPixel;
    out vec2 vLocalUV;
    out vec2 vGlobalUV;
    out vec2 vTexCoord;

    void main() {
        vLocalUV = aPos;
        vGlobalPixel = round(uRect.xy + aPos * uRect.zw + 0.001);
        vLocalPixel = vGlobalPixel - uRect.xy;
        vGlobalUV = vGlobalPixel / uViewportSize;
        vec2 pos_normalized = vGlobalUV * vec2(2.0, -2.0) + vec2(-1.0, 1.0);
        gl_Position = vec4(pos_normalized, 0.0, 1.0);

        vTexCoord = aTexCoord;
    }
)";

GLuint CUI::Util::createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}