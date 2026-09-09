#include "shader.hpp"

const char* CUI::Util::genericVertexShader = R"(
    #version 460 core

    layout(location = 0) in vec2 pos;

    uniform vec4 rect;
    uniform vec2 viewportSize;

    out vec2 globalPixel;
    out vec2 localPixel;
    out vec2 localUV;
    out vec2 globalUV;

    void main() {
        localUV = pos;
        localPixel = pos * rect.zw;
        globalPixel = rect.xy + localPixel;
        globalUV = globalPixel / viewportSize;
        vec2 pos_normalized = globalUV * vec2(2.0, -2.0) + vec2(-1.0, 1.0);
        gl_Position = vec4(pos_normalized, 0.0, 1.0);
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